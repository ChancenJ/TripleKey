#ifndef __APP_MIJIA_H
#define __APP_MIJIA_H
#include <Arduino.h>

#include "board_def.h"

#define K1 0
#define K2 1
#define K3 2
#define K4 3
#define K5 4
#define K6 5
#define K7 6
#define K8 7

#define MIJIA_SHORT 1
#define MIJIA_DOUBLE 2
#define MIJIA_LONG 3

#define SCENE_SWTICH 1
#define COMMON_SWTICH 2

struct MijiaSwitch
{
	uint8_t pin;
	String name_cn;
	uint8_t optype; // 1为单击，2为双击，3为长按
	uint8_t type;	// 1为场景开关，2为普通开关
	uint8_t on;		// 普通开关开关状态
	MijiaSwitch(uint8_t pin, String name_cn, uint8_t optype) : pin(pin), name_cn(name_cn), optype(optype), on(0)
	{
		if (pin >= 0 && pin <= 3)
		{
			type = COMMON_SWTICH;
		}
		else if (pin >= 4 && pin <= 7)
		{
			type = SCENE_SWTICH;
		}
	}
};

struct MijiaOp
{
	uint8_t _pin;
	uint8_t _optype;
	MijiaOp() : _pin(255), _optype(0) {}
	MijiaOp(uint8_t pin, uint8_t optype) : _pin(pin), _optype(optype) {}
};
extern QueueHandle_t Queue_Mijia;
extern TaskHandle_t Handle_mijia;
void Task_mijia(void *pvParameters);

void app_mijia_short(uint8_t pin);
void app_mijia_double(uint8_t pin);
void app_mijia_long(uint8_t pin);
void app_mijia_control(uint8_t pin, uint8_t type);
uint8_t app_mijia_get(uint8_t pin);

#endif