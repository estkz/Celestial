#include "../../memory/memory.h"
#include "../../memory/offsets.h"
#include "../../memory/memInterface.h"

#include "../../imgui/imgui_impl_dx9.h"

void godmode()
{
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto healthAddress = localPlayerPtr + m_iHealth;

    const int newHealth = 1337;
    memory.Write<int>(healthAddress, newHealth);
}