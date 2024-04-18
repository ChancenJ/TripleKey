#include "clock.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_audio.h"
const char *number_name[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
// char *clock_name[] = {"matrix", "nixie", "vfd", "digital", "14seg", "bina", "flip", "ink", "purple", "cyan", "nimo", "wood", "pixiepro", "predator", "rgb", "dhf"};

String clock_name[36];
static uint8_t CLOCK_TYPE_MAX = 0;
int8_t clock_type_index;
bool f_updateall = false;
static uint8_t num_old[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#define POS_Y 0

static uint8_t xpos = 64;

extern page_t page_weather;
extern page_t page_mijia;

void dispTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	char path[100];

	// gfx1->setCursor(0, 0);
	// gfx1->setTextSize(8);
	// gfx1->setTextColor(random(0xffff), BLACK);
	// gfx1->printf("%02d", hour);
	// gfx1->draw16bitBeRGBBitmap(0, POS_Y, (uint16_t *)clock_img[hour / 10], xpos, 96);
	// gfx1->draw16bitBeRGBBitmap(xpos, POS_Y, (uint16_t *)clock_img[hour % 10], xpos, 96);
	if (num_old[0] != hour / 10 || f_updateall)
	{
		num_old[0] = hour / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[hour / 10]);
		myDrawPNG(0, POS_Y, path, 0);
	}
	if (num_old[1] != hour % 10 || f_updateall)
	{
		num_old[1] = hour % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[hour % 10]);
		myDrawPNG(xpos, POS_Y, path, 0);
	}

	// gfx2->setCursor(0, 0);
	// gfx2->setTextSize(8);
	// gfx2->setTextColor(random(0xffff), BLACK);
	// gfx2->printf("%02d", min);
	// gfx2->draw16bitBeRGBBitmap(0, POS_Y, (uint16_t *)clock_img[min / 10], xpos, 96);
	// gfx2->draw16bitBeRGBBitmap(xpos, POS_Y, (uint16_t *)clock_img[min % 10], xpos, 96);
	if (num_old[2] != min / 10 || f_updateall)
	{
		num_old[2] = min / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[min / 10]);
		myDrawPNG(0, POS_Y, path, 1);
	}
	if (num_old[3] != min % 10 || f_updateall)
	{
		num_old[3] = min % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[min % 10]);
		myDrawPNG(xpos, POS_Y, path, 1);
	}

	// gfx3->setCursor(0, 0);
	// gfx3->setTextSize(8);
	// gfx3->setTextColor(random(0xffff), BLACK);
	// gfx3->printf("%02d", sec);
	// gfx3->draw16bitBeRGBBitmap(0, POS_Y, (uint16_t *)clock_img[sec / 10], xpos, 96);
	// gfx3->draw16bitBeRGBBitmap(xpos, POS_Y, (uint16_t *)clock_img[sec % 10], xpos, 96);
	if (num_old[4] != sec / 10 || f_updateall)
	{
		num_old[4] = sec / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[sec / 10]);
		myDrawPNG(0, POS_Y, path, 2);
	}
	if (num_old[5] != sec % 10 || f_updateall)
	{
		num_old[5] = sec % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[clock_type_index], number_name[sec % 10]);
		myDrawPNG(xpos, POS_Y, path, 2);
	}

	f_updateall = false;
}

// 防止烧屏，一分钟切换一次界面风格显示
void dispTimeX(uint8_t hour, uint8_t min, uint8_t sec)
{
	char path[100];

	static int8_t index = random(CLOCK_TYPE_MAX);
	if (sec == 0)
	{
		index++;
		if (index >= CLOCK_TYPE_MAX)
		{
			index = 0;
		}
		f_updateall = true;
	}

	if (num_old[0] != hour / 10 || f_updateall)
	{
		num_old[0] = hour / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[index], number_name[hour / 10]);
		// Serial.println(index);
		// Serial.println(clock_name[index]);

		myDrawPNG(0, POS_Y, path, 0);
	}
	if (num_old[1] != hour % 10 || f_updateall)
	{
		num_old[1] = hour % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[index], number_name[hour % 10]);

		myDrawPNG(xpos, POS_Y, path, 0);
	}

	if (num_old[2] != min / 10 || f_updateall)
	{
		num_old[2] = min / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[index], number_name[min / 10]);

		myDrawPNG(0, POS_Y, path, 1);
	}
	if (num_old[3] != min % 10 || f_updateall)
	{
		num_old[3] = min % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[index], number_name[min % 10]);

		myDrawPNG(xpos, POS_Y, path, 1);
	}

	if (num_old[4] != sec / 10 || f_updateall)
	{
		num_old[4] = sec / 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[index], number_name[sec / 10]);

		myDrawPNG(0, POS_Y, path, 2);
	}
	if (num_old[5] != sec % 10 || f_updateall)
	{
		num_old[5] = sec % 10;
		sprintf(path, "/clock_theme/%s/%s.png", clock_name[index], number_name[sec % 10]);

		myDrawPNG(xpos, POS_Y, path, 2);
	}

	f_updateall = false;
}
// code from https://github.com/VolosR/SimplestFontTTGO/blob/main/TTGONumbersFont/TTGONumbersFont.ino
const byte arr[10][5][3] =
	{
		{{1, 1, 1}, // 0
		 {1, 0, 1},
		 {1, 0, 1},
		 {1, 0, 1},
		 {1, 1, 1}

		},
		{{0, 1, 0}, // 1
		 {0, 1, 0},
		 {0, 1, 0},
		 {0, 1, 0},
		 {0, 1, 0}

		},
		{{1, 1, 1}, // 2
		 {0, 0, 1},
		 {1, 1, 1},
		 {1, 0, 0},
		 {1, 1, 1}

		},
		{{1, 1, 1}, // 3
		 {0, 0, 1},
		 {1, 1, 1},
		 {0, 0, 1},
		 {1, 1, 1}

		},
		{{1, 0, 1}, // 4
		 {1, 0, 1},
		 {1, 1, 1},
		 {0, 0, 1},
		 {0, 0, 1}

		},
		{{1, 1, 1}, // 5
		 {1, 0, 0},
		 {1, 1, 1},
		 {0, 0, 1},
		 {1, 1, 1}

		},
		{{1, 1, 1}, // 6
		 {1, 0, 0},
		 {1, 1, 1},
		 {1, 0, 1},
		 {1, 1, 1}

		},
		{{1, 1, 1}, // 7
		 {0, 0, 1},
		 {0, 0, 1},
		 {0, 0, 1},
		 {0, 0, 1}

		},
		{{1, 1, 1}, // 8
		 {1, 0, 1},
		 {1, 1, 1},
		 {1, 0, 1},
		 {1, 1, 1}

		},
		{{1, 1, 1}, // 9
		 {1, 0, 1},
		 {1, 1, 1},
		 {0, 0, 1},
		 {1, 1, 1}

		}};

short colors[2] = {BLACK, GREEN}; // first colour is color of background , second is color of digit

int sizee = 20;	   // size of each box
byte space = 8;	   // space between boxes
int fromTop = 14;  // positon x
int fromLeft = 10; // position y
int Round = 0;

short number = 0;
int digits = 0;

void dispDotNum(int number, Arduino_GFX *gfx)
{
	String n = String(number);

	for (int z = 0; z < n.length(); z++)
		for (int i = 0; i < 5; i++)
			for (int j = 0; j < 3; j++)
			{
				String c = n.substring(z, z + 1);
				int b = c.toInt();

				gfx->fillRoundRect((z * (sizee * 4)) + fromLeft + (j * sizee) + (j * space), fromTop + (i * sizee) + (i * space), sizee, sizee, Round, colors[arr[b][i][j]]);
				// delay(20);
			}
}

int pos = 0;
int x = 0;
int y = 30;
int changeX = 1;
int changeY = 1;

void dispTimeDot(uint8_t hour, uint8_t min, uint8_t sec)
{

	delay(400);
	number++;

	if (number == 100)
		number = 0;

	f_updateall = false;
}

// 获取时钟类型数目
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
	// Serial.printf("Listing directory: %s\r\n", dirname);

	File root = fs.open(dirname);
	if (!root)
	{
		// Serial.println("- failed to open directory");
		return;
	}
	if (!root.isDirectory())
	{
		// Serial.println(" - not a directory");
		return;
	}

	File file = root.openNextFile();
	while (file)
	{
		if (file.isDirectory())
		{
			// Serial.print("  DIR : ");
			// Serial.println(file.name());
			clock_name[CLOCK_TYPE_MAX] = file.name();
			// Serial.println(clock_name[dir_num]);
			CLOCK_TYPE_MAX++;

			if (levels)
			{
				listDir(fs, file.path(), levels - 1);
			}
		}

		file = root.openNextFile();
	}
}

static void init(void *data)
{
	if (CLOCK_TYPE_MAX == 0)
	{
		// 获取时钟类型数目
		listDir(LittleFS, "/clock_theme/", 1);

		Serial.println(CLOCK_TYPE_MAX);
		clock_type_index = CLOCK_TYPE_MAX;
	}

	for (uint8_t i = 0; i < 6; i++)
	{
		num_old[i] = 0xff;
	}
	// clock_type_index=12;
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	//
	manager_setBusy(false);
}
uint32_t t_old, t_now = 0;
uint8_t hour, minute, second = 0;
static void loop(void *data)
{
	KEY_TYPE key;

	t_now = millis();
	if (t_now - t_old >= 1000)
	{
		t_old = t_now;
		if (getLocalTime(&timeInfo))
		{

			if (clock_type_index == CLOCK_TYPE_MAX)
			{
				dispTimeX(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
			}
			else
			{
				dispTime(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
			}

			if (timeInfo.tm_min == 0 && timeInfo.tm_sec == 0 && timeInfo.tm_hour > 7 && timeInfo.tm_hour < 23)
			{
				app_audio_sayTimeCN(timeInfo.tm_hour, timeInfo.tm_min);
			}

			// Serial.printf("%02d:%02d:%02d\r\n", timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		}
	}

	key = app_key_get();
	switch (key)
	{
	case KEY1_DOUBLE:
		manager_switchToPage(&page_weather);
		break;
	case KEY1_LONG:
		manager_switchToPage(&page_mijia);
		break;
	// case KEY1_SHORT:
	case KEY2_SHORT:

		getLocalTime(&timeInfo);
		app_audio_sayTimeCN(timeInfo.tm_hour, timeInfo.tm_min);
		break;

	case KEY4_LONG:				  // 长按
		manager_switchToParent(); // 进入父项目 //退出
		break;

	case ENC_NEXT:
		clock_type_index++;
		if (clock_type_index >= (CLOCK_TYPE_MAX + 1))
		{
			clock_type_index = 0;
		}
		f_updateall = true;
		if (clock_type_index == CLOCK_TYPE_MAX)
		{
			dispTimeX(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		}
		else
		{
			dispTime(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		}
		break;
	case ENC_PREV:
		clock_type_index--;
		if (clock_type_index < 0)
		{
			clock_type_index = CLOCK_TYPE_MAX;
		}
		f_updateall = true;
		if (clock_type_index == CLOCK_TYPE_MAX)
		{
			dispTimeX(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		}
		else
		{
			dispTime(timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
		}
		break;
	default:
		break;
	}
}

static void exit(void *data)
{

	manager_setBusy(true);
}
#include "img.h"
page_t page_clock = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Clock",
	.title_cn = "时间",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};