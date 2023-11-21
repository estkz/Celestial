#pragma once
#include <d3d9.h>

// We create this namespace with all the functions used for setting up the imgui window
// The declared functions and variables down below can be accessed using the scope resolution operator '::'
// Example of usage: gui::
namespace gui
{
	// The width and height of the menu (in pixels)
	constexpr int WIDTH = 560;
	constexpr int HEIGHT = 400;

	// A boolean variable that determines whether the menu is set to open (true) or closed (false)
	inline bool isOpened = true;

	// Windows Application Programming Interface Variables
	inline HWND window = nullptr; // HWND = Window Handle | A window handle is a unique identifier that represents a window
	inline WNDCLASSEXA windowClass = { }; // WNDCLASSEXA = Window Class | WNDCLASSEXA represents a window class and is used to register a window with the WinAPI

	// We declare position as POINTS position = { }; It initializes the 'position' variable with an x and y coordinate. Both are set to zero, because we keep the curly braces empty
	inline POINTS position = { };

	// DirectX State Variables
	inline PDIRECT3D9 d3d = nullptr; // 'd3d' is a pointer to IDirect3D9. It represents the main interface for Direct3D. It's used to initialize and manage Direct3D object
	inline LPDIRECT3DDEVICE9 device = nullptr; // 'device' is a pointer to IDirect3DDevice9. It represents the interface for the Direct3D device. Used for creating and managing rendering resources and executing rendering commands
	inline D3DPRESENT_PARAMETERS presentParameters = { }; // 'presentParameters' represents the presentation paramters for the Direct3D device. The initialization of presentParameters with { } sets all its members to their default values

	// Function: responsible for creating an HWindow (handle to a window) with the specified window name. It uses the Win32 API
	void CreateHWindow(const char* windowName) noexcept;

	// Function: responsible for destroying the HWindow and performing any necessary cleanup operations. The 'noexcept' specifier indicates that these functions are not expected to throw any exceptions.
	void DestroyHWindow() noexcept;

	// Functions that are related to the creation, management and destruction of a device.
	bool CreateDevice() noexcept; // This function is responsible for creating the Direct3D device used for rendering
	void ResetDevice() noexcept; // This function is responsible for resetting the Direct3D device after a device loss
	void DestroyDevice() noexcept; // This function is responsible for releasing the Direct3D device and associated resources

	// Functions that handle the initialization, cleanup and rendering aspects of using ImGui for our menu
	void CreateImGui() noexcept; // Initializing the ImGui library, setting up the necessary resources and configuration for using ImGui in the application
	void DestroyImGui() noexcept; // Responsible for cleaning up and releasing resources associated with ImGui. Avoids memory leaks by releasing allocated memory related to ImGui

	// Functions that handle the rendering of our menu using the ImGui library
	void BeginRender() noexcept; // Speaks for itself. This function begins the rendering frame for ImGui
	void Render() noexcept; // This function is responsible for rendering the GUI using ImGui
	void EndRender() noexcept; // This function finalizes the rendering frame for ImGui
}
