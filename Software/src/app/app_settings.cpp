#include "app_settings.h"
#include <esp_littlefs.h>

const char *stocks_path = "/config_stocks.txt";
const char *mijia_path = "/config_mijia.txt";
const char *web_path = "/config_web.txt";
const char *clock_path = "/config_clock.json";
const char *enc_path = "/config_encoder.json";
const char *photo_path = "/config_photo.json";

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
	String webConfig = readConfig(web_path);
	String mijiaOption = CreateMijiaConfigHTML();
	page.replace("{{SWVERSION}}", VER_SW);
	page.replace("{{NEWVERSION}}", NewVersion);
	page.replace("{{CITY}}", stored_weather_city);
	page.replace("{{KEY}}", stored_weather_key);
	page.replace("{{API}}", stored_weather_apihost);
	page.replace("{{mijiaOption}}", mijiaOption);
	page.replace("{{stocksConfig}}", stocksConfig);
	page.replace("{{webConfig}}", webConfig);
	
	request->send(200, "text/html", page);
}

void handleConfigPost(AsyncWebServerRequest *request)
{
	// 从表单中获取输入值
	String stocksConfig = request->arg("stocksConfig");
	String webConfig = request->arg("webConfig");
	Serial.println(stocksConfig);
	Serial.println(webConfig);
	// 保存配置
	saveConfig(stocks_path, stocksConfig);
	saveConfig(web_path, webConfig);
	AnalyzeStocksConfig();
	AnalyzeWebConfig();

	String page = readHTML("/webserver/config.html");

	// 返回成功消息
	request->send(200, "text/html", page);
}

void handleMijiaPost(AsyncWebServerRequest *request){
	if(request->hasArg("plain")){
		String mijiaConfig = request->arg("plain");
		Serial.println(mijiaConfig);
		saveConfig(mijia_path, mijiaConfig);
		AnalyzeMijiaConfig();
		request->send(200);
	}
}

void handleWeather(AsyncWebServerRequest *request)
{
	DynamicJsonDocument json(1024);
	json["weather_city"] = request->arg("city");
	json["weather_key"] = request->arg("key");
	json["api_host"] = request->arg("api");
	strcpy(stored_weather_city, request->arg("city").c_str());
	strcpy(stored_weather_key, request->arg("key").c_str());
	strcpy(stored_weather_apihost, request->arg("api").c_str());
	File configFile = LittleFS.open("/config_weather.json", "w");
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
	String path = "/";
	String dir;
	if (request->hasParam("dir"))
	{
		dir = request->getParam("dir")->value();
		path = "/" + dir + "/";
		Serial.println(path);
	}
	String logmessage = "Client:" + request->client()->remoteIP().toString() + " " + request->url();
	Serial.println(logmessage);
	if (!index)
	{
		logmessage = "Upload Start: " + String(filename);
		// open the file on first call and store the file handle in the request object
		request->_tempFile = LittleFS.open(path + filename, "w");
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
		request->redirect("/upload" + dir);
	}
}

void handleUploadWeb(AsyncWebServerRequest *request)
{
	String page = readHTML("/webserver/uploadweb.html");
	page.replace("{{FILELIST}}", listFiles("/web", true));
	// Serial.println(page);
	request->send(200, "text/html", page);
}

void handleUploadPhoto(AsyncWebServerRequest *request)
{
	String page = readHTML("/webserver/uploadphoto.html");
	page.replace("{{FILELIST}}", listFiles("/photo", true));
	// Serial.println(page);
	request->send(200, "text/html", page);
}

void handleROM(AsyncWebServerRequest *request)
{
	size_t total, used;
	esp_littlefs_info("spiffs", &total, &used);
	String text = "剩余ROM: " + humanReadableSize(total - used) + " | 已用ROM: " + humanReadableSize(used) + " | ROM总量: " + humanReadableSize(total);
	request->send(200, "text/plain", text);
}

void handleListFile(AsyncWebServerRequest *request)
{
	String path = "/";
	if (request->hasParam("dir"))
	{
		path = "/" + request->getParam("dir")->value();
	}
	if (request->hasParam("deletebutton"))
	{
		uint8_t deletebutton = request->getParam("deletebutton")->value().toInt();
		if (deletebutton == 0)
		{
			request->send(200, "text/plain", listFiles(path, false));
		}
		else
		{
			request->send(200, "text/plain", listFiles(path, true));
		}
	}
	else
	{
		request->send(400, "text/plain", "ERROR: deletebutton param required");
	}
}

void handleDelete(AsyncWebServerRequest *request)
{
	if (request->hasParam("name") && request->hasParam("dir"))
	{
		String fileName = "/" + request->getParam("dir")->value() + "/" + request->getParam("name")->value();
		if (LittleFS.exists(fileName))
		{
			LittleFS.remove(fileName);
			request->send(200, "text/plain", "Deleted File: " + String(fileName));
		}
		else
		{
			Serial.println("ERROR: file does not exist");
			request->send(400, "text/plain", "ERROR: file does not exist");
		}
	}
	else
	{
		request->send(400, "text/plain", "ERROR: name param required");
	}
}

void handleMoreSettings(AsyncWebServerRequest *request)
{
	String page = readHTML("/webserver/moresettings.html");
	if (clockaudio == true)
	{
		page.replace("{{YESCHECKED_AUDIO}}", "checked");
		page.replace("{{NOCHECKED_AUDIO}}", "");
	}
	else
	{
		page.replace("{{YESCHECKED_AUDIO}}", "");
		page.replace("{{NOCHECKED_AUDIO}}", "checked");
	}
	if (autotheme == true)
	{
		page.replace("{{YESCHECKED_THEME}}", "checked");
		page.replace("{{NOCHECKED_THEME}}", "");
	}
	else
	{
		page.replace("{{YESCHECKED_THEME}}", "");
		page.replace("{{NOCHECKED_THEME}}", "checked");
	}
	if (rotary == 4)
	{
		page.replace("{{FOUR}}", "checked");
		page.replace("{{TWO}}", "");
	}
	else if (rotary == 2)
	{
		page.replace("{{FOUR}}", "");
		page.replace("{{TWO}}", "checked");
	}
	if (enc_reverse == true)
	{
		page.replace("{{YESCHECKED_REVERSE}}", "checked");
		page.replace("{{NOCHECKED_REVERSE}}", "");
	}
	else
	{
		page.replace("{{YESCHECKED_REVERSE}}", "");
		page.replace("{{NOCHECKED_REVERSE}}", "checked");
	}
	if (photoscroll == true)
	{
		page.replace("{{YESCHECKED_SCROLL}}", "checked");
		page.replace("{{NOCHECKED_SCROLL}}", "");
	}
	else
	{
		page.replace("{{YESCHECKED_SCROLL}}", "");
		page.replace("{{NOCHECKED_SCROLL}}", "checked");
	}
	request->send(200, "text/html", page);
}

void handleClock(AsyncWebServerRequest *request)
{
	String value_clockaudio = request->arg("clockaudio");
	String value_clocktheme = request->arg("clocktheme");
	DynamicJsonDocument json(1024);
	if (value_clockaudio == "yes")
	{
		clockaudio = true;
		json["clockaudio"] = true;
	}
	else if (value_clockaudio == "no")
	{
		clockaudio = false;
		json["clockaudio"] = false;
	}
	if (value_clocktheme == "yes")
	{
		autotheme = true;
		json["clocktheme"] = true;
	}
	else if (value_clocktheme == "no")
	{
		autotheme = false;
		json["clocktheme"] = false;
	}
	File configFile = LittleFS.open(clock_path, "w");
	if (!configFile)
	{
		Serial.println("failed to open config file for writing");
	}
	serializeJson(json, Serial);
	serializeJson(json, configFile);
	configFile.close();
	request->redirect("/moresettings");
}

void handleEncoder(AsyncWebServerRequest *request)
{
	uint8_t value_rotary = request->arg("rotary").toInt();
	String value_reverse = request->arg("reverse");
	DynamicJsonDocument json(1024);
	rotary = value_rotary;
	json["rotary"] = value_rotary;
	if (value_reverse == "yes")
	{
		enc_reverse = true;
		json["encreverse"] = true;
	}
	else if (value_reverse == "no")
	{
		enc_reverse = false;
		json["encreverse"] = false;
	}
	File configFile = LittleFS.open("/config_encoder.json", "w");
	if (!configFile)
	{
		Serial.println("failed to open config file for writing");
	}
	serializeJson(json, Serial);
	serializeJson(json, configFile);
	configFile.close();
	request->redirect("/moresettings");
}

void handlePhoto(AsyncWebServerRequest *request)
{
	uint8_t value_scroll = request->arg("photoscroll").toInt();
	DynamicJsonDocument json(512);
	if (value_scroll)
	{
		photoscroll = true;
		json["photoscroll"] = true;
	}
	else
	{
		photoscroll = false;
		json["photoscroll"] = false;
	}
	File configFile = LittleFS.open(photo_path, "w");
	if (!configFile)
	{
		Serial.println("failed to open config file for writing");
	}
	serializeJson(json, Serial);
	serializeJson(json, configFile);
	configFile.close();
	photonum = 0;
	request->redirect("/moresettings");
}

void handleFirmware(AsyncWebServerRequest *request)
{
	String page = readHTML("/webserver/firmware.html");
	page.replace("{{SWVERSION}}", VER_SW);
	page.replace("{{NEWVERSION}}", NewVersion);
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
		uint8_t optype = line[2].toInt();
		sws.push_back(MijiaSwitch(pin, name_cn, optype));
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
		// Serial.println(line[0]);
		// Serial.println(line[1]);
		webstring.push_back(line);
	}
	Serial.printf("网页数量:%d\n", webstring.size());
}

void AnalyzeClockConfig()
{
	String moreConfig = readConfig(clock_path);
	DynamicJsonDocument json(1024);
	deserializeJson(json, moreConfig);
	clockaudio = json["clockaudio"];
	autotheme = json["clocktheme"];
	Serial.printf("整点报时:%d\n", clockaudio);
	Serial.printf("时钟主题自动切换:%d\n", autotheme);
}

void AnalyzeEncoderConfig()
{
	String moreConfig = readConfig(enc_path);
	DynamicJsonDocument json(1024);
	deserializeJson(json, moreConfig);
	rotary = json["rotary"];
	enc_reverse = json["encreverse"];
	Serial.printf("旋转编码器限位:%d\n", rotary);
	Serial.printf("是否反向:%d\n", enc_reverse);
}

void AnalyzePhotoConfig()
{
	String moreConfig = readConfig(photo_path);
	DynamicJsonDocument json(512);
	deserializeJson(json, moreConfig);
	photoscroll = json["photoscroll"];
	Serial.printf("照片自动轮播:%d\n", photoscroll);
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

String listFiles(String path, bool DeleteButton)
{
	String returnText = "";
	Serial.println("Listing files stored on LittleFS");
	File root = LittleFS.open(path);
	File foundfile = root.openNextFile();
	returnText += "<table><tr><th align='left'>文件名</th><th align='left'>大小</th></tr>";
	while (foundfile)
	{
		returnText += "<tr align='left'><td>" + String(foundfile.name()) + "</td><td>" + humanReadableSize(foundfile.size()) + "</td>";
		if (DeleteButton)
		{
			returnText += "<td><button onclick=\"DeleteButton(\'" + String(foundfile.name()) + "\', \'delete\')\">删除</button></tr>";
		}
		else
		{
			returnText += "</tr>";
		}
		foundfile = root.openNextFile();
	}
	returnText += "</table>";
	root.close();
	foundfile.close();
	Serial.println(returnText);
	return returnText;
}

String getNewVersion() // 获取最新固件版本号
{
	String URL = "https://gitee.com/chancenj/triplekey/raw/main/Software/VERSION";
	HTTPClient httpClient;
	httpClient.begin(URL);
	Serial.println("正在获取最新固件版本号");
	Serial.println(URL);
	int httpCode = httpClient.GET();
	String version = "NA";
	if (httpCode == HTTP_CODE_OK)
	{
		version = httpClient.getString();
		Serial.println(version);
	}
	else
	{
		Serial.println(httpCode);
		Serial.println("最新固件版本号获取失败");
	}
	httpClient.end();
	return version;
}

String CreateMijiaConfigHTML()
{
	String result = "";
	for (int i = 1; i < sws.size(); i++)
	{
		uint8_t pin = sws[i].pin;
		uint8_t optype = sws[i].optype;
		String name = sws[i].name_cn;
		result += R"(
			<div class="form-container">
    			<select name="k-select">
		)";
		for (int j = 0; j < 8; j++)
		{
			String option = "<option value=\"K";
			char j_c = j + 1 + '0';
			option += j_c;
			option += "\" ";
			if (pin == j)
			{
				option += "selected";
			}
			option += ">";
			if (j < 4)
			{
				option += "普通";
			}
			else
			{
				option += "情景";
			}
			option += "开关";
			char jmod4_c = j % 4 + 1 + '0';
			option += jmod4_c;
			option += "</option>\n";
			result += option;
		}
		result += R"(</select>
    		<select name="op-select">)";
		if (optype == 1)
		{
			result += R"(
				<option value="1" selected>单击</option>
				<option value="2">双击</option>
				<option value="3">长按</option>
			)";
		}
		else if (optype == 2)
		{
			result += R"(
				<option value="1">单击</option>
				<option value="2" selected>双击</option>
				<option value="3">长按</option>
			)";
		}
		else if (optype == 3)
		{
			result += R"(
				<option value="1">单击</option>
				<option value="2">双击</option>
				<option value="3" selected>长按</option>
			)";
		}
		result += "</select>\n";
		result += "<input type=\"text\" name=\"text-input\" placeholder=\"请输入开关名称\" value=\"";
		result += name;
		result += "\">\n";
		result += R"(
				<button type="button" class="move-up-btn">上移</button>
				<button type="button" class="move-down-btn">下移</button>
				<button type="button" class="delete-btn">删除</button>
			</div>
		)";
	}
	return result;
}