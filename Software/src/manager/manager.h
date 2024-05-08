#ifndef __MANAGER_H
#define __MANAGER_H
#include <stdint.h>

#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))

typedef void (*init_cb_t)(void *data);
typedef void (*enter_cb_t)(void *data);
typedef void (*exit_cb_t)(void *data);
typedef void (*loop_cb_t)(void *data);

typedef struct page
{
    init_cb_t init; // 初始化相关变量
    enter_cb_t enter;
    exit_cb_t exit;
    loop_cb_t loop;

    char *title_en;

    char *title_cn;

    const uint8_t *icon;
    uint16_t icon_width;
    uint16_t icon_height;

    bool sleep_enable;             // 睡眠使能
    bool wakeup_btn_effect_enable; // 按钮唤醒时，按钮是否起作用
    bool acc_enable;               // 是否有加速度计参与

} page_t;

typedef struct manager
{
    /* data */
    uint8_t index; // 当前索引
    uint8_t count; // 页面数量
    page_t *cur;
    page_t *next;
    bool busy;
} manager_t;

void manager_init();
void manager_loop();
void manager_switch();
void manager_switchToNext();
void manager_switchToChild();
void manager_switchToParent();
void manager_switchToPage(page_t *topage);
void manager_setBusy(bool state);
bool manager_getBusy();

extern manager_t manager;

#endif
