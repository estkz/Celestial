#include "../../memory/memory.h"
#include "../../memory/offsets.h"
#include "../../memory/memInterface.h"

#include "../../imgui/imgui_impl_dx9.h"

void infiniteNades()
{
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto nadeAddress = localPlayerPtr + ptrNades;

    const int newNades = 10;
    memory.Write<int>(nadeAddress, newNades);
}