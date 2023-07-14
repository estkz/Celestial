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

void CreateHWindow(const char* windowName) noexcept
{

}

void DestroyHWindow() noexcept
{

}

bool CreateDevice() noexcept
{

}

void ResetDevice() noexcept
{

}

void DestroyDevice() noexcept
{

}

void CreateImGui() noexcept
{

}

void DestroyImGui() noexcept
{

}

void BeginRender() noexcept
{

}

void Render() noexcept
{

}

void EndRender() noexcept
{

}