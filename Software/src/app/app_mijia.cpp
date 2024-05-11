#include "app_mijia.h"

QueueHandle_t Queue_Mijia;
TaskHandle_t Handle_mijia;

void app_mijia_control(uint8_t pin, uint8_t type)
{
    if (type == MIJIA_SHORT)
    {
        app_mijia_short(pin);
    }
    else if (type == MIJIA_DOUBLE)
    {
        app_mijia_double(pin);
    }
    else if (type == MIJIA_LONG)
    {
        app_mijia_long(pin);
    }
}

void app_mijia_short(uint8_t pin)
{
    Serial.printf("米家单击KEY%d\n", pin + 1);
// #ifdef SUPPORT_PCF8574
//     Wire.beginTransmission(PCF8574_ADDRESS);
//     Wire.write(~(1 << pin));
//     Wire.endTransmission();
//     Serial.println(~(1 << pin), BIN);
//     delay(100);
//     Wire.beginTransmission(PCF8574_ADDRESS);
//     Wire.write(0xff);
//     Wire.endTransmission();
// #endif
#ifdef SUPPORT_CH423S
    MijiaOp op(pin, MIJIA_SHORT);
    xQueueSend(Queue_Mijia, &op, 10);
#endif
}

void app_mijia_double(uint8_t pin)
{
    Serial.printf("米家双击KEY%d\n", pin + 1);
#ifdef SUPPORT_CH423S
    MijiaOp op(pin, MIJIA_DOUBLE);
    xQueueSend(Queue_Mijia, &op, 10);
#endif
}

void app_mijia_long(uint8_t pin)
{
    Serial.printf("米家长按KEY%d\n", pin + 1);
#ifdef SUPPORT_CH423S
    MijiaOp op(pin, MIJIA_LONG);
    xQueueSend(Queue_Mijia, &op, 10);
#endif
}

void Task_mijia(void *pvParameters)
{
    MijiaOp op;
    while (1)
    {
        if (xQueueReceive(Queue_Mijia, &op, 0))
        {
            uint8_t pin = op._pin;
            if (op._optype == MIJIA_SHORT)
            {
                ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
                vTaskDelay(100 / portTICK_PERIOD_MS);
                ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
            }
            else if (op._optype == MIJIA_DOUBLE)
            {
                ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
                vTaskDelay(100 / portTICK_PERIOD_MS);
                ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
                vTaskDelay(100 / portTICK_PERIOD_MS);
                ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
                vTaskDelay(100 / portTICK_PERIOD_MS);
                ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
            }
            else if (op._optype == MIJIA_LONG)
            {
                ch423.digitalWrite(ch423.eGPO0_7, ~(1 << pin));
                vTaskDelay(2000 / portTICK_PERIOD_MS);
                ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
            }
        }
    }
}

uint8_t app_mijia_get(uint8_t pin)
{
#ifdef SUPPORT_CH423S
    uint8_t value = ch423.digitalRead(ch423.eGPIOTotal);
    uint8_t state = (value & (1 << pin)) >> pin;
    // Serial.printf("pin%d:%d\n", pin, state);
    return state;
#endif
    // #ifdef SUPPORT_PCF8574
    //     uint8_t value;
    //     Wire.requestFrom(PCF8574_ADDRESS, 1); // 请求从PCF8574读取一个字节
    //     if (Wire.available())
    //     {
    //         value = Wire.read(); // 读取引脚状态
    //     }
    //     uint8_t state = (value & (1 << pin)) >> pin;
    //     // Serial.printf("pin%d:%d\n", pin, state);
    //     return state;
    // #endif
}