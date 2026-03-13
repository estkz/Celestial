#include "menu/gui.h"
#include "memory/memory.h"
#include "features/trainer/player_modifier.h"
#include <thread>

int __stdcall wWinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev, _In_ PWSTR args, _In_ int show
) {
    Memory mem("ac_client.exe");
    const auto base = mem.GetModuleAddress("ac_client.exe");

    gui::CreateHWindow("Celestial");
    gui::CreateDevice();
    gui::CreateImGui();

    while (gui::isOpened) {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        if (base) {
            PlayerModifiers::run(mem, base);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();
    return 0;
}