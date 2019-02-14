#include <stdio.h>
#include <stdlib.h>

#include "keymap.h"
#include "hidapi.h"
#include "ghostapi.h"

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
GHOST_API_EXPORT char* GetVersion()
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
	return 0;
}
// 键弹起
int GHOST_API_EXPORT  KeyUp(char *key)
{
	return 0;
}
// 一次按键
int GHOST_API_EXPORT  KeyPress(char *key, int count)
{
	return 0;
}
// 组合键按下
int GHOST_API_EXPORT  CombinationKeyDown(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	return 0;
}
// 组合键弹起
int GHOST_API_EXPORT  CombinationKeyUp(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	return 0;
}
// 组合按键
int GHOST_API_EXPORT  CombinationKeyPress(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count)
{
	return 0;
}
// 释放所有按键
int GHOST_API_EXPORT  KeyUpAll()
{
	return 0;
}
// 模拟按键输入
int GHOST_API_EXPORT  Say(char *keys)
{
	return 0;
}

// 获取大写灯状态
int GHOST_API_EXPORT  GetCapsLock()
{
	return 0;
}
// 获取NumLock灯状态
int GHOST_API_EXPORT  GetNumLock()
{
	return 0;
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

//////////////////////////////////////////////
////////////     存储管理接口      ///////////
//////////////////////////////////////////////	   



//////////////////////////////////////////////
////////////     辅助管理接口      ///////////
//////////////////////////////////////////////


#ifdef __cplusplus
} /* extern "C" */
#endif