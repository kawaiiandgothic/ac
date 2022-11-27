#pragma once
#include <Windows.h>

namespace Hooks
{
	namespace SwapBuffers {
		using fn = BOOL(__stdcall*)(HDC hdc);
		BOOL __stdcall Hook(HDC hdc);
	};

	void Initialize();
	void Release();
}