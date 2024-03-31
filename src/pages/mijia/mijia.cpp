#include "mijia.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_mijia.h"

#define MIJIALV RGB565(25, 203, 139)

static uint32_t lastUpdateTime;

struct sw
{
	uint8_t pin;
	String name_cn;
	String name_en;
	uint8_t optype; // 1为单击，2为双击，3为长按
	uint8_t type;	// 1为场景开关，2为普通开关
	uint8_t on;		// 普通开关开关状态
	sw(uint8_t pin, String name_cn, String name_en, uint8_t optype, uint8_t type) : pin(pin), name_cn(name_cn), name_en(name_en), optype(optype), type(type), on(0) {}
};

sw sws[] = {
	{0, "场景1", "K1Short", 1, 2},
	{4, "场景1", "K5Short", 1, 1},
	{4, "场景2", "K5Double", 2, 1},
	{4, "场景3", "K5Long", 3, 1},
	{5, "场景1", "K6Short", 1, 1},
	{5, "场景2", "K6Double", 2, 1},
	{5, "场景3", "K6Long", 3, 1},
	{6, "场景1", "K7Short", 1, 1},
	{6, "场景2", "K7Double", 2, 1},
	{6, "场景3", "K7Long", 3, 1},
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
			uint16_t color = sws[(firstindex + i) % maxsw].on ? RED : MIJIALV;
			gfx[i]->setTextColor(color);
			gfx[i]->setFont(&MiSans_Demibold_12);
			gfx[i]->getTextBounds(sws[(firstindex + i) % maxsw].name_en, 0, 0, &x1, &y1, &w, &h);
			gfx[i]->setCursor((OLED_WIDTH - w) / 2, (OLED_HEIGHT - h) / 2 - y1);
			gfx[i]->print(sws[(firstindex + i) % maxsw].name_en);
		}
	}
}

static void init(void *data)
{
	firstindex = 0;
	Serial.println(maxsw);
	app_mijia_get(7);
	lastUpdateTime = millis();
}

static void enter(void *data)
{
	// insert code
	dispSwitch();

	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	if (millis() - lastUpdateTime >= 1000)
	{
		uint8_t tempon=sws[0].on;
		uint8_t on = app_mijia_get(7);
		lastUpdateTime = millis();
		if(on!=tempon){
			sws[0].on=on;
			dispSwitch();
		}
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