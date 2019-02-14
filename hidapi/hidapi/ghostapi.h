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
	////////////     设备管理接口      ///////////
	//////////////////////////////////////////////
	// 打开设备
	int GHOST_API_EXPORT GHOST_API_CALL OpenDevice();
	// 打开设备（根据设备VID和PID）
	int GHOST_API_EXPORT GHOST_API_CALL OpenDeviceEx(int vid, int pid);
	// 关闭设备
	int GHOST_API_EXPORT GHOST_API_CALL CloseDevice();
	// 检查设备是否有效
	int GHOST_API_EXPORT GHOST_API_CALL CheckDevice();
	// 断开设备连接
	int GHOST_API_EXPORT GHOST_API_CALL Disconnect(int second);
	// 设置自定义设备ID（厂商ID+产品ID）
	int GHOST_API_EXPORT GHOST_API_CALL SetDeviceID(int vid, int pid);
	// 恢复设备默认ID
	int GHOST_API_EXPORT GHOST_API_CALL RestoreDeviceID();


	// 获取序列号
	GHOST_API_EXPORT char* GHOST_API_CALL GetSN();
	// 获取设备型号
	GHOST_API_EXPORT char* GHOST_API_CALL GetModel();
	// 获取固件版本号
	GHOST_API_EXPORT char* GHOST_API_CALL GetVersion();
	// 获取生产日期
	GHOST_API_EXPORT char* GHOST_API_CALL GetProductionDate();

	//////////////////////////////////////////////
	////////////     键盘管理接口      ///////////
	//////////////////////////////////////////////
	// 键按下
	int GHOST_API_EXPORT GHOST_API_CALL KeyDown(char *key);
	// 键弹起
	int GHOST_API_EXPORT GHOST_API_CALL KeyUp(char *key);
	// 一次按键
	int GHOST_API_EXPORT GHOST_API_CALL KeyPress(char *key, int count);
	// 组合键按下
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyDown(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6);
	// 组合键弹起
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyUp(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6);
	// 组合按键
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyPress(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count);
	// 释放所有按键
	int GHOST_API_EXPORT GHOST_API_CALL KeyUpAll();
	// 模拟按键输入
	int GHOST_API_EXPORT GHOST_API_CALL Say(char *keys);

	// 获取大写灯状态
	int GHOST_API_EXPORT GHOST_API_CALL GetCapsLock();
	// 获取NumLock灯状态
	int GHOST_API_EXPORT GHOST_API_CALL GetNumLock();

	//////////////////////////////////////////////
	////////////     鼠标管理接口      ///////////
	//////////////////////////////////////////////
	// 鼠标左键按下
	int GHOST_API_EXPORT GHOST_API_CALL LeftDown();
	// 鼠标左键弹起
	int GHOST_API_EXPORT GHOST_API_CALL LeftUp();
	// 鼠标左键单击
	int GHOST_API_EXPORT GHOST_API_CALL LeftClick(int count);
	// 鼠标左键双击
	int GHOST_API_EXPORT GHOST_API_CALL LeftDoubleClick(int count);
	// 鼠标右键按下
	int GHOST_API_EXPORT GHOST_API_CALL RightDown();
	// 鼠标右键弹起
	int GHOST_API_EXPORT GHOST_API_CALL RightUp();
	// 鼠标右键单击
	int GHOST_API_EXPORT GHOST_API_CALL RightClick(int count);
	// 鼠标右键双击
	int GHOST_API_EXPORT GHOST_API_CALL RightDoubleClick(int count);
	// 鼠标中键按下
	int GHOST_API_EXPORT GHOST_API_CALL MiddleDown();
	// 鼠标中键弹起
	int GHOST_API_EXPORT GHOST_API_CALL MiddleUp();
	// 鼠标中键单击
	int GHOST_API_EXPORT GHOST_API_CALL MiddleClick(int count);
	// 鼠标中键双击
	int GHOST_API_EXPORT GHOST_API_CALL MiddleDoubleClick(int count);
	// 释放所有鼠标按键
	int GHOST_API_EXPORT GHOST_API_CALL MouseUpAll();
	// 模拟鼠标移动
	int GHOST_API_EXPORT GHOST_API_CALL MoveTo(int x, int y);
	// 相对移动鼠标(X，Y不能大于255)
	int GHOST_API_EXPORT GHOST_API_CALL MoveToR(int x, int y);
	// 鼠标滚轮滚动
	int GHOST_API_EXPORT GHOST_API_CALL WheelsMove(int y);

	// 从指定位置移动鼠标
	int GHOST_API_EXPORT GHOST_API_CALL MoveToFrom(int fx, int fy, int tx, int ty);
	// 复位移动鼠标
	int GHOST_API_EXPORT GHOST_API_CALL ReMoveTo(int x, int y);
	// 设置鼠标移动速度
	int GHOST_API_EXPORT GHOST_API_CALL SetMoveSpeed(int speed);

	//////////////////////////////////////////////
	////////////     存储管理接口      ///////////
	//////////////////////////////////////////////	   



	//////////////////////////////////////////////
	////////////     辅助管理接口      ///////////
	//////////////////////////////////////////////


#ifdef __cplusplus
}
#endif

#endif

