#include "mijia.h"
#include "board_def.h"
#include "app/app_key.h"

static void init(void *data)
{
	app_key_clean();
	digitalWrite(MIJIAKEY1_PIN,HIGH);
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	gfx1->setFont(&MiSans_Demibold_12);
	gfx1->setCursor(20, 50);
	gfx1->print("KEY5");
	
	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	delay(300);
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{

	case KEY1_SHORT:
		Serial.println("米家单击");
		digitalWrite(MIJIAKEY1_PIN,LOW);
		delay(100);
		digitalWrite(MIJIAKEY1_PIN,HIGH);
		delay(100);
		break;
		
	case KEY1_DOUBLE:
		Serial.println("米家双击");
		digitalWrite(MIJIAKEY1_PIN,LOW);
		delay(100);
		digitalWrite(MIJIAKEY1_PIN,HIGH);
		delay(100);
		digitalWrite(MIJIAKEY1_PIN,LOW);
		delay(100);
		digitalWrite(MIJIAKEY1_PIN,HIGH);
		delay(100);
		break;

	case KEY1_LONG:
		Serial.println("米家长按");
		digitalWrite(MIJIAKEY1_PIN,LOW);
		delay(2000);
		digitalWrite(MIJIAKEY1_PIN,HIGH);
		delay(200);
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
page_t page_mijia = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "MiHome",
	.title_cn = "米家",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};