#ifndef __BOARD_DEF_H
#define __BOARD_DEF_H

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

#include "Button2.h"
#include <Encoder.h>
#include <AsyncTCP.h>

#include "fonts/Orbitron_Light_32.h"
#include "fonts/Orbitron_Light_24.h"
#include "fonts/Orbitron_Medium_48.h"
#include "fonts/Orbitron_Medium_16.h"
#include "fonts/Orbitron_Medium_12.h"
#include "fonts/Calligraffitti_Regular_12.h"
#include "fonts/Coming_Soon_Regular_12.h"
#include "fonts/DSEG14_Classic_Regular_40.h"

#define VER_HW "2.0"
#define VER_SW "1.2.0"

#define NTP1 "ntp1.aliyun.com"
#define NTP2 "ntp2.aliyun.com"
#define NTP3 "ntp3.aliyun.com"

 

#define OLED_WIDTH 128
#define OLED_HEIGHT 128

extern char stored_weather_key[], stored_weather_city[];

extern Arduino_GFX *gfx1;
extern Arduino_GFX *gfx2;
extern Arduino_GFX *gfx3;
extern Arduino_GFX *gfx[3];

extern BleKeyboard bleKeyboard;

extern struct tm timeInfo;

extern Button2 button1, button2, button3, button4;
extern Encoder myEnc;

void PNGDraw1(PNGDRAW *pDraw);
void PNGDraw2(PNGDRAW *pDraw);
void PNGDraw3(PNGDRAW *pDraw);
void myDrawPNG(int16_t x, int16_t y, const char *path, uint8_t oled_index);

void board_init();

#endif
