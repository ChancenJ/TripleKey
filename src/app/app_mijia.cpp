#include "app_mijia.h"

void app_mijia_control(uint8_t pin, uint8_t type)
{
    if (type == 1)
    {
        app_mijia_short(pin);
    }
    else if (type == 2)
    {
        app_mijia_double(pin);
    }
    else if (type == 3)
    {
        app_mijia_long(pin);
    }
}

void app_mijia_short(uint8_t pin)
{
    Serial.println("米家单击");
    // digitalWrite(pin, LOW);
    // delay(100);
    // digitalWrite(pin, HIGH);
    // delay(100);
#ifdef SUPPORT_PCF8574
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    Serial.println(~(1 << pin), BIN);
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
#endif
}
void app_mijia_double(uint8_t pin)
{
    Serial.println("米家双击");
    // digitalWrite(pin, LOW);
    // delay(100);
    // digitalWrite(pin, HIGH);
    // delay(100);
    // digitalWrite(pin, LOW);
    // delay(100);
    // digitalWrite(pin, HIGH);
    // delay(100);
#ifdef SUPPORT_PCF8574
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    Serial.println(~(1 << pin), BIN);
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
#endif
}
void app_mijia_long(uint8_t pin)
{
    Serial.println("米家长按");
    // digitalWrite(pin, LOW);
    // uint16_t pressStartTime = (uint16_t)millis(); // 记录按下操作的开始时间，并转换为uint16_t
    // while ((uint16_t)millis() - pressStartTime < 2000) {}
    // digitalWrite(pin, HIGH);
    // delay(100);
#ifdef SUPPORT_PCF8574
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    Serial.println(~(1 << pin), BIN);
    uint16_t pressStartTime = (uint16_t)millis(); // 记录按下操作的开始时间，并转换为uint16_t
    while ((uint16_t)millis() - pressStartTime < 2000)
    {
    }
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
#endif
}

void app_mijia_get(uint8_t pin){
    uint8_t value;
    Wire.requestFrom(PCF8574_ADDRESS, 1); // 请求从PCF8574读取一个字节
  if (Wire.available()) {
    value = Wire.read(); // 读取引脚状态
  }
  Serial.println(value & (1 << pin),BIN);
}