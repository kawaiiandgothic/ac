#include "hooks.hpp"
#include <iostream>
#include "../minhook/MinHook.h"
#include "renderer.hpp"
Hooks::SwapBuffers::fn orig = nullptr;

struct Vec2 { float x, y; };
struct Vec3 { float x, y, z; };
struct Vec4 { float x, y, z, w; };

class Ent
{
public:
    char pad_0000[4]; //0x0000
    Vec3 HeadPos; //0x0004
    char pad_0010[36]; //0x0010
    Vec3 PlayerPos; //0x0034
    char pad_0040[184]; //0x0040
    int32_t Health; //0x00F8
}; //Size: 0x018C

bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], Vec2 WindowSize) {
    Vec4 clipCoords;
    clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
    clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
    clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
    clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

    if (clipCoords.w < 0.1f) {
        return false;
    }

    Vec3 NDC;
    NDC.x = clipCoords.x / clipCoords.w;
    NDC.y = clipCoords.y / clipCoords.w;
    NDC.z = clipCoords.z / clipCoords.w;

    screen.x = (WindowSize.x / 2 * NDC.x) + (NDC.x + WindowSize.x / 2);
    screen.y = -(WindowSize.y / 2 * NDC.y) + (NDC.y + WindowSize.y / 2);
    return true;
}

BOOL __stdcall Hooks::SwapBuffers::Hook(HDC hdc)
{
    Vec2 WindowSize = { 1024,768 };

    float matrix[16];
    uintptr_t BaseAddr = (uintptr_t)GetModuleHandleA("ac_client.exe");
    uintptr_t EntList = *(uintptr_t*)(BaseAddr + 0x10F4F8);

    memcpy(&matrix, (PBYTE*)(0x501AE8), sizeof(matrix));

    for (int i = 1; i != 12; i++) {
        Ent* Entity = *(Ent**)(EntList + (i * 0x4));
        if (!Entity) { continue; }
        std::cout << Entity->Health << std::endl;
        
        if (Entity->Health > 0) {
            Vec2 Pos2D, Head2D;
            WorldToScreen(Entity->PlayerPos, Pos2D, matrix, WindowSize);
            WorldToScreen(Entity->HeadPos, Head2D, matrix, WindowSize);
            Renderer::Line(WindowSize.x / 2, WindowSize.y, Pos2D.x, Pos2D.y);
        }
    }

    return orig(hdc);
}

void Hooks::Initialize()
{
	void* swapBaddr = (void*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");

	if (MH_Initialize() != MH_OK)
		printf("failed to init mh\n");

	if (MH_CreateHook(swapBaddr, &Hooks::SwapBuffers::Hook, reinterpret_cast<void**>(&orig)) != MH_STATUS::MH_OK)
		printf("hook failed\n");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		printf("mh failure\n");

	printf("hooks initialized\n");
}

void Hooks::Release()
{
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}