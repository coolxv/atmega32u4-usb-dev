#pragma once



typedef union {
	unsigned char type[2];
	struct {
		unsigned char tt1;
		unsigned char ttt1;//mouse
		//use
		unsigned char b;
		char x;
		char y;
		char w;
	};
	struct {
		unsigned char tt2;
		unsigned char ttt2;//keyboard
		//use
		unsigned char kb_type;//keyboard:down,up,press...
		unsigned char count;
		unsigned char k[6];
	};
	unsigned char buf[65];
} MSG_KM_DATA_T;

#define  MSG_KM_TYPE_KEYBOARD 1
#define  MSG_KM_TYPE_MOUSE 2

#define  MSG_KM_KB_TYPE_DOWN 1
#define  MSG_KM_KB_TYPE_UP 2
#define  MSG_KM_KB_TYPE_PRESS 3
#define  MSG_KM_KB_TYPE_UP_ALL 4
#define  MSG_KM_KB_TYPE_COMB_DOWN 5
#define  MSG_KM_KB_TYPE_COMB_UP 6
#define  MSG_KM_KB_TYPE_COMB_PRESS 7
