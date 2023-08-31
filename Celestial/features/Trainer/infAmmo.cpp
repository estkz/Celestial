#include "../../memory/memory.h"
#include "../../memory/offsets.h"
#include "../../memory/memInterface.h"

#include "../../imgui/imgui_impl_dx9.h"

void infiniteAmmo()
{
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto ammoAddress = localPlayerPtr + 0x140; // Temp offset, will make this work on all weapons soon

    const int newAmmo = 1337;
    memory.Write<int>(ammoAddress, newAmmo);
}