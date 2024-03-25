#ifndef __APP_MIJIA_H
#define __APP_MIJIA_H
#include <Arduino.h>

#include "board_def.h"

void app_mijia_short(uint8_t pin);
void app_mijia_double(uint8_t pin);
void app_mijia_long(uint8_t pin);
void app_mijia_control(uint8_t pin,uint8_t type);


#endif