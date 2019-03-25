#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <windows.h>
#include "keymap.h"
#include "hidapi.h"
#include "ghostapi.h"
#include "packdef.h"
#include "log.h"

#ifdef __cplusplus
} /* extern "C" */
#endif

///--------------------------------------
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))

///--------------------------------------
///--------------------------------------
#define GHOST_MOUSE_X_MAX 32767
#define GHOST_MOUSE_X_MIN -32768
#define GHOST_MOUSE_Y_MAX 32767
#define GHOST_MOUSE_Y_MIN -32768
#define GHOST_MOUSE_R_X_MAX 127
#define GHOST_MOUSE_R_X_MIN -127
#define GHOST_MOUSE_R_Y_MAX 127
#define GHOST_MOUSE_R_Y_MIN -127
///--------------------------------------
#define GHOST_VID 0x2341
#define GHOST_PID 0x8036
///--------------------------------------
static hid_device *g_handle = NULL;
static int g_initialized = 0;
///--------------------------------------
CRITICAL_SECTION g_mutex;

//////////////////////////////////////////////
////////////     设备管理接口      ///////////
//////////////////////////////////////////////
int GHOST_API_EXPORT OpenDeviceEx(int vid, int pid)
{
	EnterCriticalSection(&g_mutex);
	if (!g_initialized)
	{
		//init key map
		keymap_init();
		//init device
		if (hid_init() < 0)
		{
			log_trace("init device failed\n");
			keymap_fini();
			LeaveCriticalSection(&g_mutex);
			return -1;
		}
		log_trace("init device successful\n");
		g_handle = hid_open(vid, pid ,2 ,NULL);
		if (!g_handle) {
			log_trace("open device failed\n");
			keymap_fini();
			hid_exit();
			LeaveCriticalSection(&g_mutex);
			return -2;
		}
		log_trace("open device successful\n");
		g_initialized = 1;
	}
	LeaveCriticalSection(&g_mutex);
	return 0;
}

int GHOST_API_EXPORT OpenDevice()
{
	return OpenDeviceEx(GHOST_VID, GHOST_PID);
}

int GHOST_API_EXPORT OpenDevice2()
{
	EnterCriticalSection(&g_mutex);
	if (!g_initialized)
	{
		//init key map
		keymap_init();
		//init device
		if (hid_init() < 0)
		{
			log_trace("init device failed\n");
			keymap_fini();
			LeaveCriticalSection(&g_mutex);
			return -1;
		}
		log_trace("init device successful\n");
		g_handle = hid_open_serial_no(2, L"GHOST@857WG");
		if (!g_handle) {
			log_trace("open device failed\n");
			keymap_fini();
			hid_exit();
			LeaveCriticalSection(&g_mutex);
			return -2;
		}
		log_trace("open device successful\n");
		g_initialized = 1;
	}
	LeaveCriticalSection(&g_mutex);
	return 0;
}

int HID_API_EXPORT CloseDevice()
{
	EnterCriticalSection(&g_mutex);
	if (g_initialized)
	{
		//fini device
		if (g_handle)
		{
			hid_close(g_handle);
			log_trace("close device successful\n");
		}
		hid_exit();
		log_trace("fini device successful\n");
		//fini key map
		keymap_fini();

		//reset 
		g_handle = NULL;
		g_initialized = 0;
	}
	LeaveCriticalSection(&g_mutex);
	return 0;
}

// 检查设备是否有效
int GHOST_API_EXPORT CheckDevice()
{
	if (g_initialized)
	{
		return 1;
	}
	return 0;
}
// 检查设备是否有效
int GHOST_API_EXPORT Restart()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_FUNC;
	pkg.fc_cmd = MSG_CMD_FUNC_RESTART;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 断开设备连接
int GHOST_API_EXPORT Disconnect(int second)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_FUNC;
	pkg.fc_cmd = MSG_CMD_FUNC_DISCONNECT;
	pkg.fc_value[0] = constrain(second, 0, 255);
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 设置自定义设备ID（厂商ID+产品ID）
int GHOST_API_EXPORT SetDeviceID(int vid, int pid)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_FUNC;
	pkg.fc_cmd = MSG_CMD_FUNC_SET_DEVICE_ID;
	pkg.fc_vidpid[0] = constrain(vid, 0, 65535);
	pkg.fc_vidpid[1] = constrain(pid, 0, 65535);
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 恢复设备默认ID
int GHOST_API_EXPORT RestoreDeviceID()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_FUNC;
	pkg.fc_cmd = MSG_CMD_FUNC_RESTORE_DEVICE_ID;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}


// 获取序列号
GHOST_API_EXPORT char* GetSN()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_INFO;
	pkg.kb_cmd = MSG_CMD_INFO_SN;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return NULL;
	}
	else
	{
		log_trace("sucess to write\n");
	}
	//recv
	static MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_read(g_handle, (unsigned char*)&result, sizeof(result));
		LeaveCriticalSection(&g_mutex);
		if (res == 0)
		{
			log_trace("read for waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			log_trace("failed to read\n");
		}
	}
	if (0 < res)
	{
		log_trace("sucess to read\n");
		return result.if_value;
	}
	return NULL;
}
// 获取设备型号
GHOST_API_EXPORT char* GetModel()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_INFO;
	pkg.kb_cmd = MSG_CMD_INFO_MODEL;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return NULL;
	}
	else
	{
		log_trace("sucess to write\n");
	}
	//recv
	static MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_read(g_handle, (unsigned char*)&result, sizeof(result));
		LeaveCriticalSection(&g_mutex);
		if (res == 0)
		{
			log_trace("read for waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			log_trace("failed to read\n");
		}
	}
	if (0 < res)
	{
		log_trace("sucess to read\n");
		return result.if_value;
	}
	return NULL;
}
// 获取固件版本号
GHOST_API_EXPORT char* GetVer()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_INFO;
	pkg.kb_cmd = MSG_CMD_INFO_VERSION;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return NULL;
	}
	else
	{
		log_trace("sucess to write\n");
	}
	//recv
	static MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_read(g_handle, (unsigned char*)&result, sizeof(result));
		LeaveCriticalSection(&g_mutex);
		if (res == 0)
		{
			log_trace("read for waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			log_trace("failed to read\n");
		}
	}
	if (0 < res)
	{
		log_trace("sucess to read\n");
		return result.if_value;
	}
	return NULL;
}
// 获取生产日期
GHOST_API_EXPORT char* GetProductionDate()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_INFO;
	pkg.kb_cmd = MSG_CMD_INFO_PROD_DATE;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return NULL;
	}
	else
	{
		log_trace("sucess to write\n");
	}
	//recv
	static MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_read(g_handle, (unsigned char*)&result, sizeof(result));
		LeaveCriticalSection(&g_mutex);
		if (res == 0)
		{
			log_trace("read for waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			log_trace("failed to read\n");
		}
	}
	if (0 < res)
	{
		log_trace("sucess to read\n");
		return result.if_value;
	}
	return NULL;
}

//////////////////////////////////////////////
////////////     键盘管理接口      ///////////
//////////////////////////////////////////////
// 键按下
int GHOST_API_EXPORT  KeyDown(char *key)
{
	if (NULL == key || 0 == strlen(key))
	{
		return -1;
	}

	unsigned short keycode = keymap_map(key);
	if (!keycode)
	{
		return -2;
	}
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_DOWN;
	pkg.kb_key[0] = keycode;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}

}
// 键弹起
int GHOST_API_EXPORT  KeyUp(char *key)
{
	if (NULL == key || 0 == strlen(key))
	{
		return -1;
	}
	unsigned short keycode = keymap_map(key);
	if (!keycode)
	{
		return -2;
	}
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_UP;
	pkg.kb_key[0] = keycode;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 一次按键
int GHOST_API_EXPORT  KeyPress(char *key, int count)
{
	//send
	int res = 0;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		res = KeyDown(key);
		if (res >= 0)
		{
			Sleep(constrain((rand() % 81), 50, 80));
			res = KeyUp(key);
			if (res >= 0 && i < count)
			{
				Sleep(constrain((rand() % 601), 150, 600));
			}
		}

	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
int GHOST_API_EXPORT  KeyPress2(char *key, int count)
{
	if (NULL == key || 0 == strlen(key))
	{
		return -1;
	}
	unsigned short keycode = keymap_map(key);
	if (!keycode)
	{
		return -2;
	}
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_PRESS;
	pkg.kb_key[0] = keycode;
	//send
	int res = 0;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}

}
// 组合键按下
int GHOST_API_EXPORT  CombinationKeyDown(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	unsigned count = 0;
	unsigned short keycode = 0;
	char * key[6] = { key1, key2, key3, key4, key5, key6 };
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));

	for (int i = 0; i < 6; i++)
	{
		if (NULL != key[i] && 0 != strlen(key[i]))
		{
			keycode = keymap_map(key[i]);
			if (keycode)
			{
				pkg.kb_key[i] = keycode;
				count++;
			}
		}
	}
	if (0 == count)
	{
		return -1;
	}
	//package
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_COMB_DOWN;
	pkg.kb_count = count;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -2;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 组合键弹起
int GHOST_API_EXPORT  CombinationKeyUp(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	unsigned count = 0;
	unsigned short keycode = 0;
	char * key[6] = { key1, key2, key3, key4, key5, key6 };
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));

	for (int i = 0; i < 6; i++)
	{
		if (NULL != key[i] && 0 != strlen(key[i]))
		{
			keycode = keymap_map(key[i]);
			if (keycode)
			{
				pkg.kb_key[i] = keycode;
				count++;
			}
		}
	}
	if (0 == count)
	{
		return -1;
	}
	//package
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_COMB_UP;
	pkg.kb_count = count;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -2;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 组合按键
int GHOST_API_EXPORT  CombinationKeyPress(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count)
{
	//send
	int res = 0;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		res = CombinationKeyDown(key1, key2, key3, key4, key5, key6);
		if (res >= 0)
		{
			Sleep(constrain((rand() % 81), 50, 80));
			res = CombinationKeyUp(key1, key2, key3, key4, key5, key6);
			if (res >= 0 && i < count)
			{
				Sleep(constrain((rand() % 601), 150, 600));
			}
		}

	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
int GHOST_API_EXPORT  CombinationKeyPress2(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count)
{
	unsigned int cnt = 0;
	unsigned short keycode = 0;
	char * key[6] = { key1, key2, key3, key4, key5, key6 };
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));

	for (int i = 0; i < 6; i++)
	{
		if (NULL != key[i] && 0 != strlen(key[i]))
		{
			keycode = keymap_map(key[i]);
			if (keycode)
			{
				pkg.kb_key[i] = keycode;
				cnt++;
			}
		}
	}
	if (0 == cnt)
	{
		return -1;
	}
	//package
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_COMB_PRESS;
	pkg.kb_count = cnt;
	//send
	int res;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -2;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 释放所有按键
int GHOST_API_EXPORT  KeyUpAll()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_UP_ALL;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 模拟按键输入
int GHOST_API_EXPORT  Say(char *keys)
{
	if (NULL == keys || 0 == strlen(keys))
	{
		return 1;
	}
	int ret = 0;
	int len = strlen(keys);
	char tmp[2] = { 0 };
	for (int i = 0; i < len; i++)
	{
		tmp[0] = keys[i];
		ret |= KeyPress(tmp, 1);
	}
	return ret;
}

// 获取大写灯状态
int GHOST_API_EXPORT  GetCapsLock()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_GET_CAPS_LOCK;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
	}
	//
	MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0) 
	{
		EnterCriticalSection(&g_mutex);
		res = hid_read(g_handle, (unsigned char*)&result, sizeof(result));
		LeaveCriticalSection(&g_mutex);
		if (res == 0)
		{
			log_trace("read for waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			log_trace("failed to read\n");
		}
	}
	if (0 < res)
	{
		return result.kb_ret;
	}
	return 0;
}
// 获取NumLock灯状态
int GHOST_API_EXPORT  GetNumLock()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_GET_NUM_LOCK;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
	}
	//
	MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_read(g_handle, (unsigned char*)&result, sizeof(result));
		LeaveCriticalSection(&g_mutex);
		if (res == 0)
		{
			log_trace("read for waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			log_trace("failed to read\n");
		}
	}
	if (0 < res)
	{
		return result.kb_ret;
	}
	return 0;
}
// 获取大写灯状态
int GHOST_API_EXPORT  SetCapsLock()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_SET_CAPS_LOCK;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 获取NumLock灯状态
int GHOST_API_EXPORT  SetNumLock()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_SET_NUM_LOCK;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}

}
//////////////////////////////////////////////
////////////     鼠标管理接口      ///////////
//////////////////////////////////////////////
// 鼠标左键按下
int GHOST_API_EXPORT  LeftDown()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_LEFT_DOWN;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标左键弹起
int GHOST_API_EXPORT  LeftUp()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_LEFT_UP;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标左键单击
int GHOST_API_EXPORT  LeftClick(int count)
{
	//send
	int res = 0;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		res = LeftDown();
		if (res >= 0)
		{
			Sleep(constrain((rand() % 81), 50, 80));
			res = LeftUp();
			if (res >= 0 && i < count)
			{
				Sleep(constrain((rand() % 501), 500, 900));
			}
		}

	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
int GHOST_API_EXPORT  LeftClick2(int count)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_LEFT_CLICK;
	//send
	int res;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标左键双击
int GHOST_API_EXPORT  LeftDoubleClick(int count)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_LEFT_DCLICK;
	//send
	int res;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
		if (res >= 0 && i < count)
		{
			Sleep(constrain((rand() % 901), 500, 900));
		}
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标右键按下
int GHOST_API_EXPORT  RightDown()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_RIGHT_DOWN;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标右键弹起
int GHOST_API_EXPORT  RightUp()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_RIGHT_UP;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标右键单击
int GHOST_API_EXPORT  RightClick(int count)
{
	//send
	int res = 0;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		res = RightDown();
		if (res >= 0)
		{
			Sleep(constrain((rand() % 81), 50, 80));
			res = RightUp();
			if (res >= 0 && i < count)
			{
				Sleep(constrain((rand() % 501), 500, 900));
			}
		}

	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
int GHOST_API_EXPORT  RightClick2(int count)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_RIGHT_CLICK;
	//send
	int res;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
		if (res >= 0 && i < count)
		{
			Sleep(constrain((rand() % 901), 500, 900));
		}
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}

// 鼠标右键双击
int GHOST_API_EXPORT  RightDoubleClick(int count)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_RIGHT_DCLICK;
	//send
	int res;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
		if (res >= 0 && i < count)
		{
			Sleep(constrain((rand() % 901), 500, 900));
		}
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标中键按下
int GHOST_API_EXPORT  MiddleDown()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_MIDDLE_DOWN;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标中键弹起
int GHOST_API_EXPORT  MiddleUp()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_MIDDLE_UP;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标中键单击
int GHOST_API_EXPORT  MiddleClick(int count)
{
	//send
	int res = 0;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		res = MiddleDown();
		if (res >= 0)
		{
			Sleep(constrain((rand() % 81), 50, 80));
			res = MiddleUp();
			if (res >= 0 && i < count)
			{
				Sleep(constrain((rand() % 501), 500, 900));
			}
		}

	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
int GHOST_API_EXPORT  MiddleClick2(int count)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_MIDDLE_CLICK;
	//send
	int res;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
		if (res >= 0 && i < count)
		{
			Sleep(constrain((rand() % 901), 500, 900));
		}
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标中键双击
int GHOST_API_EXPORT  MiddleDoubleClick(int count)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_MIDDLE_DCLICK;
	//send
	int res;
	int i = 0;
	while (res >= 0 && i++ < count)
	{
		EnterCriticalSection(&g_mutex);
		res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
		LeaveCriticalSection(&g_mutex);
		if (res >= 0 && i < count)
		{
			Sleep(constrain((rand() % 901), 500, 900));
		}
	}
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 释放所有鼠标按键
int GHOST_API_EXPORT  MouseUpAll()
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_UP_ALL;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 模拟鼠标移动
int GHOST_API_EXPORT  MoveTo(int x, int y)
{
	double ex = GetSystemMetrics(SM_CXSCREEN);
	double ey = GetSystemMetrics(SM_CYSCREEN);
	double rx = ((double)GHOST_MOUSE_X_MAX / ex)*x + 1;
	double ry = ((double)GHOST_MOUSE_Y_MAX / ey)*y + 2;
	short ix = constrain((short)rx, 0, GHOST_MOUSE_X_MAX);
	short iy = constrain((short)ry, 0, GHOST_MOUSE_Y_MAX);

	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_MOVE_TO;
	pkg.ms_x = ix;
	pkg.ms_y = iy;
	//pkg.ms_x = x;
	//pkg.ms_y = y;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 相对移动鼠标
int GHOST_API_EXPORT  MoveToR(int x, int y)
{
	short ix = constrain(x, GHOST_MOUSE_R_X_MIN, GHOST_MOUSE_R_X_MAX);
	short iy = constrain(y, GHOST_MOUSE_R_Y_MIN, GHOST_MOUSE_R_Y_MAX);

	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_MOVE_TO_R;
	pkg.ms_x = ix;
	pkg.ms_y = iy;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
// 鼠标滚轮滚动
int GHOST_API_EXPORT  WheelsMove(int y)
{
	short iy = constrain(y, GHOST_MOUSE_R_Y_MIN, GHOST_MOUSE_R_Y_MAX);

	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_MOUSE;
	pkg.ms_cmd = MSG_CMD_MS_WHEEL_MOVE;
	pkg.ms_wheel = iy;
	//send
	int res;
	EnterCriticalSection(&g_mutex);
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	LeaveCriticalSection(&g_mutex);
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return -1;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}


// 设置鼠标移动速度
int GHOST_API_EXPORT  GetMouseMoveSpeed()
{
	int mouseSpeed = -1;
	BOOL bResult = SystemParametersInfo(SPI_GETMOUSESPEED, 0, &mouseSpeed, 0);
	if (bResult)
	{
		log_trace("get mouse move speed successful\n");
		return mouseSpeed;
	}
	log_trace("get mouse move speed failed\n");
	return -1;
}

// 设置鼠标移动速度
int GHOST_API_EXPORT  SetMouseMoveSpeed(int speed)
{
	int mouseSpeed = constrain(speed, 1, 20);
	;
	BOOL bResult = SystemParametersInfo(SPI_SETMOUSESPEED, 0, (VOID*)mouseSpeed, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
	if (bResult)
	{
		log_trace("set mouse move speed successful\n");
		return 0;
	}
	log_trace("set mouse move speed failed\n");
	return -1;
}
// 重置鼠标移动速度
int GHOST_API_EXPORT  ResetMouseMoveSpeed()
{
	//Sets the current mouse speed. The pvParam parameter is an integer between 1 (slowest) and 20 (fastest).
	//A value of 10 is the default. 
	//This value is typically set using the mouse control panel application.
	return SetMouseMoveSpeed(10);
}

// 设置鼠标滚轮速度
int GHOST_API_EXPORT  GetMouseWheelLines()
{
	int mouseSpeed = -1;
	BOOL bResult = SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &mouseSpeed, 0);
	if (bResult)
	{
		log_trace("get mouse wheel lines speed successful\n");
		return mouseSpeed;
	}
	log_trace("get mouse wheel lines speed failed\n");
	return -1;
}

// 设置鼠标滚轮速度
int GHOST_API_EXPORT  SetMouseWheelLines(int speed)
{
	unsigned int mouseSpeed = constrain(speed, 1, 20);
	;
	BOOL bResult = SystemParametersInfo(SPI_SETWHEELSCROLLLINES, 0, (VOID*)mouseSpeed, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
	if (bResult)
	{
		log_trace("set mouse wheel lines speed successful\n");
		return 0;
	}
	log_trace("set mouse wheel lines speed failed\n");
	return -1;
}
// 重置鼠标滚轮速度
int GHOST_API_EXPORT  ResetMouseWheelLines()
{
	//Retrieves the number of lines to scroll when the vertical mouse wheel is moved.
	//The pvParam parameter must point to a UINT variable that receives the number of lines.
	//The default value is 3.
	return SetMouseMoveSpeed(3);
}
// 设置鼠标滚轮速度
int GHOST_API_EXPORT  GetMouseWheelChars()
{
	int mouseSpeed = -1;
	BOOL bResult = SystemParametersInfo(SPI_SETWHEELSCROLLCHARS, 0, &mouseSpeed, 0);
	if (bResult)
	{
		log_trace("get mouse wheel chars speed successful\n");
		return mouseSpeed;
	}
	log_trace("get mouse wheel chars speed failed\n");
	return -1;
}

// 设置鼠标滚轮速度
int GHOST_API_EXPORT  SetMouseWheelChars(int speed)
{
	unsigned int mouseSpeed = constrain(speed, 1, 20);
	;
	BOOL bResult = SystemParametersInfo(SPI_SETWHEELSCROLLCHARS, 0, (VOID*)mouseSpeed, SPIF_SENDCHANGE | SPIF_UPDATEINIFILE);
	if (bResult)
	{
		log_trace("set mouse wheel chars speed successful\n");
		return 0;
	}
	log_trace("set mouse wheel chars speed failed\n");
	return -1;
}
// 重置鼠标滚轮速度
int GHOST_API_EXPORT  ResetMouseWheelChars()
{
	//Retrieves the number of characters to scroll when the horizontal mouse wheel is moved. 
	//The pvParam parameter must point to a UINT variable that receives the number of lines.
	//The default value is 3.
	return SetMouseMoveSpeed(3);
}

// 设置鼠标双击速度
int GHOST_API_EXPORT  GetMouseDoubleClickSpeed()
{
	unsigned int mouseSpeed = -1;
	mouseSpeed = GetDoubleClickTime();
	log_trace("get mouse double click speed failed\n");
	return (int)mouseSpeed;
}

// 设置鼠标双击速度
int GHOST_API_EXPORT  SetMouseDoubleClickSpeed(int speed)
{
	unsigned int mouseSpeed = constrain(speed, 1, 5000);
	BOOL bResult =  SetDoubleClickTime(mouseSpeed);
	if (bResult)
	{
		log_trace("set mouse double click speed successful\n");
		return 0;
	}
	log_trace("set mouse double click speed failed\n");
	return -1;
}
// 重置鼠标双击速度
int GHOST_API_EXPORT  ResetMouseDoubleClickSpeed()
{
	//The number of milliseconds that may occur between the first and second clicks of a double-click.
	//If this parameter is set to 0, the system uses the default double-click time of 500 milliseconds.
	//If this parameter value is greater than 5000 milliseconds,the system sets the value to 5000 milliseconds.
	return SetMouseDoubleClickSpeed(500);
}

// 设置日志级别
int GHOST_API_EXPORT SetLogLevel(int level)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_LOG;
	pkg.lg_level = (unsigned char)constrain(level, 0, 6);;
	//send
	int res;
	res = hid_write(g_handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		log_trace("failed to write,error: %ls\n", hid_error(g_handle));
		return 3;
	}
	else
	{
		log_trace("sucess to write\n");
		return 0;
	}
}
//////////////////////////////////////////////
////////////     存储管理接口      ///////////
//////////////////////////////////////////////	   



//////////////////////////////////////////////
////////////     辅助管理接口      ///////////
//////////////////////////////////////////////


#ifdef __cplusplus
} /* extern "C" */
#endif