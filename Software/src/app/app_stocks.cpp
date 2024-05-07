#include "app_stocks.h"

void getInfo(StockInfo *stock)
{
    String URL = "http://hq.sinajs.cn/list=" + stock->id;
    // String URL = "http://api.biyingapi.com/zs/sssj/" + stock.id + "/f37609405e1d10e291";
    //  创建 HTTPClient 对象
    WiFiClient wifiClient;
    HTTPClient httpClient;
    httpClient.begin(wifiClient, URL);
    httpClient.addHeader("referer", "https://finance.sina.com.cn");
    //  启动连接并发送HTTP请求
    int httpCode = httpClient.GET();
    Serial.println("正在获取" + stock->name);
    Serial.println(URL);

    // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
    if (httpCode == HTTP_CODE_OK)
    {
        String str = httpClient.getString();
        //Serial.println(str);
        str = sinaStringtoJson(str);
        Serial.println(str);
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, str);
        stock->price = doc["p"].as<String>();
        stock->difference = doc["ud"].as<String>();
        stock->percentage = doc["pc"].as<String>();
    }
    httpClient.end();
}

String sinaStringtoJson(String &sinastr)
{
    static const size_t capacity = JSON_OBJECT_SIZE(4) + JSON_ARRAY_SIZE(1) * 10;
    DynamicJsonDocument doc(capacity);

    int startIndex_1 = sinastr.indexOf(',') + 1;
    int endIndex_1 = sinastr.indexOf(',', startIndex_1);
    int startIndex_2 = sinastr.indexOf(',', endIndex_1) + 1;
    int endIndex_2 = sinastr.indexOf(',', startIndex_2);
    int startIndex_3 = sinastr.indexOf(',', endIndex_2) + 1;
    int endIndex_3 = sinastr.indexOf(',', startIndex_3);
    // int startIndex_4 = sinastr.indexOf(',', endIndex_3) + 1;
    // int endIndex_4 = sinastr.indexOf(',', startIndex_4);
    // int startIndex_5 = sinastr.indexOf(',', endIndex_4) + 1;
    // int endIndex_5 = sinastr.indexOf(',', startIndex_5);
    // String name = sinastr.substring(sinastr.indexOf('"') + 1, sinastr.indexOf(',')); // 股票名称

    float o = sinastr.substring(startIndex_1, endIndex_1).toFloat();  // 今日开盘价
    float yc = sinastr.substring(startIndex_2, endIndex_2).toFloat(); // 昨日收盘价
    float p = sinastr.substring(startIndex_3, endIndex_3).toFloat();  // 当前价
    // float h = sinastr.substring(startIndex_4, endIndex_4).toFloat();   // 今日最高价
    // float l = sinastr.substring(startIndex_5, endIndex_5).toFloat();   // 今日最低价
    float ud = p - yc;
    float pc = ud / yc * 100;
    
    // 构建JSON对象
    // doc["name"] = name;
    doc["o"] = SetPrecison(o, 0);         // 开盘价
    doc["yc"] = SetPrecison(yc, 0);       // 昨收盘价
    doc["p"] = SetPrecison(p, 0);         // 收盘价
    doc["ud"] = SetPrecison(ud, 0);       // 涨跌额
    doc["pc"] = SetPrecison(pc, 1) + "%"; // 涨跌幅

    // 将JSON对象转换为字符串
    String jsonString;
    serializeJson(doc, jsonString);

    return jsonString;
}
String SetPrecison(float in, int flag) // flag表示是否为涨跌幅
{
    char buff[12];
    if (abs(in) > 100 || flag == 1)
    {
        sprintf(buff, "%.2f", in);
        return String(buff);
    }
    else
    {
        sprintf(buff, "%.3f", in);
        return String(buff);
    }
}