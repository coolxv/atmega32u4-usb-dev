#include <avr/wdt.h>
#include <EEPROM.h>
#include "HID-Project.h"
//#define DISABLE_LOGGING
#include "ArduinoLog.h"
#include <xxtea-lib.h>



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
#define  MSG_CMD_ENCRYP_RESET_LOCK 1
#define  MSG_CMD_ENCRYP_INIT_LOCK 2
#define  MSG_CMD_ENCRYP_READ_STR 3
#define  MSG_CMD_ENCRYP_WRITE_STR 4
#define  MSG_CMD_ENCRYP_RESET_KEY 5
#define  MSG_CMD_ENCRYP_INIT_KEY 6
#define  MSG_CMD_ENCRYP_ENC_STR 7
#define  MSG_CMD_ENCRYP_DEC_STR 8

#pragma pack(push, 1)
typedef union {
  unsigned char type;
  //mouse
  struct {
    unsigned char ms_type;
    unsigned char ms_cmd;//right,left...
    //use
    unsigned char ms_count;
    short ms_x;
    short ms_y;
    char ms_wheel;
  };
  //keyboard
  struct {
    unsigned char kb_type;
    unsigned char kb_cmd;//keyboard:down,up,press...
    //use
    unsigned char kb_count;
    unsigned short kb_key[6];
  };
  //log
  struct {
    unsigned char lg_type;
    unsigned char lg_cmd;
    //use
    unsigned char lg_level;
  };
  //func
  struct {
    unsigned char fc_type;
    unsigned char fc_cmd;
    //use
    union {
      unsigned char fc_value[4];
      unsigned short fc_vidpid[2];
      unsigned char fc_serial[USB_SERIAL_LEN_MAX + 1];
      unsigned char fc_product[USB_PRODUCT_LEN_MAX + 1];
      unsigned char fc_manufacturer[USB_MANUFACTURER_LEN_MAX + 1];
    };
  };
  //info
  struct {
    unsigned char if_type;
    unsigned char if_cmd;
    //use
  };
  //data
  struct {
    unsigned char dt_type;
    unsigned char dt_cmd;
    //use
    unsigned char dt_index;
    unsigned char dt_rpwd[9];
    unsigned char dt_wpwd[9];
    unsigned char dt_kkey[9];
    unsigned char dt_len;
    unsigned char dt_buf[33];
  };
  unsigned char buf[64];
} MSG_DATA_T;


typedef struct {
  unsigned char type;
  unsigned char cmd;
  unsigned char error;
  unsigned char pad;
  union
  {
    //keyboard
    struct {
      unsigned char kb_ret;
    };
    //info

    union {
      unsigned char if_value[48];
      unsigned short if_vidpid[2];
    };

    //data
    struct {
      unsigned char dt_len;
      unsigned char dt_buf[33];
    };
    unsigned char buf[60];
  };

} MSG_DATA_RESULT_T;
#pragma pack(pop)


//data addr define
#define DATA_KEY_TAG_ADDR 100
#define DATA_KEY_ADDR 102
#define DATA_KEY_LEN_MAX 8

#define DATA_WRITE_PWD_TAG_ADDR 110
#define DATA_WRITE_PWD_ADDR 112
#define DATA_WRITE_PWD_LEN_MAX 8

#define DATA_READ_PWD_TAG_ADDR 120
#define DATA_READ_PWD_ADDR 122
#define DATA_READ_PWD_LEN_MAX 8

#define DATA_DATA_TAG_ADDR(index) (130 + (index - 1)*32)
#define DATA_DATA_ADDR(index) (130 + (index - 1)*32 + 2)
#define DATA_DATA_LEN_MAX 32
//info define
#define DEV_MODEL_BASE_INFO "base"
#define DEV_MODEL_ADVANCE_INFO "advance"
#define DEV_VERSION_INFO "1.0.0"
#define DEV_PRODUCTION_DATE_INFO "20190315"

//global data
const int pinLed = LED_BUILTIN;
MSG_DATA_T rawhidreadData;
MSG_DATA_RESULT_T rawhidwriteData;

int delay_time = 0;
bool delay_restart = false;
void(* resetFunc) (void) = 0;

// save some chars
const char sn_info[] PROGMEM  = {USB_SERIAL};
const char model_info[] PROGMEM  = {DEV_MODEL_BASE_INFO};
const char version_info[] PROGMEM  = {DEV_VERSION_INFO};
const char production_date_info[] PROGMEM  = {DEV_PRODUCTION_DATE_INFO};

#if 0
// Function Pototype
void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));
// Function Implementation
void wdt_init(void)
{
  MCUSR = 0;
  wdt_disable();
  return;
}
#endif
#define soft_reset()        \
  do                        \
  {                         \
    wdt_enable(WDTO_15MS);  \
    for(;;)                 \
    {                       \
    }                       \
  } while(0)


void setup() {
  //led init
  pinMode(pinLed, OUTPUT);
  digitalWrite(pinLed, HIGH);
  //if (true == delay_restart && delay_time > 0) {
  //  delay_time = 0;
  //  delay_restart = false;
  //  delay(delay_time);
  //}
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
  RawHID.begin((uint8_t *)&rawhidreadData, sizeof(rawhidreadData));
  //led show
  delay(250);
  digitalWrite(pinLed, LOW);
  Log.trace("device initial successful\n");
}

int readData()
{

  auto bytesAvailable = RawHID.available();
  if (bytesAvailable > 0 && bytesAvailable <= sizeof(rawhidreadData))
  {
    for (int i = 0; i < bytesAvailable; i++)
    {
      rawhidreadData.buf[i] = (unsigned char)RawHID.read();
    }
  }
  if (bytesAvailable > 0)
  {
    Log.verbose("read data size %d\n", bytesAvailable);
  }
  return bytesAvailable;
}
int writeData()
{
  Log.verbose("write data size %d\n", sizeof(rawhidwriteData));
  return RawHID.write((unsigned char*)&rawhidwriteData, sizeof(rawhidwriteData));
}
void keyboardProcess()
{
  Log.trace("keyboard command %d\n", rawhidreadData.kb_cmd);
  switch (rawhidreadData.kb_cmd)
  {
    case MSG_CMD_KB_DOWN:
      {
        unsigned char key = (unsigned char)(0xff & rawhidreadData.kb_key[0]);
        BootKeyboard.add(KeyboardKeycode(key));
        if (rawhidreadData.kb_key[0] & 0x8000)
        {
          BootKeyboard.add(KEY_LEFT_SHIFT);
        }
        BootKeyboard.send();
        break;
      }
    case MSG_CMD_KB_UP:
      {
        unsigned char key = (unsigned char)(0xff & rawhidreadData.kb_key[0]);
        BootKeyboard.remove(KeyboardKeycode(key));
        if (rawhidreadData.kb_key[0] & 0x8000)
        {
          BootKeyboard.remove(KEY_LEFT_SHIFT);
        }
        BootKeyboard.send();
        break;
      }
    case MSG_CMD_KB_PRESS:
      {
        unsigned char key = (unsigned char)(0xff & rawhidreadData.kb_key[0]);
        //press
        BootKeyboard.add(KeyboardKeycode(key));
        if (rawhidreadData.kb_key[0] & 0x8000)
        {
          BootKeyboard.add(KEY_LEFT_SHIFT);
        }
        BootKeyboard.send();
        //release
        BootKeyboard.remove(KeyboardKeycode(key));
        if (rawhidreadData.kb_key[0] & 0x8000)
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
          if (KeyboardKeycode(rawhidreadData.kb_key[i]) != 0)
          {
            BootKeyboard.add(KeyboardKeycode(0xff & rawhidreadData.kb_key[i]));
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
          if (KeyboardKeycode(rawhidreadData.kb_key[i]) != 0)
          {
            BootKeyboard.remove(KeyboardKeycode(0xff & rawhidreadData.kb_key[i]));
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
          if (KeyboardKeycode(rawhidreadData.kb_key[i]) != 0)
          {
            BootKeyboard.add(KeyboardKeycode(0xff & rawhidreadData.kb_key[i]));
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
          if (KeyboardKeycode(rawhidreadData.kb_key[i]) != 0)
          {
            BootKeyboard.remove(KeyboardKeycode(0xff & rawhidreadData.kb_key[i]));
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
          rawhidwriteData.cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 1;
        }
        else
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 0;
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
          rawhidwriteData.cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 1;
        }
        else
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.cmd = MSG_CMD_KB_GET_CAPS_LOCK;
          rawhidwriteData.kb_ret = 0;
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
          rawhidwriteData.cmd = MSG_CMD_KB_GET_SCROLL_LOCK;
          rawhidwriteData.kb_ret = 1;
        }
        else
        {
          rawhidwriteData.type = MSG_TYPE_KEYBOARD;
          rawhidwriteData.cmd = MSG_CMD_KB_GET_SCROLL_LOCK;
          rawhidwriteData.kb_ret = 0;
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
        Log.error("msg keyboard cmd error, cmd is %d\n", rawhidreadData.kb_cmd);
        break;
      }
  }
}
void MouseProcess()
{
  Log.trace("mouse command %d\n", rawhidreadData.ms_cmd);
  switch (rawhidreadData.ms_cmd)
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
        Log.trace("mouse absolute move %d, %d\n", rawhidreadData.ms_x, rawhidreadData.ms_y);
        AbsoluteMouse.moveTo(rawhidreadData.ms_x, rawhidreadData.ms_y);
        break;
      }
    case MSG_CMD_MS_MOVE_TO_R:
      {
        Mouse.move(rawhidreadData.ms_x, rawhidreadData.ms_y);
        break;
      }
    case MSG_CMD_MS_WHEEL_MOVE:
      {
        Mouse.move(0, 0, rawhidreadData.ms_wheel);
        break;
      }
    default:
      {
        Log.error("msg keyboard cmd error, cmd is %d\n", rawhidreadData.kb_cmd);
        break;
      }
  }
}

void LogProcess()
{
  if (Serial)
  {
    Log.begin(rawhidreadData.lg_level, &Serial);
  }
  Log.trace("log level %d\n", rawhidreadData.lg_level);
}

void FuncProcess()
{
  Log.trace("func command %d\n", rawhidreadData.fc_cmd);
  switch (rawhidreadData.fc_cmd)
  {
    case MSG_CMD_FUNC_RESTART:
      {
        //delay_time = 0;
        //delay_restart = false;
        writeData();//special need
        soft_reset();
        break;
      }
    case MSG_CMD_FUNC_DISCONNECT:
      {
        //delay_time = rawhidreadData.fc_value[0] * 1000;
        //delay_restart = true;
        writeData();//special need
        soft_reset();
        break;
      }
    case MSG_CMD_FUNC_SET_DEVICE_ID:
      {
        //  DEVICE DESCRIPTOR
        DeviceDescriptor dd_struct = D_DEVICE(0xEF, 0x02, 0x01, 64, rawhidreadData.fc_vidpid[0], rawhidreadData.fc_vidpid[1], 0x100, IMANUFACTURER, IPRODUCT, ISERIAL, 1);
        //tag
        unsigned short ee_flag = 0x1277;
        unsigned char *val = (unsigned char *)&ee_flag;
        val[0] = USB_FLAGS;
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
        val[0] = USB_FLAGS;
        val[1] = USB_SERIAL_LEN_MAX;
        eeprom_write_word(ee_addr, ee_flag);
        //write serial number
        ee_addr = USB_SERIAL_ADDR;
        eeprom_write_block(rawhidreadData.fc_serial, ee_addr, USB_SERIAL_LEN_MAX);
        break;
      }
    case MSG_CMD_FUNC_RESTORE_SERIAL_NUMBER:
      {
        unsigned short ee_flag = 0;
        int addr = USB_SERIAL_TAG_ADDR;
        EEPROM.put(addr, ee_flag);
        break;
      }
    case MSG_CMD_FUNC_SET_PRODUCT:
      {
        const u16* ee_addr = USB_PRODUCT_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        //write tag
        val[0] = USB_FLAGS;
        val[1] = USB_PRODUCT_LEN_MAX;
        eeprom_write_word(ee_addr, ee_flag);
        //write product
        ee_addr = USB_PRODUCT_ADDR;
        eeprom_write_block(rawhidreadData.fc_product, ee_addr, USB_PRODUCT_LEN_MAX);
        break;
      }
    case MSG_CMD_FUNC_RESTORE_PRODUCT:
      {
        unsigned short ee_flag = 0;
        int addr = USB_PRODUCT_TAG_ADDR;
        EEPROM.put(addr, ee_flag);
        break;
      }
    case MSG_CMD_FUNC_SET_MANUFACTURER:
      {
        const u16* ee_addr = USB_MANUFACTURER_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        //write tag
        val[0] = USB_FLAGS;
        val[1] = USB_MANUFACTURER_LEN_MAX;
        eeprom_write_word(ee_addr, ee_flag);
        //write serial number
        ee_addr = USB_MANUFACTURER_ADDR;
        eeprom_write_block(rawhidreadData.fc_manufacturer, ee_addr, USB_MANUFACTURER_LEN_MAX);
        break;
      }
    case MSG_CMD_FUNC_RESTORE_MANUFACTURER:
      {
        unsigned short ee_flag = 0;
        int addr = USB_MANUFACTURER_TAG_ADDR;
        EEPROM.put(addr, ee_flag);
        break;
      }
    default:
      {
        Log.error("msg func cmd error, cmd is %d\n", rawhidreadData.fc_cmd);
        break;
      }
  }

}


void InfoProcess()
{
  Log.trace("info command %d\n", rawhidreadData.if_cmd);
  char one_char;
  int k;  // counter variable
  switch (rawhidreadData.if_cmd)
  {
    case MSG_CMD_INFO_SN:
      {
        const u16* ee_addr = USB_SERIAL_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        ee_flag = eeprom_read_word(ee_addr);
        // read info
        if (val[0] == USB_FLAGS)
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
        rawhidwriteData.cmd = MSG_CMD_INFO_SN;
        break;
      }
    case MSG_CMD_INFO_MODEL:
      {
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.cmd = MSG_CMD_INFO_MODEL;
        // read info
        strcpy_P(rawhidwriteData.if_value, model_info);
        break;
      }
    case MSG_CMD_INFO_VERSION:
      {
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.cmd = MSG_CMD_INFO_VERSION;
        // read info
        strcpy_P(rawhidwriteData.if_value, version_info);
        break;
      }
    case MSG_CMD_INFO_PROD_DATE:
      {
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.cmd = MSG_CMD_INFO_PROD_DATE;
        // read info
        strcpy_P(rawhidwriteData.if_value, production_date_info);
        break;
      }
    case MSG_CMD_INFO_PRODUCT:
      {
        const u16* ee_addr = USB_PRODUCT_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        ee_flag = eeprom_read_word(ee_addr);
        // read info
        if (val[0] == USB_FLAGS)
        {
          ee_addr = USB_PRODUCT_ADDR;
          eeprom_read_block(rawhidwriteData.if_value, ee_addr, USB_PRODUCT_LEN_MAX);
          rawhidwriteData.if_value[USB_PRODUCT_LEN_MAX] = 0;
        }
        else
        {
          strcpy_P(rawhidwriteData.if_value, STRING_PRODUCT);
        }
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.cmd = MSG_CMD_INFO_PRODUCT;
        break;
      }
    case MSG_CMD_INFO_MANUFACTURER:
      {
        const u16* ee_addr = USB_MANUFACTURER_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        ee_flag = eeprom_read_word(ee_addr);
        // read info
        if (val[0] == USB_FLAGS)
        {
          ee_addr = USB_MANUFACTURER_ADDR;
          eeprom_read_block(rawhidwriteData.if_value, ee_addr, USB_MANUFACTURER_LEN_MAX);
          rawhidwriteData.if_value[USB_MANUFACTURER_LEN_MAX] = 0;
        }
        else
        {
          strcpy_P(rawhidwriteData.if_value, STRING_MANUFACTURER);
        }
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.cmd = MSG_CMD_INFO_MANUFACTURER;
        break;
      }
    case MSG_CMD_INFO_DEVICE_ID:
      {
        const u16* ee_addr = USB_DEVICE_DES_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        ee_flag = eeprom_read_word(ee_addr);
        // read info
        unsigned short vidpid[2];
        if (val[0] == USB_FLAGS)
        {
          ee_addr = USB_DEVICE_DES_ADDR + 8;
          vidpid[0] = eeprom_read_word(ee_addr);
          ee_addr = USB_DEVICE_DES_ADDR + 10;
          vidpid[1] = eeprom_read_word(ee_addr);
          rawhidwriteData.if_vidpid[0] =  vidpid[0];
          rawhidwriteData.if_vidpid[1] =  vidpid[1];
        }
        else
        {
          rawhidwriteData.if_vidpid[0] = USB_VID;
          rawhidwriteData.if_vidpid[1] =  USB_PID;
        }
        rawhidwriteData.type = MSG_TYPE_INFO;
        rawhidwriteData.cmd = MSG_CMD_INFO_DEVICE_ID;
        break;
      }
    default:
      {
        Log.error("msg info cmd error, cmd is %d\n", rawhidreadData.if_cmd);
        break;
      }
  }

}


void DataProcess()
{
  Log.trace("data command %d\n", rawhidreadData.dt_cmd);
  switch (rawhidreadData.dt_cmd)
  {
    case MSG_CMD_ENCRYP_RESET_LOCK:
      {
        {
          const u16* ee_addr = DATA_WRITE_PWD_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          //write tag
          val[0] = 0;
          val[1] = 0;
          eeprom_write_word(ee_addr, ee_flag);
        }
        {
          const u16* ee_addr = DATA_READ_PWD_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          //write tag
          val[0] = 0;
          val[1] = 0;
          eeprom_write_word(ee_addr, ee_flag);
        }
        {
          for (int i = 1; i < 17; i++)
          {
            const u16* ee_addr = DATA_DATA_TAG_ADDR(i);
            unsigned short ee_flag;
            unsigned char *val = (unsigned char *)&ee_flag;
            //write tag
            val[0] = 0;
            val[1] = 0;
            eeprom_write_word(ee_addr, ee_flag);
          }
        }
        break;
      }

    case MSG_CMD_ENCRYP_INIT_LOCK:
      {
        {
          const u16* ee_addr = DATA_WRITE_PWD_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          //write tag
          val[0] = USB_FLAGS;
          val[1] = DATA_WRITE_PWD_LEN_MAX;
          eeprom_write_word(ee_addr, ee_flag);
          //write write pwd
          ee_addr = DATA_WRITE_PWD_ADDR;
          eeprom_write_block(rawhidreadData.dt_wpwd, ee_addr, DATA_WRITE_PWD_LEN_MAX);
        }
        {
          const u16* ee_addr = DATA_READ_PWD_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          //write tag
          val[0] = USB_FLAGS;
          val[1] = DATA_READ_PWD_LEN_MAX;
          eeprom_write_word(ee_addr, ee_flag);
          //write read pwd
          ee_addr = DATA_READ_PWD_ADDR;
          eeprom_write_block(rawhidreadData.dt_rpwd, ee_addr, DATA_READ_PWD_LEN_MAX);
        }
        break;
      }
    case MSG_CMD_ENCRYP_WRITE_STR:
      {
        {
          const u16* ee_addr = DATA_WRITE_PWD_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          ee_flag = eeprom_read_word(ee_addr);
          // read info
          unsigned char wpwd[DATA_WRITE_PWD_LEN_MAX + 1] ;
          wpwd[0] = 0;
          if (val[0] == USB_FLAGS)
          {
            ee_addr = DATA_WRITE_PWD_ADDR;
            eeprom_read_block(wpwd, ee_addr, DATA_WRITE_PWD_LEN_MAX);
            wpwd[DATA_WRITE_PWD_LEN_MAX] = 0;
          }
          if (0 != strcmp(wpwd, rawhidreadData.dt_wpwd))
          {
            rawhidwriteData.error = 1;
            break;
          }
        }

        {
          const u16* ee_addr = DATA_DATA_TAG_ADDR(rawhidreadData.dt_index);
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          //write tag
          val[0] = USB_FLAGS;
          val[1] = DATA_DATA_LEN_MAX;
          eeprom_write_word(ee_addr, ee_flag);
          //write data
          ee_addr = DATA_DATA_ADDR(rawhidreadData.dt_index);
          eeprom_write_block(rawhidreadData.dt_buf, ee_addr, DATA_DATA_LEN_MAX);
        }
        break;
      }
    case MSG_CMD_ENCRYP_READ_STR:
      {
        {
          //read tag
          const u16* ee_addr = DATA_READ_PWD_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          ee_flag = eeprom_read_word(ee_addr);
          // read info
          unsigned char rpwd[DATA_READ_PWD_LEN_MAX + 1] ;
          memset(rpwd, 0, sizeof(rpwd));
          if (val[0] == USB_FLAGS)
          {
            ee_addr = DATA_READ_PWD_ADDR;
            eeprom_read_block(rpwd, ee_addr, DATA_READ_PWD_LEN_MAX);
          }
          if (0 != strcmp(rpwd, rawhidreadData.dt_rpwd))
          {
            rawhidwriteData.error = 1;
            break;
          }
        }
        {
          const u16* ee_addr = DATA_DATA_TAG_ADDR(rawhidreadData.dt_index);
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          ee_flag = eeprom_read_word(ee_addr);
          // read info
          if (val[0] == USB_FLAGS)
          {
            ee_addr = DATA_DATA_ADDR(rawhidreadData.dt_index);
            eeprom_read_block(rawhidwriteData.dt_buf, ee_addr, DATA_DATA_LEN_MAX);
          }
        }
        break;
      }
    case MSG_CMD_ENCRYP_RESET_KEY:
      {
        const u16* ee_addr = DATA_KEY_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        //write tag
        val[0] = 0;
        val[1] = 0;
        eeprom_write_word(ee_addr, ee_flag);
        break;
      }

    case MSG_CMD_ENCRYP_INIT_KEY:
      {
        const u16* ee_addr = DATA_KEY_TAG_ADDR;
        unsigned short ee_flag;
        unsigned char *val = (unsigned char *)&ee_flag;
        //write tag
        val[0] = USB_FLAGS;
        val[1] = DATA_KEY_LEN_MAX;
        eeprom_write_word(ee_addr, ee_flag);
        //write key
        ee_addr = DATA_KEY_ADDR;
        eeprom_write_block(rawhidreadData.dt_kkey, ee_addr, DATA_KEY_LEN_MAX);
        break;
      }
    case MSG_CMD_ENCRYP_ENC_STR:
      {
        unsigned char key[DATA_KEY_LEN_MAX + 1];
        {
          const u16* ee_addr = DATA_KEY_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          ee_flag = eeprom_read_word(ee_addr);
          // read info
          if (val[0] == USB_FLAGS)
          {
            ee_addr = DATA_KEY_ADDR;
            eeprom_read_block(key, ee_addr, DATA_KEY_LEN_MAX);
            key[DATA_KEY_LEN_MAX] = 0;
          }
          else
          {
            key[0] = 0;
          }
        }

        {
          xxtea.setKey(key);
          String result = xxtea.encrypt(rawhidreadData.dt_buf);
          strcpy(rawhidwriteData.dt_buf, result.c_str());
        }

        break;
      }
    case MSG_CMD_ENCRYP_DEC_STR:
      {
        unsigned char key[DATA_KEY_LEN_MAX + 1] ;
        {
          const u16* ee_addr = DATA_KEY_TAG_ADDR;
          unsigned short ee_flag;
          unsigned char *val = (unsigned char *)&ee_flag;
          ee_flag = eeprom_read_word(ee_addr);
          // read info
          if (val[0] == USB_FLAGS)
          {
            ee_addr = DATA_KEY_ADDR;
            eeprom_read_block(key, ee_addr, DATA_KEY_LEN_MAX);
            key[DATA_KEY_LEN_MAX] = 0;
          }
          else
          {
            key[0] = 0;
          }
        }

        {
          xxtea.setKey(key);
          String result = xxtea.decrypt(rawhidreadData.dt_buf);
          strcpy(rawhidwriteData.dt_buf, result.c_str());
        }

        break;
      }
    default:
      {
        Log.error("msg data cmd error, cmd is %d\n", rawhidreadData.dt_cmd);
        break;
      }
  }

}


void loop()
{
  // Check if there is new data from the RawHID device
  auto bytesAvailable = readData();
  if (bytesAvailable && bytesAvailable == sizeof(rawhidreadData))
  {
    //set return result
    memset(&rawhidwriteData, 0 , sizeof(rawhidwriteData));
    rawhidwriteData.type = MSG_TYPE_DEFAULT;
    //execute task by type
    switch (rawhidreadData.type)
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
      case MSG_TYPE_DATA:
        {
          DataProcess();
          break;
        }
      default:
        {
          Log.error("msg type error, type is %d\n", rawhidreadData.type);
          break;
        }
    }
    //response
    writeData();
  }
}
