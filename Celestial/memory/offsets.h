#pragma once

#include <stdint.h>

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
constexpr auto dwViewMatrix = 0x501AE8; // Maybe 0x57DFD0 or 0x17DFFC -> Must find the right offset first lol

// 0x17E010, 
// 0x17E060
// 0x192088

// Prob it: 0x17DFFC-0x6C+ 0x4*16

// Weapons
constexpr auto m_Nades = 0x144;

// Hacks
constexpr auto rapidFire = 0x164;