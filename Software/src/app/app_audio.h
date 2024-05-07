#ifndef __APP_AUDIO_H
#define __APP_AUDIO_H
 
#include <Arduino.h>

#include "board_def.h"


void app_audio_play_wav(const char *path);
void app_audio_sayTimeCN(int hour, int minutes);
#endif