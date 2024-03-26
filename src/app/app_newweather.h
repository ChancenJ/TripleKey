#ifndef __APP_NEWWEATHER_H
#define __APP_NEWWEATHER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "board_def.h"


void getCityWeather(String cityCode);



#endif