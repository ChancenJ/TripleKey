#include "newweather.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_newweather.h"
#include <ArduinoJson.h>

Weather weather;

void dispNowTemp(){
	
}

void dispWeather()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[0]->fillScreen(BLACK);
	gfx[0]->setUTF8Print(true);

	gfx[0]->setFont(&MiSans_Regular45pt_number);
	gfx[0]->getTextBounds(weather.nowweather.temp, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2-5, 75);
	gfx[0]->print(weather.nowweather.temp+"°");

	gfx[1]->setUTF8Print(true);
	gfx[1]->setFont(YousheTitleHeiCN_Weather_35);
	gfx[1]->setTextSize(2);
	gfx[1]->setTextColor(WHITE);
	gfx[1]->getTextBounds(weather.nowweather.weathertext, 0, 0, &x1, &y1, &w, &h);
	gfx[1]->setCursor((OLED_WIDTH - w) / 2, 70);
	gfx[1]->print(weather.nowweather.weathertext);

	gfx[2]->setUTF8Print(true);
	gfx[2]->setFont(YousheTitleHeiCN_Weather_35_dpi213);
	gfx[2]->setTextSize(2);
	gfx[2]->setTextColor(WHITE);
	gfx[2]->getTextBounds(weather.nowweather.weathertext, 0, 0, &x1, &y1, &w, &h);
	gfx[2]->setCursor((OLED_WIDTH - w) / 2, 70);
	gfx[2]->print(weather.nowweather.weathertext);

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
	getCity(&weather);
	getNowWeather(&weather);
	getNowAir(&weather);
	dispWeather();
	getDay3Weather(&weather);
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
	.title_en = "Weather",
	.title_cn = "天气",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};