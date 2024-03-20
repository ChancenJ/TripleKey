#include "pages.h"
#include "manager/manager.h"
#include "board_def.h"
#include "app/app_key.h"
// #include "app/app_motion.h"
// #include "app/app_batt.h"
#include "app/app_timeout.h"


#include "pages/shortcut/shortcut.h"
#include "pages/clock/clock.h"
#include "pages/media/media.h"
#include "pages/web/web.h"
#include "pages/pcstate/pcstate.h"
#include "pages/weather/weather.h"
#include "pages/pomodoro/pomodoro.h"
#include "pages/keyboard/keyboard.h"
#include "pages/bilibili/bilibili.h"

//需要添加自己的功能页面
page_t *pages[] = {
    &page_main,
    &page_clock,
    &page_media,
    &page_web,
    // &page_pcstate,
    // &page_weather,
    &page_keyboard,
    &page_bilibili,
    &page_shortcut,
    &page_pomodoro,
};

static const uint8_t pages_num = ARRAY_SIZE(pages);
static int16_t row_index = 0;
static int16_t row_num;
static int16_t page_index = 1;
// static uint8_t page_old = 1;

static uint32_t cnt = 0;
// static char *buffer;
static uint8_t key_type;
static void init(void *data)
{

    manager.index = 0;
    row_num = ((pages_num - 1) % 3) ? ((pages_num - 1) / 3 + 1) : ((pages_num - 1) / 3);
}

static void enter(void *data)
{
    Serial.printf("enter:%d  ", page_index);
    row_index = page_index / 3;
    page_index = row_index * 3 + 1;
    Serial.printf("then:%d\r\n", page_index);
    uint16_t width = pages[page_index]->icon_width;
    uint16_t height = pages[page_index]->icon_height;

    int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

    gfx1->fillScreen(BLACK);
    gfx2->fillScreen(BLACK);
    gfx3->fillScreen(BLACK);

    gfx1->setTextSize(1);
    gfx2->setTextSize(1);
    gfx3->setTextSize(1);


    for (uint8_t i = 0; i < 3; i++)
    {
        if ((i + page_index) < pages_num)
        {
            gfx[i]->setCursor(0, 40+32);
            gfx[i]->setTextColor(GREEN);
            gfx[i]->setFont(u8g2_font_maniac_tr);
            gfx[i]->printf("%d\r\n", page_index + i);

             
            gfx[i]->setFont(&Coming_Soon_Regular_12);
            gfx[i]->getTextBounds(pages[page_index + i]->title_en, 0, 0, &x1, &y1, &w, &h);
            Serial.printf("%d %d %d %d\r\n",x1,y1,w,h);
	        gfx[i]->setCursor((OLED_WIDTH - w) / 2, 62+32);
            gfx[i]->printf("%s", pages[page_index + i]->title_en);
            gfx[i]->drawXBitmap(40, 0+32, pages[page_index + i]->icon, pages[page_index + i]->icon_width, pages[page_index + i]->icon_height, random(0xffff));
        }
    }

    delay(500);

    app_key_clean();

    manager_setBusy(false);
}

static void switchevent()
{
    

    page_index = row_index * 3 + 1;
    // Serial.printf("switchevent: row_num %d    row_index %d    page_index %d\r\n", row_num, row_index, page_index);
    uint16_t width = pages[page_index]->icon_width;
    uint16_t height = pages[page_index]->icon_height;

    int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

    gfx1->fillScreen(BLACK);
    gfx2->fillScreen(BLACK);
    gfx3->fillScreen(BLACK);

    for (uint8_t i = 0; i < 3; i++)
    {
        if ((i + page_index) < pages_num)
        {
            gfx[i]->setCursor(5, 40+32);
            gfx[i]->setTextColor(GREEN);
            gfx[i]->setFont(u8g2_font_maniac_tr);
            gfx[i]->printf("%d\r\n", page_index + i);

             
            gfx[i]->setFont(&Coming_Soon_Regular_12);
            gfx[i]->getTextBounds(pages[page_index + i]->title_en, 0, 0, &x1, &y1, &w, &h);
	        gfx[i]->setCursor((OLED_WIDTH - w) / 2, 62+32);
            gfx[i]->printf("%s", pages[page_index + i]->title_en);
            gfx[i]->drawXBitmap(40, 0+32, pages[page_index + i]->icon, pages[page_index + i]->icon_width, pages[page_index + i]->icon_height, random(0xffff));
        }
    }
}

static void switchNext()
{
    row_index++;
    if (row_index == row_num)
    {
        row_index = 0;
    }
}

static void switchPrev()
{

    row_index--;
    if (row_index < 0)
        row_index = row_num - 1;
}

static void loop(void *data)
{

    KEY_TYPE key = app_key_get();
    switch (key)
    {

    case KEY1_SHORT:
        manager.index = page_index;
        Serial.println(manager.index);
        manager_switchToChild(); //进入子项目

        break;

    case KEY2_SHORT:
        if ((page_index + 1) < pages_num)
        {
            manager.index = page_index + 1;
            Serial.println(manager.index);
            manager_switchToChild(); //进入子项目
        }
        break;
    case KEY3_SHORT:
        if ((page_index + 2) < pages_num)
        {
            manager.index = page_index + 2;
            manager_switchToChild(); //进入子项目
        }
        break;

    case ENC_NEXT:
        switchNext();
        switchevent();
        break;
    case ENC_PREV:
        switchPrev();
        switchevent();
        break;
    default:
        break;
    }
}

static void exit(void *data)
{

    manager_setBusy(true);

    // free(buffer);
}

page_t page_main = {
    .init = init,
    .enter = enter,
    .exit = exit,
    .loop = loop,
    .title_en = "main",
    .title_cn = "主页",
    .icon = NULL,
    .icon_width = 0,
    .icon_height = 0,
    .sleep_enable = true,
    .wakeup_btn_effect_enable = false,
    .acc_enable = true,

};
