#include "settings.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_settings.h"

#include "QRCode_Library.h"

void dispInstruction()
{
	gfx[0]->fillScreen(BLACK);
	gfx[0]->setTextColor(QINGSHUILAN);
	gfx[0]->setCursor(0, 13);
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(u8g2_font_wqy13_t_gb2312);
	gfx[0]->println("进入网页:");
	gfx[0]->println(WiFi.localIP());
	gfx[0]->println("或");
	gfx[0]->println("扫描右侧二维码");
	gfx[0]->println(" ");
	gfx[0]->println("单击左键快捷进入网页");
	gfx[0]->println("长按中键重启设备");
	gfx[0]->println("单击右键下载固件");
}
void dispInfo()
{
	gfx[2]->fillScreen(BLACK);
	gfx[2]->setTextColor(QINGSHUILAN);
	gfx[2]->setCursor(0, 13);
	gfx[2]->setUTF8Print(true);
	gfx[2]->setFont(u8g2_font_wqy13_t_gb2312);
	gfx[2]->print("软件版本: V");
	gfx[2]->println(VER_SW);
	if (NewVersion != VER_SW)
	{
		gfx[2]->setTextColor(ORANGE);
	}
	gfx[2]->print("最新版本: V");
	gfx[2]->println(NewVersion);
	gfx[2]->setTextColor(QINGSHUILAN);
	gfx[2]->println("项目地址:");
	gfx[2]->println("https://github.com/ChancenJ/TripleKey");

	myDrawPNG(0, 100, "/Copyright.png", 2);
	// int16_t x1;
	// int16_t y1;
	// uint16_t w;
	// uint16_t h;
	// gfx[2]->setFont(&GillSansEN_Bold_12);
	// gfx[2]->setTextColor(WHITE);
	// gfx[2]->getTextBounds("©ChancenJ", 0, 0, &x1, &y1, &w, &h);
	// gfx[2]->setCursor((OLED_WIDTH - w) / 2,120);
	// gfx[2]->println("©ChancenJ");
}
void drawQRCode(QRCode qrcode, uint8_t magnification /*放大倍数*/, uint8_t xpos /*左上角x坐标*/, uint8_t ypos /*左上角y坐标*/, uint8_t screen /*屏幕*/)
{
	for (uint8_t y = 0; y < qrcode.size; y++)
	{
		for (uint8_t x = 0; x < qrcode.size; x++)
		{
			if (qrcode_getModule(&qrcode, x, y))
			{
				gfx[screen]->fillRect(x * magnification + xpos, y * magnification + ypos, magnification, magnification, WHITE);
			}
			else
			{
				gfx[screen]->fillRect(x * magnification + xpos, y * magnification + ypos, magnification, magnification, BLACK);
			}
		}
	}
}
void dispQRCode()
{
	QRCode qrcode;
	char ip[20];
	WiFi.localIP().toString().toCharArray(ip, 20);
	uint8_t qrcodeBytes[qrcode_getBufferSize(2)];
	qrcode_initText(&qrcode, qrcodeBytes, 2, ECC_MEDIUM, ip);
	Serial.println(qrcode.size);
	drawQRCode(qrcode, 4, (OLED_WIDTH - qrcode.size * 4) / 2, (OLED_HEIGHT - qrcode.size * 4) / 2, 1);
}

static void gotoConfig()
{
	String url = WiFi.localIP().toString();
	bleKeyboard.releaseAll();
	bleKeyboard.write(KEY_MEDIA_WWW_HOME);
	delay(800);
	bleKeyboard.println(url);
}

static void gotoRelease()
{
	String url = "https://gitee.com/chancenj/triplekey/releases";
	bleKeyboard.releaseAll();
	bleKeyboard.write(KEY_MEDIA_WWW_HOME);
	delay(800);
	bleKeyboard.println(url);
	delay(200);
	bleKeyboard.write(KEY_DELETE);
	delay(200);
	bleKeyboard.write(KEY_RETURN); // 回车
}

static void init(void *data)
{
}

static void enter(void *data)
{
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	dispInstruction();
	dispQRCode();
	dispInfo();
	// insert code
	Serial.print("IP address: ");
	Serial.println(WiFi.localIP());

	// 设置Web服务器的路由
	server.on("/", HTTP_GET, handleRoot);
	server.on("/config", HTTP_POST, handleConfigPost);
	server.on("/uploadweb",HTTP_GET, handleUploadWeb);
	//server.on("/uploadphoto",HTTP_GET, handleUploadPhoto);
	server.on("/rom",HTTP_GET, handleROM);
	server.on("/weather",HTTP_POST, handleWeather);
	server.on("/listfiles",HTTP_GET, handleListFile);
	server.on("/delete",HTTP_GET,handleDelete);
	server.on("/moresettings",HTTP_GET,handleMoreSettings);
	server.on("/clock",HTTP_POST,handleClock);
	server.on("/encoder",HTTP_POST,handleEncoder);
	//server.on("/photo",HTTP_POST,handlePhoto);
	server.on("/firmware",HTTP_GET,handleFirmware);
	//server.serveStatic("/uploadpng",LittleFS,"/webserver/uploadpng.html").setTemplateProcessor(processor);
	server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
      }, handleUpload);
	server.onNotFound(notFoundHandler);
	server.onFileUpload(handleUpload);

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

	case KEY1_SHORT:
		gotoConfig();
		break;
	case KEY2_LONG:
		ESP.restart();
		break;
	case KEY3_SHORT:
		gotoRelease();
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