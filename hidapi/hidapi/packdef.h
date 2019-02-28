#pragma once
//msg type
#define  MSG_TYPE_KEYBOARD 1
#define  MSG_TYPE_MOUSE 2
#define  MSG_TYPE_LOG 3
//keyboard cmd
#define  MSG_CMD_KB_DOWN 1
#define  MSG_CMD_KB_UP 2
#define  MSG_CMD_KB_PRESS 3
#define  MSG_CMD_KB_UP_ALL 4
#define  MSG_CMD_KB_COMB_DOWN 5
#define  MSG_CMD_KB_COMB_UP 6
#define  MSG_CMD_KB_COMB_PRESS 7
#define  MSG_CMD_KB_GET_CAPS_LOCK 8
#define  MSG_CMD_KB_GET_NUM_LOCK 9
#define  MSG_CMD_KB_SET_CAPS_LOCK 10
#define  MSG_CMD_KB_SET_NUM_LOCK 11
//mouse cmd
#define  MSG_CMD_MS_LEFT_DOWN 1
#define  MSG_CMD_MS_LEFT_UP 2
#define  MSG_CMD_MS_LEFT_CLICK 3
#define  MSG_CMD_MS_LEFT_DCLICK 4
#define  MSG_CMD_MS_RIGHT_DOWN 5
#define  MSG_CMD_MS_RIGHT_UP 6
#define  MSG_CMD_MS_RIGHT_CLICK 7
#define  MSG_CMD_MS_RIGHT_DCLICK 8
#define  MSG_CMD_MS_MIDDLE_DOWN 9
#define  MSG_CMD_MS_MIDDLE_UP 10
#define  MSG_CMD_MS_MIDDLE_CLICK 11
#define  MSG_CMD_MS_MIDDLE_DCLICK 12
#define  MSG_CMD_MS_UP_ALL 13
#define  MSG_CMD_MS_MOVE_TO 14
#define  MSG_CMD_MS_MOVE_TO_R 15
#define  MSG_CMD_MS_WHEEL_MOVE 16





typedef union {
	unsigned char type[2];
	//mouse
	struct {
		unsigned char ms_pad0;
		unsigned char ms_type;
		//use
		unsigned char ms_cmd;//right,left...
		unsigned char ms_count;
		short ms_x;
		short ms_y;
		char ms_wheel;
	};
	//keyboard
	struct {
		unsigned char kb_pad0;
		unsigned char kb_type;
		//use
		unsigned char kb_cmd;//down,up,press...
		unsigned char kb_count;
		unsigned char kb_key[6];
	};
	//log
	struct {
		unsigned char lg_pad0;
		unsigned char lg_type;
		//use
		unsigned char lg_level;
	};
	unsigned char buf[65];
} MSG_DATA_T;

typedef union {
	unsigned char type;
	//keyboard
	struct {
		unsigned char kb_type;
		//use
		unsigned char kb_cmd;
		unsigned char kb_ret;
	};
	unsigned char buf[64];
} MSG_DATA_RESULT_T;