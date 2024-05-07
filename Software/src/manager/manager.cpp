#include "manager.h"
#include "pages/pages.h"
#include <Arduino.h>
manager_t manager;

void manager_init()
{
    // manager.count =  ;
    // if (manager.count==0)
    //     return;
    manager.index = 1;
    manager.cur = pages[manager.index];
    manager.cur->init(nullptr);
    manager.cur->enter(nullptr);
}

void manager_loop()
{
    if (manager.busy == false)
    {
        manager.cur->loop(nullptr);
        delay(10);
    }
    else
    {
        delay(10);
    }
}

void manager_switch()
{
}
void manager_switchToChild()
{
    //
    manager.cur->exit(nullptr);

    manager.cur = pages[manager.index];

    manager.cur->init(nullptr);
    manager.cur->enter(nullptr);
}
void manager_switchToParent()
{
    manager.cur->exit(nullptr);

    //修改当前页面
    manager.cur = pages[0];

    manager.cur->init(nullptr);
    manager.cur->enter(nullptr);
}

void manager_switchToPage(page_t *topage){
    manager.cur->exit(nullptr);
    manager.cur = topage;
    manager.cur->init(nullptr);
    manager.cur->enter(nullptr);
}

void manager_switchToNext()
{
    //old exit
    // manager.cur->exit;
    // if(manager.index<(manager.count-1))
    // {
    //     manager.index ++;
    // }
    // else
    // {
    //     manager.index=0;
    // }

    //new enter
    manager.cur = pages[manager.index];

    manager.cur->init(nullptr);
    manager.cur->enter(nullptr);
}

void manager_setBusy(bool state)
{
    manager.busy = state;
}

bool manager_getBusy()
{
    return manager.busy;
}