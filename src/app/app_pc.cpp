#include "app_pc.h"

#include "WiFi.h"
#include "HTTPClient.h"
#include "board_def.h"
const char *host = "192.168.10.81"; //需要访问的域名
const int httpsPort = 80;           // 需要访问的端口
const String url = "/sse";          // 需要访问的地址

pc_t pc;

void getPrice(String s)
{
    int datStart = 0, datEnd = 0;
    String datstr;

    char cpu_temp[] = "CPU_TEMP";
    datStart = s.indexOf(cpu_temp) + strlen(cpu_temp);
    datEnd = s.indexOf("℃", datStart);
    datstr = s.substring(datStart, datEnd);
    if (datstr.toInt() == 0)
        return;
    pc.CPU_TEMP = datstr.toInt();

    char cpu_fan[] = "CPU_FAN";
    datStart = s.indexOf(cpu_fan) + strlen(cpu_fan);
    datEnd = s.indexOf("RPM", datStart);
    datstr = s.substring(datStart, datEnd);
    if (datstr.toInt() == 0)
        return;
    pc.CPU_FAN = datstr.toInt();

    char cpu_use[] = "CPU_USE";
    datStart = s.indexOf(cpu_use) + strlen(cpu_use);
    datEnd = s.indexOf("%", datStart);
    datstr = s.substring(datStart, datEnd);
    if (datstr.toInt() == 0)
        return;
    pc.CPU_USE = datstr.toInt();

    char ram_use[] = "RAM_USE";
    datStart = s.indexOf(ram_use) + strlen(ram_use);
    datEnd = s.indexOf("%", datStart);
    datstr = s.substring(datStart, datEnd);
    if (datstr.toInt() == 0)
        return;
    pc.RAM_USE = datstr.toInt();

    // Serial.println(s);
    // gfx1->fillScreen(BLACK);
    // gfx1->setCursor(0,0);
    // gfx1->print("CPU_TEMP ");
    // gfx1->println(pc.CPU_TEMP);

    // gfx1->print("CPU_FAN ");
    // gfx1->println(pc.CPU_FAN);

    // gfx1->print("RAM_USE ");
    // gfx1->println(pc.RAM_USE);
    // gfx1->print("CPU_USE ");
    // gfx1->println(pc.CPU_USE);

    // gfx1->println();
}
WiFiClient client;
void app_pc_sendrequest()
{

    /**
     * 测试是否正常连接
     */
    if (client.connected() == false)
    {
        if (!client.connect(host, httpsPort))
        {
            Serial.println("connection failed");
            return;
        }
    }
    delay(10);
    String postRequest = (String)("GET ") + url + " HTTP/1.1\r\n" +
                         //    "Content-Type: text/html;charset=utf-8\r\n" +
                         "Host: " + host + "\r\n" +
                         //"User-Agent: BuildFailureDetectorESP8266\r\n" +
                         "Connection: close\r\n\r\n";
    client.print(postRequest); // 发送HTTP请求

    /**
     * 展示返回的所有信息
     */

    char *buffer = (char *)malloc(500);
    int numdata = 0;
    delay(10);
    numdata = client.readBytes(buffer, 500);
    String temp = String(buffer);
    // client.stop();
    free(buffer);
    //   Serial.println(temp);
    getPrice(temp);
}