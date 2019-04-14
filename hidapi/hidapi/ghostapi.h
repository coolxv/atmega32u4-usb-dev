﻿#ifndef GHOSTAPI_H__
#define GHOSTAPI_H__

#ifdef _WIN32
#define GHOST_API_EXPORT __declspec(dllexport)
#define GHOST_API_CALL
#else
#define GHOST_API_EXPORT /**< API export macro */
#define GHOST_API_CALL /**< API call macro */
#endif

#ifdef __cplusplus
extern "C" {
#endif
	
	//返回值说明：
	//0一般成功，非0失败
	//如果是真假，0是假，非0是真

	//////////////////////////////////////////////
	////////////     设备管理接口      ///////////
	//////////////////////////////////////////////
	// 打开设备（根据设备VID和PID）
	GHOST_API_EXPORT int GHOST_API_CALL OpenDeviceEx(int vid, int pid);
	GHOST_API_EXPORT int GHOST_API_CALL OpenDevice();//基于默认vid、pid
	// 打开设备（根据设备serial number）
	GHOST_API_EXPORT int GHOST_API_CALL OpenDeviceBySerialEx(const char *serial);
	GHOST_API_EXPORT int GHOST_API_CALL OpenDeviceBySerial();//基于默认serial number
	// 关闭设备
	GHOST_API_EXPORT int GHOST_API_CALL CloseDevice();
	// 检查设备是否有效
	GHOST_API_EXPORT int GHOST_API_CALL CheckDevice();
	// 检查设备是否有效
	GHOST_API_EXPORT int GHOST_API_CALL Restart();
	// 断开设备连接
	GHOST_API_EXPORT int GHOST_API_CALL Disconnect(int second);


	// 设置自定义设备ID（厂商ID+产品ID）
	GHOST_API_EXPORT int GHOST_API_CALL SetDeviceID(int vid, int pid);
	// 恢复设备默认ID
	GHOST_API_EXPORT int GHOST_API_CALL RestoreDeviceID();
	// 获取ID 高两位pid，低两位vid
	GHOST_API_EXPORT int GHOST_API_CALL GetDeviceID();

	// 设置自定义设备序列号
	GHOST_API_EXPORT int GHOST_API_CALL SetSN(const char *serial);
	// 恢复设备默认序列号
	GHOST_API_EXPORT int GHOST_API_CALL RestoreSN();
	// 获取序列号
	GHOST_API_EXPORT char* GHOST_API_CALL GetSN();


	// 设置自定义设备产品名
	GHOST_API_EXPORT int GHOST_API_CALL SetProduct(const char *product);
	// 恢复设备默认产品名
	GHOST_API_EXPORT int GHOST_API_CALL RestoretProduct();
	// 获取产品名
	GHOST_API_EXPORT char* GHOST_API_CALL GetProduct();

	// 设置自定义设备生产商
	GHOST_API_EXPORT int GHOST_API_CALL SetManufacturer(const char *manufacturer);
	// 恢复设备默认生产商
	GHOST_API_EXPORT int GHOST_API_CALL RestoretManufacturer();
	// 获取生产商
	GHOST_API_EXPORT char* GHOST_API_CALL GetManufacturer();

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
	GHOST_API_EXPORT int GHOST_API_CALL KeyDown(const char *key);
	// 键弹起
	GHOST_API_EXPORT int GHOST_API_CALL KeyUp(const char *key);
	// 一次按键
	GHOST_API_EXPORT int GHOST_API_CALL KeyPress(const char *key, int count);
	GHOST_API_EXPORT int GHOST_API_CALL KeyPress2(const char *key, int count);
	// 组合键按下
	GHOST_API_EXPORT int GHOST_API_CALL CombinationKeyDown(const char *key1, const char *key2, const char *key3, const char *key4, const char *key5, const char *key6);
	// 组合键弹起
	GHOST_API_EXPORT int GHOST_API_CALL CombinationKeyUp(const char *key1, const char *key2, const char *key3, const char *key4, const char *key5, const char *key6);
	// 组合按键
	GHOST_API_EXPORT int GHOST_API_CALL CombinationKeyPress(const char *key1, const char *key2, const char *key3, const char *key4, const char *key5, const char *key6, int count);
	GHOST_API_EXPORT int GHOST_API_CALL CombinationKeyPress2(const char *key1, const char *key2, const char *key3, const char *key4, const char *key5, const char *key6, int count);
	// 释放所有按键
	GHOST_API_EXPORT int GHOST_API_CALL KeyUpAll();
	// 模拟按键输入
	GHOST_API_EXPORT int GHOST_API_CALL Say(const char *keys);

	// 获取大写灯状态
	GHOST_API_EXPORT int GHOST_API_CALL GetCapsLock();
	// 设置大写灯状态
	GHOST_API_EXPORT int GHOST_API_CALL SetCapsLock();
	// 获取NumLock灯状态
	GHOST_API_EXPORT int GHOST_API_CALL GetNumLock();
	// 设置NumLock灯状态
	GHOST_API_EXPORT int GHOST_API_CALL SetNumLock();
	// 获取ScrollLock灯状态
	GHOST_API_EXPORT int GHOST_API_CALL GetScrollLock();
	// 设置ScrollLock灯状态
	GHOST_API_EXPORT int GHOST_API_CALL SetScrollLock();

	//////////////////////////////////////////////
	////////////     鼠标管理接口      ///////////
	//////////////////////////////////////////////
	// 鼠标左键按下
	GHOST_API_EXPORT int GHOST_API_CALL LeftDown();
	// 鼠标左键弹起
	GHOST_API_EXPORT int GHOST_API_CALL LeftUp();
	// 鼠标左键单击
	GHOST_API_EXPORT int GHOST_API_CALL LeftClick(int count);
	// 鼠标左键双击
	GHOST_API_EXPORT int GHOST_API_CALL LeftDoubleClick(int count);
	// 鼠标右键按下
	GHOST_API_EXPORT int GHOST_API_CALL RightDown();
	// 鼠标右键弹起
	GHOST_API_EXPORT int GHOST_API_CALL RightUp();
	// 鼠标右键单击
	GHOST_API_EXPORT int GHOST_API_CALL RightClick(int count);
	// 鼠标右键双击
	GHOST_API_EXPORT int GHOST_API_CALL RightDoubleClick(int count);
	// 鼠标中键按下
	GHOST_API_EXPORT int GHOST_API_CALL MiddleDown();
	// 鼠标中键弹起
	GHOST_API_EXPORT int GHOST_API_CALL MiddleUp();
	// 鼠标中键单击
	GHOST_API_EXPORT int GHOST_API_CALL MiddleClick(int count);
	// 鼠标中键双击
	GHOST_API_EXPORT int GHOST_API_CALL MiddleDoubleClick(int count);
	// 释放所有鼠标按键
	GHOST_API_EXPORT int GHOST_API_CALL MouseUpAll();
	// 模拟鼠标移动
	GHOST_API_EXPORT int GHOST_API_CALL MoveTo(int x, int y);
	// 绝对移动鼠标
	GHOST_API_EXPORT int GHOST_API_CALL MoveToA(int x, int y);
	// 相对移动鼠标
	GHOST_API_EXPORT int GHOST_API_CALL MoveToR(int x, int y);
	// 鼠标滚轮滚动
	GHOST_API_EXPORT int GHOST_API_CALL WheelsMove(int y);

	// 获取鼠标移动速度
	GHOST_API_EXPORT int GHOST_API_CALL GetMouseMoveSpeed();
	// 设置鼠标移动速度
	GHOST_API_EXPORT int GHOST_API_CALL SetMouseMoveSpeed(int speed);
	// 重置鼠标移动速度,默认10
	GHOST_API_EXPORT int GHOST_API_CALL ResetMouseMoveSpeed();

	// 获取鼠标滚轮速度-行数
	GHOST_API_EXPORT int GHOST_API_CALL GetMouseWheelLines();
	// 设置鼠标滚轮速度-行数
	GHOST_API_EXPORT int GHOST_API_CALL SetMouseWheelLines(int speed);
	// 重置鼠标滚轮速度-行数,默认3
	GHOST_API_EXPORT int GHOST_API_CALL ResetMouseWheelLines();

	// 获取鼠标滚轮速度-字符
	GHOST_API_EXPORT int GHOST_API_CALL GetMouseWheelChars();
	// 设置鼠标滚轮速度-字符
	GHOST_API_EXPORT int GHOST_API_CALL SetMouseWheelChars(int speed);
	// 重置鼠标滚轮速度-字符,默认3
	GHOST_API_EXPORT int GHOST_API_CALL ResetMouseWheelChars();

	// 获取鼠标双击速度-毫秒
	GHOST_API_EXPORT int GHOST_API_CALL GetMouseDoubleClickSpeed();
	// 设置鼠标双击速度-毫秒
	GHOST_API_EXPORT int GHOST_API_CALL SetMouseDoubleClickSpeed(int speed);
	// 重置鼠标双击速度-毫秒,默认500
	GHOST_API_EXPORT int GHOST_API_CALL ResetMouseDoubleClickSpeed();

	//////////////////////////////////////////////
	////////////     加密管理接口      ///////////
	//////////////////////////////////////////////	  
	// 复位加密锁
	GHOST_API_EXPORT int GHOST_API_CALL ResetLock();
	// 初始化加密锁
	GHOST_API_EXPORT int GHOST_API_CALL InitLock(const char *wpwd, const char *rpwd);
	// 写字符串到存储器
	GHOST_API_EXPORT int GHOST_API_CALL WriteString(const char *wpwd, int index, const char *str);
	// 从存储器读字符串
	GHOST_API_EXPORT char* GHOST_API_CALL ReadString(const char *rpwd, int index);
	// 复位算法密钥
	GHOST_API_EXPORT int GHOST_API_CALL ResetKey();
	// 设置算法密钥
	GHOST_API_EXPORT int GHOST_API_CALL InitKey( const char *key);
	// 加密字符串
	GHOST_API_EXPORT char* GHOST_API_CALL EncString(const char *str);
	// 解密字符串
	GHOST_API_EXPORT char* GHOST_API_CALL DecString(const char *str);

	//////////////////////////////////////////////
	////////////     辅助管理接口      ///////////
	//////////////////////////////////////////////
	// 设置设备日志级别:0=关闭，1-6级别
	// SILENT=0;FATAL=1;ERROR=2;WARNING=3;NOTICE=4;TRACE=5;VERBOSE=6
	GHOST_API_EXPORT int GHOST_API_CALL SetDevLogLevel(int level);
	// 设置主机日志级别:0=关闭，1-6级别
	//SILENT=0;FATAL=1;ERROR=2;WARN=3;INFO=4;DEBUG=5;TRACE=6 
	GHOST_API_EXPORT int GHOST_API_CALL SetHostLogLevel(int level);
#ifdef __cplusplus
}
#endif

#endif

