#include <Arduino.h>

/*
JSON格式
deserializeJson()NotSupported当输入包含Unicode转义序列（如）时返回\u2665。

ArduinoJson可以解码Unicode转义序列，但是默认情况下此功能是禁用的，因为它会使代码更大。要启用它，必须将ARDUINOJSON_DECODE_UNICODE定义为1，如下所示：

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>
*/
#define UNIT_LEN 32
typedef struct
{
    char text_day[UNIT_LEN];   //白天天气
    char text_night[UNIT_LEN]; //夜晚
    char text_now[UNIT_LEN];   //现在

    char code_now[UNIT_LEN]; //现在天气图标代码
    char code_day[UNIT_LEN];
    char code_night[UNIT_LEN];

    char temperature_now[UNIT_LEN]; //现在温度
    char temperature_low[UNIT_LEN];
    char temperature_high[UNIT_LEN];

    char humidity_now[UNIT_LEN];

    char last_update[UNIT_LEN * 2];
    char date[UNIT_LEN];
} weather_t;
 

weather_t *app_weather_init();

int app_weather_get_now(weather_t *now);
int app_weather_get_3(weather_t *day_1, weather_t *day_2, weather_t *day_3);