#include "HID-Project.h"
//#define DISABLE_LOGGING
#include "ArduinoLog.h"

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
  unsigned char type;
  //mouse
  struct {
    unsigned char ms_type;
    //use
    unsigned char ms_button;
    char ms_x;
    char ms_y;
    char ms_wheel;
  };
  //keyboard
  struct {
    unsigned char kb_type;
    //use
    unsigned char kb_cmd;//keyboard:down,up,press...
    unsigned char kb_count;
    unsigned char kb_key[6];
  };
  //log
  struct {
    unsigned char lg_type;
    //use
    unsigned char lg_level;
  };
  unsigned char buf[64];
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


//global data
const int pinLed = LED_BUILTIN;
MSG_DATA_T rawhidData;
MSG_DATA_RESULT_T rawhidwriteData;



void setup() {
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
  while (!Serial && !Serial.available()) {}
  Log.begin(LOG_LEVEL_SILENT, &Serial);
  //
  BootKeyboard.begin();
  Mouse.begin();

  //
  RawHID.begin((uint8_t *)&rawhidData, sizeof(rawhidData));
  //
  digitalWrite(pinLed, HIGH);
  delay(500);
  digitalWrite(pinLed, LOW);
}

int readData()
{

  auto bytesAvailable = RawHID.available();
  if (bytesAvailable > 0 && bytesAvailable <= sizeof(rawhidData))
  {
    for (int i = 0; i < bytesAvailable; i++)
    {
      rawhidData.buf[i] = (unsigned char)RawHID.read();
    }
  }
  return bytesAvailable;
}
int writeData()
{
  return RawHID.write((unsigned char*)&rawhidwriteData, sizeof(rawhidwriteData));
}
void keyboardProcess()
{
  switch (rawhidData.kb_cmd)
  {
    case MSG_CMD_KB_DOWN:
      {
        BootKeyboard.release(KeyboardKeycode(rawhidData.kb_key[0]));
        break;
      }
    case MSG_CMD_KB_UP:
      {
        BootKeyboard.press(KeyboardKeycode(rawhidData.kb_key[0]));
        break;
      }
    case MSG_CMD_KB_PRESS:
      {
        BootKeyboard.write(KeyboardKeycode(rawhidData.kb_key[0]));
        break;
      }
    case MSG_CMD_KB_UP_ALL:
      {
        BootKeyboard.releaseAll();
        break;
      }
    case MSG_CMD_KB_COMB_DOWN:
      {
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            BootKeyboard.remove(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          BootKeyboard.send();
        }
        break;
      }
    case MSG_CMD_KB_COMB_UP:
      {
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            BootKeyboard.add(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          BootKeyboard.send();
        }
        break;
      }
    case MSG_CMD_KB_COMB_PRESS:
      {
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            BootKeyboard.add(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          BootKeyboard.send();
        }
        //
        j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            BootKeyboard.remove(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          BootKeyboard.send();
        }
        break;
      }
    case MSG_CMD_KB_GET_CAPS_LOCK:
      {
        if (BootKeyboard.getLeds() & LED_CAPS_LOCK)
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.kb_cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 1;
          writeData();
        }
        else
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.kb_cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 0;
          writeData();
        }
        break;
      }
    case MSG_CMD_KB_GET_NUM_LOCK:
      {
        if (BootKeyboard.getLeds() & LED_NUM_LOCK)
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.kb_cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 1;
          writeData();
        }
        else
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.kb_cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 0;
          writeData();
        }
        break;
      }
    case MSG_CMD_KB_SET_CAPS_LOCK:
      {
        BootKeyboard.write(KEY_CAPS_LOCK);
        break;
      }
    case MSG_CMD_KB_SET_NUM_LOCK:
      {
        BootKeyboard.write(KEY_NUM_LOCK);
        break;
      }
    default:
      {
        Log.error("msg keyboard cmd error, cmd is %d\n", rawhidData.kb_cmd);
        break;
      }
  }
}
void MouseProcess()
{

}

void LogProcess()
{
  if (Serial)
  {
    Log.begin(rawhidData.lg_level, &Serial);
  }

}
void loop()
{
  // Check if there is new data from the RawHID device

  auto bytesAvailable = readData();
  if (bytesAvailable && bytesAvailable == sizeof(rawhidData))
  {
    switch (rawhidData.type)
    {
      case MSG_TYPE_KEYBOARD:
        {
          keyboardProcess();
          break;
        }
      case MSG_TYPE_MOUSE:
        {
          MouseProcess();
          break;
        }
      case MSG_TYPE_LOG:
        {
          LogProcess();
          break;
        }
      default:
        {
          Log.error("msg type error, type is %d\n", rawhidData.type);
          break;
        }
    }
    Log.trace("read data sucessful\n");

  }
  else
  {
    Log.trace("read data error\n");
  }

  Log.verbose("read data size %d\n", bytesAvailable);
}
