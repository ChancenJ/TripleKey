#ifndef __APP_NEWWEATHER_H
#define __APP_NEWWEATHER_H
#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "board_def.h"
#include <ArduinoUZlib.h>

struct NowWeather
{
    String temp;        // 温度
    String weathertext; // 天气状况
    String winddir;     // 风向
    String windscale;   // 风力等级
    String humidity;    // 相对湿度
    String aqi;        // 空气质量指数
    String aircategory; // 空气质量等级
    NowWeather() : temp("88"), weathertext("NA"), winddir("NA"), windscale(-1), humidity(-1), aqi(-1), aircategory("NA") {}
};

struct DayWeather
{
    String tempmax;      // 最高温度
    String tempmin;      // 最低温度
    String sunrise;      // 日出时间
    String sunset;       // 日落时间
    String weatherday;   // 白天天气状况
    String weathernight; // 夜间天气状况
    DayWeather() : tempmax(88), tempmin(88), sunrise("NA"), sunset("NA"), weatherday("NA"), weathernight("NA") {}
};

struct Weather
{
    String cityname_cn;
    String citycode;
    NowWeather nowweather;
    DayWeather day3weather[3];
};

String getCityCode();
void getNowWeather(String citycode, NowWeather *nowweather);
void getNowAir(String citycode, NowWeather *nowweather);

void getWeather(NowWeather *nowweather);

String ProcessGzip(HTTPClient &httpClient);

#endif