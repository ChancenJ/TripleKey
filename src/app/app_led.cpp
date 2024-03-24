#include "app_led.h"
#ifdef SUPPORT_WS2812
Adafruit_NeoPixel pixels(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);
#endif
void app_led_init(void)
{
    #ifdef SUPPORT_WS2812

    pixels.begin(); // INITIALIZE NeoPixel pixels object (REQUIRED)
    pixels.clear();
    for(int i=0; i<NUM_NEOPIXEL; i++) 
    {
        pixels.setPixelColor(i, pixels.Color(random(10,255), random(10,255), random(10,255)));
        pixels.show();

  }

    #endif
}

uint32_t app_led_color(uint8_t r,uint8_t g, uint8_t b)
{
    #ifdef SUPPORT_WS2812
    return  pixels.Color(r,g,b);
    #endif

    return 0;
}

void app_led_set(uint16_t id, uint32_t color)
{
    #ifdef SUPPORT_WS2812
    pixels.setPixelColor(id, color);
    #endif
}

void app_led_update(void)
{
    #ifdef SUPPORT_WS2812
    pixels.show();
    #endif

}


void app_led_off(void)
{
    #ifdef SUPPORT_WS2812
    pixels.clear();
    pixels.show();
    #endif

}


//led mode


// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      delay(wait);

      for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

