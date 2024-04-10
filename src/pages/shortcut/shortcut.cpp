#include "shortcut.h"
#include "board_def.h"
#include "app/app_key.h"
#include "fonts/GillSansEN_Bold_12.h"

typedef struct
{
	const char *title;
	const char *imgpath;
	uint8_t key[5];
	uint8_t key_num;
} shortcut_t;

shortcut_t sc[] =
	{
		{"Copy", "/shortcut/copy.png", {KEY_LEFT_CTRL, 'c'}, 2},
		{"Paste", "/shortcut/paste.png", {KEY_LEFT_CTRL, 'v'}, 2},
		{"Cut", "/shortcut/cut.png", {KEY_LEFT_CTRL, 'x'}, 2},
		{"Select All", "/shortcut/select_all.png", {KEY_LEFT_CTRL, 'a'}, 2},
		{"Desktop", "/shortcut/desktop.png", {KEY_LEFT_GUI, 'd'}, 2},
		{"Switch", "/shortcut/switch_windows.png", {KEY_LEFT_ALT, KEY_TAB}, 2},
		{"Undo", "/shortcut/undo.png", {KEY_LEFT_CTRL, 'z'}, 2},
		{"F5", "/shortcut/refresh.png", {KEY_F5}, 1},

};

static const uint8_t maxsc = sizeof(sc) / sizeof(shortcut_t);

static int8_t pageindex;
static int8_t index_num;

static void dispShortcut()
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	for (uint8_t i = 0; i < 3; i++)
	{
		if ((pageindex * 3 + i) < maxsc)
		{
			if (sc[pageindex * 3 + i].imgpath != NULL)
			{
				myDrawPNG(40, 0 + 30, sc[pageindex * 3 + i].imgpath, i);
				gfx[i]->setTextSize(1);
				gfx[i]->setTextColor(GREEN >> 1);
				gfx[i]->setFont(&GillSansEN_Bold_12);
				gfx[i]->getTextBounds(sc[pageindex * 3 + i].title, 0, 0, &x1, &y1, &w, &h);
				gfx[i]->setCursor((OLED_WIDTH - w) / 2, 63 + 30);
				gfx[i]->print(sc[pageindex * 3 + i].title);
			}
			else
			{
				gfx[i]->setTextSize(3);
				gfx[i]->setTextColor(GREEN);
				gfx[i]->setFont(&GillSansEN_Bold_12);
				gfx[i]->getTextBounds(sc[pageindex * 3 + i].title, 0, 0, &x1, &y1, &w, &h);
				gfx[i]->setCursor((OLED_WIDTH - w) / 2, (OLED_HEIGHT - h) / 2 - y1);
				gfx[i]->print(sc[pageindex * 3 + i].title);

				Serial.printf("%d %d %d %d\r\n", x1, y1, w, h);
			}
		}
	}
}

static void sendkey(uint8_t *key, uint8_t num)
{
	if (bleKeyboard.isConnected())
	{
		for (uint8_t i = 0; i < num; i++)
		{
			bleKeyboard.press(key[i]);
		}

		bleKeyboard.releaseAll();
	}
}

static void init(void *data)
{

	pageindex = 0;
	index_num = (maxsc % 3 == 0) ? (maxsc / 3) : (maxsc / 3 + 1);

	Serial.println(sizeof(maxsc));
}

static void enter(void *data)
{
	dispShortcut();
	manager_setBusy(false);
}

static void loop(void *data)
{
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{

	case KEY1_DOWN:
		app_led_set(LED1, app_led_color(random(10, 255), random(10, 255), random(10, 255)));
		app_led_update();
		if (bleKeyboard.isConnected())
		{
			if (pageindex * 3 < maxsc)
				sendkey(sc[pageindex * 3].key, sc[pageindex * 3].key_num);
		}
		break;

	case KEY2_DOWN:
		app_led_set(LED2, app_led_color(random(10, 255), random(10, 255), random(10, 255)));
		app_led_update();
		if (bleKeyboard.isConnected())
		{
			if ((pageindex * 3 + 1) < maxsc)
				sendkey(sc[pageindex * 3 + 1].key, sc[pageindex * 3 + 1].key_num);
		}
		break;
	case KEY3_DOWN:
		app_led_set(LED3, app_led_color(random(10, 255), random(10, 255), random(10, 255)));
		app_led_update();
		if (bleKeyboard.isConnected())
		{
			if ((pageindex * 3 + 2) < maxsc)
				sendkey(sc[pageindex * 3 + 2].key, sc[pageindex * 3 + 2].key_num);
		}
		break;
	case KEY1_UP:
	case KEY2_UP:
	case KEY3_UP:
		app_led_off();
		break;
	case ENC_NEXT:
		pageindex++;
		if (pageindex >= index_num)
		{
			pageindex = 0;
		}
		dispShortcut();
		break;
	case ENC_PREV:
		pageindex--;
		if (pageindex < 0)
		{
			pageindex = index_num - 1;
		}
		dispShortcut();
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

	// bleKeyboard.end();
	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_shortcut = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Shortcut",
	.title_cn = "快捷键",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};