#ifndef __APP_SETTINGS_H
#define __APP_SETTINGS_H
#include <Arduino.h>
#include "board_def.h"


String readConfig(const char *config_path);
void handleRoot(AsyncWebServerRequest *request);


#endif