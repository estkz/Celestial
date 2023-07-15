// Menu Related Includes
#include "../menu/gui.h"

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

				if (gui::position.x >= 0 &&
					gui::position.x <= gui::WIDTH &&
					gui::position.y >= 0 && gui::position.y <= 19)
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

void gui::CreateImGui() noexcept
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO io = ::ImGui::GetIO();

	ImGuiStyle& style = ImGui::GetStyle();
	auto& colors = style.Colors;

	io.IniFilename = NULL;

	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(device);



	// Menu - Color Theme Config
	colors[ImGuiCol_WindowBg] = ImColor(24, 24, 24); // Frame Backcolor

	colors[ImGuiCol_ResizeGrip] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_ResizeGripActive] = ImColor(0, 0, 0, 0);
	colors[ImGuiCol_ResizeGripHovered] = ImColor(0, 0, 0, 0);

	colors[ImGuiCol_Button] = ImColor(18, 18, 18, 100);
	colors[ImGuiCol_ButtonActive] = ImColor(21, 21, 21, 100);
	colors[ImGuiCol_ButtonHovered] = ImColor(21, 21, 21, 100);

	colors[ImGuiCol_CheckMark] = ImColor(1, 191, 253, 255);

	colors[ImGuiCol_FrameBg] = ImColor(24, 24, 24);
	colors[ImGuiCol_FrameBgActive] = ImColor(26, 26, 26);
	colors[ImGuiCol_FrameBg] = ImColor(26, 26, 26);

	//colors[ImGuiCol_PopupBg] = ImColor(1, 191, 253, 255);
	colors[ImGuiCol_HeaderHovered] = ImColor(1, 191, 253, 255);
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

void gui::Render() noexcept
{
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

	// This is the start of the menu. Feel free to add checkboxes and other components below!

	ImGui::BeginChild("##selection_panel", ImVec2(ImGui::GetContentRegionAvail().x / 7.0f, ImGui::GetContentRegionAvail().y));

	// Left Panel
	/* TODO: Center Buttons, change button style & make wireframes for menu design */
	ImGui::Button("Aimbot", ImVec2(100, 100));
	ImGui::Button("Visuals", ImVec2(100, 100));
	ImGui::Button("Misc", ImVec2(100, 100));
	ImGui::Button("Config", ImVec2(100, 100));
	ImGui::EndChild();

	// Add the blue seperator line
	ImGui::SameLine();
	ImGui::PushStyleColor(ImGuiCol_Separator, ImColor(1, 191, 253, 255).Value);
	ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
	ImGui::PopStyleColor();
	ImGui::SameLine();



	//  This marks the end of the menu section. Please ensure that the menu code is placed above this comment!
	ImGui::End();
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