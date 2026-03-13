#include "player_modifier.h"

#include "player_modifier.h"

void PlayerModifiers::run(const Memory& mem, std::uintptr_t base) noexcept {
    const auto localPlayer = mem.Read<std::uintptr_t>(base + offsets::localPlayer);
    if (!localPlayer) return;

    if (cbox::infiniteHealth)
        mem.Write<int>(localPlayer + offsets::health, 999);

    if (cbox::infiniteArmor)
        mem.Write<int>(localPlayer + offsets::armor, 999);

    if (cbox::infiniteGunAmmo) {
        const int ammoVal = 99;

        mem.Write<int>(localPlayer + offsets::pistolClip, ammoVal);
        mem.Write<int>(localPlayer + offsets::carbineClip, ammoVal);
        mem.Write<int>(localPlayer + offsets::shotgunClip, ammoVal);
        mem.Write<int>(localPlayer + offsets::mgClip, ammoVal);
        mem.Write<int>(localPlayer + offsets::sniperClip, ammoVal);
        mem.Write<int>(localPlayer + offsets::arifleClip, ammoVal);
    }

    if (cbox::infiniteNades) {
        mem.Write<int>(localPlayer + offsets::grenadeAmmo, 5);
    }
}