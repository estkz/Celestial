#pragma once

#include <stdint.h>

// What is constexpr?
// By marking it as constexpr, you're telling the compiler that the value of localPlayer can be computed at compile time, and it can be used in contexts
// where a compile-time constant is required.

// I've marked the offsets as constexpr, because this is a way of telling the compiler that the values of each offset do NOT change during runtime.

/*
	Offsets for Assault Cube (32 bits)
*/

// Lists
constexpr auto localPlayer = 0x17E0A8;
constexpr auto dwEntityList = 0x18AC04;

// Player
constexpr auto m_iHealth = 0xEC;
constexpr auto m_iName = 0x205;
constexpr auto m_Armor = 0xF0;
constexpr auto m_XCoord = 0x28;
constexpr auto m_YCoord = 0x2C;
constexpr auto m_ZCoord = 0x30;
constexpr auto m_ViewangleWidth = 0x34;
constexpr auto m_ViewAngleHeight = 0x38;
constexpr auto m_iRecoil = 0x40;
constexpr auto m_iShotAmount = 0x180;

constexpr auto m_vecOrigin = 0x28;
constexpr auto vec3_head = 0x4;
constexpr auto iTeamNum = 0x30C;
constexpr auto dwViewMatrix = 0x501AE8;

// Weapons
constexpr auto m_Nades = 0x144;

// Hacks
constexpr auto rapidFire = 0x164;

/*
	Offsets for Assault Cube (64 bits)
*/

constexpr auto m_iHealth = 0x8;