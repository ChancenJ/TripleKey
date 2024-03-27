#include "app_weather.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiUdp.h>
#include <stdlib.h>
 

#define ARDUINOJSON_DECODE_UNICODE 1
#include <ArduinoJson.h>


#include "board_def.h"
const String URL = "http://api.seniverse.com/v3/weather/";
// const String KEY = weather_key;//换成自己的密匙即可，需要去心知天气注册
// const String LOCATION = weather_city;
const String LANGUAGE = "en"; //zh-Hans
const String UNIT = "c";	  //f

/**********************
 *   STATIC FUNCTIONS
 **********************/
 

weather_t *app_weather_init()
{
	weather_t *weather;
	Serial.println(sizeof(weather_t));
	weather = (weather_t *)malloc(sizeof(weather_t));
	if (weather != NULL)
		return weather;
	else
		return NULL;
}

int app_weather_delete(weather_t *weather)
{
	if (weather != NULL)
	{
		free(weather);
	}
	return 0;
}

/*
HTTPClient泄漏内存？
https://github.com/espressif/arduino-esp32/issues/3679
因为我发现经过11次迭代后，esp8266空闲堆稳定了。

*/

int app_weather_get_now(weather_t *now)
{ //unicode编码
	if (now == NULL)
	{
		return -1;
	}
	String key(stored_weather_key);
	String city(stored_weather_city);


	WiFiClient client;
	HTTPClient http;
	int httpCode;
	http.begin(client, URL + "now.json?key=" + key + "&location=" +  city + "&language=" + LANGUAGE + "&unit=" + UNIT);
	Serial.println(URL + "now.json?key=" + key + "&location=" +  city + "&language=" + LANGUAGE + "&unit=" + UNIT);
	httpCode = http.GET();
	Serial.printf("httpcode:%d:\n", httpCode);
	if (httpCode > 0)
	{
		String json = http.getString();
		Serial.println(json);
		DynamicJsonDocument doc(2000);
		DeserializationError error = deserializeJson(doc, json);
		if (error)
		{

			Serial.println(error.c_str());

			delay(5000);
			// while (1);
		}
		else
		{
			Serial.println("ok");
			JsonObject results_0 = doc["results"][0];

			// JsonObject results_0_location = results_0["location"];
			// const char *results_0_location_id = results_0_location["id"];							// "WX4FBXXFKE4F"
			// const char *results_0_location_name = results_0_location["name"];						// "北京"
			// const char *results_0_location_country = results_0_location["country"];					// "CN"
			// const char *results_0_location_path = results_0_location["path"];						// "北京,北京,中国"
			// const char *results_0_location_timezone = results_0_location["timezone"];				// "Asia/Shanghai"
			// const char *results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"

			JsonObject results_0_now = results_0["now"];
			const char *results_0_now_text = results_0_now["text"];				  // "晴"
			const char *results_0_now_code = results_0_now["code"];				  // "0"
			const char *results_0_now_temperature = results_0_now["temperature"]; // "16"

			const char *results_0_last_update = results_0["last_update"]; // "2020-10-24T10:40:00+08:00"

			strcpy(now->code_now, results_0_now_code);
			strcpy(now->text_now, results_0_now_text);
			strcpy(now->temperature_now, results_0_now_temperature);
			strcpy(now->last_update, results_0_last_update);

			// memcpy(now->code_now,results_0_now_code,UNIT_LEN);
			// memcpy(now->text_now,results_0_now_text,UNIT_LEN);
			// memcpy(now->temperature_now,results_0_now_temperature,UNIT_LEN);
			// memcpy(now->last_update,results_0_last_update,UNIT_LEN);
		}
	}

	http.end();

	client.stop();
	Serial.println("end weather get");

	return httpCode;
}

int app_weather_get_3(weather_t *day_1, weather_t *day_2, weather_t *day_3)
{ //unicode编码
	if (day_1 == NULL || day_2 == NULL || day_3 == NULL)
	{
		return -1;
	}
	String key(stored_weather_key);
	String city(stored_weather_city);

	WiFiClient client;
	HTTPClient http;
	int httpCode;
	http.begin(client, URL + "daily.json?key=" +  key + "&location=" +  city + "&language=" + LANGUAGE + "&unit=" + UNIT + "&start=0&days=3");

	httpCode = http.GET();
	Serial.printf("httpcode:%d:\n", httpCode);
	if (httpCode > 0)
	{
		String json = http.getString();
		Serial.println(json);
		DynamicJsonDocument doc(2000);
		DeserializationError error = deserializeJson(doc, json);
		if (error)
		{

			Serial.println(error.c_str());

			delay(5000);
			// while (1);
		}
		else
		{
			Serial.println("ok");
			JsonObject results_0 = doc["results"][0];

			// JsonObject results_0_location = results_0["location"];
			// const char *results_0_location_id = results_0_location["id"];							// "WX4FBXXFKE4F"
			// const char *results_0_location_name = results_0_location["name"];						// "北京"
			// const char *results_0_location_country = results_0_location["country"];					// "CN"
			// const char *results_0_location_path = results_0_location["path"];						// "北京,北京,中国"
			// const char *results_0_location_timezone = results_0_location["timezone"];				// "Asia/Shanghai"
			// const char *results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"

			JsonArray results_0_daily = results_0["daily"];

			JsonObject results_0_daily_0 = results_0_daily[0];
			const char *results_0_daily_0_date = results_0_daily_0["date"];									  // "2020-10-24"
			const char *results_0_daily_0_text_day = results_0_daily_0["text_day"];							  // "晴"
			const char *results_0_daily_0_code_day = results_0_daily_0["code_day"];							  // "0"
			const char *results_0_daily_0_text_night = results_0_daily_0["text_night"];						  // "多云"
			const char *results_0_daily_0_code_night = results_0_daily_0["code_night"];						  // "4"
			const char *results_0_daily_0_high = results_0_daily_0["high"];									  // "20"
			const char *results_0_daily_0_low = results_0_daily_0["low"];									  // "7"
			// const char *results_0_daily_0_rainfall = results_0_daily_0["rainfall"];							  // "0.0"
			// const char *results_0_daily_0_precip = results_0_daily_0["precip"];								  // ""
			// const char *results_0_daily_0_wind_direction = results_0_daily_0["wind_direction"];				  // "西南"
			// const char *results_0_daily_0_wind_direction_degree = results_0_daily_0["wind_direction_degree"]; // "225"
			// const char *results_0_daily_0_wind_speed = results_0_daily_0["wind_speed"];						  // "10"
			// const char *results_0_daily_0_wind_scale = results_0_daily_0["wind_scale"];						  // "2"
			const char *results_0_daily_0_humidity = results_0_daily_0["humidity"];							  // "37"

			JsonObject results_0_daily_1 = results_0_daily[1];
			const char *results_0_daily_1_date = results_0_daily_1["date"];									  // "2020-10-25"
			const char *results_0_daily_1_text_day = results_0_daily_1["text_day"];							  // "晴"
			const char *results_0_daily_1_code_day = results_0_daily_1["code_day"];							  // "0"
			const char *results_0_daily_1_text_night = results_0_daily_1["text_night"];						  // "多云"
			const char *results_0_daily_1_code_night = results_0_daily_1["code_night"];						  // "4"
			const char *results_0_daily_1_high = results_0_daily_1["high"];									  // "20"
			const char *results_0_daily_1_low = results_0_daily_1["low"];									  // "7"
			// const char *results_0_daily_1_rainfall = results_0_daily_1["rainfall"];							  // "0.0"
			// const char *results_0_daily_1_precip = results_0_daily_1["precip"];								  // ""
			// const char *results_0_daily_1_wind_direction = results_0_daily_1["wind_direction"];				  // "西南"
			// const char *results_0_daily_1_wind_direction_degree = results_0_daily_1["wind_direction_degree"]; // "225"
			// const char *results_0_daily_1_wind_speed = results_0_daily_1["wind_speed"];						  // "10"
			// const char *results_0_daily_1_wind_scale = results_0_daily_1["wind_scale"];						  // "2"
			const char *results_0_daily_1_humidity = results_0_daily_1["humidity"];							  // "48"

			JsonObject results_0_daily_2 = results_0_daily[2];
			const char *results_0_daily_2_date = results_0_daily_2["date"];									  // "2020-10-26"
			const char *results_0_daily_2_text_day = results_0_daily_2["text_day"];							  // "晴"
			const char *results_0_daily_2_code_day = results_0_daily_2["code_day"];							  // "0"
			const char *results_0_daily_2_text_night = results_0_daily_2["text_night"];						  // "晴"
			const char *results_0_daily_2_code_night = results_0_daily_2["code_night"];						  // "1"
			const char *results_0_daily_2_high = results_0_daily_2["high"];									  // "21"
			const char *results_0_daily_2_low = results_0_daily_2["low"];									  // "7"
			// const char *results_0_daily_2_rainfall = results_0_daily_2["rainfall"];							  // "0.0"
			// const char *results_0_daily_2_precip = results_0_daily_2["precip"];								  // ""
			// const char *results_0_daily_2_wind_direction = results_0_daily_2["wind_direction"];				  // "北"
			// const char *results_0_daily_2_wind_direction_degree = results_0_daily_2["wind_direction_degree"]; // "0"
			// const char *results_0_daily_2_wind_speed = results_0_daily_2["wind_speed"];						  // "20"
			// const char *results_0_daily_2_wind_scale = results_0_daily_2["wind_scale"];						  // "4"
			const char *results_0_daily_2_humidity = results_0_daily_2["humidity"];							  // "40"

			const char *results_0_last_update = results_0["last_update"]; // "2020-10-24T11:00:00+08:00"

			strcpy(day_1->code_day, results_0_daily_0_code_day);
			strcpy(day_1->code_night, results_0_daily_0_code_night);
			strcpy(day_1->text_day, results_0_daily_0_text_day);
			strcpy(day_1->text_night, results_0_daily_0_text_night);
			strcpy(day_1->temperature_low, results_0_daily_0_low);
			strcpy(day_1->temperature_high, results_0_daily_0_high);
			strcpy(day_1->humidity_now, results_0_daily_0_humidity);
			// strcpy(day_1->last_update, results_0_last_update);
			strcpy(day_1->date, results_0_daily_0_date);

			strcpy(day_2->code_day, results_0_daily_1_code_day);
			strcpy(day_2->code_night, results_0_daily_1_code_night);
			strcpy(day_2->text_day, results_0_daily_1_text_day);
			strcpy(day_2->text_night, results_0_daily_1_text_night);
			strcpy(day_2->temperature_low, results_0_daily_1_low);
			strcpy(day_2->temperature_high, results_0_daily_1_high);
			strcpy(day_2->humidity_now, results_0_daily_1_humidity);
			strcpy(day_2->last_update, results_0_last_update);
			strcpy(day_2->date, results_0_daily_1_date);

			strcpy(day_3->code_day, results_0_daily_2_code_day);
			strcpy(day_3->code_night, results_0_daily_2_code_night);
			strcpy(day_3->text_day, results_0_daily_2_text_day);
			strcpy(day_3->text_night, results_0_daily_2_text_night);
			strcpy(day_3->temperature_low, results_0_daily_2_low);
			strcpy(day_3->temperature_high, results_0_daily_2_high);
			strcpy(day_3->humidity_now, results_0_daily_2_humidity);
			strcpy(day_3->last_update, results_0_last_update);
			strcpy(day_3->date, results_0_daily_2_date);



			// memcpy(now->code_now,results_0_now_code,UNIT_LEN);
			// memcpy(now->text_now,results_0_now_text,UNIT_LEN);
			// memcpy(now->temperature_now,results_0_now_temperature,UNIT_LEN);
			// memcpy(now->last_update,results_0_last_update,UNIT_LEN);
		}
	}

	http.end();

	client.stop();
	Serial.println("end weather get");

	return httpCode;
}