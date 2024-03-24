#include "pomodoro.h"
#include "board_def.h"
#include "app/app_key.h"

#define COLOR_pomodoro (PINK << 1)
#define COLOR_SHORT_BREAK (CYAN << 2)
#define COLOR_LONG_BREAK (BLUE << 2)
#define COLOR_DISABLE (DARKGREY)

static unsigned long t_old = 0;
static int t_pomodoro, t_short, t_long;
static bool f_start_pomodoro, f_start_short, f_start_long;

typedef enum
{
	TYPE_POMODORO = 0,
	TYPE_SHORT,
	TYPE_LONG
} type_t;

static type_t type;
static void init(void *data)
{
	f_start_pomodoro = false;
	f_start_short = false;
	f_start_long = false;
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	gfx1->setTextColor(COLOR_pomodoro);
	gfx1->setFont(u8g2_font_6x10_mr);
	gfx1->setCursor(0, 15);
	gfx1->print("Pomodoro");
	// gfx1->setFont(&Orbitron_Light_24);
	gfx1->setFont(u8g2_font_freedoomr25_mn);
	gfx1->setCursor(0, 55);
	gfx1->print("25:00");

	gfx2->setTextColor(COLOR_SHORT_BREAK);
	gfx2->setFont(u8g2_font_6x10_mr);
	gfx2->setCursor(0, 15);
	gfx2->print("Short Break");
	gfx2->setFont(u8g2_font_freedoomr25_mn);
	gfx2->setCursor(0, 55);
	gfx2->print("05:00");

	gfx3->setTextColor(COLOR_LONG_BREAK);
	gfx3->setFont(u8g2_font_6x10_mr);
	gfx3->setCursor(0, 15);
	gfx3->print("Long Break");
	gfx3->setFont(u8g2_font_freedoomr25_mn);
	gfx3->setCursor(0, 55);
	gfx3->print("15:00");

	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	KEY_TYPE key;
	if (f_start_pomodoro | f_start_long | f_start_short)
	{
		if (millis() - t_old >= 1000)
		{
			t_old = millis();

			// update
			t_pomodoro--;

			switch (type)
			{
			case TYPE_POMODORO:
				if (t_pomodoro == 0)
				{
					f_start_pomodoro = false;
				}
				gfx1->fillRect(0, 20, 96, 35, BLACK);
				gfx1->setCursor(0, 55);
				gfx1->setTextColor(COLOR_pomodoro);
				gfx1->printf("%02d:%02d", t_pomodoro / 60, t_pomodoro % 60);
				break;
			case TYPE_SHORT:
				if (t_pomodoro == 0)
				{
					f_start_short = false;
				}
				gfx2->fillRect(0, 20, 96, 35, BLACK);
				gfx2->setCursor(0, 55);
				gfx2->setTextColor(COLOR_SHORT_BREAK);
				gfx2->printf("%02d:%02d", t_pomodoro / 60, t_pomodoro % 60);
				break;
			case TYPE_LONG:
				if (t_pomodoro == 0)
				{
					f_start_long = false;
				}
				gfx3->fillRect(0, 20, 96, 35, BLACK);
				gfx3->setCursor(0, 55);
				gfx3->setTextColor(COLOR_LONG_BREAK);
				gfx3->printf("%02d:%02d", t_pomodoro / 60, t_pomodoro % 60);
				break;
			default:
				break;
			}
		}
	}

	key = app_key_get();
	switch (key)
	{

	case KEY1_DOWN:
		app_led_off();
		type = TYPE_POMODORO;
		if (f_start_pomodoro == false)
		{
			t_pomodoro = 25 * 60;
			app_led_set(LED1, app_led_color(0, 0x70, 0));
			app_led_update();
		}
		else
		{
			app_led_set(LED1, app_led_color(0x70, 0x00, 0x00));
			app_led_update();
		}
		f_start_pomodoro = !f_start_pomodoro;
		f_start_short = false;
		f_start_long = false;

		gfx2->setTextColor(COLOR_DISABLE);
		gfx2->fillRect(0, 20, 96, 35, BLACK);
		gfx2->setCursor(0, 55);
		gfx2->print("05:00");

		gfx3->setTextColor(COLOR_DISABLE);
		gfx3->fillRect(0, 20, 96, 35, BLACK);
		gfx3->setCursor(0, 55);
		gfx3->print("15:00");

		break;

	case KEY2_DOWN:
		app_led_off();
		type = TYPE_SHORT;
		if (f_start_short == false)
		{
			t_pomodoro = 5 * 60;
			app_led_set(LED2, app_led_color(0, 0x70, 0));
			app_led_update();
		}
		else
		{
			app_led_set(LED2, app_led_color(0x70, 0x00, 0x00));
			app_led_update();
		}
		f_start_short = !f_start_short;

		f_start_long = false;
		f_start_pomodoro = false;

		gfx1->setTextColor(COLOR_DISABLE);
		gfx1->fillRect(0, 20, 96, 35, BLACK);
		gfx1->setCursor(0, 55);
		gfx1->print("25:00");

		gfx3->setTextColor(COLOR_DISABLE);
		gfx3->fillRect(0, 20, 96, 35, BLACK);
		gfx3->setCursor(0, 55);
		gfx3->print("15:00");

		break;

	case KEY3_DOWN:
		app_led_off();
		type = TYPE_LONG;
		if (f_start_long == false)
		{
			t_pomodoro = 15 * 60;
			app_led_set(LED3, app_led_color(0, 0x70, 0));
			app_led_update();
		}
		else
		{
			app_led_set(LED3, app_led_color(0x70, 0x00, 0x00));
			app_led_update();
		}
		f_start_long = !f_start_long;

		f_start_short = false;
		f_start_pomodoro = false;

		gfx2->setTextColor(COLOR_DISABLE);
		gfx2->fillRect(0, 20, 96, 35, BLACK);
		gfx2->setCursor(0, 55);
		gfx2->print("05:00");

		gfx1->setTextColor(COLOR_DISABLE);
		gfx1->fillRect(0, 20, 96, 35, BLACK);
		gfx1->setCursor(0, 55);
		gfx1->print("25:00");

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
page_t page_pomodoro = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Pomodoro",
	.title_cn = "番茄钟",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};
