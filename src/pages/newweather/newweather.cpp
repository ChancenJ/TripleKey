#include "newweather.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_newweather.h"
#include <ArduinoJson.h>

NowWeather nowweather;

void dispWeather()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[0]->fillScreen(BLACK);
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(u8g2_font_unifont_t_chinese2);
	gfx[0]->setTextColor(WHITE);
	gfx[0]->getTextBounds(nowweather.weathertext, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2, 50);
	gfx[0]->print(nowweather.weathertext);
	gfx[0]->setFont(&MiSans_Demibold_12);
	gfx[0]->setCursor(50, 80);
	gfx[0]->print(nowweather.temp);
}

static void init(void *data)
{
}

static void enter(void *data)
{
	// insert code

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	getWeather(&nowweather);
	dispWeather();
	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{

	case KEY1_DOWN:

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
page_t page_newweather = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "NewWeather",
	.title_cn = "New天气",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};