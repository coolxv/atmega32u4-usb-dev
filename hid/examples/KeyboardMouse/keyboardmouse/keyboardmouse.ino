#include <EEPROM.h>

#include "HID-Project.h"
//#define DISABLE_LOGGING
#include "ArduinoLog.h"

//msg type
#define  MSG_TYPE_KEYBOARD 1
#define  MSG_TYPE_MOUSE 2
#define  MSG_TYPE_LOG 3
#define  MSG_TYPE_FUNC 4
#define  MSG_TYPE_INFO 5

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
//info cmd
#define  MSG_CMD_INFO_SN 1
#define  MSG_CMD_INFO_MODEL 2
#define  MSG_CMD_INFO_VERSION 3
#define  MSG_CMD_INFO_PROD_DATE 4

typedef union {
  unsigned char type;
  //mouse
  struct {
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
    unsigned char kb_type;
    //use
    unsigned char kb_cmd;//keyboard:down,up,press...
    unsigned char kb_count;
    unsigned short kb_key[6];
  };
  //log
  struct {
    unsigned char lg_type;
    //use
    unsigned char lg_level;
  };
  //func
  struct {
    unsigned char fc_type;
    //use
    unsigned char fc_cmd;
    union {
      unsigned char fc_value[4];
      unsigned short fc_vidpid[2];
      unsigned char fc_serial[USB_SERIAL_LEN_MAX + 1];
    };
  };
  //info
  struct {
    unsigned char if_type;
    //use
    unsigned char if_cmd;
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
  //info
  struct {
    unsigned char if_type;
    //use
    unsigned char if_cmd;
    unsigned char if_value[48];
  };
  unsigned char buf[64];
} MSG_DATA_RESULT_T;

//
#define DEV_MODEL_BASE_INFO "base"
#define DEV_MODEL_ADVANCE_INFO "advance"
#define DEV_VERSION_INFO "1.0.0"
#define DEV_PRODUCTION_DATE_INFO "20190315"

//global data
const int pinLed = LED_BUILTIN;
MSG_DATA_T rawhidData;
MSG_DATA_RESULT_T rawhidwriteData;

int delay_time = 0;
bool delay_restart = false;
void(* resetFunc) (void) = 0;

// save some chars
const char sn_info[] PROGMEM  = {USB_SERIAL};
const char model_info[] PROGMEM  = {DEV_MODEL_BASE_INFO};
const char version_info[] PROGMEM  = {DEV_VERSION_INFO};
const char production_date_info[] PROGMEM  = {DEV_PRODUCTION_DATE_INFO};



void setup() {
  //led init
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, HIGH);
  if (true == delay_restart && delay_time > 0) {
    delay_time = 0;
    delay_restart = false;
    delay(delay_time);
  }
  //serial
  Serial.begin(9600);
  //log
  Log.begin(LOG_LEVEL_SILENT, &Serial);
  //keyboard
  BootKeyboard.begin();
  //mouse
  Mouse.begin();
  AbsoluteMouse.begin();
  //rawhid
  RawHID.begin((uint8_t *)&rawhidData, sizeof(rawhidData));
  //led show
  delay(500);
  digitalWrite(pinLed, LOW);
  Log.trace("device initial successful\n");
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
  Log.trace("keyboard command %d\n", rawhidData.kb_cmd);
  switch (rawhidData.kb_cmd)
  {
    case MSG_CMD_KB_DOWN:
      {
        unsigned char key = (unsigned char)(0xff & rawhidData.kb_key[0]);
        BootKeyboard.add(KeyboardKeycode(key));
        if (rawhidData.kb_key[0] & 0x8000)
        {
          BootKeyboard.add(KEY_LEFT_SHIFT);
        }
        BootKeyboard.send();
        break;
      }
    case MSG_CMD_KB_UP:
      {

        unsigned char key = (unsigned char)(0xff & rawhidData.kb_key[0]);
        BootKeyboard.remove(KeyboardKeycode(key));
        if (rawhidData.kb_key[0] & 0x8000)
        {
          BootKeyboard.remove(KEY_LEFT_SHIFT);
        }
        BootKeyboard.send();
        break;
      }
    case MSG_CMD_KB_PRESS:
      {
        unsigned char key = (unsigned char)(0xff & rawhidData.kb_key[0]);
        //press
        BootKeyboard.add(KeyboardKeycode(key));
        if (rawhidData.kb_key[0] & 0x8000)
        {
          BootKeyboard.add(KEY_LEFT_SHIFT);
        }
        BootKeyboard.send();
        //release
        BootKeyboard.remove(KeyboardKeycode(key));
        if (rawhidData.kb_key[0] & 0x8000)
        {
          BootKeyboard.remove(KEY_LEFT_SHIFT);
        }
        BootKeyboard.send();

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
          if (KeyboardKeycode(rawhidData.kb_key[i]) != 0)
          {
            BootKeyboard.add(KeyboardKeycode(0xff & rawhidData.kb_key[i]));
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
          if (KeyboardKeycode(rawhidData.kb_key[i]) != 0)
          {
            BootKeyboard.remove(KeyboardKeycode(0xff & rawhidData.kb_key[i]));
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
        //press
        int j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i]) != 0)
          {
            BootKeyboard.add(KeyboardKeycode(0xff & rawhidData.kb_key[i]));
            j++;
          }
        }
        if (j > 0)
        {
          BootKeyboard.send();
        }
        //release
        j = 0;
        for (int i = 0; i < 6; i++)
        {
          if (KeyboardKeycode(rawhidData.kb_key[i]) != 0)
          {
            BootKeyboard.remove(KeyboardKeycode(0xff & rawhidData.kb_key[i]));
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
    case MSG_CMD_KB_SET_CAPS_LOCK:
      {
        BootKeyboard.write(KEY_CAPS_LOCK);
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
    case MSG_CMD_KB_SET_NUM_LOCK:
      {
        BootKeyboard.write(KEY_NUM_LOCK);
        break;
      }
    case MSG_CMD_KB_GET_SCROLL_LOCK:
      {
        if (BootKeyboard.getLeds() & LED_SCROLL_LOCK)
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.kb_cmd = MSG_CMD_KB_GET_SCROLL_LOCK;
          rawhidwriteData.kb_ret = 1;
          writeData();
        }
        else
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.kb_cmd = MSG_CMD_KB_GET_SCROLL_LOCK;
          rawhidwriteData.kb_ret = 0;
          writeData();
        }
        break;
      }
    case MSG_CMD_KB_SET_SCROLL_LOCK:
      {
        BootKeyboard.write(KEY_SCROLL_LOCK);
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
  Log.trace("mouse command %d\n", rawhidData.ms_cmd);
  switch (rawhidData.ms_cmd)
  {
    case MSG_CMD_MS_LEFT_DOWN:
      {
        Mouse.press(MOUSE_LEFT);
        break;
      }
    case MSG_CMD_MS_LEFT_UP:
      {
        Mouse.release(MOUSE_LEFT);
        break;
      }
    case MSG_CMD_MS_LEFT_CLICK:
      {
        Mouse.click(MOUSE_LEFT);
        break;
      }
    case MSG_CMD_MS_LEFT_DCLICK:
      {
        Mouse.click(MOUSE_LEFT);
        delay(random(60, 110));
        Mouse.click(MOUSE_LEFT);
        break;
      }
    case MSG_CMD_MS_RIGHT_DOWN:
      {
        Mouse.press(MOUSE_RIGHT);
        break;
      }
    case MSG_CMD_MS_RIGHT_UP:
      {
        Mouse.release(MOUSE_RIGHT);
        break;
      }
    case MSG_CMD_MS_RIGHT_CLICK:
      {
        Mouse.click(MOUSE_RIGHT);
        break;
      }
    case MSG_CMD_MS_RIGHT_DCLICK:
      {
        Mouse.click(MOUSE_RIGHT);
        delay(random(60, 110));
        Mouse.click(MOUSE_RIGHT);
        break;
      }
    case MSG_CMD_MS_MIDDLE_DOWN:
      {
        Mouse.press(MOUSE_MIDDLE);
        break;
      }
    case MSG_CMD_MS_MIDDLE_UP:
      {
        Mouse.release(MOUSE_MIDDLE);
        break;
      }
    case MSG_CMD_MS_MIDDLE_CLICK:
      {
        Mouse.click(MOUSE_MIDDLE);
        break;
      }
    case MSG_CMD_MS_MIDDLE_DCLICK:
      {
        Mouse.click(MOUSE_MIDDLE);
        delay(random(60, 110));
        Mouse.click(MOUSE_MIDDLE);
        break;
      }
    case MSG_CMD_MS_UP_ALL:
      {
        Mouse.releaseAll();
        break;
      }
    case MSG_CMD_MS_MOVE_TO:
      {
        Log.trace("mouse absolute move %d, %d\n", rawhidData.ms_x, rawhidData.ms_y);
        AbsoluteMouse.moveTo(rawhidData.ms_x, rawhidData.ms_y);
        break;
      }
    case MSG_CMD_MS_MOVE_TO_R:
      {
        Mouse.move(rawhidData.ms_x, rawhidData.ms_y);
        break;
      }
    case MSG_CMD_MS_WHEEL_MOVE:
      {
        Mouse.move(0, 0, rawhidData.ms_wheel);
        break;
      }
    default:
      {
        Log.error("msg keyboard cmd error, cmd is %d\n", rawhidData.kb_cmd);
        break;
      }
  }
}

void LogProcess()
{
  if (Serial)
  {
    Log.begin(rawhidData.lg_level, &Serial);
  }
  Log.trace("log level %d\n", rawhidData.lg_level);
}

void FuncProcess()
{
  Log.trace("func command %d\n", rawhidData.fc_cmd);
  switch (rawhidData.fc_cmd)
  {
    case MSG_CMD_FUNC_RESTART:
      {
        delay_time = 0;
        delay_restart = false;
        resetFunc();
        break;
      }
    case MSG_CMD_FUNC_DISCONNECT:
      {
        delay_time = rawhidData.fc_value[0] * 1000;
        delay_restart = true;
        resetFunc();
        break;
      }
    case MSG_CMD_FUNC_SET_DEVICE_ID:
      {
        //  DEVICE DESCRIPTOR
        DeviceDescriptor dd_struct = D_DEVICE(0xEF, 0x02, 0x01, 64, rawhidData.fc_vidpid[0], rawhidData.fc_vidpid[1], 0x100, IMANUFACTURER, IPRODUCT, ISERIAL, 1);
        //tag
        unsigned short ee_flag = 0x1277;
        unsigned char *val = (unsigned char *)&ee_flag;
        val[0] = 0x77;
        val[1] = USB_DEVICE_DES_LEN_MAX;
        int addr = USB_DEVICE_DES_TAG_ADDR;
        EEPROM.put(addr, ee_flag);
        addr = USB_DEVICE_DES_ADDR;
        EEPROM.put(addr, dd_struct);
        break;
      }
    case MSG_CMD_FUNC_RESTORE_DEVICE_ID:
      {
        unsigned short ee_flag = 0;
        int addr = 0;
        EEPROM.put(addr, ee_flag);
        break;
      }
    case MSG_CMD_FUNC_SET_SERIAL_NUMBER:
      {
        const u16* ee_addr = USB_SERIAL_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        //write tag
        val[0] = 0x77;
        val[1] = USB_SERIAL_LEN_MAX;
        eeprom_write_word(ee_addr, ee_flag);
        //write serial number
        ee_addr = USB_SERIAL_ADDR;
        eeprom_write_block(rawhidData.fc_serial, ee_addr, USB_SERIAL_LEN_MAX);
        break;
      }
    case MSG_CMD_FUNC_RESTORE_SERIAL_NUMBER:
      {
        unsigned short ee_flag = 0;
        int addr = USB_SERIAL_TAG_ADDR;
        EEPROM.put(addr, ee_flag);
        break;
      }
    default:
      {
        Log.error("msg func cmd error, cmd is %d\n", rawhidData.fc_cmd);
        break;
      }
  }

}


void InfoProcess()
{
  Log.trace("info command %d\n", rawhidData.if_cmd);
  char one_char;
  int k;  // counter variable
  switch (rawhidData.if_cmd)
  {
    case MSG_CMD_INFO_SN:
      {
        const u16* ee_addr = USB_SERIAL_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        ee_flag = eeprom_read_word(ee_addr);
        // read info
        if (val[0] == 0x77)
        {
          ee_addr = USB_SERIAL_ADDR;
          eeprom_read_block(rawhidwriteData.if_value, ee_addr, USB_SERIAL_LEN_MAX);
          rawhidwriteData.if_value[USB_SERIAL_LEN_MAX] = 0;
        }
        else
        {
          strcpy_P(rawhidwriteData.if_value, sn_info);
        }
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.if_cmd = MSG_CMD_INFO_SN;
        writeData();

        break;
      }
    case MSG_CMD_INFO_MODEL:
      {
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.if_cmd = MSG_CMD_INFO_MODEL;
        // read info
        strcpy_P(rawhidwriteData.if_value, model_info);
        writeData();
        break;
      }
    case MSG_CMD_INFO_VERSION:
      {
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.if_cmd = MSG_CMD_INFO_VERSION;
        // read info
        strcpy_P(rawhidwriteData.if_value, version_info);
        writeData();
        break;
      }
    case MSG_CMD_INFO_PROD_DATE:
      {
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.if_cmd = MSG_CMD_INFO_PROD_DATE;
        // read info
        strcpy_P(rawhidwriteData.if_value, production_date_info);
        writeData();
        break;
      }
    default:
      {
        Log.error("msg info cmd error, cmd is %d\n", rawhidData.if_cmd);
        break;
      }
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
      case MSG_TYPE_FUNC:
        {
          FuncProcess();
          break;
        }
      case MSG_TYPE_INFO:
        {
          InfoProcess();
          break;
        }
      default:
        {
          Log.error("msg type error, type is %d\n", rawhidData.type);
          break;
        }
    }
    Log.verbose("read data sucessful\n");

  }
  else
  {
    Log.verbose("read data error\n");
  }

  Log.verbose("read data size %d\n", bytesAvailable);
}
