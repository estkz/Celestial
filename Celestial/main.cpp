/*
	[+] ---------- Celestial ---------- [+]
	
		• Developed by: estetik (loab)
		 • Build: Alpha
		 • Date: 14-7-2023
		• Educational Purposes Only

	[+] ------------------------------- [+]
*/


// Gui Function Includes
#include "menu/gui.h"
#include "memory/memory.h"

// Miscellaneous Related Includes
#include <thread>

int __stdcall wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow)
{
	gui::CreateHWindow("Celestial");
	gui::CreateDevice();
	gui::CreateImGui();

	while (gui::isOpened)
	{
		gui::BeginRender();
		gui::Render();
		gui::EndRender();

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	gui::DestroyImGui();
	gui::DestroyDevice();
	gui::DestroyHWindow();

	// Once EXIT_SUCCES gets returned it indicates that the cheat has executed successfully and is now terminating (shutting down).
	return EXIT_SUCCESS;
}

