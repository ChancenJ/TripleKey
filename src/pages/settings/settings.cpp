#include "settings.h"
#include "board_def.h"
#include "app/app_key.h"

// 定义配置文件名
const char *CONFIG_FILE = "/configstock.json";

// 定义全局变量用于存储配置参数
String configData1 = "";
String configData2 = "";
String configData3 = "";

void createNewConfigFile()
{
	DynamicJsonDocument doc(1024);
	doc["param1"] = "上证指数";
	doc["param2"] = "default_value2";
	doc["param3"] = "default_value3";

	File configFile = LittleFS.open(CONFIG_FILE, "w");
	if (!configFile)
	{
		Serial.println("Failed to open config file for writing.");
		return;
	}
	serializeJson(doc, Serial);
	serializeJson(doc, configFile);
	configFile.close();
	Serial.println("New config file created.");
}

void readConfig()
{
	File configFile = LittleFS.open(CONFIG_FILE, "r");
	if (configFile)
	{
		
		// Allocate a buffer to store contents of the file.
		String fileContent = configFile.readString();

		
		// 读取文件内容并解析参数
		DynamicJsonDocument doc(1024);
		
		deserializeJson(doc, fileContent);
		serializeJson(doc, Serial);
		configData1 = doc["param1"].as<String>();
		configData2 = doc["param2"].as<String>();
		configData3 = doc["param3"].as<String>();
		configFile.close();
	}
	else
	{
		Serial.println("Failed to open config file for reading.");
	}
}

void saveConfig(String param1, String param2, String param3)
{
	Serial.println("Saving config file...");
	File configFile = LittleFS.open(CONFIG_FILE, "w");
	if (!configFile)
	{
		Serial.println("Failed to open config file for writing.");
		return;
	}

	DynamicJsonDocument doc(1024);
	doc["param1"] = param1;
	doc["param2"] = param2;
	doc["param3"] = param3;
	serializeJson(doc, Serial);
	String jsonStr;
  	serializeJson(doc, jsonStr);
	configFile.print(jsonStr);
	configFile.close();
	Serial.println("Config saved.");
}

// 处理根路由，返回配置页面
void handleRoot(AsyncWebServerRequest *request)
{
	// 读取配置文件中的默认值
	readConfig();

	// 创建HTML表单页面
	String page = "<html><head><meta charset=\"UTF-8\"><title>TripleKey设置</title></head><body><h1>TripleKey 设置</h1><form accept-charset=\"UTF-8\" method=\"post\" action=\"/config\"><p>Parameter 1: <input type=\"text\" name=\"param1\" value=\"" + configData1 + "\"></p><p>Parameter 2: <input type=\"text\" name=\"param2\" value=\"" + configData2 + "\"></p><p>Parameter 3: <input type=\"text\" name=\"param3\" value=\"" + configData3 + "\"></p><input type=\"submit\" value=\"保存\"></form></body></html>";
	request->send(200, "text/html", page);
}

// 处理配置提交
void handleConfigPost(AsyncWebServerRequest *request)
{
	// 从表单中获取输入值
	String param1 = request->arg("param1");
	String param2 = request->arg("param2");
	String param3 = request->arg("param3");

	
	Serial.println(param1);

	// 保存配置
	saveConfig(param1, param2, param3);

	// 返回成功消息
	request->send(200, "text/html", "Configuration saved successfully.");
}

// 处理未找到的路由
void notFoundHandler(AsyncWebServerRequest *request)
{
	request->send(404, "text/html", "404: Page not found");
}



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

	// 检查配置文件是否存在，如果不存在则创建
	File configFile = LittleFS.open(CONFIG_FILE, "r");
	if (!configFile)
	{
		Serial.println("Config file not found, creating new one.");
		createNewConfigFile();
	}

	// 读取配置文件中的默认值
	readConfig();

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
		gfx[0]->setFont(DreamHanSerifCN_W17_21);
		gfx[0]->println(configData1);
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
	server.end();
	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_settings = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "settings",
	.title_cn = "设置",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};