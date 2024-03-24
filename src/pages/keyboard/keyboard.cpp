#include "keyboard.h"
#include "board_def.h"
#include "app/app_key.h"

static int8_t letter_upper_id = 0;
static int8_t letter_lower_id = 0;
static int8_t letter_number_id = 0;
static int8_t id;
const char letter_upper[27] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 0};
const char letter_lower[27] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 0};
const char letter_number[12] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 0, 0};
// const char *letter_func[12] = { "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10","F11","F12"};
static const uint16_t letter_color[9] = {BLUE, YELLOW, ORANGE, GREEN, PURPLE, BLUE << 2, CYAN, RED, MAGENTA};
static const char *p_letter[3] = {letter_upper, letter_lower, letter_number};
typedef enum
{
	CHARTYPE_UPPER = 0,
	CHARTYPE_LOWER,
	CHARTYPE_NUMBER,
	CHARTYPE_MAX
} char_type_t;

static char_type_t chartype;
static void disp_char(const char *letter, uint8_t id)
{
	int16_t x1;
	int16_t y1;
	uint16_t w;
	uint16_t h;

	char data = letter[id];

	gfx1->fillScreen(BLACK);
	gfx2->fillScreen(BLACK);
	gfx3->fillScreen(BLACK);
	for (uint8_t i = 0; i < 3; i++)
	{
		// Serial.print(letter_upper);
		// Serial.print("  ");

		gfx[i]->setTextColor(letter_color[random(9)]);
		gfx[i]->setFont(&Orbitron_Medium_48);
		gfx[i]->getTextBounds(&data, 0, 0, &x1, &y1, &w, &h);
		gfx[i]->setCursor((OLED_WIDTH - w) / 2, 48);
		gfx[i]->write(letter[id + i]);

		// Serial.println(letter_upper);
	}
}
// inc:
//		-1:减少  0：不变   1：增加
static void disp(char_type_t type, int8_t inc)
{
	switch (type)
	{
	case CHARTYPE_LOWER:
		if (inc > 0)
		{
			letter_lower_id += 3;
			if (letter_lower_id >= 27)
			{
				letter_lower_id = 0;
			}
		}
		else if (inc < 0)
		{
			letter_lower_id -= 3;
			if (letter_lower_id < 0)
			{
				letter_lower_id = 24;
			}
		}
		id = letter_lower_id;
		disp_char(letter_lower, letter_lower_id);
		break;

	case CHARTYPE_UPPER:
		if (inc > 0)
		{
			letter_upper_id += 3;
			if (letter_upper_id >= 27)
			{
				letter_upper_id = 0;
			}
		}
		else if (inc < 0)
		{
			letter_upper_id -= 3;
			if (letter_upper_id < 0)
			{
				letter_upper_id = 24;
			}
		}
		id = letter_upper_id;
		disp_char(letter_upper, letter_upper_id);
		break;

	case CHARTYPE_NUMBER:
		if (inc > 0)
		{
			letter_number_id += 3;
			if (letter_number_id >= 12)
			{
				letter_number_id = 0;
			}
		}
		else if (inc < 0)
		{
			letter_number_id -= 3;
			if (letter_number_id < 0)
			{
				letter_number_id = 9;
			}
		}
		id = letter_number_id;

		disp_char(letter_number, letter_number_id);
		break;
	default:
		break;
	}
}

static void init(void *data)
{

	chartype = CHARTYPE_UPPER;
	letter_upper_id = 0;
	letter_lower_id = 0;
	letter_number_id = 0;
}

static void enter(void *data)
{

	disp(chartype, 0);
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
		if (bleKeyboard.isConnected())
		{
			bleKeyboard.write(p_letter[chartype][id]);
		}
		break;

	case KEY2_DOWN:
		if (bleKeyboard.isConnected())
		{
			bleKeyboard.write(p_letter[chartype][id + 1]);
		}
		break;

	case KEY3_DOWN:
		if (bleKeyboard.isConnected())
		{
			bleKeyboard.write(p_letter[chartype][id + 2]);
		}
		break;

	case ENC_NEXT:
		disp(chartype, 1);
		break;
	case ENC_PREV:
		disp(chartype, -1);
		break;
	case KEY4_SHORT:
		chartype = (char_type_t)((int8_t)chartype + 1);
		if (chartype >= CHARTYPE_MAX)
		{
			chartype = (char_type_t)0;
		}
		letter_number_id = 0;
		letter_lower_id = 0;
		letter_upper_id = 0;

		disp(chartype, 0);
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

	//
	manager_setBusy(true);
}

#include "img.h"
page_t page_keyboard = {
	.init = init,
	.enter = enter,
	.exit = exit,
	.loop = loop,
	.title_en = "Keyboard",
	.title_cn = "键盘",
	.icon = img_bits,
	.icon_width = img_width,
	.icon_height = img_height,
	.sleep_enable = false,
	.wakeup_btn_effect_enable = false,
	.acc_enable = false,

};