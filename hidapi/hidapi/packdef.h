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

typedef union {
	unsigned char type[2];
	//mouse
	struct {
		unsigned char tmp1;
		unsigned char ms_type;
		//use
		unsigned char ms_button;
		char ms_x;
		char ms_y;
		char ms_wheel;
	};
	//keyboard
	struct {
		unsigned char tmp2;
		unsigned char kb_type;
		//use
		unsigned char kb_cmd;//keyboard:down,up,press...
		unsigned char kb_count;
		unsigned char kb_key[6];
	};
	//log
	struct {
		unsigned char tmp3;
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