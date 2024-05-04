#include "photo.h" //修改
#include "board_def.h"
#include "app/app_key.h"

std::vector<String> photos;

uint8_t photonum = 0;
static int8_t pageindex;
static int8_t index_num;

bool photoscroll = true;

static unsigned long lastScrollTime;

static void dispPhotos()
{
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);

	for (uint8_t i = 0; i < 3; i++)
	{
		if ((pageindex * 3 + i) < photonum)
		{
			String path = "/photo/" + photos[pageindex * 3 + i];
			DrawPNGCentre(path.c_str(), i);
		}
	}
}

static void listPhotos()
{
	// Serial.printf("Listing directory: %s\r\n", dirname);

	File root = LittleFS.open("/photo/");
	if (!root)
	{
		return;
	}
	if (!root.isDirectory())
	{
		return;
	}
	photos.clear();
	File file = root.openNextFile();
	while (file)
	{
		String filename = file.name();
		if (filename.endsWith(".png"))
		{
			Serial.println(filename);
			photos.push_back(filename);
		}
		file = root.openNextFile();
	}
}

static void init(void *data)
{
	if (photonum == 0)
	{
		listPhotos();
		photonum = photos.size();
		Serial.println(photonum);
	}
	else
	{
		photonum = photos.size();
	}
	pageindex = 0;
	index_num = (photonum % 3 == 0) ? (photonum / 3) : (photonum / 3 + 1);
}

static void enter(void *data)
{
	// insert code
	lastScrollTime = millis();
	dispPhotos();

	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	if (photoscroll && index_num > 1)
	{
		if (millis() - lastScrollTime > 10 * 1000)
		{
			pageindex++;
			if (pageindex >= index_num)
			{
				pageindex = 0;
			}
			dispPhotos();
			lastScrollTime = millis();
			Serial.println("Auto Next Page");
		}
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
		dispPhotos();
		break;
	case ENC_PREV:
		pageindex--;
		if (pageindex < 0)
		{
			pageindex = index_num - 1;
		}
		dispPhotos();
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
page_t page_photo = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Photo",
	.title_cn = "照片",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};