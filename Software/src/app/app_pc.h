#ifndef __APP_PC_H
#define __APP_PC_H
#include <Arduino.h>


typedef struct
{

    int CPU_FREQ;
    int GPU_FREQ;
    int CPU_TEMP;
    int GPU_TEMP;
    int CPU_FAN;
    int GPU_FAN;
    int CASE_FAN;
    int GPU_USE;
    int CPU_USE;
    int RAM_USE;

}pc_t;

void app_pc_sendrequest();

 
extern pc_t pc;

#endif