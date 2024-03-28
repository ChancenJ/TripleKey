#include "newweather.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_newweather.h"
#include <ArduinoJson.h>
#include "fonts/MiSans_LightCN_Weather_20.h"
Weather weather;

void dispNowTemp()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[0]->fillRect(0, 0, 128, 75, BLACK);
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(&MiSans_Regular45pt_number);
	gfx[0]->getTextBounds(weather.nowweather.temp, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2 - 5, 72);
	gfx[0]->print(weather.nowweather.temp + "°");
}
void dispAir()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	uint16_t color = DARKGREY;
	int16_t aqi = weather.nowweather.aqi.toInt();
	if (aqi >= 0 && aqi <= 50)
	{
		color = RGB565(0, 153, 102);
	}
	else if (aqi >= 51 && aqi <= 100)
	{
		color = YELLOW;
	}
	else if (aqi >= 101 && aqi <= 150)
	{
		color = ORANGE;
	}
	else if (aqi >= 151 && aqi <= 200)
	{
		color = RED;
	}
	else if (aqi >= 201 && aqi <= 300)
	{
		color = PURPLE;
	}
	else if (aqi > 300)
	{
		color = RGB565(126, 0, 35);
	}
	String airstr = weather.nowweather.aqi + " " + weather.nowweather.aircategory;
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(MiSans_LightCN_Weather_20);
	gfx[0]->setTextColor(WHITE);
	gfx[0]->getTextBounds(airstr, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->fillRect(0, 102, 128, 26, BLACK);
	gfx[0]->fillRoundRect((OLED_WIDTH - w) / 2 - 3, 104, w + 6, 23, 3, color);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2-2, 124);
	gfx[0]->print(airstr);
}

void dispHumidity()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	String humstr = weather.nowweather.humidity + "%";
	gfx[0]->fillRect(0, 75, 128, 27, BLACK);
	myDrawPNG(OLED_WIDTH / 2 - 29, 77, "/weather/Humidity.png", 0);
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(MiSans_LightCN_Weather_20);
	gfx[0]->setTextColor(WHITE);
	gfx[0]->setCursor(OLED_WIDTH / 2 , 98);
	gfx[0]->print(humstr);
}

void dispWeather()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[1]->fillScreen(BLACK);
	gfx[2]->fillScreen(BLACK);
	gfx[1]->setUTF8Print(true);
	gfx[1]->setFont(YousheTitleHeiCN_Weather_35);
	gfx[1]->setTextColor(WHITE);
	gfx[1]->getTextBounds(weather.nowweather.weathertext, 0, 0, &x1, &y1, &w, &h);
	gfx[1]->setCursor((OLED_WIDTH - w) / 2, 70);
	gfx[1]->print(weather.nowweather.weathertext);
	gfx[1]->fillRoundRect(5, 100, 20, 23, 3, RGB565(0, 153, 102));
	gfx[1]->fillRoundRect(25, 100, 20, 23, 3, YELLOW);
	gfx[1]->fillRoundRect(45, 100, 20, 23, 3, ORANGE);
	gfx[1]->fillRoundRect(65, 100, 20, 23, 3, RED);
	gfx[1]->fillRoundRect(85, 100, 20, 23, 3, PURPLE);
	gfx[1]->fillRoundRect(105, 100, 20, 23, 3, RGB565(126, 0, 35));

	gfx[2]->setUTF8Print(true);
	gfx[2]->setFont(YousheTitleHeiCN_Weather_35_dpi213);
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
	dispProcessing();
	getCity(&weather);
	getNowWeather(&weather);
	dispNowTemp();
	dispHumidity();
	getNowAir(&weather);
	dispAir();
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