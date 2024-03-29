#include "app_newweather.h"

#define NOWWEATHER "weather/now"
#define DAY3WEATHER "weather/3d"
#define NOWAIR "air/now"

const String weatherapi = "https://devapi.qweather.com/v7/";
const String citycodeapi = "https://geoapi.qweather.com/v2/city/lookup?location=";
// const String apikey = "6c643fe9ed644de789830619cbea6a95";
//  const String cityname="宜兴";
//  const String citycode="101190203";

void getCity(Weather *weather)
{
    String city, apikey;
    if (stored_weather_city[0] == '\0')
    {
        city = "yixing";
        apikey = "6c643fe9ed644de789830619cbea6a95";
    }
    else
    {
        city = stored_weather_city;
        apikey = stored_weather_key;
    }
    String URL = citycodeapi + city + "&key=" + apikey;
    HTTPClient httpClient;
    httpClient.begin(URL);
    // 启动连接并发送HTTP请求
    int httpCode = httpClient.GET();
    Serial.println("正在获取城市代码");
    Serial.println(URL);
    // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK)
    {
        String payload = ProcessGzip(httpClient);
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, payload);
        JsonObject json = doc.as<JsonObject>();
        int16_t hefengcode = json["code"].as<int16_t>();
        if (hefengcode == HTTP_CODE_OK)
        {
            JsonArray location = json["location"].as<JsonArray>();
            weather->citycode = location[0]["id"].as<String>();
            weather->cityname_cn = location[0]["name"].as<String>();
            Serial.println(weather->cityname_cn + ":" + weather->citycode);
        }
        else
        {
            Serial.println("请求城市代码错误：");
            Serial.print(hefengcode);
        }
    }
    else
    {
        Serial.println("请求城市代码错误：");
        Serial.print(httpCode);
    }
    httpClient.end();
}

void getNowWeather(Weather *weather)
{
    String apikey;
    if (stored_weather_key[0] == '\0')
    {
        apikey = "6c643fe9ed644de789830619cbea6a95";
    }
    else
    {
        apikey = stored_weather_key;
    }
    String URL = weatherapi + NOWWEATHER + "?location=" + weather->citycode + "&key=" + apikey;
    // String URL = "http://chenyuebo.cn:8080/hello240/api/weather?cityCode=" + cityCode; // 原来
    //  创建 HTTPClient 对象
    HTTPClient httpClient;
    httpClient.begin(URL);
    // 启动连接并发送HTTP请求
    int httpCode = httpClient.GET();
    Serial.println("正在获取实时天气数据");
    Serial.println(URL);
    // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK)
    {
        // String str = httpClient.getString();
        String payload = ProcessGzip(httpClient);
        Serial.println(payload);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        JsonObject json = doc.as<JsonObject>();
        int16_t hefengcode = json["code"].as<int16_t>();
        if (hefengcode == HTTP_CODE_OK)
        {
            JsonObject now = json["now"].as<JsonObject>();
            weather->nowweather.temp = now["temp"].as<String>();
            weather->nowweather.weathertext = now["text"].as<String>();
            weather->nowweather.weathercode = now["icon"].as<String>();
            weather->nowweather.winddir = now["windDir"].as<String>();
            weather->nowweather.windscale = now["windScale"].as<String>();
            weather->nowweather.humidity = now["humidity"].as<String>();
            Serial.println("获取成功");
        }
        else
        {
            Serial.println("请求实时天气错误：");
            Serial.print(hefengcode);
        }
    }
    else
    {
        Serial.println("请求实时天气错误：");
        Serial.print(httpCode);
    }
    httpClient.end();
}

void getNowAir(Weather *weather)
{
    String apikey;
    if (stored_weather_key[0] == '\0')
    {
        apikey = "6c643fe9ed644de789830619cbea6a95";
    }
    else
    {
        apikey = stored_weather_key;
    }
    String URL = weatherapi + NOWAIR + "?location=" + weather->citycode + "&key=" + apikey;
    HTTPClient httpClient;
    httpClient.begin(URL);
    int httpCode = httpClient.GET();
    Serial.println("正在获取实时空气数据");
    Serial.println(URL);
    // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK)
    {
        // String str = httpClient.getString();
        String payload = ProcessGzip(httpClient);
        Serial.println(payload);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        JsonObject json = doc.as<JsonObject>();
        int16_t hefengcode = json["code"].as<int16_t>();
        if (hefengcode == HTTP_CODE_OK)
        {
            JsonObject now = json["now"].as<JsonObject>();
            weather->nowweather.aqi = now["aqi"].as<String>();
            weather->nowweather.aircategory = now["category"].as<String>();
            Serial.println("获取成功");
        }
        else
        {
            Serial.println("请求实时空气错误：");
            Serial.print(hefengcode);
        }
    }
    else
    {
        Serial.println("请求实时空气错误：");
        Serial.print(httpCode);
    }
    httpClient.end();
}

void getDay3Weather(Weather *weather)
{
    String apikey;
    if (stored_weather_key[0] == '\0')
    {
        apikey = "6c643fe9ed644de789830619cbea6a95";
    }
    else
    {
        apikey = stored_weather_key;
    }
    String URL = weatherapi + DAY3WEATHER + "?location=" + weather->citycode + "&key=" + apikey;
    HTTPClient httpClient;
    httpClient.begin(URL);
    int httpCode = httpClient.GET();
    Serial.println("正在获取三天预报数据");
    Serial.println(URL);
    // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK)
    {
        // String str = httpClient.getString();
        String payload = ProcessGzip(httpClient);
        Serial.println(payload);
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, payload);
        JsonObject json = doc.as<JsonObject>();
        int16_t hefengcode = json["code"].as<int16_t>();
        if (hefengcode == HTTP_CODE_OK)
        {
            JsonArray daily = json["daily"].as<JsonArray>();
            for (int i = 0; i < 3; i++)
            {
                JsonObject day = daily[i].as<JsonObject>();
                weather->day3weather[i].tempmax = day["tempMax"].as<String>();
                weather->day3weather[i].tempmin = day["tempMin"].as<String>();
                weather->day3weather[i].sunrise = day["sunrise"].as<String>();
                weather->day3weather[i].sunset = day["sunset"].as<String>();
                weather->day3weather[i].weatherday = day["textDay"].as<String>();
                weather->day3weather[i].weathernight = day["textNight"].as<String>();
                weather->day3weather[i].daycode = day["iconDay"].as<String>();
                weather->day3weather[i].nightcode = day["iconNight"].as<String>();
            }
            Serial.println("获取成功");
        }
        else
        {
            Serial.println("请求三天预报错误：");
            Serial.print(hefengcode);
        }
    }
    else
    {
        Serial.println("请求三天预报错误：");
        Serial.print(httpCode);
    }
    httpClient.end();
}

String ProcessGzip(HTTPClient &httpClient)
{
    WiFiClient *stream = httpClient.getStreamPtr();
    int size = httpClient.getSize();
    uint8_t inbuff[size];
    stream->readBytes(inbuff, size);
    uint8_t *outbuf = NULL;
    uint32_t out_size = 0;
    int result = ArduinoUZlib::decompress(inbuff, size, outbuf, out_size);
    String payload = String(outbuf, out_size);
    stream->stop();
    return payload;
}

// cityName = data["cityName"].as<String>();
// temp = data["temp"].as<String>();
// humidity = data["SD"].as<String>();
// weather = data["weather"].as<String>();
// weatherCode = data["weatherCode"].as<String>();
// String aqi_pm25 = data["aqi_pm25"].as<String>();
// pm25 = atoi(aqi_pm25.c_str());

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