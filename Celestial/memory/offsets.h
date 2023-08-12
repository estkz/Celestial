#pragma once

#include <stdint.h>

/*

	TODO: Define all offsets in stead of commented out.

	dsPlayer = ac_client.exe+17E0A8
	dsEntity = "ac_client.exe"+18AC04

	// Gun Ammo
	MK = 0x12C
	TPM = 0x130
	V19 = 0x134
	ARD = 0x138
	AD = 0x13C
	MK(r) = 0x148
	MTP = 0x140
	Nade = 0x144
*/

// Lists
constexpr auto localPlayer = 0x17E0A8;
static const uintptr_t entityList = 0x18AC04;

// Player
constexpr auto ptrHealth = 0xEC;
static const uintptr_t ptrName = 0x205;
static const uintptr_t ptrArmor = 0xF0;
static const uintptr_t ptrXCoord = 0x28;
static const uintptr_t ptrYCoord = 0x2C;
static const uintptr_t ptrZCoord = 0x30;
static const uintptr_t ptrViewangleWidth = 0x34;
static const uintptr_t ptrViewAngleHeight = 0x38;

static const uintptr_t vec3_origin = 0x28;
static const uintptr_t vec3_head = 0x4;
static const uintptr_t iTeamNum = 0x30C;

// Hacks
static const uintptr_t rapidFire = 0x164;