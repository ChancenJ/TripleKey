#ifndef __APP_LED_H
#define __APP_LED_H
#include <Arduino.h>

#include "board_def.h"


#ifdef SUPPORT_WS2812
#include <Adafruit_NeoPixel.h>
#define LED1 2
#define LED2 1
#define LED3 0

// extern Adafruit_NeoPixel pixels;
#endif



void app_led_init(void);
void app_led_set(uint16_t id, uint32_t color);
uint32_t app_led_color(uint8_t r,uint8_t g, uint8_t b);
void app_led_update(void);
void app_led_off(void);


void rainbow(uint8_t wait);

#endif