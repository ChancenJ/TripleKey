#include "weather.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_weather.h"
#include <ArduinoJson.h>
#include "fonts/MiSans_LightCN_Weather_20.h"
#include "fonts/DreamHanSerifCN_W17_NoWarning_24.h"

extern page_t page_clock;

Weather weather;
String day[3] = {"今天", "明天", "后天"};
int8_t dispIndex;

void dispNowTemp()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	String tempstr = weather.nowweather.temp + "°";
	gfx[0]->fillRect(0, 0, 128, 75, BLACK);
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(&MiSans_Regular45pt_number);
	gfx[0]->setTextColor(QINGSHUILAN);
	gfx[0]->getTextBounds(tempstr, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2, 72);
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
		color = RGB565(0, 180, 100);
	}
	else if (aqi >= 51 && aqi <= 100)
	{
		color = RGB565(255, 225, 0);
	}
	else if (aqi >= 101 && aqi <= 150)
	{
		color = RGB565(255, 120, 0);
	}
	else if (aqi >= 151 && aqi <= 200)
	{
		color = RGB565(200, 0, 0);
	}
	else if (aqi >= 201 && aqi <= 300)
	{
		color = PURPLE;
	}
	else if (aqi > 300)
	{
		color = RGB565(90, 30, 0);
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
	gfx[0]->setCursor(OLED_WIDTH / 2 - 5, 98);
	gfx[0]->print(humstr);
}

void dispWeather()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[1]->fillRect(0, 0, 128, 90, BLACK);
	gfx[1]->setUTF8Print(true);
	gfx[1]->setFont(DouyinSansBoldCN_Weather_38);
	gfx[1]->setTextColor(QINGSHUILAN);
	gfx[1]->getTextBounds(weather.nowweather.weathertext, 0, 0, &x1, &y1, &w, &h);
	gfx[1]->setCursor((OLED_WIDTH - w) / 2, 88);
	gfx[1]->print(weather.nowweather.weathertext);
	char path[30];
	sprintf(path, "/weather/weather/%s.png", weather.nowweather.weathercode);
	Serial.println(path);
	myDrawPNG((OLED_WIDTH - 48) / 2, 0, path, 1);
}
void dispWind()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[1]->fillRect(0, 90, 128, 38, BLACK);
	gfx[1]->setUTF8Print(true);
	gfx[1]->setFont(MiSans_LightCN_Weather_20);
	gfx[1]->setTextColor(WHITE);
	String windstr = weather.nowweather.winddir + " " + weather.nowweather.windscale + "级";
	gfx[1]->getTextBounds(windstr, 0, 0, &x1, &y1, &w, &h);
	gfx[1]->setCursor((OLED_WIDTH - w) / 2, 122);
	gfx[1]->print(windstr);
}
void dispMtempAndSun()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[2]->fillScreen(BLACK);
	gfx[2]->setUTF8Print(true);
	gfx[2]->setFont(MiSans_LightCN_Weather_20);
	gfx[2]->setCursor(0, 30);
	gfx[2]->setTextColor(ORANGE);
	gfx[2]->print("最高温度" + weather.day3weather[0].tempmax + "℃");
	gfx[2]->setCursor(0, 60);
	gfx[2]->setTextColor(JINZI);
	gfx[2]->print("最低温度" + weather.day3weather[0].tempmin + "℃");
	gfx[2]->setCursor(0, 90);
	gfx[2]->setTextColor(ORANGE);
	gfx[2]->print("日出    " + weather.day3weather[0].sunrise);
	gfx[2]->setCursor(0, 120);
	gfx[2]->setTextColor(JINZI);
	gfx[2]->print("日落    " + weather.day3weather[0].sunset);
}

void dispPredict()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	char path[30];
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	for (int i = 0; i < 3; i++)
	{
		gfx[i]->setTextColor(BLACK);
		gfx[i]->setFont(MiSans_LightCN_Weather_20);
		gfx[i]->fillRect(0, 0, 128, 23, QINGSHUILAN);
		gfx[i]->getTextBounds(day[i], 0, 0, &x1, &y1, &w, &h);
		gfx[i]->setCursor((OLED_WIDTH - w) / 2, 20);
		gfx[i]->print(day[i]);
		sprintf(path, "/weather/weather/%s.png", weather.day3weather[i].daycode);
		Serial.println(path);
		myDrawPNG(0, 24, path, i);
		gfx[i]->drawLine(0, 74, 48, 74, QINGSHUILAN);
		sprintf(path, "/weather/weather/%s.png", weather.day3weather[i].nightcode);
		Serial.println(path);
		myDrawPNG(0, 76, path, i);
		// gfx[i]->setCursor(48, 46);
		// gfx[i]->print(weather.day3weather[i].weatherday);
		// gfx[i]->setCursor(48, 124);
		// gfx[i]->print(weather.day3weather[i].weathernight);
		myDrawPNG(65, 37, "/weather/maxtemp.png", i);
		myDrawPNG(65, 98, "/weather/mintemp.png", i);
		gfx[i]->setFont(MiSans_LightCN_Weather_20);
		gfx[i]->setTextColor(WHITE);
		gfx[i]->setCursor(84, 53);
		gfx[i]->print(weather.day3weather[i].tempmax + "℃");
		gfx[i]->setCursor(84, 114);
		gfx[i]->print(weather.day3weather[i].tempmin + "℃");
	}
}

void dispToday()
{
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	dispNowTemp();
	dispHumidity();
	dispWeather();
	dispWind();
	dispAir();
	dispMtempAndSun();
}

void dispWarning()
{
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	uint8_t num = weather.warnings.size() <= 3 ? weather.warnings.size() : 3;
	if (num == 0)
	{
		int16_t x1;
		int16_t y1;
		uint16_t w;
		uint16_t h;
		String text = "无预警信息";
		gfx[1]->setTextColor(QINGSHUILAN);
		gfx[1]->setUTF8Print(TRUE);
		gfx[1]->setFont(DreamHanSerifCN_W17_NoWarning_24);
		gfx[1]->getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
		gfx[1]->setCursor((OLED_WIDTH - w) / 2, (OLED_HEIGHT - h) / 2 - y1);
		gfx[1]->print(text);
	}
	else
	{
		for (int i = 0; i < num; i++)
		{
			uint16_t color;
			if (weather.warnings[i].color == "Blue")
			{
				color = RGB565(50, 101, 254);
			}
			else if (weather.warnings[i].color == "Yellow")
			{
				color = YELLOW;
			}
			else if (weather.warnings[i].color == "Orange")
			{
				color = RGB565(255, 120, 0);
			}
			else if (weather.warnings[i].color == "Red")
			{
				color = RED;
			}
			String path = "/weather/warning/" + weather.warnings[i].type + weather.warnings[i].color + ".png";
			File file = LittleFS.open(path);
			if (file)
			{
				myDrawPNG(32, 0, path.c_str(), i);
				gfx[i]->setTextColor(color);
				gfx[i]->setUTF8Print(TRUE);
				gfx[i]->setFont(u8g2_font_wqy13_t_gb2312);
				gfx[i]->setCursor(0, 65);
				gfx[i]->println(weather.warnings[i].title);
				gfx[i]->println(weather.warnings[i].starttime);
			}
			else
			{
				String unipath = "/weather/warning/Uni" + weather.warnings[i].color + ".png";
				myDrawPNG(32, 0, unipath.c_str(), i);
				gfx[i]->setTextColor(BLACK);
				// gfx[i]->fillRoundRect(32,0,64,30,3,color);
				// gfx[i]->setUTF8Print(TRUE);
				// gfx[i]->setFont(DreamHanSerifCN_W17_NoWarning_24);
				// gfx[i]->setCursor(40, 26);
				// gfx[i]->print("预警");
				gfx[i]->setFont(u8g2_font_wqy13_t_gb2312);
				gfx[i]->setTextColor(color);
				gfx[i]->setCursor(0, 65);
				gfx[i]->println(weather.warnings[i].title);
				gfx[i]->println(weather.warnings[i].starttime);
			}
		}
	}
}
void dispCity()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	gfx[2]->fillScreen(BLACK);
	gfx[2]->setTextColor(QINGSHUILAN);
	gfx[2]->setUTF8Print(TRUE);
	gfx[2]->setFont(u8g2_font_wqy13_t_gb2312);
	gfx[2]->getTextBounds(weather.cityname_cn, 0, 0, &x1, &y1, &w, &h);
	gfx[2]->setCursor((OLED_WIDTH - w) / 2, 60);
	gfx[2]->print(weather.cityname_cn);
	gfx[2]->getTextBounds(weather.citycode, 0, 0, &x1, &y1, &w, &h);
	gfx[2]->setCursor((OLED_WIDTH - w) / 2, 75);
	gfx[2]->print(weather.citycode);
}

typedef void (*dispContent)();
dispContent disp[] = {
	dispToday,
	dispPredict,
	dispWarning,
};
const uint8_t Indexnum = ARRAY_SIZE(disp);

static void init(void *data)
{
	dispIndex = 0;
}

static void enter(void *data)
{
	// insert code

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	dispProcessing(1);
	if (weather.citycode == "NA")
	{
		getCity(&weather);
	}
	dispCity();
	getNowWeather(&weather);
	dispNowTemp();
	dispHumidity();
	dispWeather();
	dispWind();
	dispProcessing(2);
	getNowAir(&weather);
	dispAir();
	getDay3Weather(&weather);
	dispMtempAndSun();
	getWarning(&weather);
	if (weather.warnings.size() > 0)
	{
		getCNWarningTitle(&weather);
	}
	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{
	case ENC_NEXT:
		dispIndex++;
		if (dispIndex >= Indexnum)
		{
			dispIndex = 0;
		}
		disp[dispIndex]();
		break;
	case ENC_PREV:
		dispIndex--;
		if (dispIndex < 0)
		{
			dispIndex = Indexnum - 1;
		}
		disp[dispIndex]();
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
page_t page_weather = {
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