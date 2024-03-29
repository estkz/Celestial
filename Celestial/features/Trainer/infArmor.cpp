#include "../../memory/memory.h"
#include "../../memory/offsets.h"
#include "../../memory/memInterface.h"

#include "../../imgui/imgui_impl_dx9.h"

void infiniteArmor()
{
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto armorAddress = localPlayerPtr + m_Armor;

    const int newArmor = 100;
    memory.Write<int>(armorAddress, newArmor);
}