#pragma once

#include <stdint.h>

/*
	Offsets for Assault Cube (32 bits) 1.3.0.2
*/

//namespace offsets {
//	constexpr auto localPlayer = 0x17E0A8;
//	constexpr auto dwEntityList = 0x18AC04;
//
//	constexpr auto health = 0xEC;
//	constexpr auto name = 0x205;
//	constexpr auto armor = 0xF0;
//	constexpr auto postionX = 0x28;
//	constexpr auto positionY = 0x2C;
//	constexpr auto positionZ = 0x30;
//	constexpr auto yaw = 0x34;
//	constexpr auto pitch = 0x38;
//	constexpr auto recoil = 0x40;
//	constexpr auto shotCount = 0x180;
//
//	constexpr auto vecOrigin = 0x28;
//	constexpr auto head = 0x4; // vec3
//	constexpr auto team = 0x30C;
//	constexpr auto viewMatrix = 0x501AE8;
//
//	constexpr auto nades = 0x144;
//
//	constexpr auto rapidFire = 0x164;
//}

/*
	Offsets for Assault Cube (32 bits) 1.2.0.2
*/

namespace offsets {
	inline constexpr std::uintptr_t localPlayer = 0x10F4F4; // ptr to Player
	inline constexpr std::uintptr_t entityList = 0x10F4F8; // ptr to EntityList
	inline constexpr std::uintptr_t entityEntrySize = 0x4; // distance between entities
	inline constexpr std::uintptr_t headPos = 0x4; // vec3
	inline constexpr std::uintptr_t feetPos = 0x34; // vec3
	inline constexpr std::uintptr_t health = 0xF8; // int
	inline constexpr std::uintptr_t armor = 0xFC; // int
	inline constexpr std::uintptr_t yaw = 0x40; // angles (yaw, pitch, roll)
	inline constexpr std::uintptr_t team = 0x32C; // int (team id)
	inline constexpr std::uintptr_t renderPos = 0x408; // vec3
	inline constexpr std::uintptr_t pistolClip = 0x13C; // int
	inline constexpr std::uintptr_t carbineClip = 0x140; // int
	inline constexpr std::uintptr_t shotgunClip = 0x144; // int
	inline constexpr std::uintptr_t mgClip = 0x148; // int
	inline constexpr std::uintptr_t sniperClip = 0x14C; // int
	inline constexpr std::uintptr_t arifleClip = 0x150; // int
	inline constexpr std::uintptr_t grenadeAmmo = 0x158; // int
}