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
	String tempstr=weather.nowweather.temp + "°";
	gfx[0]->fillRect(0, 0, 128, 75, BLACK);
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(&MiSans_Regular45pt_number);
	gfx[0]->getTextBounds(tempstr, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2 , 72);
	gfx[0]->print(tempstr);
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
	gfx[0]->setCursor((OLED_WIDTH - w) / 2, 124);
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
	gfx[0]->setCursor(OLED_WIDTH / 2 -5, 98);
	gfx[0]->print(humstr);
}

void dispWeather()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[1]->fillRect(0,0,128,90,BLACK);
	gfx[1]->setUTF8Print(true);
	gfx[1]->setFont(DouyinSansBoldCN_Weather_38);
	gfx[1]->setTextColor(QINGSHUILAN);
	gfx[1]->getTextBounds(weather.nowweather.weathertext, 0, 0, &x1, &y1, &w, &h);
	Serial.println(w);
	gfx[1]->setCursor((OLED_WIDTH - w) / 2, 88);
	gfx[1]->print(weather.nowweather.weathertext);
	char path[30];
	sprintf(path,"/weather/weather/%s.png",weather.nowweather.weathercode);
	Serial.println(path);
	myDrawPNG((OLED_WIDTH - 48) / 2, 0,path,1);



	gfx[2]->fillRoundRect(5, 100, 20, 23, 3, DARKGREEN);
	gfx[2]->fillRoundRect(25, 100, 20, 23, 3, YELLOW);
	gfx[2]->fillRoundRect(45, 100, 20, 23, 3, ORANGE);
	gfx[2]->fillRoundRect(65, 100, 20, 23, 3, RED);
	gfx[2]->fillRoundRect(85, 100, 20, 23, 3, PURPLE);
	gfx[2]->fillRoundRect(105, 100, 20, 23, 3, RGB565(100, 0, 35));

	
}
void dispWind(){
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[1]->fillRect(0,90,128,38,BLACK);
	gfx[1]->setUTF8Print(true);
	gfx[1]->setFont(MiSans_LightCN_Weather_20);
	gfx[1]->setTextColor(WHITE);
	String windstr = weather.nowweather.winddir + " " + weather.nowweather.windscale + "级";
	gfx[1]->getTextBounds(windstr, 0, 0, &x1, &y1, &w, &h);
	gfx[1]->setCursor((OLED_WIDTH - w) / 2, 125);
	gfx[1]->print(windstr);
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
	dispWeather();
	dispWind();
	getNowAir(&weather);
	dispAir();
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