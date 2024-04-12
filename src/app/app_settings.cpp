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
    // 读取配置文件中的默认值
    String stocksConfig = readConfig(stocks_path);
    String mijiaConfig = readConfig(mijia_path);

    // 创建HTML表单页面
    String page = "<html> \
	<head>\
	<meta charset=\"UTF-8\">\
	<title>TripleKey设置</title>\
	<style>\
    .custom-input {\
      width: 200px; /* 自定义宽度 */\
      height: 30px;  /* 自定义高度 */\
    }\
    </style>\
	</head>\
	<body>\
	<h1>TripleKey 设置</h1>\
	<form accept-charset=\"UTF-8\" method=\"post\" action=\"/config\"> \
    <div> \
	<p>实时股票配置（每行格式：名称,ID）:</p> \
    <p>例：上证指数,sh000001（沪市加sh，深市加sz）</p> \
    <textarea name=\"stocksConfig\" rows=\"10\" cols=\"50\" >" +
                  stocksConfig +
                  "</textarea>\n \
    </div> \
    <div> \
    <p>米家配置（每行格式：）:</p> \
    <p>例：上证指数,sh000001（沪市加sh，深市加sz）</p> \
    <textarea name=\"mijiaConfig\" rows=\"16\" cols=\"50\" >" +
                  mijiaConfig +
                  "</textarea>\n \
    </div> \
    <div> \
    <input type=\"submit\" value=\"保存\">\n \
    </div> \
	</form>\
	</body>\
	</html>";
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

    String page = "<html> \
	<head>\
	<meta charset=\"UTF-8\">\
	<title>TripleKey设置</title>\
	</head>\
    <body>\
    <p>已保存</p> \
    </body>\
	</html>";

	// 返回成功消息
	request->send(200, "text/html", page);
}

// 处理未找到的路由
void notFoundHandler(AsyncWebServerRequest *request)
{
	request->send(404, "text/html", "404: Page not found");
}



