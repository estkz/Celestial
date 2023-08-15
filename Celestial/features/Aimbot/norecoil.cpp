#include "norecoil.h"
#include "../../memory/memInterface.h"
#include "../../memory/offsets.h"

void ApplyNoRecoilPatch() {
	const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
	const auto localPlayerPtr = memory.Read<std::uintptr_t>(moduleBase + localPlayer);
	const auto recoilAddress = localPlayerPtr + m_iRecoil;

	if (moduleBase) {
		const uint8_t noRecoilPatch[] = { 0x90, 0x90, 0x90, 0x90, 0x90 }; // Your no recoil patch bytes
		for (std::size_t i = 0; i < sizeof(noRecoilPatch); ++i) {
			memory.Write<uint8_t>(recoilAddress + i, noRecoilPatch[i]);
		}
	}
}

void noRecoil() {
	ApplyNoRecoilPatch();
}

// Works to some extend, can improve on it.