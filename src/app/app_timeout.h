#ifndef __APP_TIMEOUT_H
#define __APP_TIMEOUT_H


#include <Arduino.h>
#include "SoftwareTimer.h"

extern SoftwareTimer timeoutTimer;

#define TIME_SLEEP (10*1000)

void app_timeout_init(TimerCallbackFunction_t cb);
void app_timeout_reset();
void app_timeout_stop();

bool app_timeout_getState();
void app_timeout_SetState(bool state);
#endif
