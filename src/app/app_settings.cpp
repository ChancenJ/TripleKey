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
	<h2>实时股票配置：</h2> \
    <p>每行格式：名称,ID</p> \
    <p>例：上证指数,sh000001（沪市加sh，深市加sz）</p> \
    <textarea name=\"stocksConfig\" rows=\"12\" cols=\"50\" >" +
                  stocksConfig +
                  "</textarea>\n \
    </div> \
    <div> \
    <h2>米家配置：</h2> \
    <p>每行格式：开关编号,中文名称,英文名称,操作类型,开关类型</p> \
    <p>操作类型：1单击2双击3长按；开关类型：1无线开关2普通开关（可显示开关状态）</p> \
    <p>※开关K1用于显示人体存在传感器状态，可用于米家联动，请勿在下方添加K1<br> \
    ※K2-4为普通开关，只能单击；K5-8为无线开关，可单击双击长按</p> \
    <p>例：K5,睡眠模式,K5Short,1,1<br> \
    例：K2,吸顶灯,K2Short,1,2</p> \
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
    AnalyzeStocksConfig();
    AnalyzeMijiaConfig();

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
            //Serial.println(split);
            newstrings.push_back(split);
            break;
        }
        split = origin.substring(startIndex, endIndex);
        while (split.endsWith("\n") || split.endsWith("\r"))
        {
            split = split.substring(0, split.length() - 1);
        }
        //Serial.println(split);
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
        //Serial.println(name);
        //Serial.println(id);
        stocks.push_back(StockInfo(name, id));
    }
    Serial.printf("股票数量:%d\n",stocks.size());
}

void AnalyzeMijiaConfig()
{
    String mijiaConfig = readConfig(mijia_path);
    sws.erase(sws.begin()+1,sws.end());
    std::vector<String> lines = SplitString(mijiaConfig, '\n');
    for (int i = 0; i < lines.size(); i++)
    {
        std::vector<String> line = SplitString(lines[i], ',');
        uint8_t pin = line[0][1]-'1';
        String name_cn = line[1];
        String name_en = line[2];
        uint8_t optype = line[3].toInt();
        uint8_t type = line[4].toInt();
        sws.push_back(MijiaSwitch(pin,name_cn,name_en,optype,type));
    }
    Serial.printf("米家开关数量:%d\n",sws.size());
}
