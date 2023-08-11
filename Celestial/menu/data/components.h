#pragma once

#include "../../imgui/imgui.h"

// Buttons
namespace btn
{

}

// Checkboxes
namespace cbox
{
	// Aim
	static bool enableAimbot = false;
	static bool enableRecoil = false;
	static bool enableTriggerbot = false;
	
	// Visuals
	static bool enableESP = false;
	static bool enableTeamESP = false;
	static bool enableHealthESP = false;
	static bool enableSnaplines = false;
	static bool enableNameESP = false;
	static bool enableArmorESP = false;
	static bool enableFullbright = false;

	// Misc
	static bool enableBhop = false;
	static bool enableRadar = false;
	static bool enableTeleport = false;
	static bool enableSpeed = false;
	static bool enableMagnet = false;

	// Trainer
	static bool infiniteHealth = false;
	static bool infiniteArmor = false;
	static bool infiniteGunAmmo = false;
	static bool infiniteNades = false;


	// Config
	static bool cbConfig = false;
}

// Integer Sliders
namespace sInt
{

}

// Float Sliders
namespace sFloat
{

}

// Color Pickers
namespace clr
{
	ImColor espColor = ImColor{ 117, 183, 69, 255 };
	ImColor teamColor = ImColor{ 117, 183, 69, 255 };
	ImColor healthColor = ImColor{ 117, 183, 69, 255 };
	ImColor snaplineColor = ImColor{ 117, 183, 69, 255 };
	ImColor nameColor = ImColor{ 117, 183, 69, 255 };
	ImColor armorColor = ImColor{ 117, 183, 69, 255 };
}