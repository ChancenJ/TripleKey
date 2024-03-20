#include "app_key.h"

// #include "app_motor.h"
#include "app_timeout.h"

QueueHandle_t Key_Queue;
BaseType_t err;

SoftwareTimer keyTimer;
SoftwareTimer longTimer; //长按提示

// extern SoftwareTimer timeoutTimer;

static KEY_TYPE key_type;

static void handler(Button2 &btn);
static void key_callback(void);

/////////////////////////////////////////////////////////////////

static void pressed(Button2 &btn)
{
    // Serial.println("pressed");
    if (btn == button1)
    {
        key_type = KEY1_DOWN;
    }
    else if (btn == button2)
    {
        key_type = KEY2_DOWN;
    }
    else if (btn == button3)
    {
        key_type = KEY3_DOWN;
    }
    else if (btn == button4)
    {
        key_type = KEY4_DOWN;
    }

    // app_motor_once(60);
    //   app_timeout_reset();
    longTimer.reset();
    longTimer.start();

    if (Key_Queue != 0)
    {
        err = xQueueSend(Key_Queue, &key_type, 10);
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}
static void released(Button2 &btn)
{
    // Serial.print("released: ");
    // Serial.println(btn.wasPressedFor());

    if (btn == button1)
    {
        key_type = KEY1_UP;
    }
    else if (btn == button2)
    {
        key_type = KEY2_UP;
    }
    else if (btn == button3)
    {
        key_type = KEY3_UP;
    }
    else if (btn == button4)
    {
        key_type = KEY4_UP;
    }

    longTimer.stop();

    if (Key_Queue != 0)
    {
        err = xQueueSend(Key_Queue, &key_type, 10);
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}
static void changed(Button2 &btn)
{
    // Serial.println("changed");
}
static void click(Button2 &btn)
{
    // Serial.println("click\n");
    if (btn == button1)
    {
        key_type = KEY1_SHORT;
    }
    else if (btn == button2)
    {
        key_type = KEY2_SHORT;
    }
    else if (btn == button3)
    {
        key_type = KEY3_SHORT;
    }
    else if (btn == button4)
    {
        key_type = KEY4_SHORT;
    }

    if (Key_Queue != 0)
    {
        err = xQueueSend(Key_Queue, &key_type, 10);
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}
static void longClick(Button2 &btn)
{
    // Serial.println("long click\n");
    if (btn == button1)
    {
        key_type = KEY1_LONG;
    }
    else if (btn == button2)
    {
        key_type = KEY2_LONG;
    }
    else if (btn == button3)
    {
        key_type = KEY3_LONG;
    }
    else if (btn == button4)
    {
        key_type = KEY4_LONG;
    }

    // Serial.println("long");

    if (Key_Queue != 0)
    {
        err = xQueueSend(Key_Queue, &key_type, 10);
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}
static void doubleClick(Button2 &btn)
{
    // Serial.println("double click\n");
    // Serial.println("long click\n");
    if (btn == button1)
    {
        key_type = KEY1_DOUBLE;
    }

    if (Key_Queue != 0)
    {
        err = xQueueSend(Key_Queue, &key_type, 10);
        if (err == errQUEUE_FULL)
        {
            // 队列已满，数据发送失败
        }
    }
}
static void tripleClick(Button2 &btn)
{
    // Serial.println("triple click\n");
}
static void tap(Button2 &btn)
{
    // Serial.println("tap");
}
/////////////////////////////////////////////////////////////////
//   avoid using pins with LEDs attached
long oldPosition = 0;
static void key_timer_callback(TimerHandle_t xTimerID)
{
    // freeRTOS timer ID, ignored if not used
    (void)xTimerID;
    button1.loop();
    button2.loop();
    button3.loop();
    button4.loop();

    long newPosition = myEnc.read() / 4;
    if (newPosition != oldPosition)
    {
        if (newPosition < oldPosition)
        {
            if (Key_Queue != 0)
            {
                key_type = ENC_NEXT;
                err = xQueueSend(Key_Queue, &key_type, 10);
                if (err == errQUEUE_FULL)
                {
                    // 队列已满，数据发送失败
                }
            }
        }
        else if (newPosition > oldPosition)
        {
            if (Key_Queue != 0)
            {
                key_type = ENC_PREV;
                err = xQueueSend(Key_Queue, &key_type, 10);
                if (err == errQUEUE_FULL)
                {
                    // 队列已满，数据发送失败
                }
            }
        }
        oldPosition = newPosition;
        // Serial.println(newPosition);
    }
}

static void long_timer_callback(TimerHandle_t xTimerID)
{
    // freeRTOS timer ID, ignored if not used
    (void)xTimerID;
    //   app_motor_once(70);
}

void app_key_init(void)
{

    button1.begin(BUTTON1_PIN,INPUT);
    button2.begin(BUTTON2_PIN,INPUT);
    button3.begin(BUTTON3_PIN,INPUT);
    button4.begin(BUTTON4_PIN,INPUT);

    button1.setLongClickTime(500);
    button2.setLongClickTime(500);
    button3.setLongClickTime(500);
    button4.setLongClickTime(500);

    // button1.setChangedHandler(changed);
    button1.setPressedHandler(pressed);
    button1.setReleasedHandler(released);
    // button1.setTapHandler(tap);
    button1.setClickHandler(click);
    button1.setLongClickHandler(longClick);
    button1.setDoubleClickHandler(doubleClick);
    // button1.setTripleClickHandler(tripleClick);

    // button1.setChangedHandler(changed);
    button2.setPressedHandler(pressed);
    button2.setReleasedHandler(released);
    // button1.setTapHandler(tap);
    button2.setClickHandler(click);
    button2.setLongClickHandler(longClick);
    button2.setDoubleClickHandler(doubleClick);
    // button1.setTripleClickHandler(tripleClick);

    // button1.setChangedHandler(changed);
    button3.setPressedHandler(pressed);
    button3.setReleasedHandler(released);
    // button1.setTapHandler(tap);
    button3.setClickHandler(click);
    button3.setLongClickHandler(longClick);
    button3.setDoubleClickHandler(doubleClick);
    // button1.setTripleClickHandler(tripleClick);

    // button1.setChangedHandler(changed);
    button4.setPressedHandler(pressed);
    button4.setReleasedHandler(released);
    // button1.setTapHandler(tap);
    button4.setClickHandler(click);
    button4.setLongClickHandler(longClick);
    button4.setDoubleClickHandler(doubleClick);
    // button1.setTripleClickHandler(tripleClick);

    //创建按键消息队列
    Key_Queue = xQueueCreate(30, sizeof(uint8_t));
    if (Key_Queue == 0)
    {
        // Queue was not created and must not be used.
    }

    // tpkey.setLongClickHandler(handler);
    // // Configure the timer with 1000 ms interval, with our callback
    keyTimer.begin(5, key_timer_callback);

    // // Start the timer
    if (!keyTimer.start())
    {
        // tft.println("timer start fail");
    }

    // tpkey.setLongClickHandler(handler);
    // // Configure the timer with 1000 ms interval, with our callback
    longTimer.begin(1000, long_timer_callback, NULL, false);
}

void app_key_deinit(void)
{

    keyTimer.~SoftwareTimer();
}

void app_key_config_wakeup(void)
{
}

KEY_TYPE app_key_get()
{
    key_type = KEY_NONE;
    if (Key_Queue != 0)
    {
        // if(xQueueReceive(Key_Queue, &key_type,portMAX_DELAY))    //  10/portTICK_PERIOD_MS
        if (xQueueReceive(Key_Queue, &key_type, 0))
        {
            return key_type;
        }
    }

    return key_type;
}

//清楚多余按键信息
void app_key_clean()
{
    if (Key_Queue != 0)
    {
        xQueueReset(Key_Queue);
    }
}

//停止loop
void app_key_stopLoop()
{
    keyTimer.stop();
}

//开始loop
void app_key_startLoop()
{
    keyTimer.reset();
    keyTimer.start();
}