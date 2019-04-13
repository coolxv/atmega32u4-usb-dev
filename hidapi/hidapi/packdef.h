#pragma once
//msg type
#define  MSG_TYPE_DEFAULT 1
#define  MSG_TYPE_KEYBOARD 2
#define  MSG_TYPE_MOUSE 3
#define  MSG_TYPE_LOG 4
#define  MSG_TYPE_FUNC 5
#define  MSG_TYPE_INFO 6
#define  MSG_TYPE_DATA 7
//keyboard cmd
#define  MSG_CMD_KB_DOWN 1
#define  MSG_CMD_KB_UP 2
#define  MSG_CMD_KB_PRESS 3
#define  MSG_CMD_KB_UP_ALL 4
#define  MSG_CMD_KB_COMB_DOWN 5
#define  MSG_CMD_KB_COMB_UP 6
#define  MSG_CMD_KB_COMB_PRESS 7
#define  MSG_CMD_KB_GET_CAPS_LOCK 8
#define  MSG_CMD_KB_SET_CAPS_LOCK 9
#define  MSG_CMD_KB_GET_NUM_LOCK 10
#define  MSG_CMD_KB_SET_NUM_LOCK 11
#define  MSG_CMD_KB_GET_SCROLL_LOCK 12
#define  MSG_CMD_KB_SET_SCROLL_LOCK 13
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
//function cmd
#define  MSG_CMD_FUNC_RESTART 1
#define  MSG_CMD_FUNC_DISCONNECT 2
#define  MSG_CMD_FUNC_SET_DEVICE_ID 3
#define  MSG_CMD_FUNC_RESTORE_DEVICE_ID 4
#define  MSG_CMD_FUNC_SET_SERIAL_NUMBER 5
#define  MSG_CMD_FUNC_RESTORE_SERIAL_NUMBER 6
#define  MSG_CMD_FUNC_SET_PRODUCT 7
#define  MSG_CMD_FUNC_RESTORE_PRODUCT 8
#define  MSG_CMD_FUNC_SET_MANUFACTURER 9
#define  MSG_CMD_FUNC_RESTORE_MANUFACTURER 10
//info cmd
#define  MSG_CMD_INFO_SN 1
#define  MSG_CMD_INFO_MODEL 2
#define  MSG_CMD_INFO_VERSION 3
#define  MSG_CMD_INFO_PROD_DATE 4
#define  MSG_CMD_INFO_PRODUCT 5
#define  MSG_CMD_INFO_MANUFACTURER 6
#define  MSG_CMD_INFO_DEVICE_ID 7
//data cmd
#define  MSG_CMD_ENCRYP_INIT_LOCK 1
#define  MSG_CMD_ENCRYP_READ_STR 2
#define  MSG_CMD_ENCRYP_WRITE_STR 3
#define  MSG_CMD_ENCRYP_INIT_KEY 4
#define  MSG_CMD_ENCRYP_ENC_STR 5
#define  MSG_CMD_ENCRYP_DEC_STR 6

#pragma pack(push, 1)
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
		unsigned short kb_key[6];
	};
	//log
	struct {
		unsigned char lg_pad0;
		unsigned char lg_type;
		//use
		unsigned char lg_level;
	};
	//func
	struct {
		unsigned char fc_pad0;
		unsigned char fc_type;
		//use
		unsigned char fc_cmd;
		union {
			unsigned char fc_value[4];
			unsigned short fc_vidpid[2];
			unsigned char fc_serial[33];
			unsigned char fc_product[17];
			unsigned char fc_manufacturer[17];
		};
	};
	//info
	struct {
		unsigned char if_pad0;
		unsigned char if_type;
		//use
		unsigned char if_cmd;
	};
	//data
	struct {
		unsigned char dt_pad0;
		unsigned char dt_type;
		//use
		unsigned char dt_cmd;
		unsigned char dt_index;
		unsigned char dt_rpwd[9];
		unsigned char dt_wpwd[9];
		unsigned char dt_kkey[9];
		unsigned char dt_len;
		unsigned char dt_buf[33];
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
	//info
	struct {
		unsigned char if_type;
		//use
		unsigned char if_cmd;
		union {
			unsigned char if_value[48];
			unsigned short if_vidpid[2];
		};
	};
	//data
	struct {
		unsigned char dt_type;
		//use
		unsigned char dt_cmd;
		unsigned char dt_len;
		unsigned char dt_buf[33];
	};
	//comm
	struct {
		unsigned char cm_type;
		//use
		unsigned char cm_cmd;
		unsigned char cm_error;
	};
	unsigned char buf[64];
} MSG_DATA_RESULT_T;
#pragma pack(pop)