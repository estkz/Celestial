#include "../../memory/memory.h"
#include "../../memory/offsets.h"
#include "../../memory/proc.h"

#include "../../imgui/imgui_impl_dx9.h"

void godmode()
{
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto healthAddress = localPlayerPtr + ptrHealth;

    const int newHealth = 1337;
    //memory.Write<int>(healthAddress, newHealth);

    ImGui::Begin("God Mode - Developer Screen");
    ImGui::Text("Module Base Address: 0x%X", moduleBase);
    ImGui::Text("Health Address: 0x%X", healthAddress);
    ImGui::Text("Health: %d", memory.Read<int>(healthAddress));
    ImGui::End();
}