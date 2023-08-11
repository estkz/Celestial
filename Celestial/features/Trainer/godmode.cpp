#include "godmode.h"
#include "../../memory/memory.h"
#include "../../memory/offsets.h"

#include "../../menu/gui.h"
#include "../../imgui/imgui_impl_dx9.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_internal.h"

#include <iostream>


Memory memory("ac_client.exe");

// Get the base address of the module
std::uintptr_t moduleBase = memory.GetModuleAddress("ac_client.exe");
std::uintptr_t healthAddress = moduleBase + ptrHealth;


void godmode()
{
    int health = memory.Read<int>(healthAddress);

    ImGui::Begin("God Mode");
    ImGui::Text("Module Base Address: 0x%X", moduleBase);
    ImGui::Text("Health: %d", health);
    ImGui::End();
}

// Fixing soon