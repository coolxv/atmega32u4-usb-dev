#include <locale.h>
#include <windows.h>

extern CRITICAL_SECTION g_mutex;


BOOL WINAPI DllMain(
	HINSTANCE hinstDLL,  // handle to DLL module
	DWORD fdwReason,     // reason for calling function
	LPVOID lpReserved)  // reserved
{
	// Perform actions based on the reason for calling.
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		// Initialize once for each new process.
		// Return FALSE to fail DLL load.
		//setlocale(LC_ALL, "");//������� write ����ʧ�ܣ���֪ɶԭ�򣬡�����ϵͳ�ϵ��豸û�з������á�

		InitializeCriticalSection(&g_mutex);
		break;

	case DLL_THREAD_ATTACH:
		// Do thread-specific initialization.
		break;

	case DLL_THREAD_DETACH:
		// Do thread-specific cleanup.
		break;

	case DLL_PROCESS_DETACH:
		// Perform any necessary cleanup.
		DeleteCriticalSection(&g_mutex);
		break;
	}
	return TRUE;  // Successful DLL_PROCESS_ATTACH.
}