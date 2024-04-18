#ifndef __APP_WEATHER_H
#define __APP_WEATHER_H
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
    String weathercode; //图标代码
    String winddir;     // 风向
    String windscale;   // 风力等级
    String humidity;    // 相对湿度
    String aqi;        // 空气质量指数
    String aircategory; // 空气质量等级
    NowWeather() : temp("88"), weathertext("NA"), weathercode("NA"),winddir("NA"), windscale("-1"), humidity("-1"), aqi("-1"), aircategory("NA") {}
};

struct DayWeather
{
    String tempmax;      // 最高温度
    String tempmin;      // 最低温度
    String sunrise;      // 日出时间
    String sunset;       // 日落时间
    String weatherday;   // 白天天气状况
    String daycode;   // 白天天气代码
    String weathernight; // 夜间天气状况
    String nightcode;   // 夜间天气代码
    DayWeather() : tempmax("88"), tempmin("88"), sunrise("NA"), sunset("NA"), weatherday("NA"), weathernight("NA"),daycode("NA"),nightcode("NA") {}
};

struct Weather
{
    String cityname_cn;
    String citycode;
    NowWeather nowweather;
    DayWeather day3weather[3];
    Weather():cityname_cn("NA"),citycode("NA"){}
};

void getCity(Weather *weather);
void getNowWeather(Weather *weather);
void getNowAir(Weather *weather);
void getDay3Weather(Weather *weather);


String ProcessGzip(HTTPClient &httpClient);

#endif