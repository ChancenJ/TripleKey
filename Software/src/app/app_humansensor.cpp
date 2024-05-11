#include "app/app_humansensor.h"
#include "app/app_led.h"

TaskHandle_t Handle_humanstate;
TaskHandle_t Handle_AutoScreenOnOff;
uint8_t lastHumanState = 1;

void Task_UpdateHumanState(void *pvParam)
{
    while (1)
    {
        uint8_t value = ch423.digitalRead(ch423.eGPIOTotal);
        HumanState = (value & (1 << 7)) >> 7; // GPIO7
        uint8_t swstate = value & 1;          // GPIO0
        if (HumanState != swstate)
        {
            ch423.digitalWrite(ch423.eGPO0_7, 0xFFFE);
            vTaskDelay(100 / portTICK_PERIOD_MS);
            ch423.digitalWrite(ch423.eGPO0_7, 0xFFFF);
            Serial.println("有人无人状态切换");
        }
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void Task_Screen_Control_by_HumanSensor(void *pvParam)
{
    while (1)
    {
        if (lastHumanState != HumanState)
        {
            if (HumanState == 0)
            {
                gfx1->displayOff();
                gfx2->displayOff();
                gfx3->displayOff();
                digitalWrite(PIN_BL, 0);
                app_led_off();
                Serial.println("无人关闭屏幕");
                delay(200);
            }
            else
            {
                digitalWrite(PIN_BL, 1);
                gfx1->displayOn();
                gfx2->displayOn();
                gfx3->displayOn();
                Serial.println("有人开启屏幕");
            }
        }
        lastHumanState = HumanState;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}