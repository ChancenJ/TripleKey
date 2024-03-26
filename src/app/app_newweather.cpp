#include "app_newweather.h"

int16_t bgColor = BLACK;

String cityName;
String temp;
String temp_h;
String temp_l;
String humidity;

static String weather;
String weatherCode;
int pm25 = 0; // PM2.5空气指数

int frameIndex = 0;
long lastAnimTime;

// String cityCode="101190203";

void getCityWeather(String cityCode)
{


    String URL = "http://chenyuebo.cn:8080/hello240/api/weather?cityCode=" + cityCode; // 原来
    // 创建 HTTPClient 对象
    HTTPClient httpClient;
    WiFiClient wifiClient;
    httpClient.begin(wifiClient, URL);
    // 启动连接并发送HTTP请求
    int httpCode = httpClient.GET();
    Serial.println("正在获取天气数据");
    Serial.println(URL);

    // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK)
    {
        String str = httpClient.getString();
        Serial.println(str);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, str);
        JsonObject json = doc.as<JsonObject>();
        JsonObject data = json["data"].as<JsonObject>();
        cityName = data["cityName"].as<String>();
        temp = data["temp"].as<String>();
        humidity = data["SD"].as<String>();
        weather = data["weather"].as<String>();
        weatherCode = data["weatherCode"].as<String>();
        String aqi_pm25 = data["aqi_pm25"].as<String>();
        pm25 = atoi(aqi_pm25.c_str());

        // scrollText[0] = "实时天气 " + weather;
        // scrollText[1] = "空气质量 " + aqi_pm25;
        // scrollText[2] = "风向 " + data["WD"].as<String>() + data["WS"].as<String>();
        // scrollText[3] = "今日 " + weather;
        // scrollText[4] = "最高温度" + data["tempH"].as<String>() + "℃";
        // scrollText[5] = "最低温度" + data["tempL"].as<String>() + "℃";

        // Serial.printf("cityName=%s\n", cityName.c_str());
        // Serial.printf("temp=%s\n", temp.c_str());
        // Serial.printf("date=%s\n", date.c_str());
        // Serial.printf("week=%s\n", week.c_str());
        Serial.println("获取成功");
        
    }
    else
    {
        Serial.println("请求城市天气错误：");
        Serial.print(httpCode);
    }
    // 关闭ESP8266与服务器连接
    httpClient.end();

}