#include "gui.h"

#define STB_IMAGE_IMPLEMENTATION
#include "data/stb_image.h"

#include "../memory/memory.h"
#include "data/font_awesome.cpp"
#include "data/font_awesome.h"
#include "data/components.h"
#include "data/logo.h"

#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

#include <d3d9.h>
#include <vector>
#include <string>

#pragma comment (lib, "d3d9.lib")

// --- Globals ---
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);
int windowX = (screenWidth - gui::WIDTH) / 2;
int windowY = (screenHeight - gui::HEIGHT) / 2;

IDirect3DTexture9* celestialLogo = nullptr;
ImFont* icons_font = nullptr;
bool single_render = true;

// --- Helper Functions ---
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter);

long __stdcall WindowProcess(HWND window, UINT message, WPARAM wideParameter, LPARAM longParameter)
{
    if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
        return true;

    switch (message)
    {
    case WM_SIZE:
        if (gui::device && wideParameter != SIZE_MINIMIZED)
        {
            gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
            gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
            gui::ResetDevice();
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wideParameter & 0xfff0) == SC_KEYMENU) return 0;
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        gui::position = MAKEPOINTS(longParameter);
        return 0;
    case WM_MOUSEMOVE:
        if (wideParameter == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(longParameter);
            auto rect = ::RECT{};
            GetWindowRect(gui::window, &rect);
            rect.left += points.x - gui::position.x;
            rect.top += points.y - gui::position.y;

            if (gui::position.x >= 0 && gui::position.x <= 160 && gui::position.y >= 0 && gui::position.y <= 100)
                SetWindowPos(gui::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
        }
        return 0;
    }
    return DefWindowProc(window, message, wideParameter, longParameter);
}

bool LoadTextureFromMemory(const unsigned char* data, size_t size, IDirect3DTexture9** out_texture) {
    int width, height, channels;
    unsigned char* image_data = stbi_load_from_memory(data, (int)size, &width, &height, &channels, 4);
    if (!image_data) return false;

    HRESULT hr = gui::device->CreateTexture(width, height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, out_texture, NULL);
    if (FAILED(hr)) { stbi_image_free(image_data); return false; }

    D3DLOCKED_RECT rect;
    if (SUCCEEDED((*out_texture)->LockRect(0, &rect, NULL, 0))) {
        unsigned char* dest = (unsigned char*)rect.pBits;
        for (int y = 0; y < height; y++) memcpy(dest + y * rect.Pitch, image_data + y * width * 4, width * 4);
        (*out_texture)->UnlockRect(0);
    }
    stbi_image_free(image_data);
    return true;
}

inline void CustomCheckbox(const char* label, bool* value) {
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
    ImGui::PushStyleColor(ImGuiCol_Border, ImColor(117, 183, 69, 255).Value);
    ImGui::PushStyleColor(ImGuiCol_CheckMark, ImColor(255, 255, 255, 255).Value);

    if (*value) {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(117, 183, 69, 255).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(137, 203, 89, 255).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(117, 183, 69, 255).Value);
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImColor(20, 20, 20, 255).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImColor(30, 30, 30, 255).Value);
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImColor(25, 25, 25, 255).Value);
    }

    ImGui::Checkbox(label, value);
    ImGui::PopStyleColor(5);
    ImGui::PopStyleVar();
}

// --- GUI Methods ---
void gui::CreateHWindow(const char* windowName) noexcept {
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_CLASSDC;
    windowClass.lpfnWndProc = WindowProcess;
    windowClass.hInstance = GetModuleHandleA(0);
    windowClass.lpszClassName = "Celestial";
    RegisterClassEx(&windowClass);
    window = CreateWindowExA(0, "Celestial", windowName, WS_POPUP, windowX, windowY, WIDTH, HEIGHT, 0, 0, windowClass.hInstance, 0);
    ShowWindow(window, SW_SHOWDEFAULT);
    UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept {
    DestroyWindow(window);
    UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept {
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (!d3d) return false;
    ZeroMemory(&presentParameters, sizeof(presentParameters));
    presentParameters.Windowed = TRUE;
    presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
    presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;
    presentParameters.EnableAutoDepthStencil = TRUE;
    presentParameters.AutoDepthStencilFormat = D3DFMT_D16;
    presentParameters.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
    if (d3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &presentParameters, &device) < 0)
        return false;
    return true;
}

void gui::ResetDevice() noexcept {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    if (device->Reset(&presentParameters) == D3DERR_INVALIDCALL) IM_ASSERT(0);
    ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept {
    if (device) { device->Release(); device = nullptr; }
    if (d3d) { d3d->Release(); d3d = nullptr; }
}

void gui::CreateImGui() noexcept {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    io.IniFilename = nullptr;

    ImGui_ImplWin32_Init(window);
    ImGui_ImplDX9_Init(device);

    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14.0f);
    static const ImWchar icon_ranges[]{ 0xf000, 0xf3ff, 0 };
    ImFontConfig icons_config;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 13.0f, &icons_config, icon_ranges);

    style.WindowRounding = 0.0f;
    style.ChildRounding = 0.0f;
    style.FrameRounding = 0.0f;
    style.GrabRounding = 0.0f;
    style.PopupRounding = 0.0f;
    style.ScrollbarRounding = 0.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.WindowPadding = ImVec2(0, 0);
    style.ItemSpacing = ImVec2(10, 8);

    auto& colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImColor(12, 12, 12, 255);
    colors[ImGuiCol_ChildBg] = ImColor(18, 18, 18, 255);
    colors[ImGuiCol_Border] = ImColor(40, 40, 40, 255);
    colors[ImGuiCol_Separator] = ImColor(117, 183, 69, 100);
    colors[ImGuiCol_Text] = ImColor(230, 230, 230, 255);
}

void gui::DestroyImGui() noexcept {
    ImGui_ImplDX9_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void gui::BeginRender() noexcept {
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
        if (message.message == WM_QUIT) { isOpened = false; return; }
    }
    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void gui::EndRender() noexcept {
    ImGui::EndFrame();
    device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);
    if (device->BeginScene() >= 0) {
        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
        device->EndScene();
    }
    if (device->Present(0, 0, 0, 0) == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
        ResetDevice();
}

void gui::Render() noexcept {
    static int selectedTab = 0;
    if (single_render) {
        if (LoadTextureFromMemory(logo_celestial, sizeof(logo_celestial), &celestialLogo))
            single_render = false;
    }

    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Always);
    ImGui::SetNextWindowSize({ (float)WIDTH, (float)HEIGHT });

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Celestial", &isOpened, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

    // --- SIDEBAR ---
    ImGui::BeginChild("##sidebar", ImVec2(160, 0), true);

    // Logo
    ImGui::SetCursorPos({ 15, 20 });
    if (celestialLogo)
        ImGui::Image((void*)celestialLogo, ImVec2(130, 50), ImVec2(0, 0), ImVec2(1, 1), ImVec4(117 / 255.f, 183 / 255.f, 69 / 255.f, 1.f));

    ImGui::SetCursorPosY(85);
    ImGui::Separator();
    ImGui::Spacing();

    // Sharp Tab Selection Buttons
    auto AddTab = [&](const char* label, int index) {
        bool active = (selectedTab == index);
        ImGui::SetCursorPosX(0);

        if (active) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor(117, 183, 69, 160).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(117, 183, 69, 160).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(117, 183, 69, 180).Value);
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImColor(0, 0, 0, 0).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor(30, 30, 30, 255).Value);
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor(25, 25, 25, 255).Value);
        }

        if (ImGui::Button(label, ImVec2(160, 45))) selectedTab = index;
        ImGui::PopStyleColor(3);
        };

    AddTab("TRAINER", 0);
    AddTab("EDUCATIONAL", 1);

    // Sidebar Version Info
    ImGui::SetCursorPos({ 15, (float)HEIGHT - 30 });
    ImGui::TextDisabled("v1.0.0 | Final");

    ImGui::EndChild();
    ImGui::SameLine(0, 0);

    // --- MAIN CONTENT ---
    ImGui::BeginChild("##content", ImVec2(0, 0), false);
    ImGui::SetCursorPos({ 25, 25 });
    ImGui::BeginGroup();

    if (selectedTab == 0) {
        ImGui::TextColored(ImColor(117, 183, 69), "SYSTEM :: PLAYER_MODIFIER");
        ImGui::Separator();
        ImGui::Spacing();

        CustomCheckbox("Infinite Health", &cbox::infiniteHealth);
        CustomCheckbox("Infinite Armor", &cbox::infiniteArmor);
        CustomCheckbox("Infinite Ammo", &cbox::infiniteGunAmmo);
        CustomCheckbox("Infinite Nades", &cbox::infiniteNades);
    }
    else if (selectedTab == 1)
    {
        ImGui::TextColored(ImColor(117, 183, 69), "SYSTEM :: RESEARCH_MODULE");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::TextWrapped("Use the picker below to test real-time variable manipulation.");
        ImGui::Spacing();

        ImGui::Text("Theme Tint:");
        if (ImGui::ColorEdit4("##themePicker", (float*)&clr::colorPicker,ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_AlphaBar))
        {
            // Whatever
        }

        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Live Preview Text:");
        ImGui::SameLine();
        ImGui::TextColored(clr::colorPicker, "CELESTIAL_DATA_STREAM");

        if (ImGui::Button("Reset to Default", ImVec2(150, 30))) {
            clr::colorPicker = ImColor(117, 183, 69, 255);
        }
    }

    ImGui::EndGroup();
    ImGui::EndChild();

    ImGui::End();
    ImGui::PopStyleVar();
}