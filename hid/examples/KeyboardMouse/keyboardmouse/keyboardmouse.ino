#include "HID-Project.h"
//#define DISABLE_LOGGING
#include "ArduinoLog.h"

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





//global data
const int pinLed = LED_BUILTIN;
MSG_DATA_T rawhidData;




void setup() {
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
  while (!Serial && !Serial.available()) {}
  Log.begin(LOG_LEVEL_SILENT, &Serial);
  //
  Keyboard.begin();
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
  // Simple debounce
  // Check if there is new data from the RawHID device
  auto bytesAvailable = RawHID.available();
  if (bytesAvailable)
  {
    // Mirror data via Serial
    for (int i = 0; i < bytesAvailable; i++)
    {
      rawhidData.buf[i] = (unsigned char)RawHID.read();
    }
  }
  return bytesAvailable;
}
void keyboardProcess()
{
  switch (rawhidData.kb_cmd)
  {
    case MSG_KB_TYPE_DOWN:
      {
        Keyboard.release(KeyboardKeycode(rawhidData.kb_key[0]));
        break;
      }
    case MSG_KB_TYPE_UP:
      {
        Keyboard.press(KeyboardKeycode(rawhidData.kb_key[0]));
        break;
      }
    case MSG_KB_TYPE_PRESS:
      {
        Keyboard.write(KeyboardKeycode(rawhidData.kb_key[0]));
      }
    case MSG_KB_TYPE_UP_ALL:
      {
        Keyboard.releaseAll();
        break;

      }
    case MSG_KB_TYPE_COMB_DOWN:
      {
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            Keyboard.remove(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          Keyboard.send();
        }
        break;
      }
    case MSG_KB_TYPE_COMB_UP:
      {
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            Keyboard.add(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          Keyboard.send();
        }
        break;
      }
    case MSG_KB_TYPE_COMB_PRESS:
      {
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            Keyboard.add(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          Keyboard.send();
        }
        //
        j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i] != 0))
          {
            Keyboard.remove(KeyboardKeycode(rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          Keyboard.send();
        }
        break;
      }
    default:
      {
        Log.error("msg keyboard cmd error, cmd is %d", rawhidData.kb_cmd);
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
      case MSG_KB_TYPE_UP:
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
          Log.error("msg type error, type is %d", rawhidData.type);
          break;
        }
    }
    Log.trace("read data sucessful");

  }
  else
  {
    Log.error("read data error");
  }

  Log.verbose("read data size %d", bytesAvailable);
}
