#ifndef StocksPage_H
#define StocksPage_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "board_def.h"

#define STOCKSNUM 12

struct StockInfo
{
    String name;       // 股票名称
    String id;         // 股票代码;
    String price;      // 当前价
    String percentage; // 涨跌幅
    String difference; // 涨跌额
    StockInfo() : name(), id(), price("0.00"), percentage("0.00"), difference("0.00") {}
    StockInfo(String inname, String inid) : name(inname), id(inid), price("0.00"), percentage("0.00"), difference("0.00") {}
};

StockInfo Stocks[STOCKSNUM];

long lastUpdateTime;
long lastScrollTime;
int scrollIndex = 0;
int firstgot = 0; // 首次是否已获取数据

void initializeStocks();
void getInfo(StockInfo Stocks[]);
void drawInfo(int32_t x, int32_t y, StockInfo stock);
void Scroll();
String sinaStringtoJson(String &s);
String SetPrecison(float in, int flag);



#endif