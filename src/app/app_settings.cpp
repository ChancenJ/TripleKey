#include "app_settings.h"
#include <esp_littlefs.h>

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
		Serial.println("成功读取HTML");
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
	String webConfig = readConfig(web_path);

	page.replace("{{SWVERSION}}", VER_SW);
	page.replace("{{CITY}}", stored_weather_city);
	page.replace("{{KEY}}", stored_weather_key);
	page.replace("{{stocksConfig}}", stocksConfig);
	page.replace("{{mijiaConfig}}", mijiaConfig);
	page.replace("{{FILELIST}}", listFiles(true));
	page.replace("{{webConfig}}", webConfig);

	request->send(200, "text/html", page);
}

void handleConfigPost(AsyncWebServerRequest *request)
{
	// 从表单中获取输入值
	String stocksConfig = request->arg("stocksConfig");
	String mijiaConfig = request->arg("mijiaConfig");
	String webConfig = request->arg("webConfig");
	Serial.println(stocksConfig);
	Serial.println(mijiaConfig);
	Serial.println(webConfig);
	// 保存配置
	saveConfig(stocks_path, stocksConfig);
	saveConfig(mijia_path, mijiaConfig);
	saveConfig(web_path, webConfig);
	AnalyzeStocksConfig();
	AnalyzeMijiaConfig();
	AnalyzeWebConfig();

	String page = readHTML("/webserver/config.html");

	// 返回成功消息
	request->send(200, "text/html", page);
}

void handleWeather(AsyncWebServerRequest *request)
{
	DynamicJsonDocument json(1024);
	json["weather_city"] = request->arg("city");
	json["weather_key"] = request->arg("key");
	strcpy(stored_weather_city, request->arg("city").c_str());
	strcpy(stored_weather_key, request->arg("key").c_str());
	File configFile = LittleFS.open("/config.json", "w");
	if (!configFile)
	{
		Serial.println("failed to open config file for writing");
	}
	serializeJson(json, Serial);
	serializeJson(json, configFile);
	configFile.close();
	request->redirect("/");
}

void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final)
{
	String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
	Serial.println(logmessage);

	if (!index)
	{
		logmessage = "Upload Start: " + String(filename);
		// open the file on first call and store the file handle in the request object
		request->_tempFile = LittleFS.open("/web/" + filename, "w");
		Serial.println(logmessage);
	}

	if (len)
	{
		// stream the incoming chunk to the opened file
		request->_tempFile.write(data, len);
		logmessage = "Writing file: " + String(filename) + " index=" + String(index) + " len=" + String(len);
		Serial.println(logmessage);
	}

	if (final)
	{
		logmessage = "Upload Complete: " + String(filename) + ",size: " + String(index + len);
		// close the file handle as the upload is now done
		request->_tempFile.close();
		Serial.println(logmessage);
		request->redirect("/uploadpng");
	}
}

void handleUploadPNG(AsyncWebServerRequest *request)
{
	String page = readHTML("/webserver/uploadpng.html");
	size_t total, used;
	esp_littlefs_info("spiffs", &total, &used);
	page.replace("{{FREEROM}}", humanReadableSize(total - used));
	page.replace("{{USEDROM}}", humanReadableSize(used));
	page.replace("{{TOTALROM}}", humanReadableSize(total));
	page.replace("{{FILELIST}}", listFiles(true));
	// Serial.println(page);
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

void AnalyzeWebConfig()
{
	String webConfig = readConfig(web_path);
	webstring.clear();
	std::vector<String> lines = SplitString(webConfig, '\n');
	for (int i = 0; i < lines.size(); i++)
	{
		std::vector<String> line = SplitString(lines[i], ',');
		//Serial.println(line[0]);
		//Serial.println(line[1]);
		webstring.push_back(line);
	}
	Serial.printf("网页数量:%d\n", webstring.size());
}

String humanReadableSize(const size_t bytes)
{
	if (bytes < 1024)
		return String(bytes) + " B";
	else if (bytes < (1024 * 1024))
		return String(bytes / 1024.0) + " KB";
	else if (bytes < (1024 * 1024 * 1024))
		return String(bytes / 1024.0 / 1024.0) + " MB";
	else
		return String(bytes / 1024.0 / 1024.0 / 1024.0) + " GB";
}

String listFiles(bool ishtml)
{
	String returnText = "";
	Serial.println("Listing files stored on LittleFS");
	File root = LittleFS.open("/web");
	File foundfile = root.openNextFile();
	if (ishtml)
	{
		returnText += "<table><tr><th align='left'>文件名</th><th align='left'>大小</th></tr>";
	}
	while (foundfile)
	{
		if (ishtml)
		{
			returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td></tr>";
		}
		else
		{
			returnText += "File: " + String(foundfile.name()) + "\n";
		}
		foundfile = root.openNextFile();
	}
	if (ishtml)
	{
		returnText += "</table>";
	}
	root.close();
	foundfile.close();
	Serial.println(returnText);
	return returnText;
}