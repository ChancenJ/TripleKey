#include "countdown.h"//修改
#include "board_def.h"
#include "app/app_key.h"

static uint8_t setmin;
static uint8_t setsec;

static uint8_t nowmin;
static uint8_t nowsec;

static unsigned long lastUpdateTime;
static unsigned long NewTime;

void dispCountDown(){
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	char timestr[15];
	sprintf(timestr,"%02d:%02d",nowmin,nowsec);
	gfx[0]->fillScreen(BLACK);
	gfx[0]->setFont(&MiSans_Demibold_12);
	gfx[0]->getTextBounds(timestr, 0, 0, &x1, &y1, &w, &h);
	gfx[0]->setCursor((OLED_WIDTH - w) / 2, (OLED_HEIGHT - h) / 2 - y1);
	gfx[0]->print(timestr);
}

static void init(void *data)
{
	setmin=1;
	setsec=0;
	nowmin=setmin;
	nowsec=setsec;
	lastUpdateTime=millis();
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	gfx[0]->setTextColor(RED);
	dispCountDown();
	//
	manager_setBusy(false);
}

static void loop(void *data)
{	
	NewTime=millis();
	if(NewTime-lastUpdateTime>=1000){
		nowsec--;
		if(nowsec<0){
			nowmin--;
			nowsec=59;
		}
	}
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
page_t page_countdown = { 
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "countdown",
	.title_cn = "倒计时",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};