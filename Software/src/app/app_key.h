#ifndef __APP_KEY_H
#define __APP_KEY_H
#include "Button2.h"
#include <Arduino.h>

#include "board_def.h"

extern Button2 key0;

typedef enum
{
    KEY_NONE = 0,

 

    KEY1_DOWN,  //  按下
    KEY1_UP,    //  释放
    KEY1_SHORT, //  短按
    KEY1_LONG,  //  长按
    KEY1_DOUBLE, //  双击

    KEY2_DOWN,  //  按下
    KEY2_UP,    //  释放
    KEY2_SHORT, //  短按
    KEY2_LONG,  //  长按
    KEY2_DOUBLE, //  双击

    KEY3_DOWN,  //  按下
    KEY3_UP,    //  释放
    KEY3_SHORT, //  短按
    KEY3_LONG,  //  长按
    KEY3_DOUBLE, //  双击

    KEY4_DOWN,  //  按下
    KEY4_UP,    //  释放
    KEY4_SHORT, //  短按
    KEY4_LONG,  //  长按
    KEY4_DOUBLE, //  双击


    ENC_NEXT,
    ENC_PREV,

} KEY_TYPE;

void app_key_init(void);

void app_key_config_wakeup(void);

KEY_TYPE app_key_get();
void app_key_clean();


void app_key_stopLoop();
void app_key_startLoop();
#endif
