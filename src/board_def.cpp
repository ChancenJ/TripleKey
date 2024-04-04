#include "board_def.h"

WiFiManager wm;

#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
AsyncWebServer server(80);

char stored_weather_key[40];
char stored_weather_city[40];

BleKeyboard bleKeyboard("TripleKey", "ChancenJ", 100);

DFRobot_CH423 ch423;

// flag for saving data
bool shouldSaveConfig = false;

// callback notifying us of the need to save config
void saveConfigCallback()
{
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

struct tm timeInfo;

Button2 button1, button2, button3, button4;

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(PIN_EC_A, PIN_EC_B);

int rst = PIN_RST;
int backlight = PIN_BL;

int mosi = PIN_MOSI;
int sclk = PIN_SCLK;
int dc = PIN_DC;

int cs1 = PIN_CS1;
int cs2 = PIN_CS2;
int cs3 = PIN_CS3;

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus1 = new Arduino_ESP32SPI(dc /*dc*/, cs1 /*cs*/, sclk /*sclk*/, mosi /*mosi*/);
// Arduino_DataBus *bus1 = new Arduino_SWSPI(dc /*dc*/, cs1 /*cs*/, sclk /*sclk*/, mosi /*mosi*/,GFX_NOT_DEFINED /*miso*/);
/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx1 = new Arduino_NV3023(bus1, -1, 2 /* rotation */,false,128,128);

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus2 = new Arduino_ESP32SPI(dc /*dc*/, cs2 /*cs*/, sclk /*sclk*/, mosi /*mosi*/);
// Arduino_DataBus *bus2 = new Arduino_SWSPI(dc /*dc*/, cs2 /*cs*/, sclk /*sclk*/, mosi /*mosi*/,GFX_NOT_DEFINED /*miso*/);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx2 = new Arduino_NV3023(bus2, -1, 2 /* rotation */,false,128,128);

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus3 = new Arduino_ESP32SPI(dc /*dc*/, cs3 /*cs*/, sclk /*sclk*/, mosi /*mosi*/);
// Arduino_DataBus *bus3 = new Arduino_SWSPI(dc /*dc*/, cs3 /*cs*/, sclk /*sclk*/, mosi /*mosi*/,GFX_NOT_DEFINED /*miso*/);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx3 = new Arduino_NV3023(bus3, -1, 2 /* rotation */,false,128,128);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

Arduino_GFX *gfx[3] = {gfx1, gfx2, gfx3};

PNG png;

int16_t w, h, xOffset, yOffset;

// Functions to access a file on the SD card
File pngFile;

void *myOpen(const char *filename, int32_t *size)
{

    pngFile = LittleFS.open(filename, "r");
    // pngFile = SPIFFS.open(filename, "r");

    if (!pngFile || pngFile.isDirectory())
    {
        // Serial.println(F("ERROR: Failed to open  file for reading"));
        // gfx2->println(F("ERROR: Failed to open  file for reading"));
    }
    else
    {
        *size = pngFile.size();
        // Serial.printf("Opened '%s', size: %d\n", filename, *size);
    }

    return &pngFile;
}

void myClose(void *handle)
{
    if (pngFile)
        pngFile.close();
}

int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length)
{
    if (!pngFile)
        return 0;
    return pngFile.read(buffer, length);
}

int32_t mySeek(PNGFILE *handle, int32_t position)
{
    if (!pngFile)
        return 0;
    return pngFile.seek(position);
}

// Function to draw pixels to the display
void PNGDraw1(PNGDRAW *pDraw)
{
    uint16_t usPixels[320];
    uint8_t usMask[320];

    // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
    png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
    png.getAlphaMask(pDraw, usMask, 1);
    gfx1->draw16bitRGBBitmap(xOffset, yOffset + pDraw->y, usPixels,  pDraw->iWidth, 1);
}

void PNGDraw2(PNGDRAW *pDraw)
{
    uint16_t usPixels[320];
    uint8_t usMask[320];

    // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
    png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
    png.getAlphaMask(pDraw, usMask, 1);
    gfx2->draw16bitRGBBitmap(xOffset, yOffset + pDraw->y, usPixels,  pDraw->iWidth, 1);
}

void PNGDraw3(PNGDRAW *pDraw)
{
    uint16_t usPixels[320];
    uint8_t usMask[320];

    // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
    png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
    png.getAlphaMask(pDraw, usMask, 1);
    gfx3->draw16bitRGBBitmap(xOffset, yOffset + pDraw->y, usPixels,  pDraw->iWidth, 1);
}


void myDrawPNG(int16_t x, int16_t y, const char *path, uint8_t oled_index)
{
    unsigned long start = millis();
    int rc;
    PNG_DRAW_CALLBACK *pfnDraw = NULL;
    switch (oled_index)
    {
    case 0:
        pfnDraw = PNGDraw1;
        break;
    case 1:
        pfnDraw = PNGDraw2;
        break;
    case 2:
        pfnDraw = PNGDraw3;
        break;
    default:
        break;
    }

    rc = png.open(path, myOpen, myClose, myRead, mySeek, pfnDraw);
    if (rc == PNG_SUCCESS)
    {
        // int16_t pw = png.getWidth();
        // int16_t ph = png.getHeight();

        xOffset = x; //+(OLED_WIDTH - pw) / 2;
        yOffset = y; //+(OLED_HEIGHT - ph) / 2;

        rc = png.decode(NULL, 0);

        // Serial.printf("Draw offset: (%d, %d), time used: %lu\r\n", xOffset, yOffset, millis() - start);
        // Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\r\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
        png.close();
        // delay(5000);
    }
    else
    {
        gfx[oled_index]->setFont(u8g_font_5x7);
        gfx[oled_index]->setCursor(x, y + 20);
        gfx[oled_index]->println("png failed!");
    }
}

void DrawPNGCentre(const char *path, uint8_t oled_index){
    unsigned long start = millis();
    int rc;
    PNG_DRAW_CALLBACK *pfnDraw = NULL;
    switch (oled_index)
    {
    case 0:
        pfnDraw = PNGDraw1;
        break;
    case 1:
        pfnDraw = PNGDraw2;
        break;
    case 2:
        pfnDraw = PNGDraw3;
        break;
    default:
        break;
    }
    rc = png.open(path, myOpen, myClose, myRead, mySeek, pfnDraw);
    if (rc == PNG_SUCCESS)
    {
        int16_t pw = png.getWidth();
        int16_t ph = png.getHeight();
        xOffset =(OLED_WIDTH - pw) / 2;
        yOffset =(OLED_HEIGHT - ph) / 2;
        rc = png.decode(NULL, 0);

        // Serial.printf("Draw offset: (%d, %d), time used: %lu\r\n", xOffset, yOffset, millis() - start);
        // Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\r\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
        png.close();
        // delay(5000);
    }
    else
    {
        gfx[oled_index]->setFont(u8g_font_5x7);
        gfx[oled_index]->setCursor(5, 20);
        gfx[oled_index]->println("png failed!");
    }

}

void configModeCallback(WiFiManager *myWiFiManager)
{
    gfx2->setTextColor(RED);
    gfx2->println("Entered config mode");
    gfx2->setTextColor(RED, WHITE);
    gfx2->println(WiFi.softAPIP());
    gfx2->setTextColor(RED);
    gfx2->println("Please connect to");
    gfx2->println(myWiFiManager->getConfigPortalSSID());
    gfx2->setTextColor(QINGSHUILAN);
}


void audio_poweroff()
{
    pinMode(PIN_AUDIO_SD,OUTPUT);
    digitalWrite(PIN_AUDIO_SD,LOW);
 
}

void audio_poweron()
{
    pinMode(PIN_AUDIO_SD,OUTPUT);
    digitalWrite(PIN_AUDIO_SD,HIGH);
 
}




unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void dispProcessing(uint8_t screen){
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;
	String processing = "正在查询…";
	gfx[screen]->setTextColor(QINGSHUILAN);
	gfx[screen]->setUTF8Print(TRUE);
	gfx[screen]->setFont(DreamHanSerifCN_W17_Processing_24);
	gfx[screen]->getTextBounds(processing, 0, 0, &x1, &y1, &w, &h);
	gfx[screen]->setCursor((OLED_WIDTH - w) / 2, (OLED_HEIGHT - h) / 2 - y1);
	gfx[screen]->print(processing);
}






void board_init()
{
    WiFi.mode(WIFI_STA);
    randomSeed(0);
    // Serial.begin(115200);
    delay(10);
    Serial.println("start");

    audio_poweroff();

    app_led_init();

    pinMode(rst, OUTPUT);
    digitalWrite(rst, 1);
    delay(100);
    digitalWrite(rst, 0);
    delay(100);
    digitalWrite(rst, 1);
    delay(100);


    
    pinMode(backlight, OUTPUT);
    digitalWrite(backlight, 1);

    pinMode(BUTTON3_PIN, INPUT);


#ifdef SUPPORT_PCF8574
    Wire.begin(SDA_PIN, SCL_PIN);  // 初始化I2C总线，指定SDA和SCL引脚
    Wire.setClock(400000); // 设置I2C时钟频率为400kHz
    Wire.beginTransmission(PCF8574_ADDRESS); // 开始I2C传输
    Wire.write(0xff); // 初始化所有引脚为高电平
    Wire.endTransmission(); // 结束I2C传输
#endif
#ifdef SUPPORT_CH423S
    Wire.begin(SDA_PIN, SCL_PIN); 
    ch423.begin();
    ch423.pinMode(ch423.eGPO, ch423.ePUSH_PULL);
#endif

    gfx1->begin(40000000);
    gfx1->fillScreen(BLACK);
    w = gfx1->width(), h = gfx1->height();

    gfx2->begin(40000000);
    gfx2->fillScreen(BLACK);

    gfx3->begin(40000000);
    gfx3->fillScreen(BLACK);

    if (LittleFS.begin(true))
    {
        Serial.println("mounted file system");
        if (LittleFS.exists("/config.json"))
        {
            // file exists, reading and loading
            Serial.println("reading config file");
            File configFile = LittleFS.open("/config.json", "r");
            if (configFile)
            {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);

#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
                DynamicJsonDocument json(1024);
                auto deserializeError = deserializeJson(json, buf.get());
                serializeJson(json, Serial);
                if (!deserializeError)
                {
#else
                DynamicJsonBuffer jsonBuffer;
                JsonObject &json = jsonBuffer.parseObject(buf.get());
                json.printTo(Serial);
                if (json.success())
                {
#endif
                    Serial.println("\nparsed json");
                    strcpy(stored_weather_city, json["weather_city"]);
                    strcpy(stored_weather_key, json["weather_key"]);
                }
                else
                {
                    Serial.println("failed to load json config");
                }
                configFile.close();
            }
        }
    }
    else
    {
        Serial.println("failed to mount FS");
    }

    myDrawPNG(0, 0, "/TRI.png", 0);
    myDrawPNG(0, 0, "/PLE.png", 1);
    myDrawPNG(0, 0, "/KEY.png", 2);

    delay(1000);

    for (uint8_t i = 0; i < OLED_HEIGHT; i++)
    {
        gfx1->drawFastHLine(0, i, OLED_WIDTH, BLACK);
        gfx2->drawFastHLine(0, i, OLED_WIDTH, BLACK);
        gfx3->drawFastHLine(0, i, OLED_WIDTH, BLACK);
        delay(5);
    }

    gfx1->setTextColor(QINGSHUILAN);
    gfx2->setTextColor(QINGSHUILAN);
    gfx3->setTextColor(QINGSHUILAN);

    gfx1->setFont(u8g2_font_6x10_mr);
    gfx2->setFont(u8g2_font_6x10_mr);
    gfx3->setFont(u8g2_font_6x10_mr);

    // if (!SPIFFS.begin())

    gfx1->setCursor(0, 7);
    gfx1->printf("HW: %s\r\n", VER_HW);
    gfx1->printf("SW: %s\r\n", VER_SW);

    gfx1->printf("Free rom: %dKB\r\n", (LittleFS.totalBytes() - LittleFS.usedBytes()) / 1024);
    // gfx3->printf("Free rom: %dKB\r\n", (SPIFFS.totalBytes() - SPIFFS.usedBytes()) / 1024);

    // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    wm.setAPCallback(configModeCallback);
    // set config save notify callback
    wm.setSaveConfigCallback(saveConfigCallback);

    WiFiManagerParameter custom_text("<H1><font color=\"blue\">和风天气</font></H1>");
    wm.addParameter(&custom_text);
    WiFiManagerParameter custom_weather_city("weather_city", "城市拼音", stored_weather_city, 40);
    WiFiManagerParameter custom_weather_key("weather_key", "私钥", stored_weather_key, 40);

    wm.addParameter(&custom_weather_city);
    wm.addParameter(&custom_weather_key);

    // wm.setSaveConfigCallback();
    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    if (digitalRead(BUTTON2_PIN) == 0)
    {
        wm.resetSettings();
    }

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    // res = wm.autoConnect("AutoConnectAP","password"); // password protected ap
    gfx2->setCursor(0, 7);
    gfx2->println("WiFi Connecting ");

    // and goes into a blocking loop awaiting configuration
    if (!wm.autoConnect("TripleKey"))
    {
        gfx2->println("failed to connect and hit timeout");
        delay(3000);
        // reset and try again, or maybe put it to deep sleep
        ESP.restart();
        delay(5000);
    }

    // read updated parameters
    strcpy(stored_weather_city, custom_weather_city.getValue());
    strcpy(stored_weather_key, custom_weather_key.getValue());

    Serial.println("The values in the file are: ");
    Serial.println("\tweather_city : " + String(stored_weather_city));
    Serial.println("\tweather_key: " + String(stored_weather_key));

    // save the custom parameters to FS
    if (shouldSaveConfig)
    {
        Serial.println("saving config");
#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        DynamicJsonDocument json(1024);
#else
        DynamicJsonBuffer jsonBuffer;
        JsonObject &json = jsonBuffer.createObject();
#endif
        json["weather_city"] = stored_weather_city;
        json["weather_key"] = stored_weather_key;

        File configFile = LittleFS.open("/config.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
        }

#if defined(ARDUINOJSON_VERSION_MAJOR) && ARDUINOJSON_VERSION_MAJOR >= 6
        serializeJson(json, Serial);
        serializeJson(json, configFile);
#else
        json.printTo(Serial);
        json.printTo(configFile);
#endif
        configFile.close();
        // end save
    }

    gfx2->println("WiFi Connected ");
    gfx2->println("IP address: ");
    gfx2->setTextColor(QINGSHUILAN);
    gfx2->println(WiFi.localIP()); // 显示连接WIFI后的IP地址
    gfx2->setTextColor(QINGSHUILAN);
    gfx2->print("RSSI: ");      // 显示连接WIFI后的IP地址
    gfx2->println(WiFi.RSSI()); // 显示连接WIFI后的IP地址
    delay(1000);
    if (digitalRead(BUTTON3_PIN) == 0)
    {
        gfx3->setCursor(0, 14);
        gfx3->setTextSize(2);
        gfx3->println("OTA"); //
        gfx3->setTextSize(1);
        gfx3->print("http://");      // ota地址    http://<IPAddress>/update
        gfx3->print(WiFi.localIP()); // ota地址
        gfx3->println("/update");    // ota地址

        server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
                  { request->send(200, "text/plain", "http://<IPAddress>/update"); });

        ElegantOTA.begin(&server); // Start ElegantOTA
          // ElegantOTA callbacks
        ElegantOTA.onStart(onOTAStart);
        ElegantOTA.onProgress(onOTAProgress);
        ElegantOTA.onEnd(onOTAEnd);
        server.begin();
        while (1)
            ;
    }

    gfx3->setCursor(0, 7);

    gfx3->print("time updating"); // 显示连接WIFI后的IP地址
    configTime(8 * 3600, 0, NTP1, NTP2, NTP3);
    while (!getLocalTime(&timeInfo))
    {
        if (gfx3->getCursorY() > (63 - 7))
        {
            gfx3->setCursor(0, 7);
            gfx3->fillScreen(BLACK);
        }
        gfx3->print('.');
        delay(100);
    }

    // WiFi.mode(WIFI_OFF);
    WiFi.setSleep(true);

    bleKeyboard.begin();
    app_led_off();
}
