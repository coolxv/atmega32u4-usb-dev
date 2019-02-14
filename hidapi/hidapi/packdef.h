#pragma once



typedef union {
	unsigned char type[2];
	struct {
		unsigned char tt1;
		unsigned char ms_type;//mouse
		//use
		unsigned char b;
		char x;
		char y;
		char w;
	};
	struct {
		unsigned char tt2;
		unsigned char kb_type;//keyboard
		//use
		unsigned char count;
		unsigned char k[6];
	};
	unsigned char buf[65];
} MSG_KEY_MOUSE_DATA_T;

