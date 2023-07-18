// Menu Related Includes

#include "../menu/gui.h"

#include "data/font_awesome.cpp"
#include "data/font_awesome.h"
#include "data/components.h"
#include "data/logo.h"

#include "../imgui/imgui_impl_dx9.h"
#include "../imgui/imgui_impl_win32.h"
#include "../imgui/imgui_internal.h"

#include <d3dx9.h>
#include <d3d9.h>
#include <d3dx9tex.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")


// Miscellaneous Related Includes
#include <vector>
#include <string>

// Here I store the width and height of our mnonitor in screenWidth and screenHeight
int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

// To center the menu on the screen, we subtract the width and height of our monitor with the width and height of our menu. We then divide both with 2 and store windowX and windowY
int windowX = (screenWidth - gui::WIDTH) / 2;
int windowY = (screenHeight - gui::HEIGHT) / 2;

IDirect3DTexture9* celestialLogo = nullptr;
bool single_render = true;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter
);

long __stdcall WindowProcess(
	HWND window,
	UINT message,
	WPARAM wideParameter,
	LPARAM longParameter) 
{
	if (ImGui_ImplWin32_WndProcHandler(window, message, wideParameter, longParameter))
		return true;

		switch (message)
		{
		case WM_SIZE: 
		{
			if (gui::device && wideParameter != SIZE_MINIMIZED)
			{
				gui::presentParameters.BackBufferWidth = LOWORD(longParameter);
				gui::presentParameters.BackBufferHeight = HIWORD(longParameter);
				gui::ResetDevice();
			}
		} return 0;


		case WM_SYSCOMMAND:
		{
			if ((wideParameter & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
				return 0;
		} break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} return 0;


		case WM_LBUTTONDOWN:
		{
			gui::position = MAKEPOINTS(longParameter); // Set click Points
		} return 0;


		case WM_MOUSEMOVE:
		{
			if (wideParameter == MK_LBUTTON)
			{
				const auto points = MAKEPOINTS(longParameter);
				auto rect = ::RECT{ };

				GetWindowRect(gui::window, &rect);

				rect.left += points.x - gui::position.x;
				rect.top += points.y - gui::position.y;
				SetWindowPos(gui::window, HWND_TOPMOST, rect.left, rect.top, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER);
			}
		} return 0;

		}
		return DefWindowProc(window, message, wideParameter, longParameter);
}

void gui::CreateHWindow(const char* windowName) noexcept
{
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_CLASSDC;
	windowClass.lpfnWndProc = WindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandleA(0);
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = 0;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = "Celestial";
	windowClass.hIconSm = 0;

	RegisterClassEx(&windowClass);

	window = CreateWindowExA(0, "Celestial", windowName, WS_POPUP, windowX, windowY, WIDTH, HEIGHT, 0, 0, windowClass.hInstance, 0);

	ShowWindow(window, SW_SHOWDEFAULT);
	UpdateWindow(window);
}

void gui::DestroyHWindow() noexcept
{
	DestroyWindow(window);
	UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

bool gui::CreateDevice() noexcept
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d)
		return false;

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

void gui::ResetDevice() noexcept
{
	ImGui_ImplDX9_InvalidateDeviceObjects();

	const auto result = device->Reset(&presentParameters);

	if (result == D3DERR_INVALIDCALL)
		IM_ASSERT(0);

	ImGui_ImplDX9_CreateDeviceObjects();
}

void gui::DestroyDevice() noexcept
{
	if (device)
	{
		device->Release();
		device = nullptr;
	}

	if (d3d)
	{
		d3d->Release();
		d3d = nullptr;
	}
}

// Used for loading icons
inline ImFont* icons_font = nullptr;

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ::ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Verdana.ttf", 14.0f);

	// Icons In Menu
	static const ImWchar icon_ranges[]{ 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;

	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	icons_font = io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 13.0f, &icons_config, icon_ranges);

	// Menu - Color Theme Config
	colors[ImGuiCol_WindowBg] = ImColor(20, 20, 20); // Frame Backcolor
	colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24); // Childform Backcolor

		// Button
	colors[ImGuiCol_Button] = ImColor(24, 24, 24); // Button Backcolor
	colors[ImGuiCol_ButtonActive] = ImColor(34, 34, 34); // Button Active
	colors[ImGuiCol_ButtonHovered] = ImColor(24, 24, 24); // Hover Color

		// Checkbox
	colors[ImGuiCol_FrameBg] = ImColor(50, 50, 50); // Checkbox Inside Color
	colors[ImGuiCol_FrameBgHovered] = ImColor(50, 50, 50); // Checkbox Inside Hover Color
	colors[ImGuiCol_FrameBgActive] = ImColor(117, 183, 69); // Checked Color

}

void gui::DestroyImGui() noexcept
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);

		if (message.message == WM_QUIT)
		{
			isOpened = !isOpened;
			return;
		}
	}

	// Start of the ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
	ImGui::EndFrame();

	device->SetRenderState(D3DRS_ZENABLE, FALSE);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);

	device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_RGBA(0, 0, 0, 255), 1.0f, 0);

	if (device->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}

	const auto result = device->Present(0, 0, 0, 0);

	// handle loss of D3D9 device
	if (result == D3DERR_DEVICELOST && device->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
		ResetDevice();
}

/*
	[+] ---------- Styling ---------- [+]

		    • Component Styling
		    • Render Function

	[+] ----------------------------- [+]
*/

inline void CustomCheckbox(const char* format, bool* value) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 colorFrameBg = style.Colors[ImGuiCol_FrameBg];

	// If the checkbox is checked (true), change the color of the space inside the checkbox to green
	if (*value) {
		style.Colors[ImGuiCol_FrameBg] = ImVec4(117 / 255.0f, 183 / 255.0f, 69 / 255.0f, 1.0f);
	}

	ImGui::Checkbox(format, value);

	// Restore the original color after rendering the checkbox
	style.Colors[ImGuiCol_FrameBg] = colorFrameBg;

	ImGui::Dummy(ImVec2());
}

void gui::Render() noexcept
{
	// Logo Size
	ImVec2 logo_size(140, 60);

	// Cheat Tabs
	static int selectedTabIndex = 0;
	static const char* tabs[] = { ICON_FA_CROSSHAIRS "  Aimbot", ICON_FA_EYE "  Visual", ICON_FA_COG "  Misc", ICON_FA_FOLDER "  Config" };

	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::SetNextWindowSize({ WIDTH, HEIGHT });
	ImGui::Begin(
		"Celestial",
		&isOpened,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoScrollbar
	);

	if (single_render)
	{
		D3DXCreateTextureFromFileInMemoryEx(gui::device, &logo_celestial, sizeof(logo_celestial), 140, 50, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &celestialLogo);
		single_render = false;
	}

	// This is the start of the menu. Feel free to add checkboxes and other components below!

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f); // Set the child panel rounding

	ImGui::BeginChild("##selection_panel", ImVec2(ImGui::GetContentRegionAvail().x / 3.8f, ImGui::GetContentRegionAvail().y));

	ImGui::Image(celestialLogo, logo_size);

	// Add the grey separator line
	ImGui::Separator();

	// Add some empty space to make buttons appear a bit lower
	ImGui::Dummy(ImVec2(0, 17));

	// Adjust the button height here (default height is 0, which means auto-sizing)
	ImVec2 buttonSize = ImVec2(-1, 30); // Set the height to 30 pixels

	ImColor unselectedColor(24, 24, 24); // RGB values (0-255) converted to ImColor
	ImColor selectedColor(34, 34, 34);   // RGB values (0-255) converted to ImColor

	for (int i = 0; i < sizeof(tabs) / sizeof(tabs[0]); ++i) {
		ImVec4 buttonColor = (selectedTabIndex == i) ? selectedColor : unselectedColor;
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColor);
		if (ImGui::Button(tabs[i], buttonSize)) {
			selectedTabIndex = i;
		}
		ImGui::PopStyleColor();
	}

	ImGui::EndChild();

	ImGui::PopStyleVar(); // Restore the original style

	// Add the blue separator line
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImColor(117, 183, 69).Value);
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::PopStyleColor();
	ImGui::SameLine();

	ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f); // Set the cheat panel rounding

	ImGui::BeginChild("##cheat_panel", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));

	// Set the starting position for rendering components within the child window
	ImGui::SetCursorPos(ImVec2(10, 10)); // X=10, Y=10 (adjust as needed)

	// Render the appropriate content for the selected tab
	if (selectedTabIndex == 0) // Aim Tab
	{
		CustomCheckbox(" Aimbot", &cbox::enableAimbot);
	}
	else if (selectedTabIndex == 1) // Visual Tab
	{
		CustomCheckbox(" ESP", &cbox::enableESP);
	}
	else if (selectedTabIndex == 2) // Misc Tab
	{
		CustomCheckbox(" Bunnyhop", &cbox::enableBhop);
	}
	else if (selectedTabIndex == 3) // Config Tab
	{
		CustomCheckbox(" Config", &cbox::cbConfig);
	}

	ImGui::EndChild();

	ImGui::PopStyleVar(); // Restore the original style

	//  This marks the end of the menu section. Please ensure that the menu code is placed above this comment!
	ImGui::End();
}