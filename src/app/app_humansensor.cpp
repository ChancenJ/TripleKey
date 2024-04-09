#include "app/app_humansensor.h"

unsigned long lastSensorUpdateTime = millis();

uint8_t getHumanSensorState()
{
#ifdef SUPPORT_CH423S
    uint8_t value = ch423.digitalRead(ch423.eGPIO7);
    // Serial.printf("HumanState: %d\n",value);
    return value;
#else
    return 0;
#endif
}

void Mijia_UpdateHumanState()
{
    if (millis() - lastSensorUpdateTime <= 200)
    {
        return;
    }
    lastSensorUpdateTime = millis();
#ifdef SUPPORT_CH423S
    uint8_t value = ch423.digitalRead(ch423.eGPIOTotal);
    uint8_t humanstate = (value & (1 << 7)) >> 7; // GPIO7
    uint8_t swstate = value & 1;                  // GPIO0
    // uint8_t humanstate = getHumanSensorState();
    // uint8_t swstate = app_mijia_get(K1);
    if (humanstate != swstate)
    {
        app_mijia_short(K1);
        Serial.println("有人无人状态切换");
    }
#endif
}