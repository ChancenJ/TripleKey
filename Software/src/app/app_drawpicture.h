#ifndef _APP_DRAWPICTURE_H
#define _APP_DRAWPICTURE_H
#include <Arduino.h>

#include "board_def.h"

void myDrawPNG(int16_t x, int16_t y, const char *path, uint8_t oled_index);
void DrawPNGCentre(const char *path, uint8_t oled_index);
void DrawJPEG(int16_t x, int16_t y, const char *path, uint8_t oled_index);
void DrawJPEGCentre(const char *path, uint8_t oled_index);

#endif