#include "board_def.h"
#include "manager/manager.h"
#include "app/app_key.h"
#include "app/app_pc.h"
//#include "app/app_weather.h"

void setup()
{
	
	Serial.begin(115200);

	// while (!Serial);

	board_init();
	app_key_init();
	manager_init();
}

void loop()
{
	manager_loop();
	Mijia_UpdateHumanState();
	Screen_Control_by_HumanSensor();
}
