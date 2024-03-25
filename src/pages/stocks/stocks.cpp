#include "stocks.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_stocks.h"

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
    StockInfo("新能源车LOF", "sz161028"),
}
;

static const uint8_t maxstocks = sizeof(stocks) / sizeof(StockInfo);

long lastUpdateTime;
long lastScrollTime;
static int8_t pageindex;
static int8_t index_num;
static int8_t firstgot; // 首次是否已获取数据

static uint8_t hour = timeInfo.tm_hour;
static uint8_t minute = timeInfo.tm_min;

void dispStocks()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	gfx1->setUTF8Print(true);
	gfx2->setUTF8Print(true);
	gfx3->setUTF8Print(true);

	for (uint8_t i = 0; i < 3; i++)
	{
		if ((pageindex * 3 + i) < maxstocks)
		{
			uint16_t color = (stocks[pageindex * 3 + i].difference.toFloat() > 0) ? RED : ((stocks[pageindex * 3 + i].difference.toFloat() < 0) ? DARKGREEN : DARKGREY);
			gfx[i]->setTextColor(color);
			gfx[i]->setFont(DreamHanSerifCN_W15_21);
			gfx[i]->getTextBounds(stocks[pageindex * 3 + i].name, 0, 0, &x1, &y1, &w, &h);
			gfx[i]->setCursor((OLED_WIDTH - w) / 2, (OLED_HEIGHT - h) / 2 - y1);
			gfx[i]->print(stocks[pageindex * 3 + i].name);
		}
	}
}

static void init(void *data)
{
	lastUpdateTime = LONG_MIN; // 重置时间
	lastScrollTime = LONG_MIN;
	pageindex = 0;
	index_num = (maxstocks % 3 == 0) ? (maxstocks / 3) : (maxstocks / 3 + 1);
	firstgot = 0;
	Serial.println(maxstocks);
	
}

static void enter(void *data)
{
	// insert code
	for (int i = 0; i < maxstocks; i++)
	{
		getInfo(&stocks[i]);
	}
	dispStocks();
	firstgot = 1;
	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	if (millis() - lastUpdateTime >= 2 * 60 * 1000)
	{
		if ((hour >= 16 || hour <= 8 || hour == 12 || (hour == 9 && minute <= 20) || (hour == 15 && minute >= 35)) && firstgot == 1)
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
		// 限制请求频率
	}
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{

	case ENC_NEXT:
		pageindex++;
		if (pageindex >= index_num)
		{
			pageindex = 0;
		}
		dispStocks();
		break;
	case ENC_PREV:
		pageindex--;
		if (pageindex < 0)
		{
			pageindex = index_num - 1;
		}
		dispStocks();
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