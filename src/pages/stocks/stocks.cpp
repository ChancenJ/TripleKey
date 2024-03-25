#include "stocks.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_stocks.h"

#define QINGSHUILAN RGB565(147, 213, 220)

StockInfo stocks[] = {
	StockInfo("上证指数", "sh000001"),
	StockInfo("深证成指", "sz399001"),
	StockInfo("创业板指", "sz399006"),
	StockInfo("中证白酒", "sz399997"),
	StockInfo("中证医疗", "sz399989"),
	StockInfo("科创100", "sh000698"),
	StockInfo("科创50", "sh000688"),
	StockInfo("沪深300", "sz399300"),
	StockInfo("半导体ETF", "sh512480"),
	StockInfo("中证消费", "sh000932"),
	StockInfo("中证军工", "sz399967"),
	//StockInfo("中证银行", "sz399986"),
	StockInfo("新能源车LOF", "sz161028"),
};

static const uint8_t maxstocks = sizeof(stocks) / sizeof(StockInfo);

unsigned long lastUpdateTime;
unsigned long lastScrollTime;
unsigned long lastBreathelTime;
static int8_t pageindex;
static int8_t index_num;
static int8_t firstgot; // 首次是否已获取数据

static uint32_t t_old, t_now = 0;

static uint8_t brightness;  //呼吸灯亮度
static uint8_t lighttend;  //呼吸灯变化趋势，0为变亮，1为变暗

char timestr[20];
char datastr[40];
char *nowtimetitle="时间";

void dispStocks()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	for (uint8_t i = 0; i < 3; i++)
	{
		gfx[i]->fillRect(0,26,128,102,BLACK);
		gfx[i]->setUTF8Print(true);
		if ((pageindex * 3 + i) < maxstocks)
		{
			uint16_t color = (stocks[pageindex * 3 + i].difference.toFloat() > 0) ? RED : ((stocks[pageindex * 3 + i].difference.toFloat() < 0) ? DARKGREEN : DARKGREY);
			gfx[i]->setFont(DreamHanSerifCN_W15_21);
			gfx[i]->setTextColor(color);
			gfx[i]->setCursor(5, 120);
			if(stocks[pageindex * 3 + i].difference.toFloat() > 0){
				gfx[i]->print("▲");
			}else if(stocks[pageindex * 3 + i].difference.toFloat() < 0){
				gfx[i]->print("▼");
			}
			gfx[i]->getTextBounds(stocks[pageindex * 3 + i].name, 0, 0, &x1, &y1, &w, &h);
			gfx[i]->setCursor((OLED_WIDTH - w) / 2, 60);
			gfx[i]->print(stocks[pageindex * 3 + i].name);
			gfx[i]->setFont(&MiSans_Demibold_12);
			gfx[i]->getTextBounds(stocks[pageindex * 3 + i].price, 0, 0, &x1, &y1, &w, &h);
			gfx[i]->setCursor((OLED_WIDTH - w) / 2, 90);
			gfx[i]->print(stocks[pageindex * 3 + i].price);
			gfx[i]->setTextSize(0.5);
			gfx[i]->getTextBounds(stocks[pageindex * 3 + i].percentage, 0, 0, &x1, &y1, &w, &h);
			gfx[i]->setCursor((OLED_WIDTH - w) / 2, 120);
			gfx[i]->print(stocks[pageindex * 3 + i].percentage);
		}
	}
}

void dispTime()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	
	for(int i=0;i<3;i++){
		gfx[i]->setTextColor(BLUE);
		gfx[i]->setFont(&MiSans_Demibold_12);
	}
	gfx[0]->setFont(DreamHanSerifCN_W15_21);
	gfx[0]->getTextBounds(nowtimetitle, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2, 22);
	gfx[0]->print(nowtimetitle);
	sprintf(datastr, "%d-%d-%d", timeInfo.tm_year+1900, timeInfo.tm_mon+1, timeInfo.tm_mday);
	gfx[1]->getTextBounds(datastr, 0, 0, &x1, &y1, &w, &h);
	gfx[1]->setCursor((OLED_WIDTH - w) / 2, 22);
	gfx[1]->fillRect(0,0,128,26,QINGSHUILAN);
	gfx[1]->print(datastr);
	sprintf(timestr, "%02d:%02d:%02d", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
	gfx[2]->getTextBounds(timestr, 0, 0, &x1, &y1, &w, &h);
	gfx[2]->setCursor((OLED_WIDTH - w) / 2, 22);
	gfx[2]->fillRect(0,0,128,26,QINGSHUILAN);
	gfx[2]->print(timestr);
}

void Scroll() // 轮播股票
{
	if (millis() - lastScrollTime > 10 * 1000)
	{
		lastScrollTime = millis();
		pageindex++;
		if (pageindex >= index_num)
		{
			pageindex = 0;
		}
		dispStocks();
		Serial.println("Auto Next Page");
	}
}

void BreatheLight(uint8_t brigthness){
	for(int i=0;i<3;i++){
		if(stocks[pageindex * 3 + i].difference.toFloat()>0){
			app_led_set(2-i,app_led_color(0x70, 0x00, 0x00));
		}else if(stocks[pageindex * 3 + i].difference.toFloat()<0){
			app_led_set(2-i, app_led_color(0, 0x70, 0));
		}
	}
	app_led_brightness(brigthness);
	app_led_update();
}

static void init(void *data)
{
	lastUpdateTime = millis(); // 重置时间

	pageindex = 0;
	index_num = (maxstocks % 3 == 0) ? (maxstocks / 3) : (maxstocks / 3 + 1);
	firstgot = 0;
	Serial.println(maxstocks);
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	for (int i = 0; i < maxstocks; i++)
	{
		getInfo(&stocks[i]);
	}
	dispStocks();
	lastScrollTime = millis();
	lastBreathelTime = millis();
	brightness=255;
	lighttend=1;
	t_now = millis();
	t_old = t_now;
	gfx[0]->fillRect(0,0,128,26,QINGSHUILAN);
	if (getLocalTime(&timeInfo))
	{
		dispTime();
	}
	firstgot = 1;
	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	t_now = millis();
	if (t_now - t_old >= 1000)  //更新时间
	{
		t_old = t_now;
		if (getLocalTime(&timeInfo))
		{
			dispTime();
		}
	}
	if(millis()-lastBreathelTime>=50){  //呼吸灯
		if(lighttend==0){
			brightness++;
			if(brightness==255){
				lighttend=1;
			}
		}else if(lighttend==1){
			brightness--;
			if(brightness==0){
				lighttend=0;
			}
		}
		BreatheLight(brightness);
		lastBreathelTime=millis();
	}
	if (millis() - lastUpdateTime >= 2 * 60 * 1000) // 限制请求频率
	{
		if ((timeInfo.tm_hour >= 16 || timeInfo.tm_hour <= 8 || timeInfo.tm_hour == 12 || (timeInfo.tm_hour == 9 && timeInfo.tm_min <= 20) || (timeInfo.tm_hour == 15 && timeInfo.tm_min >= 35)) && firstgot == 1)
		{
			Serial.println("休市时间暂停获取数据"); // 限制请求时间
		}
		else
		{
			for (int i = 0; i < maxstocks; i++)
			{
				getInfo(&stocks[i]);
			}
		}
		lastUpdateTime = millis();
	}
	Scroll();

	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{

	case ENC_NEXT:
		lastScrollTime = millis();
		pageindex++;
		if (pageindex >= index_num)
		{
			pageindex = 0;
		}
		dispStocks();
		break;
	case ENC_PREV:
		lastScrollTime = millis();
		pageindex--;
		if (pageindex < 0)
		{
			pageindex = index_num - 1;
		}
		dispStocks();
		break;
	case KEY4_LONG:				  // 长按
		app_led_off();
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
page_t page_stocks = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Stocks",
	.title_cn = "股票",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};