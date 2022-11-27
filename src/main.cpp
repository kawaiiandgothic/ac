#include <Windows.h>
#include <iostream>
#include <thread>
#include "hooks.hpp"

void Attach(HINSTANCE instance)
{
	AllocConsole();
	freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
	Hooks::Initialize();

	while (!GetAsyncKeyState(VK_F5))
		std::this_thread::sleep_for(std::chrono::milliseconds(100));

	FreeLibrary(instance);
}

bool __stdcall DllMain(HINSTANCE instance, uint32_t reason, void* _)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		std::thread attachThread(Attach, instance);
		attachThread.detach();
	}

	if (reason == DLL_PROCESS_DETACH)
	{
		Hooks::Release();
		fclose((FILE*)stdout);
		FreeConsole();

		Sleep(100);
	}

	return true;
}