# DFRobot_CH423

* [English Version](./README.md)

这是一款I2C转IO扩展模块，采用CH423两线串行接口的通用远程 I/O 扩展芯片。它具有以下特点：
 1. 提供8个双向输入输出引脚：GPIO0 ~ GPIO7，可被配置为输入模式或输出模式；
    * 输入模式：在此模式下，可检测GPIO引脚的电平状态，如果GPIO引脚悬空，默认为高电平
    * 输出模式：在此模式下，可输出高电平或低电平
    * 注意： GPIO引脚同一时刻只能被配置为一种模式，不能被配置为不同的模式
 2. 提供16个通用输出引脚：GPO0 ~ GPO15，可被配置为推挽输出模式或开漏输出模式；
    * 推挽输出模式：在此模式下，可输出高电平或低电平
    * 开漏输出模式：在此模式下，可输出低电平或不输出
    * 注意：GPO引脚同一时刻只能被配置为一种模式，不能被配置为不同的输出模式
 3. GPIO外部中断：进入中断模式后，如果GPIO的电平和初始写入的电平不一样，GPO15引脚将会持续输出一个低电平信号，直到所有引脚的电平恢复初始电平，GPO15才恢复为输出高电平状态；
    * 低电平中断： GPIO引脚初始电平为高电平，如果GPIO引脚检测到低电平，GPO15引脚将输出一个低电平信号
    * 高电平中断： GPIO引脚初始电平为低电平，如果GPIO引脚检测到高电平，GPO15引脚将输出一个低电平信号
    * 上升沿中断： GPIO引脚初始电平为低电平，如果GPIO引脚检测到上升沿，GPO15引脚将输出一个由高到低的电平信号
    * 下降沿中断： GPIO引脚初始电平为高电平，如果GPIO引脚检测到下降沿，GPO15引脚将输出一个由高到低的电平信号
    * 双边沿中断： GPIO引脚初始电平为高电平，每发生一次中断，初始电平切换一次，如果GPIO引脚检测到下降沿或上升沿，GPO15引脚将输出一个由高到低的电平信号
    * 注意：GPO15引脚用来检测GPIO0 ~ GPIO7内的引脚是否发生电平改变，默认输出高电平信号，如果有一个引脚状态改变，GPO15将持续输出一个低电平，直到所有GPIO引脚的电平都恢复为初始电平值，GPO15才会恢复输出高电平状态。如果在GPO处于低电平信号期间，某个引脚发生中断，GPO引脚信号将不会产生变化。
 4. 睡眠模式：进入此模式后，可通过2种方式唤醒：
    * GPIO引脚电平发生变化
    * 和模块进行I2C数据通信


![产品效果图](./resources/images/DFR0979.png)


## 产品链接（[https://www.dfrobot.com.cn](https://www.dfrobot.com.cn)）
    SKU: DFR0979
    SKU: DFR0980   
   
## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性)
* [历史](#历史)
* [创作者](#创作者)

## 概述

提供一个Arduino库给CH423 I2C转IO扩展模块，该库具有以下功能：
1. 配置引脚模式：GPIO组引脚可配置为输入模式或输出模式，GPO组引脚可配置为推挽输出或开漏输出模式；
   * 两种类型引脚：GPIO引脚、GPO引脚
   * 以类型为单位配置引脚模式：同一类型的引脚在同一时刻只能配置为同一种模式，例：GPIO的所有引脚同一时刻只能配置为输入模式或输出模式，不能有些引脚被配置为输入模式，有些被配置为输出模式。GPO类型引脚类似
2. 设置引脚输出状态，支持以下几种设置方式：
   * 单独设置某一个引脚输出状态；
   * 按组为单位设置各引脚的输出状态，例：同时设置GPIO组（GPIO0 ~ GPIO7）、GPO0_7组（GPO0 ~ GPO7）、GPO8_15组（GPO8 ~ GPO15）、GPO组（GPO0 ~ GPO15）的引脚输出相同或不同的引脚状态
3. 读取GPIO引脚的电平状态，支持2种读取方式：
   * 单独读取某一个GPIO引脚的状态
   * 同时读取GPIO0 ~ GPIO7引脚的状态
4. GPIO外部中断，包含以下几种中断：
   * 低电平中断： 此中断发生，GPO15将输出一个低电平信号
   * 高电平中断： 此中断发生，GPO15将输出一个低电平信号
   * 上升沿中断： 此中断发生，GPO15将输出一个由高到低的电平信号
   * 下降沿中断： 此中断发生，GPO15将输出一个由高到低的电平信号
   * 双边沿中断： 此中断发生，GPO15将输出一个由高到低的电平信号
   * 注意： GPO15引脚处于低电平信号期间，其它GPIO引脚发生中断，GPO15引脚输出电平不会发生变化
5. 睡眠模式：此模块支持睡眠模式，在睡眠模式下功耗将降低，进入此模式后有2种方式唤醒：
   * GPIO引脚电平发生变化
   * 和模块发生了I2C通信

## 库安装

这里有2种安装方法：
1. 使用此库前，请首先下载库文件，将其粘贴到\Arduino\libraries目录中，然后打开examples文件夹并在该文件夹中运行演示。
2. 直接在Arduino软件库管理中搜索下载 DFRobot_CH423 库

## 方法

```C++
  /**
   * @fn DFRobot_CH423
   * @brief 构造函数
   * @param wire TwoWire类对象引用，默认为Wire
   */
  DFRobot_CH423(TwoWire &wire = Wire);

  /**
   * @fn begin
   * @brief 初始化模块，此模块具有2组引脚，一组是双向输入输出引脚GPIO0~GPIO7，此组引脚可同时被设置为输入或输出模式，
   * @n 另一组是通用输出引脚GPO0~GPO15，此组引脚可被设置为开漏输出或推挽输出模式。
   * @param gpio 设置GPIO组引脚的模式，默认为 eINPUT 输入模式，可填参数
   * @n     eINPUT       GPIO引脚输入模式，浮空时为高电平
   * @n     eOUTPUT      GPIO引脚输出模式， 可输出高低电平
   * @param gpo  设置gpo组引脚的模式，默认为 ePUSH_PULL 推挽输出模式， 可填参数
   * @n     eOPEN_DRAIN  GPO引脚开漏输出模式, 在此模式下，GPO引脚只能输出低电平或不输出
   * @n     ePUSH_PULL   GPO引脚推挽输出模式, 在此模式下，GPO引脚可输出高电平或低电平
   * @return Return 0 if initialization succeeds, otherwise return non-zero.
   */
  int begin(eMode_t gpio = eINPUT, eMode_t gpo = ePUSH_PULL);
  
  /**
   * @fn pinMode
   * @brief 设置引脚组集合的模式，此模块包含2组引脚，分别为GPIO（GPIO0~GPIO7）和GPO（GPO0~GPO15）。
   * @note 此模块，按组设置模式，同一组内的引脚在同一时刻，只能被设置成同一种模式，不同组之间可以设置成不同的模式
   * @param group   引脚组参数,ePinGroup_t枚举变量成员
   * @n     eGPIO                    双向输入输出引脚，GPIO0~GPIO7，此组引脚可被设置为输入(eINPUT)或输出(eOUTPUT)模式,设置为其他模式无效
   * @n     eGPO/eGPO0_7/eGPO8_15    3种参数都表示同一个含义，设置GPO组引脚的模式，选用此参数，通用输出引脚，只能被配置为开漏(eOPEN_DRAIN)或推挽(ePUSH_PULL)输出模式,其他模式无效
   * @param mode    组引脚模式参数，为eMode_t枚举变量的成员
   * @n     eINPUT       GPIO引脚输入模式，浮空时为高电平，此模式只能用于eGPIO组数字口
   * @n     eOUTPUT      GPIO引脚输出模式， 可输出高低电平，此模式只能用于eGPIO组数字口
   * @n     eOPEN_DRAIN  GPO引脚开漏输出模式, 在此模式下，GPO引脚只能输出低电平或不输出，此模式只能用于eGPO组数字口
   * @n     ePUSH_PULL   GPO引脚推挽输出模式， 在此模式下，GPO引脚可输出高电平或低电平, 此模式只能用于eGPO组数字口
   */
  void pinMode(ePinGroup_t group, eMode_t mode);
  void pinMode(int group, int mode);

  /**
   * @fn 
   * @brief 设置引脚输出高低电平。
   * @param gpioPin   GPIO组内引脚，eGPIOPin_t 枚举变量成员
   * @n     eGPIO0     双向输入输出引脚，GPIO0，表示设置引脚GPIO0的输出值
   * @n     eGPIO1     双向输入输出引脚，GPIO1，表示设置引脚GPIO1的输出值
   * @n     eGPIO2     双向输入输出引脚，GPIO2，表示设置引脚GPIO2的输出值
   * @n     eGPIO3     双向输入输出引脚，GPIO3，表示设置引脚GPIO3的输出值
   * @n     eGPIO4     双向输入输出引脚，GPIO4，表示设置引脚GPIO4的输出值
   * @n     eGPIO5     双向输入输出引脚，GPIO5，表示设置引脚GPIO5的输出值
   * @n     eGPIO6     双向输入输出引脚，GPIO6，表示设置引脚GPIO6的输出值
   * @n     eGPIO7     双向输入输出引脚，GPIO7，表示设置引脚GPIO7的输出值
   * @n     eGPIOTotal 设置GPIO组内所有引脚的值，使用此参数时，参数level的bit0~bit7均为有效值，分别对应引脚GPIO0~GPIO7的输出值
   * @param level    输出电平
   * @n     HIGH or 1    参数level，8位数据中bit0有效，表示输出高电平
   * @n     LOW  or 0    参数level，8位数据中bit0有效，表示输出低电平
   * @n     0x00~0xFF    如果参数gpioPin为GPIOTotal时，参数level的bit0~bit7均为有效数据，分别对应GPIO0~GPIO7引脚。
   */
  void  digitalWrite(eGPIOPin_t gpioPin, uint8_t level);

  /**
   * @fn digitalWrite
   * @brief 设置引脚输出高低电平或 控制低电平输出或停止（中断）。
   * @param gpoPin   eGPOPin_t 枚举变量成员
   * @n     eGPO0     通用输出引脚，GPO0，表示设置引脚GPO0的输出值
   * @n     eGPO1     通用输出引脚，GPO1，表示设置引脚GPO1的输出值
   * @n     eGPO2     通用输出引脚，GPO2，表示设置引脚GPO2的输出值
   * @n     eGPO3     通用输出引脚，GPO3，表示设置引脚GPO3的输出值
   * @n     eGPO4     通用输出引脚，GPO4，表示设置引脚GPO4的输出值
   * @n     eGPO5     通用输出引脚，GPO5，表示设置引脚GPO5的输出值
   * @n     eGPO6     通用输出引脚，GPO6，表示设置引脚GPO6的输出值
   * @n     eGPO7     通用输出引脚，GPO7，表示设置引脚GPO7的输出值
   * @n     eGPO8     通用输出引脚，GPO8，表示设置引脚GPO8的输出值
   * @n     eGPO9     通用输出引脚，GPO9，表示设置引脚GPO9的输出值
   * @n     eGPO10    通用输出引脚，GPO10，表示设置引脚GPO10的输出值
   * @n     eGPO11    通用输出引脚，GPO11，表示设置引脚GPO11的输出值
   * @n     eGPO12    通用输出引脚，GPO12，表示设置引脚GPO12的输出值
   * @n     eGPO13    通用输出引脚，GPO13，表示设置引脚GPO13的输出值
   * @n     eGPO14    通用输出引脚，GPO14，表示设置引脚GPO14的输出值
   * @n     eGPO15    通用输出引脚，GPO15，表示设置引脚GPO15的输出值
   * @n     eGPOTotal 设置GPO组0~15内的所有引脚，使用此参数时，参数8位数据level的bit0~bit7分别对应GPO0~GPO7或GPO8~GPO15引脚的输出值
   * @param level     输出电平或低电平信号输出或终止
   * @n     HIGH or 1    如果引脚组GPO被配置为推挽输出模式，则输出高电平，若配置为开漏模式，则代表输出低电平信号
   * @n     LOW  or 0    如果引脚组GPO被配置为推挽输出模式，则输出低电平，若配置为开漏模式，则代表不输出任何信号
   * @n     0x00~0xFF    如果gpoPin参数为eGPOTotal时，level的bit0~bit7都为有效数据，分别对应GPO0~GPO7或GPO8~GPO15引脚
   */
  void  digitalWrite(eGPOPin_t gpoPin, uint8_t level);
  /**
   * @fn digitalWrite
   * @brief 按组为单位，设置CH423各组IO引脚的输出值
   * @param group   组引脚，ePinGroup_t枚举变量成员
   * @n     eGPIO    GPIO组引脚0~7，设置此值时，参数level低8位有效， bit0~bit7分别对应GPIO0~GPIO7引脚的输出值，表示设置GPIO组0~7引脚的输出值。
   * @n     eGPO     GPO组引脚0~15，设置此值时，参数level 16位居有效， bit0~bit15分别对应GPO0~GPIO15引脚的输出值，表示设置GPO组0~15引脚的输出值。
   * @n     eGPO0_7  GPO组引脚0~7，设置此值时，参数level低8位有效， bit0~bit7分别对应GPO0~GPO7引脚的输出值，表示设置GPO组0~7引脚的输出值。
   * @n     eGPO8_15 GPO组引脚8~15，设置此值时，参数level高8位有效， bit8~bit15分别对应GPO8~GPO15引脚的输出值，表示设置GPO组8~15引脚的输出值。
   * @param level    16位数据或uGroupValue_t共用体值，配合group参数，表示某组引脚的值，bit0~bit15分别对应GPIO0~GPIO7和GPO0~GPO15，对应GPIO0~GPIO7时，
   * @n 低八位bit0~bit7有效，高八位无效
   * @n     0x0000~0xFFFF  16位数据，bit0~bit15按参数group的值分别代表不同的含义
   * @note uGroupValue_t共用体,用户可以操作这个来配置各引脚的值
   */
  void  digitalWrite(ePinGroup_t group, uGroupValue_t level);
  void  digitalWrite(ePinGroup_t group, uint16_t level);

  /**
   * @fn digitalRead
   * @brief 读取GPIO组引脚的电平状态值
   * @param pin GPIO组内引脚，eGPIOPin_t 枚举变量成员
   * @n     eGPIO0     双向输入输出引脚，GPIO0，表示读取引脚GPIO0的电平状态，0表示低电平，1表示高电平
   * @n     eGPIO1     双向输入输出引脚，GPIO1，表示读取引脚GPIO1的电平状态，0表示低电平，1表示高电平
   * @n     eGPIO2     双向输入输出引脚，GPIO2，表示读取引脚GPIO2的电平状态，0表示低电平，1表示高电平
   * @n     eGPIO3     双向输入输出引脚，GPIO3，表示读取引脚GPIO3的电平状态，0表示低电平，1表示高电平
   * @n     eGPIO4     双向输入输出引脚，GPIO4，表示读取引脚GPIO4的电平状态，0表示低电平，1表示高电平
   * @n     eGPIO5     双向输入输出引脚，GPIO5，表示读取引脚GPIO5的电平状态，0表示低电平，1表示高电平
   * @n     eGPIO6     双向输入输出引脚，GPIO6，表示读取引脚GPIO6的电平状态，0表示低电平，1表示高电平
   * @n     eGPIO7     双向输入输出引脚，GPIO7，表示读取引脚GPIO7的电平状态，0表示低电平，1表示高电平
   * @n     eGPIOTotal 双向输入输出引脚，GPIO组GPIO0~GPIO7，表示读取GPIO组所有引脚的电平状态，返回值bit0~bit8分别表示GPIO0~GPIO7各引脚的电平值
   * @return 电平状态值
   */
  uint8_t digitalRead(eGPIOPin_t pin);
  
  /**
   * @fn attachInterrupt
   * @brief 设置GPIO引脚的外部中断模式和中断服务函数
   * @note CH423模块的GPO15引脚用来表示在中断模式下GPIO0~GPIO7是否发生中断，如果任意引脚发生了中断，GPO15将持续输出一个低电平，否则将输出高电平
   * @n 如果一个引脚发生中断，持续触发，在此期间如果其他引脚发生了中断，GPO15引脚的电平信号依然为低，不会产生任何变化
   * @param gpioPin   GPIO组内引脚，eGPIOPin_t 枚举变量成员
   * @n     eGPIO0     双向输入输出引脚，GPIO0，表示设置引脚GPIO0的外部中断模式和中断服务函数
   * @n     eGPIO1     双向输入输出引脚，GPIO1，表示设置引脚GPIO1的外部中断模式和中断服务函数
   * @n     eGPIO2     双向输入输出引脚，GPIO2，表示设置引脚GPIO2的外部中断模式和中断服务函数
   * @n     eGPIO3     双向输入输出引脚，GPIO3，表示设置引脚GPIO3的外部中断模式和中断服务函数
   * @n     eGPIO4     双向输入输出引脚，GPIO4，表示设置引脚GPIO4的外部中断模式和中断服务函数
   * @n     eGPIO5     双向输入输出引脚，GPIO5，表示设置引脚GPIO5的外部中断模式和中断服务函数
   * @n     eGPIO6     双向输入输出引脚，GPIO6，表示设置引脚GPIO6的外部中断模式和中断服务函数
   * @n     eGPIO7     双向输入输出引脚，GPIO7，表示设置引脚GPIO7的外部中断模式和中断服务函数
   * @n     eGPIOTotal 设置GPIO组内所有引脚的值，表示将引脚GPIO0~GPIO7设置为同一中断模式和中断服务函数
   * @param mode    中断模式
   * @n     eLOW       低电平中断，GPIO引脚初始电平为高电平，如果GPIO引脚检测到低电平，GPO15引脚将输出一个低电平信号
   * @n     eHIGH      高电平中断，GPIO引脚初始电平为低电平，如果GPIO引脚检测到高电平，GPO15引脚将输出一个低电平信号
   * @n     eRISING    上升沿中断，GPIO引脚初始电平为低电平，如果GPIO引脚检测到上升沿，GPO15引脚将输出一个由高到低的电平信号（下降沿）
   * @n     eFALLING   下降沿中断，GPIO引脚初始电平为高电平，如果GPIO引脚检测到下降沿，GPO15引脚将输出一个由高到低的电平信号（下降沿）
   * @n     eCHANGE    双边沿中断，GPIO引脚初始电平为高电平，每发生一次中断，初始电平切换一次，如果GPIO引脚检测到下降沿或上升沿，GPO15引脚将输出一个由高到低的电平信号（下降沿）
   * @note GPIO0~GPIO7范围内任意引脚电平发生改变，GPO15会输出一个低电平信号，直到GPIO0~GPIO7范围内所有引脚恢复初始电平状态，GPO15才会恢复输出高电平
   * @param cb  指向中断服务函数
   */
  void attachInterrupt(eGPIOPin_t gpioPin, eInterruptMode_t mode, CH423_INT_CB cb);  
  
  /**
   * @fn enableInterrupt
   * @brief 使能GPIO的外部中断
   */
  void enableInterrupt();

  /**
   * @fn disableInterrupt
   * @brief 关闭GPIO的外部中断
   */
  void disableInterrupt();
  
  /**
   * @fn pollInterrupts
   * @brief 轮询GPIO中断事件
   */
  void pollInterrupts();

  /**
   * @fn sleep
   * @brief 进入睡眠模式
   * @note 进入此模式后，可通过2种方式唤醒
   * @n 1. GPIO引脚产生了外部中断
   * @n 2. 执行了引脚操作
   */
  void sleep();
  /**
   * @fn pinDescription
   * @brief 描述GPIO组内引脚
   * @param pin GPIO组内引脚，eGPIOPin_t 枚举变量成员
   * @n     eGPIO0     双向输入输出引脚，GPIO0，表示获取引脚eGPIO0的描述
   * @n     eGPIO1     双向输入输出引脚，GPIO1，表示获取引脚eGPIO1的描述
   * @n     eGPIO2     双向输入输出引脚，GPIO2，表示获取引脚eGPIO2的描述
   * @n     eGPIO3     双向输入输出引脚，GPIO3，表示获取引脚eGPIO3的描述
   * @n     eGPIO4     双向输入输出引脚，GPIO4，表示获取引脚eGPIO4的描述
   * @n     eGPIO5     双向输入输出引脚，GPIO5，表示获取引脚eGPIO5的描述
   * @n     eGPIO6     双向输入输出引脚，GPIO6，表示获取引脚eGPIO6的描述
   * @n     eGPIO7     双向输入输出引脚，GPIO7，表示获取引脚eGPIO7的描述
   * @return Return pin description string
   * @n such as "GPIO0" "GPIO1" "GPIO2" "GPIO3" "GPIO4" "GPIO5" "GPIO6" "GPIO7"
   */
  String pinDescription(eGPIOPin_t pin);

  /**
   * @fn pinDescription
   * @brief Convert pin into string description 
   * @param pin  eGPOPin_t 枚举变量成员
   * @n     eGPO0     通用输出引脚，GPO0，表示获取引脚eGPO0的描述
   * @n     eGPO1     通用输出引脚，GPO1，表示获取引脚eGPO1的描述
   * @n     eGPO2     通用输出引脚，GPO2，表示获取引脚eGPO2的描述
   * @n     eGPO3     通用输出引脚，GPO3，表示获取引脚eGPO3的描述
   * @n     eGPO4     通用输出引脚，GPO4，表示获取引脚eGPO4的描述
   * @n     eGPO5     通用输出引脚，GPO5，表示获取引脚eGPO5的描述
   * @n     eGPO6     通用输出引脚，GPO6，表示获取引脚eGPO6的描述
   * @n     eGPO7     通用输出引脚，GPO7，表示获取引脚eGPO7的描述
   * @n     eGPO8     通用输出引脚，GPO8，表示获取引脚eGPO8的描述
   * @n     eGPO9     通用输出引脚，GPO9，表示获取引脚eGPO9的描述
   * @n     eGPO10    通用输出引脚，GPO10，表示获取引脚eGPO10的描述
   * @n     eGPO11    通用输出引脚，GPO11，表示获取引脚eGPO11的描述
   * @n     eGPO12    通用输出引脚，GPO12，表示获取引脚eGPO12的描述
   * @n     eGPO13    通用输出引脚，GPO13，表示获取引脚eGPO13的描述
   * @n     eGPO14    通用输出引脚，GPO14，表示获取引脚eGPO14的描述
   * @n     eGPO15    通用输出引脚，GPO15，表示获取引脚eGPO15的描述
   * @return Return pin description string
   * @n such as "GPO0" "GPO1" "GPO2"  "GPO3"  "GPO4"  "GPO5"  "GPO6"  "GPO7"
   * @n         "GPO8" "GPO9" "GPO10" "GPO11" "GPO12" "GPO13" "GPO14" "GPO15"
   */
  String pinDescription(eGPOPin_t pin);
```

## 兼容性

MCU                |  Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |       √       |              |             | 
Mega2560           |      √       |              |             | 
Leonardo           |      √       |              |             | 不支持模块中断功能
ESP32              |      √       |              |             | 
ESP8266            |      √       |              |             | 
micro:bit          |      √       |              |             | 不支持模块低电平中断功能
FireBeetle M0      |      √       |              |             |  

## 历史

- 2022/03/14 - 1.0.0 版本

## 创作者

Written by Arya(xue.peng@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))




