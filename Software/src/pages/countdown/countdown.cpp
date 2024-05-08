#include "countdown.h" //修改
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_audio.h"

typedef enum
{
	SET,
	RUNNING,
	PAUSE,
} Mode;

static Mode mode = SET;
static uint8_t set;

static bool flicker;

static int8_t settime[3] = {0, 5, 0};
static int8_t nowtime[3] = {0, 5, 0};

static unsigned long lastFlickerTime;
static unsigned long lastCountdownTime;
static unsigned long lastBarUpdateTime;
static unsigned long StartTime;
static unsigned long PauseTime;
static unsigned long ContinueTime;
static long spendms = 0;

void dispSetNum(uint8_t set)
{
	gfx[set]->setCursor(5, 90);
	gfx[set]->printf("%02d", settime[set]);
}
void dispNowNum(uint8_t set)
{
	gfx[set]->fillRect(0, 10, 112, 108, BLACK);
	gfx[set]->setCursor(5, 90);
	gfx[set]->printf("%02d", nowtime[set]);
}

void dispHMS()
{
	for (int i = 0; i < 3; i++)
	{
		gfx[i]->setFont(&GillSansEN_Bold_12);
		gfx[i]->setCursor(112, 90);
	}
	gfx[0]->print("h");
	gfx[1]->print("m");
	gfx[2]->print("s");
	for (int i = 0; i < 3; i++)
	{
		gfx[i]->setFont(&MiSans_Regular45pt_number);
	}
}

void CountDown()
{
	if (millis() - lastCountdownTime >= 1000)
	{
		lastCountdownTime = millis();
		nowtime[2]--;
		if (nowtime[2] >= 0)
		{
			dispNowNum(2);
		}
		else
		{
			nowtime[1]--;
			nowtime[2] = 59;
			if (nowtime[1] >= 0)
			{
				dispNowNum(2);
				dispNowNum(1);
			}
			else
			{
				nowtime[0]--;
				nowtime[1] = 59;
				if (nowtime[0] >= 0)
				{
					dispNowNum(2);
					dispNowNum(1);
					dispNowNum(0);
				}
				else
				{
					mode = PAUSE;
					for (int i = 0; i < 3; i++)
					{
						gfx[i]->setTextColor(RED);
						nowtime[i] = 0;
						dispNowNum(i);
						app_led_set(i, app_led_color(0x70, 0x00, 0x00));
						app_led_update();
					}
					dispHMS();
					app_audio_play_wav("/end.wav");
				}
			}
		}
	}
}

void SetFlicker()
{
	if (millis() - lastFlickerTime >= 500)
	{
		lastFlickerTime = millis();
		if (flicker)
		{
			gfx[set]->fillRect(0, 10, 112, 108, BLACK);
		}
		else
		{
			dispSetNum(set);
		}
		flicker = !flicker;
		
	}
}

void dispProgressBar()
{
	unsigned long setms = (settime[0] * 3600 + settime[1] * 60 + settime[2]) * 1000;
	float ratio = (float)setms / spendms;
	uint16_t barlength = ceil(128 * 3 / ratio);
	if (barlength <= 128)
	{
		gfx[2]->fillRect(128 - barlength, 118, barlength, 10, BLACK);
	}
	else if (barlength <= 256)
	{
		gfx[2]->fillRect(0, 118, 128, 10, BLACK);
		gfx[1]->fillRect(256 - barlength, 118, barlength - 128, 10, BLACK);
	}
	else
	{
		gfx[2]->fillRect(0, 118, 128, 10, BLACK);
		gfx[1]->fillRect(0, 118, 128, 10, BLACK);
		gfx[0]->fillRect(384 - barlength, 118, barlength - 256, 10, BLACK);
	}
}

static void init(void *data)
{
	set = 2;
	flicker = true;
}

static void enter(void *data)
{
	// insert code
	if (mode == SET)
	{
		for (int i = 0; i < 3; i++)
		{
			gfx[i]->fillScreen(BLACK);
			gfx[i]->setFont(&MiSans_Regular45pt_number);
			gfx[i]->setTextColor(JINZI);
			dispSetNum(i);
		}
		dispHMS();
	}
	else if (mode == PAUSE)
	{
		for (int i = 0; i < 3; i++)
		{
			gfx[i]->fillScreen(BLACK);
			gfx[i]->setFont(&MiSans_Regular45pt_number);
			gfx[i]->setTextColor(ORANGE);
			dispNowNum(i);
			gfx[i]->fillRect(0, 118, 128, 10, ORANGE);
			app_led_set(i, app_led_color(255, 220, 0));
			app_led_update();
		}
		dispProgressBar();
		dispHMS();
	}
	else
	{
		unsigned long spendsec = (millis() - lastCountdownTime) / 1000;
		Serial.printf("页面外消耗时间：%ds\n", spendsec);
		unsigned long remainsec = nowtime[0] * 3600 + nowtime[1] * 60 + nowtime[2];
		Serial.printf("退出前剩余时间：%ds\n", remainsec);
		if (spendsec < remainsec)
		{
			remainsec -= spendsec;
			nowtime[0] = remainsec / 3600;
			remainsec -= nowtime[0] * 3600;
			nowtime[1] = remainsec / 60;
			remainsec -= nowtime[1] * 60;
			nowtime[2] = remainsec;
		}
		else
		{
			nowtime[0] = 0;
			nowtime[1] = 0;
			nowtime[2] = 0;
		}
		lastCountdownTime = millis();
		for (int i = 0; i < 3; i++)
		{
			gfx[i]->fillScreen(BLACK);
			gfx[i]->setFont(&MiSans_Regular45pt_number);
			gfx[i]->setTextColor(JINZI);
			dispNowNum(i);
			gfx[i]->fillRect(0, 118, 128, 10, ORANGE);
		}
		dispHMS();
	}
	lastFlickerTime = millis();
	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	if (mode == SET)
	{
		SetFlicker();
	}
	if (mode == RUNNING)
	{
		CountDown();
		if (millis() - lastBarUpdateTime >= 100)
		{
			spendms += millis() - lastBarUpdateTime;
			lastBarUpdateTime = millis();
			dispProgressBar();
		}
	}
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{

	case KEY1_DOWN:
		if (mode == SET)
		{
			dispSetNum(set);
			set = 0;
		}

		break;
	case KEY2_DOWN:
		if (mode == SET)
		{
			dispSetNum(set);
			set = 1;
		}
		break;
	case KEY3_DOWN:
		if (mode == SET)
		{
			dispSetNum(set);
			set = 2;
		}
		break;
	case KEY4_SHORT:
		if (mode == SET)
		{
			for (int i = 0; i < 3; i++)
			{
				nowtime[i] = settime[i];
				dispNowNum(i);
				gfx[i]->fillRect(0, 118, 128, 10, ORANGE);
			}
			lastCountdownTime = millis();
			lastBarUpdateTime = millis();
			StartTime = millis();
			spendms = 0;
			mode = RUNNING;
			Serial.println("开始");
		}
		else if (mode == RUNNING)
		{
			mode = PAUSE;
			PauseTime = millis();
			Serial.println("暂停");
			for (int i = 0; i < 3; i++)
			{
				gfx[i]->setTextColor(ORANGE);
				dispNowNum(i);
				app_led_set(i, app_led_color(255, 220, 0));
				app_led_update();
			}
			dispHMS();
		}
		else
		{
			mode = RUNNING;
			ContinueTime = millis();
			spendms -= ContinueTime - PauseTime;
			Serial.println("继续");
			for (int i = 0; i < 3; i++)
			{
				gfx[i]->setTextColor(JINZI);
				dispNowNum(i);
			}
			dispHMS();
			app_led_off();
		}
		break;
	case KEY4_DOUBLE:
		if (mode == RUNNING || mode == PAUSE)
		{
			mode = SET;
			Serial.println("终止");
			for (int i = 0; i < 3; i++)
			{
				gfx[i]->fillRect(0, 10, 112, 108, BLACK);
				gfx[i]->setTextColor(JINZI);
				dispSetNum(i);
				gfx[i]->fillRect(0, 118, 128, 10, ORANGE);
			}
			dispHMS();
			lastFlickerTime = millis();
			app_led_off();
		}
		break;
	case ENC_NEXT:
		settime[set]++;
		if (settime[set] >= 60)
		{
			settime[set] = 0;
		}
		break;
	case ENC_PREV:
		settime[set]--;
		if (settime[set] < 0)
		{
			settime[set] = 59;
		}
		break;

	case KEY4_LONG: // 长按
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