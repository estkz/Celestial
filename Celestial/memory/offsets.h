#pragma once

#include <stdint.h>

/*

	TODO: Define all offsets in stead of commented out.

	dsPlayer = ac_client.exe+17E0A8
	dsEntity = "ac_client.exe"+18AC04

	// Local Player
	health = 0xEC
	armor = 0xF0

	// Hacks
	rapidfire = 0x164

	// Entities
	distance between entities = 4 bytes 0x4

	// Gun Ammo
	MK = 0x12C
	TPM = 0x130
	V19 = 0x134
	ARD = 0x138
	AD = 0x13C
	MK(r) = 0x148
	MTP = 0x140
	Nade = 0x144

	// Viewangles
	pViewangleWidth = 0x34
	pViewangleHeight = 0x38

	// Coordinates
	coordX = 0x28
	coordY = 0x2c
	coordZ = 0x30

	
*/

// Lists
static const uintptr_t localPlayer = 0x17E0A8;
static const uintptr_t entityList = 0x18AC04;

// Player
static const uintptr_t health = 0xEC;
static const uintptr_t name = 0x205;
static const uintptr_t xCoord = 0x28;
static const uintptr_t yCoord = 0x2C;
static const uintptr_t zCoord = 0x30;
static const uintptr_t viewangleWidth = 0x34;
static const uintptr_t viewAngleHeight = 0x38;

static const uintptr_t vec3_origin = 0x28;
static const uintptr_t vec3_head = 0x4;
static const uintptr_t iTeamNum = 0x30C;