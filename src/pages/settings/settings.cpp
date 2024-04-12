#include "settings.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_settings.h"

#include "QRCode_Library.h"

void dispInfo()
{
	gfx[0]->fillScreen(BLACK);
	gfx[0]->setFont(u8g2_font_6x10_mr);
	gfx[0]->setTextColor(QINGSHUILAN);
	gfx[0]->setCursor(0, 10);
	gfx[0]->println("Enter the URL:");
	gfx[0]->println(WiFi.localIP());
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
	uint8_t qrcodeBytes[qrcode_getBufferSize(3)];
	qrcode_initText(&qrcode, qrcodeBytes, 3, ECC_LOW, ip);
	Serial.println(qrcode.size);
	drawQRCode(qrcode,3,(OLED_WIDTH-qrcode.size*3)/2,5,1);
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
		gotoConfig();
		break;
	case KEY3_DOWN:
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