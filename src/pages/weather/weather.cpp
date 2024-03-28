#include "weather.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_weather.h"
TaskHandle_t weatherTask_Handler = NULL;
QueueHandle_t weatherQueue = NULL;
static bool f_init = false;
static weather_t weather[3];
void drawWeatherNow(weather_t *weather)
{
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	gfx1->setFont(&Orbitron_Medium_12);

	char filepath[48];
	sprintf(filepath, "/weather/white/%s@1x.png", weather->code_now);

	myDrawPNG(24, 0, filepath, 0);

	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

	gfx1->setTextColor(GREENYELLOW);
	gfx1->setFont(&Orbitron_Medium_16);
	gfx1->getTextBounds(weather->text_now, 0, 0, &x1, &y1, &w, &h);
	gfx1->setCursor((OLED_WIDTH - w) / 2, 45 + h);
	gfx1->println(weather->text_now);

	myDrawPNG(0, 32, "/weather/Temperature.png", 1);

	gfx2->setTextColor(GREEN);
	gfx2->setFont(&DSEG14_Classic_Regular_40);
	gfx2->getTextBounds(weather->temperature_now, 0, 0, &x1, &y1, &w, &h);
	gfx2->setCursor((24), h + 10);
	gfx2->print(weather->temperature_now);

	myDrawPNG(0, 32, "/weather/Humidity.png", 2);
	gfx3->setTextColor(BLUE);
	gfx3->setFont(&DSEG14_Classic_Regular_40);
	gfx3->getTextBounds(weather->humidity_now, 0, 0, &x1, &y1, &w, &h);
	gfx3->setCursor((24), h + 10);
	gfx3->print(weather->humidity_now);
}
void drawWeather3days(weather_t *weather)
{
	// gfx2->setFont(&Orbitron_Medium_12);
	// gfx2->setCursor(0, 12);

	// gfx2->println(weather[0].text_day);
	// gfx2->println(weather[1].text_day);
	// gfx2->println(weather[2].text_day);
}
void TaskWeather(void *pvParameters)
{
	(void)pvParameters;
	weather_t weather[3];

	for (;;)
	{	
		app_weather_get_now(&weather[0]);
		app_weather_get_3(&weather[0], &weather[1], &weather[2]);
		// Serial.println(weather_1->text_now);
		xQueueSend(weatherQueue, weather, 0);

		vTaskDelay(10 * 60000 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}
static void init(void *data)
{

	if (f_init == false)
	{
		gfx1->fillScreen(BLACK);
		gfx2->fillScreen(BLACK);
		gfx3->fillScreen(BLACK);

		gfx1->setFont(&Orbitron_Medium_12);

		myDrawPNG(24, 0, "/weather/white/99@1x.png", 0);

		int16_t x1;
		int16_t y1;
		uint16_t w;
		uint16_t h;

		gfx1->setTextColor(GREENYELLOW);
		gfx1->setFont(&Orbitron_Medium_16);
		gfx1->getTextBounds("___", 0, 0, &x1, &y1, &w, &h);
		gfx1->setCursor((OLED_WIDTH - w) / 2, 47 + h);
		gfx1->print("___");

		myDrawPNG(0, 32, "/weather/Temperature.png", 1);

		myDrawPNG(0, 32, "/weather/Humidity.png", 2);

		f_init = true;
	}
	else
	{
		drawWeatherNow(&weather[0]);
	}

	if (weatherTask_Handler == NULL)
	{
		xTaskCreate(TaskWeather, "TaskWeather" // A name just for humans
					,
					10240 // This stack size can be checked & adjusted by reading the Stack Highwater
					,
					NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
					,
					&weatherTask_Handler);
	}
	else
	{
		vTaskResume(weatherTask_Handler);
	}

	//创建消息队列
	if (weatherQueue == NULL)
	{
		weatherQueue = xQueueCreate(2, sizeof(weather_t) * 3);
	}
}

static void enter(void *data)
{
	// insert code

	//
	manager_setBusy(false);
}

static void loop(void *data)
{

	KEY_TYPE key;
	if (xQueueReceive(weatherQueue, weather, 1) == pdPASS)
	{
		f_init = true;
		drawWeatherNow(&weather[0]);
		drawWeather3days(weather);
	}
	key = app_key_get();
	switch (key)
	{

	case KEY1_DOWN:

		break;

	case KEY4_LONG:				  //长按
		manager_switchToParent(); //进入父项目 //退出
		break;
	default:
		break;
	}
}

static void exit(void *data)
{
	// insert code
	// vTaskDelete(weatherTask_Handler);
	vTaskSuspend(weatherTask_Handler);
	xQueueReset(weatherQueue);
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