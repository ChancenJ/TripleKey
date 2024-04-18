#include "settings.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_settings.h"

#include "QRCode_Library.h"

void dispInfo()
{
	gfx[0]->fillScreen(BLACK);
	gfx[0]->setTextColor(QINGSHUILAN);
	gfx[0]->setCursor(0, 13);
	//gfx[0]->setFont(u8g2_font_7x13B_mr);
	//gfx[0]->println("Enter the URL:");
	gfx[0]->setUTF8Print(true);
	gfx[0]->setFont(u8g2_font_wqy13_t_gb2312a);
	gfx[0]->println("进入网页:");
	gfx[0]->println(WiFi.localIP());
	gfx[0]->println("或");
	gfx[0]->println("扫描右侧二维码");
	gfx[0]->println(" ");
	gfx[0]->println("短按左键快捷进入网页");
	gfx[0]->println("长按中键重启设备");

	gfx[2]->fillScreen(BLACK);
	gfx[2]->setTextColor(QINGSHUILAN);
	gfx[2]->setCursor(0, 13);
	gfx[2]->setUTF8Print(true);
	gfx[2]->setFont(u8g2_font_wqy13_t_gb2312a);
	gfx[2]->print("软件版本: V");
	gfx[2]->println(VER_SW);
	gfx[2]->println(" ");
	gfx[2]->println("项目地址:");
	gfx[2]->println("https://github.com/ChancenJ/TripleKey");
	
	myDrawPNG(0,100,"/Copyright.png",2);
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
void drawQRCode(QRCode qrcode, uint8_t magnification /*放大倍数*/, uint8_t xpos /*左上角x坐标*/, uint8_t ypos /*左上角y坐标*/,uint8_t screen/*屏幕*/)
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
	drawQRCode(qrcode,4,(OLED_WIDTH-qrcode.size*4)/2,(OLED_HEIGHT-qrcode.size*4)/2,1);
}


static void gotoConfig()
{
	String url = WiFi.localIP().toString();
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

	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
	}
	dispInfo();
	dispQRCode();
	Serial.println("");
	Serial.print("Connected to WiFi. IP address: ");
	Serial.println(WiFi.localIP());

	// // 挂载LittleFS文件系统
	// if (!LittleFS.begin())
	// {
	// 	Serial.println("LittleFS mount failed");
	// 	return;
	// }
	// Serial.println("LittleFS mounted");

	// 设置Web服务器的路由
	server.on("/", HTTP_GET, handleRoot);
	server.on("/config", HTTP_POST, handleConfigPost);
	server.on("/uploadpng",HTTP_GET, handleUploadPNG);
	server.on("/weather",HTTP_POST, handleWeather);
	server.on("/listfiles",HTTP_GET, handleListFile);
	server.on("/delete",HTTP_GET,handleDelete);
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