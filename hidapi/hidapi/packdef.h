#pragma once
//msg type
#define  MSG_TYPE_KEYBOARD 1
#define  MSG_TYPE_MOUSE 2
#define  MSG_TYPE_LOG 3
//keyboard cmd
#define  MSG_KB_TYPE_DOWN 1
#define  MSG_KB_TYPE_UP 2
#define  MSG_KB_TYPE_PRESS 3
#define  MSG_KB_TYPE_UP_ALL 4
#define  MSG_KB_TYPE_COMB_DOWN 5
#define  MSG_KB_TYPE_COMB_UP 6
#define  MSG_KB_TYPE_COMB_PRESS 7


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

