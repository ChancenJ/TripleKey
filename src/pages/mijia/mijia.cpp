#include "mijia.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_mijia.h"

#define MIJIALV RGB565(25, 203, 139)

static uint32_t lastUpdateTime;

extern page_t page_clock;

sw sws[] = {
	{K1, "有人存在", "Sensor", 1, 2},  //K1作为人体存在传感器状态，同步米家，用于智能联动
	{K5, "场景1", "K5Short", 1, 1},
	{K5, "场景2", "K5Double", 2, 1},
	{K5, "场景3", "K5Long", 3, 1},
	{K6, "场景1", "K6Short", 1, 1},
	{K6, "场景2", "K6Double", 2, 1},
	{K6, "场景3", "K6Long", 3, 1},
	{K7, "场景1", "K7Short", 1, 1},
	{K7, "场景2", "K7Double", 2, 1},
	{K7, "场景3", "K7Long", 3, 1},
	{K2, "场景1", "K2Short", 1, 2},
	{K3, "场景1", "K3Short", 1, 2},
	{K4, "场景1", "K4Short", 1, 2},
	// {MIJIAKEY1_PIN, "场景4", "Scene10", 2, 1},
	// {MIJIAKEY1_PIN, "场景4", "Scene11", 3, 1},
	// {MIJIAKEY1_PIN, "场景4", "Scene12", 2, 1},
	// {MIJIAKEY1_PIN, "开关1", "Switch1", 1, 2},
	// {MIJIAKEY1_PIN, "开关2", "Switch2", 1, 2},
	// {MIJIAKEY1_PIN, "开关3", "Switch3", 1, 2},
	// {MIJIAKEY1_PIN, "开关4", "Switch4", 1, 2},
};

static const uint8_t maxsw = sizeof(sws) / sizeof(sw);

static int8_t firstindex;

static void dispSwitch()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	for (uint8_t i = 0; i < 3; i++)
	{
		if (sws[(firstindex + i) % maxsw].type == 1) // 情景开关
		{
			gfx[i]->setTextColor(MIJIALV);
			gfx[i]->setFont(&MiSans_Demibold_12);
			gfx[i]->getTextBounds(sws[(firstindex + i) % maxsw].name_en, 0, 0, &x1, &y1, &w, &h);
			gfx[i]->setCursor((OLED_WIDTH - w) / 2, (OLED_HEIGHT - h) / 2 - y1);
			gfx[i]->print(sws[(firstindex + i) % maxsw].name_en);
		}
		else if (sws[(firstindex + i) % maxsw].type == 2) // 普通开关（显示开关状态）
		{
			gfx[i]->setTextColor(MIJIALV);
			gfx[i]->setFont(&MiSans_Demibold_12);
			gfx[i]->getTextBounds(sws[(firstindex + i) % maxsw].name_en, 0, 0, &x1, &y1, &w, &h);
			gfx[i]->setCursor((OLED_WIDTH - w) / 2, 40);
			gfx[i]->print(sws[(firstindex + i) % maxsw].name_en);
			if (sws[(firstindex + i) % maxsw].on)
			{
				myDrawPNG((OLED_WIDTH - 61) / 2, 70, "/mijia/on.png", i);
			}
			else
			{
				myDrawPNG((OLED_WIDTH - 61) / 2, 70, "/mijia/off.png", i);
			}
		}
	}
}

void dispUpdatedState(uint8_t i)
{
	uint8_t screen = firstindex == i ? 0 : ((firstindex + 1) % maxsw == i ? 1 : ((firstindex + 2) % maxsw == i ? 2 : -1));
	if (screen >= 0)
	{
		if (sws[i].on)
		{
			myDrawPNG((OLED_WIDTH - 61) / 2, 70, "/mijia/on.png", screen);
		}
		else
		{
			myDrawPNG((OLED_WIDTH - 61) / 2, 70, "/mijia/off.png", screen);
		}
	}
}

void UpdateState()
{
	for (int i = 0; i < maxsw; i++)
	{
		if (sws[i].type == 2)
		{
			uint8_t on = app_mijia_get(sws[i].pin);
			if (on != sws[i].on)
			{
				sws[i].on = on;
				dispUpdatedState(i);
			}
		}
	}
}

static void init(void *data)
{
	firstindex = 0;
	Serial.println(maxsw);
	lastUpdateTime = millis();
}

static void enter(void *data)
{
	// insert code
	dispSwitch();
	UpdateState();
	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	if (millis() - lastUpdateTime >= 500)
	{
	// 	uint8_t tempon=sws[0].on;
	// 	uint8_t on = app_mijia_get(7);
	// 	lastUpdateTime = millis();
	// 	if(on!=tempon){
	// 		sws[0].on=on;
	// 		dispUpdateState(0);
	// 	}
		UpdateState();
		lastUpdateTime = millis();
	}
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{
	case KEY1_DOWN:
		app_led_set(LED1, app_led_color(25, 203, 139));
		app_led_update();
		app_mijia_control(sws[firstindex].pin, sws[firstindex].optype);
		break;
	case KEY2_DOWN:
		app_led_set(LED2, app_led_color(25, 203, 139));
		app_led_update();
		app_mijia_control(sws[(firstindex + 1) % maxsw].pin, sws[(firstindex + 1) % maxsw].optype);
		break;
	case KEY3_DOWN:
		app_led_set(LED3, app_led_color(25, 203, 139));
		app_led_update();
		app_mijia_control(sws[(firstindex + 2) % maxsw].pin, sws[(firstindex + 2) % maxsw].optype);
		break;
	case KEY1_UP:
	case KEY2_UP:
	case KEY3_UP:
		app_led_off();
		break;

	case ENC_NEXT:
		firstindex++;
		if (firstindex >= maxsw)
		{
			firstindex = 0;
		}
		dispSwitch();
		break;
	case ENC_PREV:
		firstindex--;
		if (firstindex < 0)
		{
			firstindex = maxsw - 1;
		}
		dispSwitch();
		break;
	case KEY4_SHORT:
		manager_switchToPage(&page_clock);
		break;
	case KEY4_LONG:				  // 长按
		manager_switchToParent(); // 进入父项目 //退出
		break;
	default:
		break;
	}
}

static void exit(void *data)
{
	// insert code

	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_mijia = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "MiHome",
	.title_cn = "米家",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};