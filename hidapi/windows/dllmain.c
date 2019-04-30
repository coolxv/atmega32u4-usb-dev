#include <windows.h>
#include "keymap.h"
#include "hidapi.h"


extern CRITICAL_SECTION g_mutex;

BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	BOOL ret = TRUE;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.

		InitializeCriticalSection(&g_mutex);
		EnterCriticalSection(&g_mutex);
		//init key map
		keymap_init();
		//init device
		if (hid_init() < 0)
		{
			keymap_fini();
			ret = FALSE;
		}
		LeaveCriticalSection(&g_mutex);
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		InitializeCriticalSection(&g_mutex);
		hid_exit();
		keymap_fini();
		LeaveCriticalSection(&g_mutex);
		DeleteCriticalSection(&g_mutex);
		break;
	}
	return ret;  // Successful DLL_PROCESS_ATTACH.
}