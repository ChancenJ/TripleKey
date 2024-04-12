#include "bilibili.h"
#include "board_def.h"
#include "app/app_key.h"

#include "yijiansanlian.h"
#include "danmu.h"
#include "quanping.h"


/*
		按下F，就可以进入全屏模式，再按下F就可以退出全屏模式
		按下Q，可以为视频点赞。
		按下W，可以为视频投币。
		按下E，可以收藏视频。
		按下M，可以静音视频。
		按下D，可以关闭/开启弹幕。
		shift + 1: 一倍速；shift + 2: 二倍速
*/

static const char *key[] = {"Q", "W", "E", "F", "M", "D"};

static void init(void *data)
{
}

static void enter(void *data)
{
	// insert code
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

 
	gfx[0]->drawXBitmap(40, 0, yijiansanlian_bits,yijiansanlian_width, yijiansanlian_height, random(0xffff));
	gfx[1]->drawXBitmap(40, 0, danmu_bits,danmu_width, danmu_height, random(0xffff));
	gfx[2]->drawXBitmap(40, 0, quanping_bits,quanping_width, quanping_height, random(0xffff));


 
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
			// bleKeyboard.releaseAll();
			// bleKeyboard.release(KEY_LEFT_SHIFT);
			uint16_t color = random(0xffff);
			bleKeyboard.press('q');
			gfx[0]->drawRoundRect(37,50,54,10,3,color<<1);
			gfx[0]->fillRoundRect(40,52,50,6,2,BLACK);
			int x=50;
			while(x)
			{
				gfx[0]->drawXBitmap(40, 0, yijiansanlian_bits,yijiansanlian_width, yijiansanlian_height,color );
				gfx[0]->drawRoundRect(37,50,54,10,3,color<<1);
				gfx[0]->fillRoundRect(40,52,50-x,6,2,color);


				delay(40);
				x--;
			}
			// delay(2000);
			bleKeyboard.release('q');
			
		}
		break;
	case KEY2_DOWN:
		if (bleKeyboard.isConnected())
		{

			// bleKeyboard.releaseAll();
			// bleKeyboard.release(KEY_LEFT_SHIFT);
			
			bleKeyboard.write('d');
			gfx[1]->drawXBitmap(40, 0, danmu_bits,danmu_width, danmu_height, random(0xffff));
	
		}
		break;
	case KEY3_DOWN:
		if (bleKeyboard.isConnected())
		{
			// bleKeyboard.releaseAll();

			// bleKeyboard.release(KEY_LEFT_SHIFT);

			bleKeyboard.write('f');
			gfx[2]->drawXBitmap(40, 0, quanping_bits,quanping_width, quanping_height, random(0xffff));
		}
		break;
	case KEY4_SHORT:
		if (bleKeyboard.isConnected())
		{
			bleKeyboard.write(KEY_MEDIA_MUTE);
		}
		break;
	case ENC_NEXT:
		if (bleKeyboard.isConnected())
		{
			bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
		}
		break;
	case ENC_PREV:
		if (bleKeyboard.isConnected())
		{
			bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
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

	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_bilibili = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Bilibili",
	.title_cn = "哔哩哔哩",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};