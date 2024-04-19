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
void handleWeather(AsyncWebServerRequest *request);
void notFoundHandler(AsyncWebServerRequest *request);
void handleUploadPNG(AsyncWebServerRequest *request);
void handleListFile(AsyncWebServerRequest *request);
void handleDelete(AsyncWebServerRequest *request);
void handleMoreSettings(AsyncWebServerRequest *request);
void handleClock(AsyncWebServerRequest *request);
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final);

std::vector<String> SplitString(String origin, char flag);
void AnalyzeStocksConfig();
void AnalyzeMijiaConfig();
void AnalyzeWebConfig();
void AnalyzeMoreSettings();

String humanReadableSize(const size_t bytes);
String listFiles(bool DeleteButton);
#endif