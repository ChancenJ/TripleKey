#include "app_settings.h"

const char *stocks_path = "/config_stocks.txt";
const char *mijia_path = "/config_mijia.txt";
const char *web_path = "/config_web.txt";

String readConfig(const char *config_path)
{
    String fileContent;
    File configFile = LittleFS.open(config_path, "r");
    if (configFile)
    {
        fileContent = configFile.readString();
        Serial.println(fileContent);
        configFile.close();
    }
    else
    {
        fileContent = "Fail";
        Serial.printf("%s读取失败\n", config_path);
    }
    return fileContent;
}

String readHTML(const char *html_path)
{
    String page;
    File configFile = LittleFS.open(html_path, "r");
    if (configFile)
    {
        page = configFile.readString();
        configFile.close();
    }
    else
    {
        page = "Failed to read HTML";
        Serial.printf("%s读取失败\n", html_path);
    }
    return page;
}

void saveConfig(const char *config_path, String config_content)
{
    File configFile = LittleFS.open(config_path, "w");
    if (configFile)
    {
        configFile.print(config_content);
        configFile.close();
        Serial.printf("%s保存成功\n", config_path);
    }
    else
    {
        Serial.printf("%s不存在，写入失败\n", config_path);
    }
}

void handleRoot(AsyncWebServerRequest *request)
{
    // 读取HTML
    String page = readHTML("/webserver/root.html");

    // 读取配置文件中的默认值
    String stocksConfig = readConfig(stocks_path);
    String mijiaConfig = readConfig(mijia_path);

    page.replace("{{stocksConfig}}", stocksConfig);
    page.replace("{{mijiaConfig}}", mijiaConfig);

    request->send(200, "text/html", page);
}

void handleConfigPost(AsyncWebServerRequest *request)
{
    // 从表单中获取输入值
    String stocksConfig = request->arg("stocksConfig");
    String mijiaConfig = request->arg("mijiaConfig");
    Serial.println(stocksConfig);
    Serial.println(mijiaConfig);
    // 保存配置
    saveConfig(stocks_path, stocksConfig);
    saveConfig(mijia_path, mijiaConfig);
    AnalyzeStocksConfig();
    AnalyzeMijiaConfig();

    String page = readHTML("/webserver/config.html");

    // 返回成功消息
    request->send(200, "text/html", page);
}

void handleWeb(AsyncWebServerRequest *request)
{
    String page = "<html> \
	<head>\
	<meta charset=\"UTF-8\">\
	<title>TripleKey设置web</title>\
	</head>\
	<body>\
	<h1>TripleKey 设置</h1>\
	<form accept-charset=\"UTF-8\" method=\"post\" action=\"/\"> \
    <div> \
	<input type=\"file\" name=\"file\" id=\"file\">\
    <input type=\"submit\" value=\"Upload Image\" name=\"submit\">\
    </div> \
	</form>\
	</body>\
	</html>";
    request->send(200, "text/html", page);
}

// 处理未找到的路由
void notFoundHandler(AsyncWebServerRequest *request)
{
    request->send(404, "text/html", "404: Page not found");
}

std::vector<String> SplitString(String origin, char flag)
{
    std::vector<String> newstrings;
    int16_t startIndex = 0;
    int16_t endIndex;
    // Serial.println("length");
    // Serial.println(origin.length());
    while (startIndex < origin.length())
    {
        endIndex = origin.indexOf(flag, startIndex);
        String split;
        if (endIndex == -1)
        {
            split = origin.substring(startIndex, origin.length());
            while (split.endsWith("\n") || split.endsWith("\r"))
            {
                split = split.substring(0, split.length() - 1);
            }
            // Serial.println(split);
            newstrings.push_back(split);
            break;
        }
        split = origin.substring(startIndex, endIndex);
        while (split.endsWith("\n") || split.endsWith("\r"))
        {
            split = split.substring(0, split.length() - 1);
        }
        // Serial.println(split);
        newstrings.push_back(split);
        startIndex = endIndex + 1;
    }
    return newstrings;
}

void AnalyzeStocksConfig()
{
    String stocksConfig = readConfig(stocks_path);
    stocks.clear();
    std::vector<String> lines = SplitString(stocksConfig, '\n');
    for (int i = 0; i < lines.size(); i++)
    {
        std::vector<String> line = SplitString(lines[i], ',');
        String name = line[0];
        String id = line[1];
        // Serial.println(name);
        // Serial.println(id);
        stocks.push_back(StockInfo(name, id));
    }
    Serial.printf("股票数量:%d\n", stocks.size());
}

void AnalyzeMijiaConfig()
{
    String mijiaConfig = readConfig(mijia_path);
    sws.erase(sws.begin() + 1, sws.end());
    std::vector<String> lines = SplitString(mijiaConfig, '\n');
    for (int i = 0; i < lines.size(); i++)
    {
        std::vector<String> line = SplitString(lines[i], ',');
        uint8_t pin = line[0][1] - '1';
        String name_cn = line[1];
        String name_en = line[2];
        uint8_t optype = line[3].toInt();
        uint8_t type = line[4].toInt();
        sws.push_back(MijiaSwitch(pin, name_cn, name_en, optype, type));
    }
    Serial.printf("米家开关数量:%d\n", sws.size());
}
