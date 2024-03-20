
#include "app_timeout.h"

SoftwareTimer timeoutTimer;
static volatile bool flag_timeout;

// static void timeout_timer_callback(TimerHandle_t xTimerID)
// {
//   // freeRTOS timer ID, ignored if not used
//   (void)xTimerID;
//   // lv_demo_printer_anim_out(label_batt,0);
//   // lv_demo_printer_anim_out(label_date,0);
//   // manager.cur->exit(NULL);
// //   sleep();
// }


void app_timeout_init(TimerCallbackFunction_t cb)
{
    app_timeout_SetState(false);
    timeoutTimer.begin(TIME_SLEEP, cb,NULL,false);
    // timeoutTimer.begin(TIME_SLEEP, cb);


    timeoutTimer.start();
}


void app_timeout_reset()
{
    timeoutTimer.stop();
    timeoutTimer.reset();
}

void app_timeout_stop()
{
    timeoutTimer.stop();
}



bool app_timeout_getState()
{
    return flag_timeout;
}
void app_timeout_SetState(bool state)
{
    flag_timeout=state;
}
