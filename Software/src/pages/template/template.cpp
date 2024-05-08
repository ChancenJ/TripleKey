#include "template.h"//修改
#include "board_def.h"
#include "app/app_key.h"

static void init(void *data)
{
}

static void enter(void *data)
{
	// insert code

	//
	manager_setBusy(false);
}

static void loop(void *data)
{
	KEY_TYPE key;
	key = app_key_get();
	switch (key)
	{

	case KEY1_DOWN:

		break;

	case KEY4_LONG:				  // 长按
		manager_switchToParent(); // 进入父项目 //退出
		break;
	default:
		break;
	}
}

static void exit(void *data)
{
	// insert code

	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_template = {  //修改
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "template",//修改
	.title_cn = "模板",//修改
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};