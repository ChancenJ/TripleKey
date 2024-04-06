#include "app/app_humansensor.h"

uint8_t getHumanSensorState()
{
#ifdef SUPPORT_CH423S
    uint8_t value = ch423.digitalRead(ch423.eGPIO7);
    uint8_t state = (value & (1 << 7)) >> 7;
    // Serial.printf("HumanState: %d\n",state);
    return state;
#else
    return 0;
#endif
}

void Mijia_UpdateHumanState()
{
    uint8_t humanstate = getHumanSensorState();
    uint8_t swstate = app_mijia_get(0);
    if(humanstate!=swstate){
        app_mijia_short(0);
    }
    
}