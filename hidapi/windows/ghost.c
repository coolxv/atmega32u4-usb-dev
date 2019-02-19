#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "keymap.h"
#include "hidapi.h"
#include "ghostapi.h"
#include "packdef.h"

#ifdef __cplusplus
} /* extern "C" */
#endif


#define GHOST_VID 0x16C0
#define GHOST_PID 0x0480

static hid_device *handle = NULL;
static int initialized = 0;



//////////////////////////////////////////////
////////////     设备管理接口      ///////////
//////////////////////////////////////////////
int GHOST_API_EXPORT OpenDevice()
{
	if (!initialized)
	{
		keymap_init();

		if (hid_init() < 0)
		{
			printf("unable to init device\n");
			CloseDevice();
			return -1;
		}
		handle = hid_open(GHOST_VID, GHOST_PID, NULL);
		if (!handle) {
			printf("unable to open device\n");
			CloseDevice();
			return -1;
		}
		initialized = 1;
	}
	return 0;
}

int HID_API_EXPORT CloseDevice()
{
	if (initialized)
	{
		if (handle)
		{
			hid_close(handle);
		}
		hid_exit();
		keymap_fini();
		//reset 
		handle = NULL;
		initialized = 0;
	}
	return 0;
}



int GHOST_API_EXPORT OpenDeviceEx(int vid, int pid)
{
	if (!initialized)
	{
		if (hid_init() < 0)
		{
			printf("unable to init device\n");
			CloseDevice();
			return -1;
		}
		handle = hid_open(vid, pid, NULL);
		if (!handle) {
			printf("unable to open device\n");
			CloseDevice();
			return -1;
		}
		initialized = 1;
	}
	return 0;
}
// 检查设备是否有效
int GHOST_API_EXPORT  CheckDevice()
{
	return 0;
}
// 断开设备连接
int GHOST_API_EXPORT  Disconnect(int second)
{
	return 0;
}
// 设置自定义设备ID（厂商ID+产品ID）
int GHOST_API_EXPORT  SetDeviceID(int vid, int pid)
{
	return 0;
}
// 恢复设备默认ID
int GHOST_API_EXPORT  RestoreDeviceID()
{
	return 0;
}


// 获取序列号
GHOST_API_EXPORT char* GetSN()
{
	return NULL;
}
// 获取设备型号
GHOST_API_EXPORT char* GetModel()
{
	return NULL;
}
// 获取固件版本号
GHOST_API_EXPORT char* GetVer()
{
	return NULL;
}
// 获取生产日期
GHOST_API_EXPORT char* GetProductionDate()
{
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
		return 1;
	}

	unsigned char keycode = keymap_map(key);
	if (!keycode)
	{
		return 2;
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
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
		return 0;
	}

}
// 键弹起
int GHOST_API_EXPORT  KeyUp(char *key)
{
	if (NULL == key || 0 == strlen(key))
	{
		return 1;
	}
	unsigned char keycode = keymap_map(key);
	if (!keycode)
	{
		return 2;
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
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
		return 0;
	}
}
// 一次按键
int GHOST_API_EXPORT  KeyPress(char *key, int count)
{
	if (NULL == key || 0 == strlen(key))
	{
		return 1;
	}
	unsigned char keycode = keymap_map(key);
	if (!keycode)
	{
		return 2;
	}
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_PRESS;
	pkg.kb_key[0] = keycode;
	//send
	int res;
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
		return 0;
	}
}
// 组合键按下
int GHOST_API_EXPORT  CombinationKeyDown(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	unsigned count = 0;
	unsigned char keycode = 0;
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
				pkg.kb_key[0] = keycode;
				count++;
			}
		}
	}
	if (0 == count)
	{
		return 1;
	}
	//package
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_COMB_DOWN;
	pkg.kb_count = count;
	//send
	int res;
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 2;
	}
	else
	{
		printf("sucess to write()\n");
		return 0;
	}
	return 0;
}
// 组合键弹起
int GHOST_API_EXPORT  CombinationKeyUp(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	unsigned count = 0;
	unsigned char keycode = 0;
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
				pkg.kb_key[0] = keycode;
				count++;
			}
		}
	}
	if (0 == count)
	{
		return 1;
	}
	//package
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_COMB_UP;
	pkg.kb_count = count;
	//send
	int res;
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 2;
	}
	else
	{
		printf("sucess to write()\n");
		return 0;
	}
	return 0;
}
// 组合按键
int GHOST_API_EXPORT  CombinationKeyPress(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count)
{
	unsigned int cnt = 0;
	unsigned char keycode = 0;
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
				pkg.kb_key[0] = keycode;
				cnt++;
			}
		}
	}
	if (0 == cnt)
	{
		return 1;
	}
	//package
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_KEYBOARD;
	pkg.kb_cmd = MSG_CMD_KB_COMB_PRESS;
	pkg.kb_count = cnt;
	//send
	int res;
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 2;
	}
	else
	{
		printf("sucess to write()\n");
		return 0;
	}
	return 0;
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
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
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
	for (int i = 0; i < len; i++)
	{
		ret |= KeyPress(&keys[i], 1);
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
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
	}
	//
	MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0) 
	{
		res = hid_read(handle, (unsigned char*)&result, sizeof(result));
		if (res == 0)
		{
			printf("waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			printf("Unable to read()\n");
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
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
	}
	//
	MSG_DATA_RESULT_T result;
	res = 0;
	while (res == 0)
	{
		res = hid_read(handle, (unsigned char*)&result, sizeof(result));
		if (res == 0)
		{
			printf("waiting...\n");
			Sleep(500);
		}

		if (res < 0)
		{
			printf("Unable to read()\n");
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
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
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
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
		return 0;
	}

}
//////////////////////////////////////////////
////////////     鼠标管理接口      ///////////
//////////////////////////////////////////////
// 鼠标左键按下
int GHOST_API_EXPORT  LeftDown()
{
	return 0;
}
// 鼠标左键弹起
int GHOST_API_EXPORT  LeftUp()
{
	return 0;
}
// 鼠标左键单击
int GHOST_API_EXPORT  LeftClick(int count)
{
	return 0;
}
// 鼠标左键双击
int GHOST_API_EXPORT  LeftDoubleClick(int count)
{
	return 0;
}
// 鼠标右键按下
int GHOST_API_EXPORT  RightDown()
{
	return 0;
}
// 鼠标右键弹起
int GHOST_API_EXPORT  RightUp()
{
	return 0;
}
// 鼠标右键单击
int GHOST_API_EXPORT  RightClick(int count)
{
	return 0;
}
// 鼠标右键双击
int GHOST_API_EXPORT  RightDoubleClick(int count)
{
	return 0;
}
// 鼠标中键按下
int GHOST_API_EXPORT  MiddleDown()
{
	return 0;
}
// 鼠标中键弹起
int GHOST_API_EXPORT  MiddleUp()
{
	return 0;
}
// 鼠标中键单击
int GHOST_API_EXPORT  MiddleClick(int count)
{
	return 0;
}
// 鼠标中键双击
int GHOST_API_EXPORT  MiddleDoubleClick(int count)
{
	return 0;
}
// 释放所有鼠标按键
int GHOST_API_EXPORT  MouseUpAll()
{
	return 0;
}
// 模拟鼠标移动
int GHOST_API_EXPORT  MoveTo(int x, int y)
{
	return 0;
}
// 相对移动鼠标(X，Y不能大于255)
int GHOST_API_EXPORT  MoveToR(int x, int y)
{
	return 0;
}
// 鼠标滚轮滚动
int GHOST_API_EXPORT  WheelsMove(int y)
{
	return 0;
}

// 从指定位置移动鼠标
int GHOST_API_EXPORT  MoveToFrom(int fx, int fy, int tx, int ty)
{
	return 0;
}
// 复位移动鼠标
int GHOST_API_EXPORT  ReMoveTo(int x, int y)
{
	return 0;
}
// 设置鼠标移动速度
int GHOST_API_EXPORT  SetMoveSpeed(int speed)
{
	return 0;
}
// 设置日志级别
int GHOST_API_EXPORT SetLogLevel(int level)
{
	//package
	MSG_DATA_T pkg;
	memset(&pkg, 0, sizeof(pkg));
	pkg.type[0] = 0x1;
	pkg.type[1] = MSG_TYPE_LOG;
	pkg.lg_level = (unsigned char)level;
	//send
	int res;
	res = hid_write(handle, (unsigned char*)&pkg, sizeof(pkg));
	if (res < 0)
	{
		printf("Unable to write()\n");
		printf("Error: %ls\n", hid_error(handle));
		return 3;
	}
	else
	{
		printf("sucess to write()\n");
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