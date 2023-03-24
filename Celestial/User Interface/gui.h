#pragma once
#include <d3d9.h>

namespace gui {

	constexpr int WIDTH = 300;
	constexpr int HEIGHT = 600;

	inline bool exit = true;

	// Winapi variables for the UI window
	inline HWND window = nullptr;
	inline WNDCLASSEXA windowClass = { };

	// Points for UI dragging
	inline POINTS position = { };

	// DirectX state variables
	inline PDIRECT3D9 d3d = nullptr;
	inline LPDIRECT3DDEVICE9 device = nullptr;
	inline D3DPRESENT_PARAMETERS presentParameters = { };

	// Handle the creation and destruction of the window
	void CreateHWindow(const char* windowName, const char* className) noexcept;
	void DestroyHWindow() noexcept;
	void ResetDevice() noexcept;

	// Handle the creation and destruction of ImGui
	void CreateImGui() noexcept;
	void DestroyImGui() noexcept;

	void BeginRender() noexcept;
	void EndRender() noexcept;
	void Render() noexcept;
}
