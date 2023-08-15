#include "norecoil.h"
#include "../../memory/memInterface.h"
#include "../../memory/offsets.h"

int previousTimesShot = 0; // Store the previous timesShot value

void ApplyNoRecoilPatch() {
    const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
    const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
    const auto recoilAddress = localPlayerPtr + m_iRecoil;

    // Read the timesShot value
    const int timesShot = memory.Read<int>(localPlayerPtr + m_iShotAmount);

    // Check if the player is shooting (timesShot increased since last check)
    if (timesShot != previousTimesShot) {
        const uint8_t noRecoilPatch[] = { 0x90, 0x90, 0x90, 0x90, 0x90 }; // Your no recoil patch bytes
        for (std::size_t i = 0; i < sizeof(noRecoilPatch); ++i) {
            memory.Write<uint8_t>(recoilAddress + i, noRecoilPatch[i]);
        }

        // Update the previousTimesShot value
        previousTimesShot = timesShot;
    }
}

void noRecoil() {
    ApplyNoRecoilPatch();
}


// Works to some extend, the delay between each shot is still causing the crosshair to move up a little bit.
// To potentially fix this I should find the 'm_isPlayerShooting' offset for more accurate results.