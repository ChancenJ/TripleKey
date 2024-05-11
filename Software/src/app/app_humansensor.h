#ifndef __APP_HUMANSENSOR_H
#define __APP_HUMANSENSOR_H
#include <Arduino.h>

#include "board_def.h"

extern TaskHandle_t Handle_humanstate;
extern TaskHandle_t Handle_AutoScreenOnOff;
void Task_UpdateHumanState(void *pvParam);
void Task_Screen_Control_by_HumanSensor(void *pvParam);
#endif