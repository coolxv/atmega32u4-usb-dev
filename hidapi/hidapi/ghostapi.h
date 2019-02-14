#ifndef GHOSTAPI_H__
#define GHOSTAPI_H__

#ifdef _WIN32
#define GHOST_API_EXPORT __declspec(dllexport)
#define GHOST_API_CALL
#else
#define GHOST_API_EXPORT /**< API export macro */
#define GHOST_API_CALL /**< API call macro */
#endif

#define GHOST_API_EXPORT_CALL GHOST_API_EXPORT GHOST_API_CALL /**< API export and call macro*/

#ifdef __cplusplus
extern "C" {
#endif
	


	//////////////////////////////////////////////
	////////////     �豸����ӿ�      ///////////
	//////////////////////////////////////////////
	// ���豸
	int GHOST_API_EXPORT GHOST_API_CALL OpenDevice();
	// ���豸�������豸VID��PID��
	int GHOST_API_EXPORT GHOST_API_CALL OpenDeviceEx(int vid, int pid);
	// �ر��豸
	int GHOST_API_EXPORT GHOST_API_CALL CloseDevice();
	// ����豸�Ƿ���Ч
	int GHOST_API_EXPORT GHOST_API_CALL CheckDevice();
	// �Ͽ��豸����
	int GHOST_API_EXPORT GHOST_API_CALL Disconnect(int second);
	// �����Զ����豸ID������ID+��ƷID��
	int GHOST_API_EXPORT GHOST_API_CALL SetDeviceID(int vid, int pid);
	// �ָ��豸Ĭ��ID
	int GHOST_API_EXPORT GHOST_API_CALL RestoreDeviceID();


	// ��ȡ���к�
	GHOST_API_EXPORT char* GHOST_API_CALL GetSN();
	// ��ȡ�豸�ͺ�
	GHOST_API_EXPORT char* GHOST_API_CALL GetModel();
	// ��ȡ�̼��汾��
	GHOST_API_EXPORT char* GHOST_API_CALL GetVersion();
	// ��ȡ��������
	GHOST_API_EXPORT char* GHOST_API_CALL GetProductionDate();

	//////////////////////////////////////////////
	////////////     ���̹���ӿ�      ///////////
	//////////////////////////////////////////////
	// ������
	int GHOST_API_EXPORT GHOST_API_CALL KeyDown(char *key);
	// ������
	int GHOST_API_EXPORT GHOST_API_CALL KeyUp(char *key);
	// һ�ΰ���
	int GHOST_API_EXPORT GHOST_API_CALL KeyPress(char *key, int count);
	// ��ϼ�����
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyDown(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6);
	// ��ϼ�����
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyUp(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6);
	// ��ϰ���
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyPress(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count);
	// �ͷ����а���
	int GHOST_API_EXPORT GHOST_API_CALL KeyUpAll();
	// ģ�ⰴ������
	int GHOST_API_EXPORT GHOST_API_CALL Say(char *keys);

	// ��ȡ��д��״̬
	int GHOST_API_EXPORT GHOST_API_CALL GetCapsLock();
	// ��ȡNumLock��״̬
	int GHOST_API_EXPORT GHOST_API_CALL GetNumLock();

	//////////////////////////////////////////////
	////////////     ������ӿ�      ///////////
	//////////////////////////////////////////////
	// ����������
	int GHOST_API_EXPORT GHOST_API_CALL LeftDown();
	// ����������
	int GHOST_API_EXPORT GHOST_API_CALL LeftUp();
	// ����������
	int GHOST_API_EXPORT GHOST_API_CALL LeftClick(int count);
	// ������˫��
	int GHOST_API_EXPORT GHOST_API_CALL LeftDoubleClick(int count);
	// ����Ҽ�����
	int GHOST_API_EXPORT GHOST_API_CALL RightDown();
	// ����Ҽ�����
	int GHOST_API_EXPORT GHOST_API_CALL RightUp();
	// ����Ҽ�����
	int GHOST_API_EXPORT GHOST_API_CALL RightClick(int count);
	// ����Ҽ�˫��
	int GHOST_API_EXPORT GHOST_API_CALL RightDoubleClick(int count);
	// ����м�����
	int GHOST_API_EXPORT GHOST_API_CALL MiddleDown();
	// ����м�����
	int GHOST_API_EXPORT GHOST_API_CALL MiddleUp();
	// ����м�����
	int GHOST_API_EXPORT GHOST_API_CALL MiddleClick(int count);
	// ����м�˫��
	int GHOST_API_EXPORT GHOST_API_CALL MiddleDoubleClick(int count);
	// �ͷ�������갴��
	int GHOST_API_EXPORT GHOST_API_CALL MouseUpAll();
	// ģ������ƶ�
	int GHOST_API_EXPORT GHOST_API_CALL MoveTo(int x, int y);
	// ����ƶ����(X��Y���ܴ���255)
	int GHOST_API_EXPORT GHOST_API_CALL MoveToR(int x, int y);
	// �����ֹ���
	int GHOST_API_EXPORT GHOST_API_CALL WheelsMove(int y);

	// ��ָ��λ���ƶ����
	int GHOST_API_EXPORT GHOST_API_CALL MoveToFrom(int fx, int fy, int tx, int ty);
	// ��λ�ƶ����
	int GHOST_API_EXPORT GHOST_API_CALL ReMoveTo(int x, int y);
	// ��������ƶ��ٶ�
	int GHOST_API_EXPORT GHOST_API_CALL SetMoveSpeed(int speed);

	//////////////////////////////////////////////
	////////////     �洢����ӿ�      ///////////
	//////////////////////////////////////////////	   



	//////////////////////////////////////////////
	////////////     ��������ӿ�      ///////////
	//////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif

