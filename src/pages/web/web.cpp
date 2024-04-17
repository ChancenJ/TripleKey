#include "web.h"
#include "board_def.h"
#include "app/app_key.h"
// static const char *webstring[][2] = {
// 	//{"https://www.mouser.cn/", "/web/mouser.png"},
// 	{"https://www.bilibili.com/", "/web/bili.png"},
// 	//{"https://hackaday.io/projects", "/web/hackaday.png"},
// 	//{"http://bbs.eeworld.com.cn/", "/web/eeworld.png"},
// 	//{"https://www.digikey.cn/", "/web/digikey.png"},
// 	{"https://gitee.com/chancenj","/web/gitee.png"},
// 	{"https://www.szlcsc.com/", "/web/lcsc.png"},
// 	{"https://oshwhub.com/", "/web/oshwhub.png"},
// 	{"https://github.com/", "/web/github.png"},
	
// 	// {"https://www.mouser.cn/", "/web/mouser.png"},

// };

static uint8_t maxweb = webstring.size();

static int8_t pageindex;
static int8_t index_num;

static void dispWeb()
{
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	for(uint8_t i =0 ;i<3;i++){
		if ((pageindex * 3 + i) < maxweb)
		DrawPNGCentre(("/web/"+webstring[pageindex * 3 + i][1]).c_str(), i);
	}
	// gfx[1]->setTextColor(BLUE >> 1);
	// // gfx[1]->setFont(u8g2_font_10x20_mr);
	// gfx[1]->setFont(&Orbitron_Medium_12);
	// gfx[1]->setCursor(0, 63);
	// gfx[1]->printf("%d %d %d",maxweb,pageindex,index_num);
}
static void gotoWeb(String web)
{
 
	bleKeyboard.releaseAll();
	bleKeyboard.write(KEY_MEDIA_WWW_HOME);
	delay(800);
	bleKeyboard.println(web);
	delay(200);

	bleKeyboard.write(KEY_DELETE);
	delay(200);
	bleKeyboard.write(KEY_RETURN); //回车
}

static void init(void *data)
{
	// bleKeyboard.begin();
	maxweb = webstring.size();
	pageindex = 0;
	index_num = (maxweb % 3 == 0) ? (maxweb / 3) : (maxweb / 3 + 1);
	Serial.println(sizeof(webstring));
}

static void enter(void *data)
{
	// insert code

	dispWeb();
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
			if (pageindex * 3 < maxweb)
				gotoWeb(webstring[pageindex * 3][0]);
		}
		break;

	case KEY2_DOWN:
		if (bleKeyboard.isConnected())
		{
			if ((pageindex * 3 + 1) < maxweb)
				gotoWeb(webstring[pageindex * 3 + 1][0]);
		}
		break;
	case KEY3_DOWN:
		if (bleKeyboard.isConnected())
		{
			if ((pageindex * 3 + 2) < maxweb)
				gotoWeb(webstring[pageindex * 3 + 2][0]);
		}
		break;
	case ENC_NEXT:
		pageindex++;
		if (pageindex >= index_num)
		{
			pageindex = 0;
		}
		dispWeb();

		break;
	case ENC_PREV:
		pageindex--;
		if (pageindex < 0)
		{
			pageindex = index_num - 1;
		}
		dispWeb();

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
page_t page_web = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Web",
	.title_cn = "网页",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};