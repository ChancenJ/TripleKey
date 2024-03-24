#include "stocks.h"
#include "board_def.h"
#include "app/app_key.h"

static void init(void *data)
{
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	gfx1->setFont(u8g2_font_6x10_mr);
	gfx1->setCursor(0, 15);
	gfx1->print("Stocks");
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