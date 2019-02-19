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
////////////     �豸����ӿ�      ///////////
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
// ����豸�Ƿ���Ч
int GHOST_API_EXPORT  CheckDevice()
{
	return 0;
}
// �Ͽ��豸����
int GHOST_API_EXPORT  Disconnect(int second)
{
	return 0;
}
// �����Զ����豸ID������ID+��ƷID��
int GHOST_API_EXPORT  SetDeviceID(int vid, int pid)
{
	return 0;
}
// �ָ��豸Ĭ��ID
int GHOST_API_EXPORT  RestoreDeviceID()
{
	return 0;
}


// ��ȡ���к�
GHOST_API_EXPORT char* GetSN()
{
	return NULL;
}
// ��ȡ�豸�ͺ�
GHOST_API_EXPORT char* GetModel()
{
	return NULL;
}
// ��ȡ�̼��汾��
GHOST_API_EXPORT char* GetVer()
{
	return NULL;
}
// ��ȡ��������
GHOST_API_EXPORT char* GetProductionDate()
{
	return NULL;
}

//////////////////////////////////////////////
////////////     ���̹���ӿ�      ///////////
//////////////////////////////////////////////
// ������
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
// ������
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
// һ�ΰ���
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
// ��ϼ�����
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
// ��ϼ�����
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
// ��ϰ���
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
// �ͷ����а���
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
// ģ�ⰴ������
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

// ��ȡ��д��״̬
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
// ��ȡNumLock��״̬
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
// ��ȡ��д��״̬
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
// ��ȡNumLock��״̬
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
////////////     ������ӿ�      ///////////
//////////////////////////////////////////////
// ����������
int GHOST_API_EXPORT  LeftDown()
{
	return 0;
}
// ����������
int GHOST_API_EXPORT  LeftUp()
{
	return 0;
}
// ����������
int GHOST_API_EXPORT  LeftClick(int count)
{
	return 0;
}
// ������˫��
int GHOST_API_EXPORT  LeftDoubleClick(int count)
{
	return 0;
}
// ����Ҽ�����
int GHOST_API_EXPORT  RightDown()
{
	return 0;
}
// ����Ҽ�����
int GHOST_API_EXPORT  RightUp()
{
	return 0;
}
// ����Ҽ�����
int GHOST_API_EXPORT  RightClick(int count)
{
	return 0;
}
// ����Ҽ�˫��
int GHOST_API_EXPORT  RightDoubleClick(int count)
{
	return 0;
}
// ����м�����
int GHOST_API_EXPORT  MiddleDown()
{
	return 0;
}
// ����м�����
int GHOST_API_EXPORT  MiddleUp()
{
	return 0;
}
// ����м�����
int GHOST_API_EXPORT  MiddleClick(int count)
{
	return 0;
}
// ����м�˫��
int GHOST_API_EXPORT  MiddleDoubleClick(int count)
{
	return 0;
}
// �ͷ�������갴��
int GHOST_API_EXPORT  MouseUpAll()
{
	return 0;
}
// ģ������ƶ�
int GHOST_API_EXPORT  MoveTo(int x, int y)
{
	return 0;
}
// ����ƶ����(X��Y���ܴ���255)
int GHOST_API_EXPORT  MoveToR(int x, int y)
{
	return 0;
}
// �����ֹ���
int GHOST_API_EXPORT  WheelsMove(int y)
{
	return 0;
}

// ��ָ��λ���ƶ����
int GHOST_API_EXPORT  MoveToFrom(int fx, int fy, int tx, int ty)
{
	return 0;
}
// ��λ�ƶ����
int GHOST_API_EXPORT  ReMoveTo(int x, int y)
{
	return 0;
}
// ��������ƶ��ٶ�
int GHOST_API_EXPORT  SetMoveSpeed(int speed)
{
	return 0;
}
// ������־����
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
////////////     �洢����ӿ�      ///////////
//////////////////////////////////////////////	   



//////////////////////////////////////////////
////////////     ��������ӿ�      ///////////
//////////////////////////////////////////////


#ifdef __cplusplus
} /* extern "C" */
#endif