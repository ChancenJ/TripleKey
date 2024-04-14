#ifndef __APP_SETTINGS_H
#define __APP_SETTINGS_H
#include <Arduino.h>
#include "board_def.h"
#include "app_mijia.h"
#include "app_stocks.h"



String readConfig(const char *config_path);
String readHTML(const char *html_path);
void saveConfig(const char *config_path, String config_content);
void handleRoot(AsyncWebServerRequest *request);
void handleConfigPost(AsyncWebServerRequest *request);
void notFoundHandler(AsyncWebServerRequest *request);
void handleWeb(AsyncWebServerRequest *request);
std::vector<String> SplitString(String origin, char flag);
void AnalyzeStocksConfig();
void AnalyzeMijiaConfig();



#endif