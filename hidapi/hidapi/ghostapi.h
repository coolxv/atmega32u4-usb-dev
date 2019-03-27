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
	
	//返回值说明：
	//0一般成功，非0失败
	//如果是真假，0是假，1是真

	//////////////////////////////////////////////
	////////////     设备管理接口      ///////////
	//////////////////////////////////////////////
	// 打开设备（根据设备VID和PID）
	int GHOST_API_EXPORT GHOST_API_CALL OpenDeviceEx(int vid, int pid);
	int GHOST_API_EXPORT GHOST_API_CALL OpenDevice();//基于默认vid、pid
	// 打开设备（根据设备serial number）
	int GHOST_API_EXPORT GHOST_API_CALL OpenDeviceBySerialEx(const char *serial);
	int GHOST_API_EXPORT GHOST_API_CALL OpenDeviceBySerial();//基于默认serial number
	// 关闭设备
	int GHOST_API_EXPORT GHOST_API_CALL CloseDevice();
	// 检查设备是否有效
	int GHOST_API_EXPORT GHOST_API_CALL CheckDevice();
	// 检查设备是否有效
	int GHOST_API_EXPORT GHOST_API_CALL Restart();
	// 断开设备连接
	int GHOST_API_EXPORT GHOST_API_CALL Disconnect(int second);


	// 设置自定义设备ID（厂商ID+产品ID）
	int GHOST_API_EXPORT GHOST_API_CALL SetDeviceID(int vid, int pid);
	// 恢复设备默认ID
	int GHOST_API_EXPORT GHOST_API_CALL RestoreDeviceID();

	// 设置自定义设备serial number
	int GHOST_API_EXPORT GHOST_API_CALL SetSN(const char *serial);
	// 恢复设备默认serial number
	int GHOST_API_EXPORT GHOST_API_CALL RestoreSN();
	// 获取序列号
	GHOST_API_EXPORT char* GHOST_API_CALL GetSN();
	// 获取设备型号
	GHOST_API_EXPORT char* GHOST_API_CALL GetModel();
	// 获取固件版本号
	GHOST_API_EXPORT char* GHOST_API_CALL GetVer();
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
	int GHOST_API_EXPORT GHOST_API_CALL KeyPress2(char *key, int count);
	// 组合键按下
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyDown(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6);
	// 组合键弹起
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyUp(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6);
	// 组合按键
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyPress(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count);
	int GHOST_API_EXPORT GHOST_API_CALL CombinationKeyPress2(char *key1, char *key2, char *key3, char *key4, char *key5, char *key6, int count);
	// 释放所有按键
	int GHOST_API_EXPORT GHOST_API_CALL KeyUpAll();
	// 模拟按键输入
	int GHOST_API_EXPORT GHOST_API_CALL Say(char *keys);

	// 获取大写灯状态
	int GHOST_API_EXPORT GHOST_API_CALL GetCapsLock();
	// 设置大写灯状态
	int GHOST_API_EXPORT GHOST_API_CALL SetCapsLock();
	// 获取NumLock灯状态
	int GHOST_API_EXPORT GHOST_API_CALL GetNumLock();
	// 设置NumLock灯状态
	int GHOST_API_EXPORT GHOST_API_CALL SetNumLock();
	// 获取ScrollLock灯状态
	int GHOST_API_EXPORT GHOST_API_CALL GetScrollLock();
	// 设置ScrollLock灯状态
	int GHOST_API_EXPORT GHOST_API_CALL SetScrollLock();
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
	// 绝对移动鼠标
	int GHOST_API_EXPORT GHOST_API_CALL MoveToA(int x, int y);
	// 相对移动鼠标
	int GHOST_API_EXPORT GHOST_API_CALL MoveToR(int x, int y);
	// 鼠标滚轮滚动
	int GHOST_API_EXPORT GHOST_API_CALL WheelsMove(int y);

	// 获取鼠标移动速度
	int GHOST_API_EXPORT GHOST_API_CALL GetMouseMoveSpeed();
	// 设置鼠标移动速度
	int GHOST_API_EXPORT GHOST_API_CALL SetMouseMoveSpeed(int speed);
	// 重置鼠标移动速度,默认10
	int GHOST_API_EXPORT GHOST_API_CALL ResetMouseMoveSpeed();

	// 获取鼠标滚轮速度-行数
	int GHOST_API_EXPORT GHOST_API_CALL GetMouseWheelLines();
	// 设置鼠标滚轮速度-行数
	int GHOST_API_EXPORT GHOST_API_CALL SetMouseWheelLines(int speed);
	// 重置鼠标滚轮速度-行数,默认3
	int GHOST_API_EXPORT GHOST_API_CALL ResetMouseWheelLines();

	// 获取鼠标滚轮速度-字符
	int GHOST_API_EXPORT GHOST_API_CALL GetMouseWheelChars();
	// 设置鼠标滚轮速度-字符
	int GHOST_API_EXPORT GHOST_API_CALL SetMouseWheelChars(int speed);
	// 重置鼠标滚轮速度-字符,默认3
	int GHOST_API_EXPORT GHOST_API_CALL ResetMouseWheelChars();

	// 获取鼠标双击速度-毫秒
	int GHOST_API_EXPORT GHOST_API_CALL GetMouseDoubleClickSpeed();
	// 设置鼠标双击速度-毫秒
	int GHOST_API_EXPORT GHOST_API_CALL SetMouseDoubleClickSpeed(int speed);
	// 重置鼠标双击速度-毫秒,默认500
	int GHOST_API_EXPORT GHOST_API_CALL ResetMouseDoubleClickSpeed();
	//////////////////////////////////////////////
	////////////     存储管理接口      ///////////
	//////////////////////////////////////////////	   



	//////////////////////////////////////////////
	////////////     辅助管理接口      ///////////
	//////////////////////////////////////////////
	// 设置设备日志级别:0=关闭，1-6级别
	// LOG_LEVEL_SILENT  0
	// LOG_LEVEL_FATAL   1
	// LOG_LEVEL_ERROR   2
	// LOG_LEVEL_WARNING 3
	// LOG_LEVEL_NOTICE  4
	// LOG_LEVEL_TRACE   5
	// LOG_LEVEL_VERBOSE 6
	int GHOST_API_EXPORT GHOST_API_CALL SetDevLogLevel(int level);
	// 设置设备日志级别:0=关闭，1-6级别
	//LOG_SILENT  0
	//LOG_FATAL	  1
	//LOG_WARN    2
	//LOG_WARN    3
	//LOG_INFO    4
	//LOG_DEBUG   5 
	//LOG_TRACE   6 
	int GHOST_API_EXPORT GHOST_API_CALL SetHostLogLevel(int level);
	int GHOST_API_EXPORT GHOST_API_CALL SetHostLogFile(char *file);
#ifdef __cplusplus
}
#endif

#endif

