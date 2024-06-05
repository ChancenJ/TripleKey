#include "pcstate.h"
#include "board_def.h"
#include "app/app_key.h"
#include "app/app_pc.h"

#define PC_NUM_COLOR GREEN
#define PC_LABLE_COLOR GREENYELLOW
#define PC_BAR_LOW_COLOR GREEN
#define PC_BAR_MIDDLE_COLOR ORANGE
#define PC_BAR_HIGH_COLOR RED
#define PC_BAR_BACK_COLOR 0x5bf0


TaskHandle_t pcTask_Handler = NULL;
QueueHandle_t pcQueue = NULL;
int old = 45;
int old_cpu_use, old_cpu_temp, old_ram_use;

static void dispPCState()
{

	char buffer[50];

	// Draw some text with botttom right datum
	gfx1->setFont(&MiSans_Demibold_12);
	gfx1->setCursor(0, 24);
	gfx1->setTextColor(PC_LABLE_COLOR);
	gfx1->print("CPU");
	uint8_t percent = pc.CPU_USE / 10;

	for (uint8_t i = 0; i <= percent; i++)
	{

		if (i < 5)
		{
			gfx1->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_LOW_COLOR);
		}
		else if (i < 9)
		{
			gfx1->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, ORANGE);
		}
		else
		{
			gfx1->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_HIGH_COLOR);
		}
	}
	for (uint8_t i = percent + 1; i < 11; i++)
	{
		// gfx1->fillRoundRect(0 + i * (6 + 2), 40, 6, 20, 2, 0X5BF0>>1);
		gfx1->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_BACK_COLOR);
	}

	gfx1->setFont(&MiSans_Demibold_12);
	if (old_cpu_use != pc.CPU_USE)
	{

		gfx1->setTextColor(BLACK);
		sprintf(buffer, "%03d", old_cpu_use);

		gfx1->setCursor(35, 24);
		gfx1->print(buffer);

		gfx1->setCursor(35, 24);
		if (pc.CPU_USE == 100)
		{
			gfx1->setTextColor(PC_NUM_COLOR);
			gfx1->print(pc.CPU_USE);
		}
		else if (pc.CPU_USE >= 10)
		{
			gfx1->setTextColor(PC_BAR_BACK_COLOR);
			gfx1->print('0');
			gfx1->setTextColor(PC_NUM_COLOR);
			gfx1->print(pc.CPU_USE);
		}
		else if (pc.CPU_USE > 0)
		{
			gfx1->setTextColor(PC_BAR_BACK_COLOR);
			gfx1->print("00");
			gfx1->setTextColor(PC_NUM_COLOR);
			gfx1->print(pc.CPU_USE);
		}
		else
		{
			gfx1->setTextColor(PC_BAR_BACK_COLOR);
			gfx1->print("000");
		}

		// gfx1->drawString(buffer, 240 - 60, 24);

		old_cpu_use = pc.CPU_USE;
	}

	// temp
	// Draw some text with botttom right datum
	gfx2->setFont(&MiSans_Demibold_12);
	gfx2->setCursor(0, 24);
	gfx2->setTextColor(PC_LABLE_COLOR);
	gfx2->print("TMP");
	percent = pc.CPU_TEMP / 10;

	for (uint8_t i = 0; i <= percent; i++)
	{

		if (i < 4)
		{
			gfx2->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_LOW_COLOR);
		}
		else if (i < 8)
		{
			gfx2->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, ORANGE);
		}
		else
		{
			gfx2->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_HIGH_COLOR);
		}
	}
	for (uint8_t i = percent + 1; i < 11; i++)
	{
		// gfx2->fillRoundRect(0 + i * (6 + 2), 40, 6, 20, 2, 0X5BF0>>1);
		gfx2->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_BACK_COLOR);
	}

	gfx2->setFont(&MiSans_Demibold_12);
	if (old_cpu_temp != pc.CPU_TEMP)
	{

		gfx2->setTextColor(BLACK);
		sprintf(buffer, "%03d", old_cpu_temp);

		gfx2->setCursor(35, 24);
		gfx2->print(buffer);

		gfx2->setCursor(35, 24);

		if (pc.CPU_TEMP == 100)
		{
			gfx2->setTextColor(PC_NUM_COLOR);
			gfx2->print(pc.CPU_TEMP);
		}
		else if (pc.CPU_TEMP >= 10)
		{
			gfx2->setTextColor(PC_BAR_BACK_COLOR);
			gfx2->print('0');
			gfx2->setTextColor(PC_NUM_COLOR);
			gfx2->print(pc.CPU_TEMP);
		}
		else if (pc.CPU_TEMP > 0)
		{
			gfx2->setTextColor(PC_BAR_BACK_COLOR);
			gfx2->print("00");
			gfx2->setTextColor(PC_NUM_COLOR);
			gfx2->print(pc.CPU_TEMP);
		}
		else
		{
			gfx2->setTextColor(PC_BAR_BACK_COLOR);
			gfx2->print("000");
		}

		// gfx1->drawString(buffer, 240 - 60, 24);

		old_cpu_temp = pc.CPU_TEMP;
	}

	// ram
	// Draw some text with botttom right datum
	gfx3->setFont(&MiSans_Demibold_12);
	gfx3->setCursor(0, 24);
	gfx3->setTextColor(PC_LABLE_COLOR);
	gfx3->print("RAM");
	percent = pc.RAM_USE / 10;

	for (uint8_t i = 0; i <= percent; i++)
	{

		if (i < 5)
		{
			gfx3->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_LOW_COLOR);
		}
		else if (i < 9)
		{
			gfx3->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, ORANGE);
		}
		else
		{
			gfx3->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_HIGH_COLOR);
		}
	}
	for (uint8_t i = percent + 1; i < 11; i++)
	{
		// gfx3->fillRoundRect(0 + i * (6 + 2), 40, 6, 20, 2, 0X5BF0>>1);
		gfx3->fillRoundRect(0 + i * (6 + 3), 40, 6, 20, 2, PC_BAR_BACK_COLOR);
	}

	gfx3->setFont(&MiSans_Demibold_12);
	if (old_ram_use != pc.RAM_USE)
	{

		gfx3->setTextColor(BLACK);
		sprintf(buffer, "%03d", old_ram_use);

		gfx3->setCursor(35, 24);
		gfx3->print(buffer);
		gfx3->setCursor(35, 24);

		if (pc.RAM_USE == 100)
		{
			gfx3->setTextColor(PC_NUM_COLOR);
			gfx3->print(pc.RAM_USE);
		}
		else if (pc.RAM_USE >= 10)
		{
			gfx3->setTextColor(PC_BAR_BACK_COLOR);
			gfx3->print('0');
			gfx3->setTextColor(PC_NUM_COLOR);
			gfx3->print(pc.RAM_USE);
		}
		else if (pc.RAM_USE > 0)
		{
			gfx3->setTextColor(PC_BAR_BACK_COLOR);
			gfx3->print("00");
			gfx3->setTextColor(PC_NUM_COLOR);
			gfx3->print(pc.RAM_USE);
		}
		else
		{
			gfx3->setTextColor(PC_BAR_BACK_COLOR);
			gfx3->print("000");
		}

		old_ram_use = pc.RAM_USE;
	}
}

void TaskPC(void *pvParameters)
{
	(void)pvParameters;
	for (;;)
	{
		app_pc_sendrequest();
		xQueueSend(pcQueue, &pc, 0);

		vTaskDelay(1000 / portTICK_PERIOD_MS);
	}

	vTaskDelete(NULL);
}

static void init(void *data)
{
	old_cpu_use = 0xff;
	old_cpu_temp = 0xff;
	old_ram_use = 0xff;

	xTaskCreate(TaskPC, "TaskPC" // A name just for humans
				,
				10240 // This stack size can be checked & adjusted by reading the Stack Highwater
				,
				NULL, 2 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
				,
				&pcTask_Handler);

	//创建消息队列
	if (pcQueue == NULL)
		pcQueue = xQueueCreate(2, sizeof(pc_t));
}

static void enter(void *data)
{
	// insert code
	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	dispPCState();

	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	KEY_TYPE key;
 
	pc_t pcstate;

	if (xQueueReceive(pcQueue, &pcstate, 1) == pdPASS)
	{
		 dispPCState();
	}

	key = app_key_get();
	switch (key)
	{

	case KEY1_DOWN:

		break;

	case KEY4_LONG:				  //长按
		manager_switchToParent(); //进入父项目 //退出
		break;
	default:
		break;
	}
}

static void exit(void *data)
{
	// insert code
	// client.stop();
	vTaskDelete(pcTask_Handler);
	xQueueReset(pcQueue);
	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_pcstate = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "PCState",
	.title_cn = "电脑状态",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};