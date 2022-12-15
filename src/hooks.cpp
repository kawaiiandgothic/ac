#include <iostream>
#include "hooks.hpp"
#include "../minhook/MinHook.h"
#include "renderer.hpp"

Hooks::SwapBuffers::fn orig = nullptr;

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

// Copied these values and this ESP scaling method from a tutorial on GuidedHacking (https://www.youtube.com/watch?v=kGDKQXgxIrY&t=1125s)
// they are used for scaling an ESP box
#define VIRTUAL_SCREEN_WIDTH 800
#define GAME_UNIT_MAGIC 400
#define PLAYER_HEIGHT 7.25
#define PLAYER_WIDTH 3.5
#define PLAYER_ASPECT_RATIO PLAYER_HEIGHT / PLAYER_WIDTH;

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
auto color = rgb::green;

BOOL __stdcall Hooks::SwapBuffers::Hook(HDC hdc)
{
    Vec2 WindowSize = { 1024,768 };
    
    float matrix[16];
    uintptr_t BaseAddr = (uintptr_t)GetModuleHandleA("ac_client.exe");
    uintptr_t EntList = *(uintptr_t*)(BaseAddr + 0x10F4F8);
    Ent* localPlayer = *(Ent**)(BaseAddr + 0x10F4F4);
    auto players_in_game = *(uintptr_t*)(BaseAddr + 0x10F500);

    memcpy(&matrix, (PBYTE*)(0x501AE8), sizeof(matrix));

    for (int i = 1; i < players_in_game; i++) {
        Ent* Entity = *(Ent**)(EntList + (i * 0x4));
        if (!Entity) { continue; }
        
        if (Entity->Health > 0 || Entity->Health >= 100) {
            Vec2 Pos2D, Head2D;
            WorldToScreen(Entity->PlayerPos, Pos2D, matrix, WindowSize);
            WorldToScreen(Entity->HeadPos, Head2D, matrix, WindowSize);

            float dist = localPlayer->PlayerPos.Distance(Entity->PlayerPos);

            //float scale = (GAME_UNIT_MAGIC / dist) * (viewport[2] / VIRTUAL_SCREEN_WIDTH);

            Renderer::Box(Pos2D.x, Pos2D.y, 25, 50, 2.0f, color);
            // need idk
            auto left = Pos2D.x = PLAYER_WIDTH;
            auto right = Pos2D.y = PLAYER_HEIGHT;
            Renderer::Box(Pos2D.x, right, Pos2D.y, left, 2.0f, color);

            //Renderer::Line(Pos2D.x - 50, Pos2D.y - 50, Head2D.x - 50, Head2D.y - 50);
            //Renderer::Line(WindowSize.x / 2, WindowSize.y, Pos2D.x, Pos2D.y);
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