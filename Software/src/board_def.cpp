#include "board_def.h"
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

WiFiManager wm;
AsyncWebServer server(80);

char stored_weather_key[40];
char stored_weather_city[40];
char stored_weather_apihost[40];
std::vector<StockInfo> stocks;
std::vector<MijiaSwitch> sws;
std::vector<std::vector<String>> webstring;
uint8_t rotary = 4;       // 旋转编码器限位值（2或4）
bool enc_reverse = false; // 旋转编码器是否反向

BleKeyboard bleKeyboard("TripleKey", "ChancenJ", 100);

DFRobot_CH423 ch423;

String NewVersion = "NA";

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
Arduino_DataBus *bus2 = new Arduino_ESP32SPI(dc /*dc*/, cs2 /*cs*/, sclk /*sclk*/, mosi /*mosi*/);
// Arduino_DataBus *bus2 = new Arduino_SWSPI(dc /*dc*/, cs2 /*cs*/, sclk /*sclk*/, mosi /*mosi*/,GFX_NOT_DEFINED /*miso*/);
Arduino_DataBus *bus3 = new Arduino_ESP32SPI(dc /*dc*/, cs3 /*cs*/, sclk /*sclk*/, mosi /*mosi*/);
// Arduino_DataBus *bus3 = new Arduino_SWSPI(dc /*dc*/, cs3 /*cs*/, sclk /*sclk*/, mosi /*mosi*/,GFX_NOT_DEFINED /*miso*/);

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
#ifdef SCREEN_NV3023
Arduino_GFX *gfx1 = new Arduino_NV3023(bus1, -1, 2 /* rotation */, false, 128, 128);
Arduino_GFX *gfx2 = new Arduino_NV3023(bus2, -1, 2 /* rotation */, false, 128, 128);
Arduino_GFX *gfx3 = new Arduino_NV3023(bus3, -1, 2 /* rotation */, false, 128, 128);
#endif
#ifdef SCREEN_GC9107
Arduino_GFX *gfx1 = new Arduino_GC9107(bus1, -1, 0 /* rotation */, true, 128, 128);
Arduino_GFX *gfx2 = new Arduino_GC9107(bus2, -1, 0 /* rotation */, true, 128, 128);
Arduino_GFX *gfx3 = new Arduino_GC9107(bus3, -1, 0 /* rotation */, true, 128, 128);
#endif

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

Arduino_GFX *gfx[3] = {gfx1, gfx2, gfx3};

uint8_t HumanState;
uint8_t WireErr;

void configModeCallback(WiFiManager *myWiFiManager)
{
    gfx2->setTextColor(RED);
    gfx2->println("进入配置模式");
    gfx2->print("请连接热点");
    gfx2->println(myWiFiManager->getConfigPortalSSID());
    gfx2->print("进入");
    gfx2->println(WiFi.softAPIP());
    gfx2->setTextColor(QINGSHUILAN);
}

void audio_poweroff()
{
    pinMode(PIN_AUDIO_SD, OUTPUT);
    digitalWrite(PIN_AUDIO_SD, LOW);
}

void audio_poweron()
{
    pinMode(PIN_AUDIO_SD, OUTPUT);
    digitalWrite(PIN_AUDIO_SD, HIGH);
}

unsigned long ota_progress_millis = 0;

void onOTAStart()
{
    // Log when OTA has started
    Serial.println("OTA update started!");
    gfx3->setCursor(38, 100);
    gfx3->setFont(&GillSansEN_Bold_12);
    gfx3->setTextColor(GREEN, BLACK);
    gfx3->print("START");
    gfx3->drawRoundRect(12, 105, 104, 10, 5, ORANGE);
    gfx3->fillRoundRect(14, 107, 100, 6, 3, BLACK);

    // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final)
{
    // Log every 1 second
    if (millis() - ota_progress_millis > 1000)
    {
        ota_progress_millis = millis();
        Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
        gfx3->setCursor(38, 100);
        gfx3->setFont(&GillSansEN_Bold_12);
        gfx3->setTextColor(ORANGE, BLACK);
        gfx3->printf(" %03d %%  ", current * 100 / final);
        gfx3->drawRoundRect(12, 105, 104, 10, 5, ORANGE);
        gfx3->fillRoundRect(14, 107, current * 100 / final, 6, 3, ORANGE);
    }
}

void onOTAEnd(bool success)
{
    // Log when OTA has finished
    if (success)
    {
        Serial.println("OTA update finished successfully!");
        gfx3->setCursor(38, 100);
        gfx3->setFont(&GillSansEN_Bold_12);
        gfx3->setTextColor(GREEN, BLACK);
        gfx3->print(" 100 %  ");
        gfx3->drawRoundRect(12, 105, 104, 10, 5, GREEN);
        gfx3->fillRoundRect(14, 107, 100, 6, 3, GREEN);
    }
    else
    {
        Serial.println("There was an error during OTA update!");
        gfx3->setCursor(38, 100);
        gfx3->setFont(&GillSansEN_Bold_12);
        gfx3->setTextColor(RED, BLACK);
        gfx3->print("ERROR");
        gfx3->drawRoundRect(12, 105, 104, 10, 5, RED);
        gfx3->fillRoundRect(14, 107, 100, 6, 3, RED);
    }
    // <Add your own code here>
}

void dispProcessing(uint8_t screen)
{
    int16_t x1;
    int16_t y1;
    uint16_t w;
    uint16_t h;
    String processing = "正在查询…";
    gfx[screen]->fillScreen(BLACK);
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

    HumanState = 1;

// #ifdef SUPPORT_PCF8574
//     Wire.begin(SDA_PIN, SCL_PIN);            // 初始化I2C总线，指定SDA和SCL引脚
//     Wire.setClock(400000);                   // 设置I2C时钟频率为400kHz
//     Wire.beginTransmission(PCF8574_ADDRESS); // 开始I2C传输
//     Wire.write(0xff);                        // 初始化所有引脚为高电平
//     Wire.endTransmission();                  // 结束I2C传输
// #endif
#ifdef SUPPORT_CH423S
    Wire.begin(SDA_PIN, SCL_PIN);
    ch423.begin();
    Wire.beginTransmission(0x44 >> 1);
    WireErr = Wire.endTransmission(); // 检查I2C是否正常（是否接拓展芯片），0为正常
    ch423.pinMode(ch423.eGPO, ch423.ePUSH_PULL);
    delay(100);
    ch423.digitalWrite(ch423.eGPO, 0xFFFF);
#endif

    pinMode(BUTTON3_PIN, INPUT);

    gfx1->begin(40000000);
    gfx2->begin(40000000);
    gfx3->begin(40000000);
    gfx1->fillScreen(BLACK);
    gfx2->fillScreen(BLACK);
    gfx3->fillScreen(BLACK);

    delay(30);

    pinMode(backlight, OUTPUT);
    digitalWrite(backlight, 1);

    
    if (WireErr == 0)
    {
        xTaskCreate(Task_UpdateHumanState, "Task_UpdateHumanState", 4096, NULL, 1, &Handle_humanstate);
        xTaskCreate(Task_Screen_Control_by_HumanSensor, "Task_AutoScreenOnOff", 2048, NULL, 1, &Handle_AutoScreenOnOff);
    }
    else
    {
        Serial.println("米家及人在传感器异常");
    }

    if (LittleFS.begin(true))
    {
        Serial.println("mounted file system");
        if (LittleFS.exists("/config_weather.json"))
        {
            // file exists, reading and loading
            Serial.println("reading config file");
            File configFile = LittleFS.open("/config_weather.json", "r");
            if (configFile)
            {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.
                std::unique_ptr<char[]> buf(new char[size]);

                configFile.readBytes(buf.get(), size);

                DynamicJsonDocument json(1024);
                auto deserializeError = deserializeJson(json, buf.get());
                serializeJson(json, Serial);
                if (!deserializeError)
                {
                    Serial.println("\nparsed json");
                    strcpy(stored_weather_city, json["weather_city"]);
                    strcpy(stored_weather_key, json["weather_key"]);
                    strcpy(stored_weather_apihost, json["api_host"]);
                }
                else
                {
                    Serial.println("failed to load json config");
                }
                configFile.close();
            }
        }
        AnalyzeEncoderConfig();
    }
    else
    {
        Serial.println("failed to mount FS");
    }

    myDrawPNG(0, 0, "/TRI.png", 0);
    myDrawPNG(0, 0, "/PLE.png", 1);
    myDrawPNG(0, 0, "/KEY.png", 2);

    delay(800);

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

    gfx1->setUTF8Print(true);
    gfx2->setUTF8Print(true);
    gfx3->setUTF8Print(true);

    gfx1->setFont(u8g2_font_wqy13_t_gb2312);
    gfx2->setFont(u8g2_font_wqy13_t_gb2312);
    gfx3->setFont(u8g2_font_wqy13_t_gb2312);

    // if (!SPIFFS.begin())

    gfx1->setCursor(0, 13);
    gfx1->printf("HW: %s\r\n", VER_HW);
    gfx1->printf("SW: V%s\r\n", VER_SW);

    gfx1->printf("剩余空间: %dKB\r\n", (LittleFS.totalBytes() - LittleFS.usedBytes()) / 1024);
    // gfx3->printf("Free rom: %dKB\r\n", (SPIFFS.totalBytes() - SPIFFS.usedBytes()) / 1024);

    // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;
    wm.setAPCallback(configModeCallback);
    // set config save notify callback
    wm.setSaveConfigCallback(saveConfigCallback);

    WiFiManagerParameter custom_text1("<H1><font color=\"blue\">旋转编码器</font></H1>");
    wm.addParameter(&custom_text1);
    WiFiManagerParameter custom_rotary("rotary", "限位值（2或4）", String(rotary).c_str(), 2);
    WiFiManagerParameter custom_reverse("reverse", "是否反向（1或0）", String(enc_reverse).c_str(), 2);
    wm.addParameter(&custom_rotary);
    wm.addParameter(&custom_reverse);

    WiFiManagerParameter custom_text("<H1><font color=\"blue\">和风天气</font></H1><p><font color=\"red\">建议开机后进入自定义设置页面进行配置，有详细说明</font></p>");
    wm.addParameter(&custom_text);
    WiFiManagerParameter custom_weather_city("weather_city", "城市拼音或代码", stored_weather_city, 40);
    WiFiManagerParameter custom_weather_key("weather_key", "私钥", stored_weather_key, 40);
    WiFiManagerParameter custom_api_host("api_host", "API HOST", stored_weather_apihost, 40);
    wm.addParameter(&custom_weather_city);
    wm.addParameter(&custom_weather_key);
    wm.addParameter(&custom_api_host);

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
    gfx2->setCursor(0, 13);
    gfx2->println("连接WIFI中");

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
    strcpy(stored_weather_apihost, custom_api_host.getValue());
    uint8_t value_custom_rotary = String(custom_rotary.getValue()).toInt();
    if (value_custom_rotary == 4 || value_custom_rotary == 2)
    {
        rotary = value_custom_rotary;
        Serial.printf("限位值：%d\n", rotary);
    }
    else
    {
        Serial.println("限位值错误");
    }
    enc_reverse = String(custom_reverse.getValue()).toInt();
    Serial.printf("反向：%d\n", enc_reverse);

    Serial.println("The values in the file are: ");
    Serial.println("\tweather_city : " + String(stored_weather_city));
    Serial.println("\tweather_key: " + String(stored_weather_key));
    Serial.println("\tapi_host: " + String(stored_weather_apihost));

    // save the custom parameters to FS
    if (shouldSaveConfig)
    {
        Serial.println("saving config");

        DynamicJsonDocument json(1024);

        json["weather_city"] = stored_weather_city;
        json["weather_key"] = stored_weather_key;
        json["api_host"] = stored_weather_apihost;

        File configFile = LittleFS.open("/config_weather.json", "w");
        if (!configFile)
        {
            Serial.println("failed to open config file for writing");
        }

        serializeJson(json, Serial);
        serializeJson(json, configFile);
        configFile.close();
        json.clear();
        json["rotary"] = rotary;
        json["encreverse"] = enc_reverse;

        File configFile1 = LittleFS.open("/config_encoder.json", "w");
        if (!configFile1)
        {
            Serial.println("failed to open config file for writing");
        }
        serializeJson(json, Serial);
        serializeJson(json, configFile1);
        configFile1.close();
        // end save
    }

    gfx2->println("WiFi连接成功");
    gfx2->println("IP地址: ");
    gfx2->println(WiFi.localIP()); // 显示连接WIFI后的IP地址
    gfx2->print("RSSI: ");      // 显示连接WIFI后的IP地址
    gfx2->println(WiFi.RSSI()); // 显示连接WIFI后的IP地址
    delay(1000);
    if (digitalRead(BUTTON3_PIN) == 0)
    {
        gfx3->setCursor(0, 26);
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

    NewVersion = getNewVersion();
    if (NewVersion != VER_SW && NewVersion != "NA")
    {
        gfx1->setTextColor(ORANGE);
        gfx1->printf("新固件 (V%s) 已发布\n", NewVersion);
        gfx1->setTextColor(QINGSHUILAN);
    }

    gfx3->setCursor(0, 13);

    gfx3->print("读取配置中 ");
    AnalyzeStocksConfig();
    sws.push_back(MijiaSwitch(K1, "有人存在", MIJIA_SHORT)); // 固定KEY1用于人在传感器显示有人无人
    AnalyzeMijiaConfig();
    AnalyzeWebConfig();
    AnalyzeClockConfig();
    AnalyzePhotoConfig();
    gfx3->println("已完成");

    gfx3->print("时间更新中");
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
