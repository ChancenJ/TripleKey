#include "app_mijia.h"

QueueHandle_t Queue_Mijia;
TaskHandle_t Handle_mijia_long;

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
    Serial.printf("米家单击KEY%d\n", pin + 1);
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
#ifdef SUPPORT_CH423S
    ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
    // Serial.println(~(1 << pin), BIN);
    delay(100);
    ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
#endif
}
void app_mijia_double(uint8_t pin)
{
    Serial.printf("米家双击KEY%d\n", pin + 1);
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
#ifdef SUPPORT_CH423S
    ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
    // Serial.println(~(1 << pin), BIN);
    delay(100);
    ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
    delay(100);
    ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
    delay(100);
    ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
#endif
}
void Task_mijia_long(void *pvParameters)
{
    uint8_t pin;
    while (1)
    {
        if (xQueueReceive(Queue_Mijia, &pin, 0))
        {
            ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
            // Serial.println(~(1 << pin), BIN);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
            ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
        }
    }
}
void app_mijia_long(uint8_t pin)
{
    Serial.printf("米家长按KEY%d\n", pin + 1);
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
#ifdef SUPPORT_CH423S
    uint8_t _pin = pin;
    xQueueSend(Queue_Mijia, &_pin, 10);
#endif
}

uint8_t app_mijia_get(uint8_t pin)
{
#ifdef SUPPORT_PCF8574
    uint8_t value;
    Wire.requestFrom(PCF8574_ADDRESS, 1); // 请求从PCF8574读取一个字节
    if (Wire.available())
    {
        value = Wire.read(); // 读取引脚状态
    }
    uint8_t state = (value & (1 << pin)) >> pin;
    // Serial.printf("pin%d:%d\n", pin, state);
    return state;
#endif
#ifdef SUPPORT_CH423S
    uint8_t value = ch423.digitalRead(ch423.eGPIOTotal);
    uint8_t state = (value & (1 << pin)) >> pin;
    // Serial.printf("pin%d:%d\n", pin, state);
    return state;
#endif
}