#include "../../memory/memory.h"
#include "../../memory/offsets.h"

#include "../../menu/gui.h"
#include "../../imgui/imgui_impl_dx9.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_internal.h"

Memory memory{ "ac_client.exe" };
const auto moduleBase = memory.GetModuleAddress("ac_client.exe");

void godmode()
{
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto healthAddress = localPlayerPtr + ptrHealth;

    const int newValue = 1337;
    memory.Write<int>(healthAddress, newValue);

    ImGui::Begin("God Mode - Developer Screen");
    ImGui::Text("Module Base Address: 0x%X", moduleBase);
    ImGui::Text("Health Address: 0x%X", healthAddress);
    ImGui::Text("Health: %d", newValue);
    ImGui::End();
}