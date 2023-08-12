#pragma once

#include <stdint.h>

// Lists
constexpr auto localPlayer = 0x17E0A8;
constexpr auto entityList = 0x18AC04;

// Player
constexpr auto ptrHealth = 0xEC;
constexpr auto ptrName = 0x205;
constexpr auto ptrArmor = 0xF0;
constexpr auto ptrXCoord = 0x28;
constexpr auto ptrYCoord = 0x2C;
constexpr auto ptrZCoord = 0x30;
constexpr auto ptrViewangleWidth = 0x34;
constexpr auto ptrViewAngleHeight = 0x38;

constexpr auto vec3_origin = 0x28;
constexpr auto vec3_head = 0x4;
constexpr auto iTeamNum = 0x30C;

// Hacks
constexpr auto rapidFire = 0x164;