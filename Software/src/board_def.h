#ifndef __BOARD_DEF_H
#define __BOARD_DEF_H

#include <vector>

#include <WiFi.h>
#include <BleKeyboard.h>
#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>
// #include <SPIFFS.h>

#include <WiFiManager.h>
#include <ArduinoJson.h> //https://github.com/bblanchon/ArduinoJson
#include "FS.h"
#include <LittleFS.h>
// https://github.com/bitbank2/PNGdec.git
#include <PNGdec.h>
#include "JpegFunc.h"
#include <ESPAsyncWebServer.h>

#include "Button2.h"
#include <Encoder.h>
#include <AsyncTCP.h>
#include <Wire.h>
#include "DFRobot_CH423.h"

#include "fonts/Calligraffitti_Regular_12.h"
#include "fonts/Coming_Soon_Regular_12.h"
#include "fonts/MiSans_Demibold_12.h"
#include "fonts/DreamHanSerifCN_W15_21.h"
#include "fonts/MiSans_MediumEN_12.h"
#include "fonts/DreamHanSerifCN_W17_Weekday_21.h"
#include "fonts/DreamHanSerifCN_W17_Processing_24.h"
#include "fonts/MiSans_Regular45pt_num.h"
#include "fonts/DouyinSansBoldCN_Weather_38.h"
#include "fonts/GillSansEN_Bold_12.h"
#include "fonts/Guangliang_Title_26.h"
#include "fonts/Chillax_MediumEN_50.h"

#include "app/app_led.h"
#include "app/app_humansensor.h"
#include "app/app_settings.h"
#include "app/app_drawpicture.h"

#define VER_HW "TRIPLEKEY_V2"
#define VER_SW "1.0.2"

#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"


#define OLED_WIDTH 128
#define OLED_HEIGHT 128

#define QINGSHUILAN RGB565(147, 213, 220)  //清水蓝
#define JINZI RGB565(128,109,158)  //槿紫

// #define PCF8574_ADDRESS 0x20

extern char stored_weather_key[], stored_weather_city[];

extern Arduino_GFX *gfx1;
extern Arduino_GFX *gfx2;
extern Arduino_GFX *gfx3;
extern Arduino_GFX *gfx[3];

extern BleKeyboard bleKeyboard;

extern AsyncWebServer server;

extern struct tm timeInfo;

extern Button2 button1, button2, button3, button4;
extern Encoder myEnc;

extern DFRobot_CH423 ch423;
extern uint8_t HumanState;
extern uint8_t WireErr;

struct StockInfo;
struct MijiaSwitch;
extern std::vector<StockInfo> stocks;
extern std::vector<MijiaSwitch> sws;
extern std::vector<std::vector<String>> webstring;
extern bool clockaudio;
extern bool autotheme;
extern uint8_t rotary;
extern bool enc_reverse;
extern bool photoscroll;

extern String NewVersion;

void dispProcessing(uint8_t screen);

void board_init();

#endif
