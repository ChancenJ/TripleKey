#include "app/app_humansensor.h"

uint8_t getHumanSensorState()
{
#ifdef SUPPORT_CH423S
    uint8_t value = ch423.digitalRead(ch423.eGPIO7);
    //Serial.printf("HumanState: %d\n",value);
    return value;
#else
    return 0;
#endif
}

void Mijia_UpdateHumanState()
{
    uint8_t humanstate = getHumanSensorState();
    uint8_t swstate = app_mijia_get(K1);
    if(humanstate!=swstate){
        app_mijia_short(K1);
        Serial.println("有人无人状态切换");
    }
    
}