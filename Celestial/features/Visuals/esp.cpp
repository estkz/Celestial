#include "esp.h"
#include "../../memory/offsets.h"
#include "../../memory/memInterface.h"

#include <Windows.h>
#include <TlHelp32.h>

// Global Variables
unsigned char eESPColor = 0x000000FF;

#define EnemyPen = eESPColor;
HBRUSH EnemyBrush = CreateSolidBrush(eESPColor);

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);

const auto moduleBase = memory.GetModuleAddress("ac_client.exe");
HDC hdc = GetDC(FindWindow(NULL, " AssaultCube"));

// Math Structures
struct Vector3 {
	float x, y, z;
};

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}
	float matrix[4][4];
};

// WorldToScreen Function | Convert 3d world to our 2d monitor
Vector3 WorldToScreen(const Vector3 position, view_matrix_t matrix) {
	// Transform the x-coordinate and the y-coordinate of the point from world space to screen space
	float _x = matrix[0][0] * position.x + matrix[0][1] * position.y + matrix[0][2] * position.z + matrix[0][3];
	float _y = matrix[1][0] * position.x + matrix[1][1] * position.y + matrix[1][2] * position.z + matrix[1][3];

	// Perform the perspective division to get the w (homogeneous coordinate)
	float w = matrix[3][0] * position.x + matrix[3][1] * position.y + matrix[3][2] * position.z + matrix[3][3];

	// Calculate the inverse of the w value
	float inv_w = 1.0f / w;
	_x *= inv_w;
	_y *= inv_w;

	// Calculate half of the screen width and height
	float x = screenX * .5f;
	float y = screenY * .5f;

	// Apply the final screen transformation and center the coordinates
	x += 0.5f * _x * screenX + 0.5f;
	y += 0.5f * _y * screenX + 0.5f;

	// Return the transformed coordinates along with the original w value
	return { x, y, w };
}

void DrawFilledRect(int x, int y, int w, int h) {
	RECT rectangle = { x, y, x + w, y + h };
	FillRect(hdc, &rectangle, EnemyBrush);
}

void DrawBorderBox(int x, int y, int w, int h, int thickness) {
	DrawFilledRect(x, y, h, thickness);					// Draw the upper horizontal line
	DrawFilledRect(x, y, thickness, h);					// Draw the left vertical line
	DrawFilledRect((x + w), y, thickness, h);			// Draw the left vertical line
	DrawFilledRect(x, y + h, w + thickness, thickness); // Draw the bottom horizontal line
}

void drawESP()
{
	view_matrix_t viewmatrix = memory.Read<view_matrix_t>(moduleBase + dwViewMatrix);
	int localTeam = memory.Read<DWORD>(moduleBase + dwEntityList + iTeamNum);

	for (int i = 1; i < 32; i++) {
		uintptr_t pEnt = memory.Read<DWORD>(moduleBase + dwEntityList + (i + 0x04));

		// Get Entity Information
		int health = memory.Read<int>(pEnt + m_iHealth);
		int team = memory.Read<int>(pEnt + iTeamNum);
		Vector3 position = memory.Read<Vector3>(pEnt + m_vecOrigin);
		Vector3 head;
		head.x = position.x;
		head.y = position.y;
		head.z = position.z + 75.f;

		Vector3 screenPosition = WorldToScreen(position, viewmatrix);
		Vector3 screenHead = WorldToScreen(head, viewmatrix);

		// Calculate the height of the player
		float playerHeight = screenHead.y - screenPosition.y;
		float playerWidth = playerHeight / 2.4f;

		if (screenPosition.z >= 0.01f && team != localTeam && health > 0 && health < 101) {
			DrawBorderBox(screenPosition.y - (playerWidth / 2), screenPosition.y, playerWidth, playerHeight, 1);
		}
	}
}

// Fixing the ESP soon!