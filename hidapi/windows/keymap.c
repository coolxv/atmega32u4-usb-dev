#include <ctype.h>
#include "map.h"
#include "keymap.h"

static int initialized = 0;
static map_int_t m;


static init_keymap()
{
	//Ð¡Ð´×ÖÄ¸a-z
	map_set(&m, "a", KEY_A);
	map_set(&m, "b", KEY_B);
	map_set(&m, "c", KEY_C);
	map_set(&m, "d", KEY_D);
	map_set(&m, "e", KEY_E);
	map_set(&m, "f", KEY_F);
	map_set(&m, "g", KEY_G);
	map_set(&m, "h", KEY_H);
	map_set(&m, "i", KEY_I);
	map_set(&m, "j", KEY_J);
	map_set(&m, "k", KEY_K);
	map_set(&m, "l", KEY_L);
	map_set(&m, "m", KEY_M);
	map_set(&m, "n", KEY_N);
	map_set(&m, "o", KEY_O);
	map_set(&m, "p", KEY_P);
	map_set(&m, "q", KEY_Q);
	map_set(&m, "r", KEY_R);
	map_set(&m, "s", KEY_S);
	map_set(&m, "t", KEY_T);
	map_set(&m, "u", KEY_U);
	map_set(&m, "v", KEY_V);
	map_set(&m, "w", KEY_W);
	map_set(&m, "x", KEY_X);
	map_set(&m, "y", KEY_Y);
	map_set(&m, "z", KEY_Z);
	//´óÐ´×ÖÄ¸A-Z
	map_set(&m, "A", KEY_A | SHIFT);
	map_set(&m, "B", KEY_B | SHIFT);
	map_set(&m, "C", KEY_C | SHIFT);
	map_set(&m, "D", KEY_D | SHIFT);
	map_set(&m, "E", KEY_E | SHIFT);
	map_set(&m, "F", KEY_F | SHIFT);
	map_set(&m, "G", KEY_G | SHIFT);
	map_set(&m, "H", KEY_H | SHIFT);
	map_set(&m, "I", KEY_I | SHIFT);
	map_set(&m, "J", KEY_J | SHIFT);
	map_set(&m, "K", KEY_K | SHIFT);
	map_set(&m, "L", KEY_L | SHIFT);
	map_set(&m, "M", KEY_M | SHIFT);
	map_set(&m, "N", KEY_N | SHIFT);
	map_set(&m, "O", KEY_O | SHIFT);
	map_set(&m, "P", KEY_P | SHIFT);
	map_set(&m, "Q", KEY_Q | SHIFT);
	map_set(&m, "R", KEY_R | SHIFT);
	map_set(&m, "S", KEY_S | SHIFT);
	map_set(&m, "T", KEY_T | SHIFT);
	map_set(&m, "U", KEY_U | SHIFT);
	map_set(&m, "V", KEY_V | SHIFT);
	map_set(&m, "W", KEY_W | SHIFT);
	map_set(&m, "X", KEY_X | SHIFT);
	map_set(&m, "Y", KEY_Y | SHIFT);
	map_set(&m, "Z", KEY_Z | SHIFT);
	//Êý×Ö0-9
	map_set(&m, "0", KEY_0);
	map_set(&m, "1", KEY_1);
	map_set(&m, "2", KEY_2);
	map_set(&m, "3", KEY_3);
	map_set(&m, "4", KEY_4);
	map_set(&m, "5", KEY_5);
	map_set(&m, "6", KEY_6);
	map_set(&m, "7", KEY_7);
	map_set(&m, "8", KEY_8);
	map_set(&m, "9", KEY_9);
	//·ûºÅ
	map_set(&m, "`", KEY_TILDE);
	map_set(&m, "~", KEY_TILDE | SHIFT);
	map_set(&m, "!", KEY_1 | SHIFT);
	map_set(&m, "@", KEY_2 | SHIFT);
	map_set(&m, "#", KEY_3 | SHIFT);
	map_set(&m, "$", KEY_4 | SHIFT);
	map_set(&m, "%", KEY_5 | SHIFT);
	map_set(&m, "^", KEY_6 | SHIFT);
	map_set(&m, "&", KEY_7 | SHIFT);
	map_set(&m, "*", KEY_8 | SHIFT);
	map_set(&m, "(", KEY_9 | SHIFT);
	map_set(&m, ")", KEY_0 | SHIFT);
	map_set(&m, "-", KEY_MINUS);
	map_set(&m, "_", KEY_MINUS | SHIFT);
	map_set(&m, "=", KEY_EQUAL);
	map_set(&m, "+", KEY_EQUAL | SHIFT);
	map_set(&m, "[", KEY_LEFT_BRACE);
	map_set(&m, "{", KEY_LEFT_BRACE | SHIFT);
	map_set(&m, "]", KEY_RIGHT_BRACE);
	map_set(&m, "}", KEY_RIGHT_BRACE | SHIFT);
	map_set(&m, "\\", KEY_BACKSLASH);
	map_set(&m, "|", KEY_BACKSLASH | SHIFT);
	map_set(&m, ";", KEY_SEMICOLON);
	map_set(&m, ":", KEY_SEMICOLON | SHIFT);
	map_set(&m, "'", KEY_QUOTE);
	map_set(&m, "\"", KEY_QUOTE | SHIFT);
	map_set(&m, ",", KEY_COMMA);
	map_set(&m, "<", KEY_COMMA | SHIFT);
	map_set(&m, ".", KEY_PERIOD);
	map_set(&m, ">", KEY_PERIOD | SHIFT);
	map_set(&m, "/", KEY_SLASH);
	map_set(&m, "?", KEY_SLASH | SHIFT);
	/*
			Enter¡¢Esc¡¢Backspace¡¢Tab¡¢CapsLock¡¢
			F1¡¢F2¡¢F3¡¢F4¡¢F5¡¢F6¡¢F7¡¢F8¡¢F9¡¢F10¡¢F11¡¢F12¡¢
			Num0~Num9¡¢NumEnter¡¢NumDot¡¢NumAdd¡¢NumDec¡¢NumMul¡¢NumDiv¡¢NumLock¡¢
			Space¡¢PrtSc¡¢ScrLk¡¢Pause¡¢Insert¡¢Delete¡¢
			Home¡¢End¡¢PageUp¡¢PageDown¡¢
			Right¡¢Left¡¢Down¡¢Up¡¢
			Ctrl¡¢Shift¡¢Alt¡¢Win¡¢£¨±¾ÐÐÎª×ó²à¼ü£©
			RCtrl¡¢RShift¡¢RAlt¡¢RWin£¨±¾ÐÐÎªÓÒ²à¼ü£©
	*/
	map_set(&m, "enter", KEY_ENTER);
	map_set(&m, "esc", KEY_ESC);
	map_set(&m, "backspace", KEY_BACKSPACE);
	map_set(&m, "capslock", KEY_CAPS_LOCK);

	map_set(&m, "f1", KEY_F1);
	map_set(&m, "f2", KEY_F2);
	map_set(&m, "f3", KEY_F3);
	map_set(&m, "f4", KEY_F4);
	map_set(&m, "f5", KEY_F5);
	map_set(&m, "f6", KEY_F6);
	map_set(&m, "f7", KEY_F7);
	map_set(&m, "f8", KEY_F8);
	map_set(&m, "f9", KEY_F9);
	map_set(&m, "f10", KEY_F10);
	map_set(&m, "f11", KEY_F11);
	map_set(&m, "f12", KEY_F12);

	map_set(&m, "num0", KEYPAD_0);
	map_set(&m, "num1", KEYPAD_1);
	map_set(&m, "num2", KEYPAD_2);
	map_set(&m, "num3", KEYPAD_3);
	map_set(&m, "num4", KEYPAD_4);
	map_set(&m, "num5", KEYPAD_5);
	map_set(&m, "num6", KEYPAD_6);
	map_set(&m, "num7", KEYPAD_7);
	map_set(&m, "num8", KEYPAD_8);
	map_set(&m, "num9", KEYPAD_9);
	map_set(&m, "numenter", KEYPAD_ENTER);
	map_set(&m, "numdot", KEYPAD_DOT);
	map_set(&m, "numadd", KEYPAD_ADD);
	map_set(&m, "numdec", KEYPAD_SUBTRACT);
	map_set(&m, "nummul", KEYPAD_MULTIPLY);
	map_set(&m, "numdiv", KEYPAD_DIVIDE);
	map_set(&m, "numlock", KEY_NUM_LOCK);

	map_set(&m, "space", KEY_SPACE);

	map_set(&m, "prtsc", KEY_PRINTSCREEN);
	map_set(&m, "scrlk", KEY_SCROLL_LOCK);
	map_set(&m, "pause", KEY_PAUSE);

	map_set(&m, "insert", KEY_INSERT);
	map_set(&m, "delete", KEY_DELETE);
	map_set(&m, "home", KEY_HOME);
	map_set(&m, "end", KEY_END);
	map_set(&m, "pageup", KEY_PAGE_UP);
	map_set(&m, "pagedown", KEY_PAGE_DOWN);

	map_set(&m, "right", KEY_RIGHT_ARROW);
	map_set(&m, "left", KEY_LEFT_ARROW);
	map_set(&m, "down", KEY_DOWN_ARROW);
	map_set(&m, "up", KEY_UP_ARROW); 

	map_set(&m, "ctrl", KEY_LEFT_CTRL);
	map_set(&m, "shift", KEY_LEFT_SHIFT);
	map_set(&m, "alt", KEY_LEFT_ALT);
	map_set(&m, "win", KEY_LEFT_WINDOWS);

	map_set(&m, "rctrl", KEY_RIGHT_CTRL);
	map_set(&m, "rshift", KEY_RIGHT_SHIFT);
	map_set(&m, "ralt", KEY_RIGHT_ALT);
	map_set(&m, "rwin", KEY_RIGHT_WINDOWS);
}

static char *strlowr(char *str)
{
	char *orign = str;
	for (; *str != '\0'; str++)
		*str = tolower(*str);
	return orign;
}

void keymap_init()
{
	if (initialized)
	{
		return;
	}
	map_init(&m);
	init_keymap();
	initialized = 1;
	return;
}

unsigned char keymap_map(const char *key)
{
	char tmp[17];
	strcpy_s(tmp,sizeof(tmp), key);
	strlowr(tmp);
	int *val = map_get(&m, tmp);
	if (val)
	{
		return (unsigned char)(*val && 0xff);
	}
	else
	{
		return 0;//KEY_RESERVED
	}
}
void keymap_fini()
{
	if (!initialized)
	{
		return;
	}

	map_deinit(&m);
	initialized = 0;
	return;
}