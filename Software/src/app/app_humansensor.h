#ifndef __APP_HUMANSENSOR_H
#define __APP_HUMANSENSOR_H
#include <Arduino.h>

#include "board_def.h"
#include "app_mijia.h"

uint8_t getHumanSensorState();
void Mijia_UpdateHumanState();
void Screen_Control_by_HumanSensor();

#endif