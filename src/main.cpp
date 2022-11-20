#include <thread>
#include <Windows.h>

void attach(HINSTANCE instance) {
	Beep(100, 100);

	while (!GetAsyncKeyState(VK_RSHIFT))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibrary(instance);
}

void detach() {
	Beep(500, 100);

	Sleep(100);
}

bool __stdcall DllMain(HINSTANCE instance, uint32_t reason, void* _) {
	if (reason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(instance);

		std::thread attach_thread(attach, instance);
		attach_thread.detach();
	}

	// Only called if using load library to inject
	else detach();

	return true;
}