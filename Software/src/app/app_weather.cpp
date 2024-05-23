#include "app_weather.h"

#define NOWWEATHER "weather/now"
#define DAY3WEATHER "weather/3d"
#define NOWAIR "air/now"
#define WARNING "warning/now"

const String weatherapi = "https://devapi.qweather.com/v7/";
const String citycodeapi = "https://geoapi.qweather.com/v2/city/lookup?location=";

void getCity(Weather *weather)
{
    String city, apikey;
    city = stored_weather_city;
    apikey = stored_weather_key;
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
    apikey = stored_weather_key;
    String URL = weatherapi + NOWWEATHER + "?location=" + weather->citycode + "&key=" + apikey;
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
    apikey = stored_weather_key;
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
    apikey = stored_weather_key;
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

void getWarning(Weather *weather)
{
    String apikey;
    apikey = stored_weather_key;
    String URL = weatherapi + WARNING + "?location=" + weather->citycode + "&lang=en&key=" + apikey;
    HTTPClient httpClient;
    httpClient.begin(URL);
    int httpCode = httpClient.GET();
    Serial.println("正在气象灾害预警：");
    Serial.println(URL);
    if (httpCode == HTTP_CODE_OK)
    {
        String payload = ProcessGzip(httpClient);
        Serial.println(payload);
        weather->warnings.clear();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, payload);
        JsonObject json = doc.as<JsonObject>();
        int16_t hefengcode = json["code"].as<int16_t>();
        if (hefengcode == HTTP_CODE_OK)
        {
            JsonArray warnings = json["warning"].as<JsonArray>();
            for (int i = 0; i < warnings.size(); i++)
            {
                JsonObject warningjson = warnings[i].as<JsonObject>();
                Warning warning;
                warning.color = warningjson["severityColor"].as<String>();
                warning.type = warningjson["typeName"].as<String>();
                String stime = warningjson["startTime"].as<String>();
                uint8_t index_1 = stime.indexOf('T');
                uint8_t index_2 = stime.indexOf('+');
                warning.starttime = stime.substring(0, index_1) + " " + stime.substring(index_1 + 1, index_2);
                warning.title = warningjson["title"].as<String>();
                weather->warnings.push_back(warning);
            }
            Serial.println("获取成功");
        }
        else
        {
            Serial.println("请求预警信息错误：");
            Serial.print(hefengcode);
        }
    }
    else
    {
        Serial.println("请求预警信息错误：");
        Serial.print(httpCode);
    }
    httpClient.end();
}

void getCNWarningTitle(Weather *weather){
    String apikey;
    apikey = stored_weather_key;
    String URL = weatherapi + WARNING + "?location=" + weather->citycode + "&lang=cn&key=" + apikey;
    HTTPClient httpClient;
    httpClient.begin(URL);
    int httpCode = httpClient.GET();
    Serial.println("正在气象灾害预警名称：");
    Serial.println(URL);
    if (httpCode == HTTP_CODE_OK)
    {
        String payload = ProcessGzip(httpClient);
        Serial.println(payload);
        DynamicJsonDocument doc(2048);
        deserializeJson(doc, payload);
        JsonObject json = doc.as<JsonObject>();
        int16_t hefengcode = json["code"].as<int16_t>();
        if (hefengcode == HTTP_CODE_OK)
        {
            JsonArray warnings = json["warning"].as<JsonArray>();
            for (int i = 0; i < warnings.size(); i++)
            {
                JsonObject warningjson = warnings[i].as<JsonObject>();
                weather->warnings[i].title = warningjson["title"].as<String>();
            }
            Serial.println("获取成功");
        }
        else
        {
            Serial.println("请求预警名称错误：");
            Serial.print(hefengcode);
        }
    }
    else
    {
        Serial.println("请求预警名称错误：");
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