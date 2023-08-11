#include "esp.h"

#include <Windows.h>
#include <TlHelp32.h>

// Global Variables
unsigned char eESPColor = 0x000000FF;

#define EnemyPen = eESPColor;
HBRUSH EnemyBrush = CreateSolidBrush(eESPColor);

int screenX = GetSystemMetrics(SM_CXSCREEN);
int screenY = GetSystemMetrics(SM_CYSCREEN);

void drawESP()
{

}