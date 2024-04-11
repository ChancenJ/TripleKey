#include "app_settings.h"



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

void handleRoot(AsyncWebServerRequest *request)
{
    // 读取配置文件中的默认值
    String stocksConfig = readConfig("/config_stocks.txt");

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
	<p>实时股票配置（每行格式：名称,ID）:</p> \
    <p>例：上证指数,sh000001（沪市加sh，深市加sz）</p> \
    <textarea name=\"stocksConfig\" rows=\"10\" cols=\"50\" >" +
                 stocksConfig+
                  "</textarea>\n \
	<input type=\"submit\" value=\"保存\">\
	</form>\
	</body>\
	</html>";
    request->send(200, "text/html", page);
}