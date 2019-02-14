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
GHOST_API_EXPORT char* GetVersion()
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
	return 0;
}
// ������
int GHOST_API_EXPORT  KeyUp(char *key)
{
	return 0;
}
// һ�ΰ���
int GHOST_API_EXPORT  KeyPress(char *key, int count)
{
	return 0;
}
// ��ϼ�����
int GHOST_API_EXPORT  CombinationKeyDown(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	return 0;
}
// ��ϼ�����
int GHOST_API_EXPORT  CombinationKeyUp(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6)
{
	return 0;
}
// ��ϰ���
int GHOST_API_EXPORT  CombinationKeyPress(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count)
{
	return 0;
}
// �ͷ����а���
int GHOST_API_EXPORT  KeyUpAll()
{
	return 0;
}
// ģ�ⰴ������
int GHOST_API_EXPORT  Say(char *keys)
{
	return 0;
}

// ��ȡ��д��״̬
int GHOST_API_EXPORT  GetCapsLock()
{
	return 0;
}
// ��ȡNumLock��״̬
int GHOST_API_EXPORT  GetNumLock()
{
	return 0;
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

//////////////////////////////////////////////
////////////     �洢����ӿ�      ///////////
//////////////////////////////////////////////	   



//////////////////////////////////////////////
////////////     ��������ӿ�      ///////////
//////////////////////////////////////////////


#ifdef __cplusplus
} /* extern "C" */
#endif