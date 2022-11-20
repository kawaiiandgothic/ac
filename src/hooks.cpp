#include "hooks.hpp"
#include <iostream>
#include "../minhook/MinHook.h"
#include "renderer.hpp"
Hooks::SwapBuffers::fn orig = nullptr;

// https://www.unknowncheats.me/forum/other-fps-games/289550-assault-cube-camera-world-position.html
static struct glmatrixf
{
	float v[16];

	float operator[](int i) const { return v[i]; }
	float& operator[](int i) { return v[i]; }
};

static glmatrixf* GetViewMatrix() {
	return (glmatrixf*)0x501AE8; // viewMatrix offset
};

BOOL __stdcall Hooks::SwapBuffers::Hook(HDC hdc)
{
	auto matrix = GetViewMatrix();
	std::cout << matrix->v[0] - 15;
	Renderer::Line(1, 500, 1000, 500);

	return orig(hdc);
}

void Hooks::Initialize()
{
	void* swapBaddr = (void*)GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers");
	std::cout << swapBaddr << std::endl;

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