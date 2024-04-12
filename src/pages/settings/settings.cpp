#include "settings.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_settings.h"


// 处理根路由，返回配置页面

	// <p>股票: <input type=\"text\" name=\"param1\" class=\"custom-input\" value=\"" +
	// 			  configData1 + "\"></p>\
	// <p>米家: <input type=\"text\" name=\"param2\" value=\"" +
	// 			  configData2 + "\"></p>\
	// <p>天气: <input type=\"text\" name=\"param3\" value=\"" +
	// 			  configData3 + "\"></p>\


static void init(void *data)
{
}

static void enter(void *data)
{

	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	gfx1->setUTF8Print(true);
	dispProcessing(1);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
	}
	Serial.println("");
	Serial.print("Connected to WiFi. IP address: ");
	Serial.println(WiFi.localIP());

	// 挂载LittleFS文件系统
	if (!LittleFS.begin())
	{
		Serial.println("LittleFS mount failed");
		return;
	}
	Serial.println("LittleFS mounted");

	// // 检查配置文件是否存在，如果不存在则创建
	// File configFile = LittleFS.open(CONFIG_FILE, "r");
	// if (!configFile)
	// {
	// 	Serial.println("Config file not found, creating new one.");
	// 	createNewConfigFile();
	// }


	// 设置Web服务器的路由
	server.on("/", HTTP_GET, handleRoot);
	server.on("/config", HTTP_POST, handleConfigPost);
	server.onNotFound(notFoundHandler);

	// 启动Web服务器
	server.begin();
	Serial.println("Web server started");
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
page_t page_settings = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Settings",
	.title_cn = "设置",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};