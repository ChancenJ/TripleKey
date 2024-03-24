#include "media.h"
#include "board_def.h"
#include "app/app_key.h"

static void init(void *data)
{
	// bleKeyboard.begin();
}

static void enter(void *data)
{
	// insert code

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	myDrawPNG(16, 0, "/media/prev.png", 0);
	myDrawPNG(16, 0, "/media/play.png", 1);
	myDrawPNG(16, 0, "/media/next.png", 2);

	// while (bleKeyboard.isConnected() == false)
	// {
	// 	delay(20);
	// }
 

	
	// gfx[0]->setFont(u8g2_font_4x6_mr);
	// gfx[0]->drawChar(0, 6, 'B', BLUE, BLACK);

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
		if (bleKeyboard.isConnected())
		{

			bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
		}
		break;
	case KEY2_DOWN:
		if (bleKeyboard.isConnected())
		{

			bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
		}
		break;
	case KEY3_DOWN:
		if (bleKeyboard.isConnected())
		{

			bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
		}
		break;

	case ENC_NEXT:
		if (bleKeyboard.isConnected())
		{

			bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
		}
		break;
	case ENC_PREV:
		if (bleKeyboard.isConnected())
		{

			bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
		}
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
	
	// bleKeyboard.end();
	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_media = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Media",
	.title_cn = "多媒体",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};