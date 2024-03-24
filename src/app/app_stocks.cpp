#include "app_stocks.h"

void initializeStocks()
{
    Stocks[0] = StockInfo("上证指数", "sh000001");
    Stocks[1] = StockInfo("深证成指", "sz399001");
    Stocks[2] = StockInfo("创业板指", "sz399006");
    Stocks[3] = StockInfo("中证白酒", "sz399997");
    Stocks[4] = StockInfo("中证医疗", "sz399989");
    Stocks[5] = StockInfo("科创100", "sh000698");
    Stocks[6] = StockInfo("科创50", "sh000688");
    Stocks[7] = StockInfo("沪深300", "sz399300");
    Stocks[8] = StockInfo("半导体ETF", "sh512480");
    Stocks[9] = StockInfo("中证消费", "sh000932");
    Stocks[10] = StockInfo("中证军工", "sz399967");
    Stocks[11] = StockInfo("新能源车LOF", "sz161028");
    // Stocks[11] = StockInfo("中证yh","sz399986");
}

// void StocksPage::onStart()
// {
//     Serial.println("StocksPage::onStart()");
//     lastUpdateTime = LONG_MIN; // 重置时间
//     lastScrollTime = LONG_MIN;
//     scrollIndex = 0;
//     firstgot = 0;
//     _tft->fillScreen(TFT_BLACK);                // 全屏黑色
//     _tft->fillRect(0, 0, 240, 30, TFT_SKYBLUE); // 系统状态栏背景色

//     // 状态栏时间
//     _tft->setTextDatum(BL_DATUM);
//     _tft->setTextColor(TFT_BLUE, TFT_SKYBLUE);
//     String hms = _ntpClient->getFormattedTime();
//     _tft->drawString(hms, 10, 30, 4);
//     _tft->loadFont(FontZhongsong_25);
//     _tft->drawString(title, 130, 27.5);
//     _tft->unloadFont();

//     initializeStocks();
//     getInfo(Stocks);
//     firstgot = 1;
//     for (int i = 0; i < 4; i++)
//     {
//         drawInfo(10, 40 + 50 * i, Stocks[i]);
//     }
// }

// void StocksPage::onLoop()
// {

//     _tft->setTextDatum(BL_DATUM); // 设置文本数据
//     _tft->setTextColor(TFT_BLUE, TFT_SKYBLUE);
//     String hms = _ntpClient->getFormattedTime();
//     _tft->drawString(hms, 10, 30, 4);
//     getInfo(Stocks);
//     Scroll();
// }

// void StocksPage::onStop()
// {
//     _tft->unloadFont();
//     Serial.println("StocksPage::onStop()");
// }

// void drawInfo(int32_t x, int32_t y, StockInfo stock)
// {
//     uint16_t color = (stock.difference.toFloat() > 0) ? TFT_RED : ((stock.difference.toFloat() < 0) ? TFT_DARKGREEN : TFT_DARKGREY);
//     _sprite->createSprite(220, 40);
//     _sprite->loadFont(FontZhongsong_25);
//     _sprite->fillSprite(color);
//     _sprite->setTextDatum(ML_DATUM);
//     _sprite->setTextColor(TFT_WHITE, color);
//     _sprite->drawString(stock.name, 25, 20);
//     if (stock.difference.toFloat() > 0)
//     {
//         _sprite->drawString("▲", 2, 20);
//     }
//     if (stock.difference.toFloat() < 0)
//     {
//         _sprite->drawString("▼", 2, 20);
//     }
//     _sprite->unloadFont();
//     _sprite->setTextDatum(MR_DATUM);
//     _sprite->loadFont(FontXihei_17);
//     _sprite->drawString(stock.price, 215, 10);
//     //_sprite->drawString(stock.difference, 215, 30);
//     _sprite->drawString(stock.percentage, 215, 30);
//     _sprite->pushSprite(x, y);
//     _sprite->unloadFont();
//     _sprite->deleteSprite();
// }

void Scroll()
{ // 轮播股票
    if (millis() - lastScrollTime > 10 * 1000)
    {
        lastScrollTime = millis();
        if (scrollIndex > ceil(STOCKSNUM / 4))
        {
            scrollIndex = 0;
        }
        for (int i = 0; i < 4; i++)
        {
            if (i + 4 * scrollIndex < STOCKSNUM)
            {
                drawInfo(10, 40 + 50 * i, Stocks[i + 4 * scrollIndex]);
            }
        }
        scrollIndex++;
    }
}

void getInfo(StockInfo Stocks[])
{
    if (millis() - lastUpdateTime < 2 * 60 * 1000)
    {
        // 限制请求频率
        return;
    }
    lastUpdateTime = millis();

    int hour = timeInfo.tm_hour;
    int minute = timeInfo.tm_min;
    if ((hour >= 16 || hour <= 8 || hour == 12 || (hour == 9 && minute <= 20) || (hour == 15 && minute >= 35)) && firstgot == 1)
    {
        Serial.println("休市时间暂停获取数据");
        return; // 限制请求时间
    }

    for (int i = 0; i < STOCKSNUM; i++)
    {
        String URL = "http://hq.sinajs.cn/list=" + Stocks[i].id;
        // String URL = "http://api.biyingapi.com/zs/sssj/" + stock.id + "/f37609405e1d10e291";
        //  创建 HTTPClient 对象
        WiFiClient wifiClient;
        HTTPClient httpClient;
        httpClient.begin(wifiClient, URL);
        httpClient.addHeader("referer", "https://finance.sina.com.cn");
        //  启动连接并发送HTTP请求
        int httpCode = httpClient.GET();
        Serial.println("正在获取" + Stocks[i].name);
        Serial.println(URL);

        // 如果服务器响应OK则从服务器获取响应体信息并通过串口输出
        if (httpCode == HTTP_CODE_OK)
        {
            String str = httpClient.getString();
            str = sinaStringtoJson(str);
            Serial.println(str);
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, str);
            Stocks[i].price = doc["p"].as<String>();
            Stocks[i].difference = doc["ud"].as<String>();
            Stocks[i].percentage = doc["pc"].as<String>();
        }
    }
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
    char buff[8];
    if (in > 100 || flag == 1)
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