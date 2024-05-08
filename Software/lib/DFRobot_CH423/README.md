# DFRobot_CH423

* [中文版](./README_CN.md)

This is a I2C to IO expansion board based on CH432, an universal remote I/O expansion chip with a two-wire serial interface. It features:
 1. 8 bi-directional input/output pins: GPIO0 ~ GPIO7, which can be configured as input or output mode;
    * Input mode: detect the level status of GPIO pin, default to be high if the GPIO pin is floating
    * Output mode: output high level or low level
    * Note: GPIO pins can only be set as one mode at the same time.
 2. 16 GPO pins: GPO0~GPO15, which can be configured as push-pull or open-drain output mode;
    * Push-pull output mode: output high or low level
    * Open-drain output mode: output low level or do not output
    * Note: GPO pins can only be set as one mode at the same time.
 3. GPIO external interrupt: after entering interrupt mode, pin GPO15 will continuously output low level signal if the GPIO level is different from the initial written level. And it will not output high level until the levels of all pins return to the initial level;
    * Low level interrupt: initial level of the GPIO pin is high, if GPIO pins detect a low level, pin GPO15 will output a low level signal
    * High level interrupt: initial level of the GPIO pin is low, if GPIO pins detect a high level, pin GPO15 will output a low level signal
    * Rising edge interrupt: initial level of the GPIO pin is low, if GPIO pins detect a rising edge, pin GPO15 will output a high-to-low level signal
    * Falling edge interrupt: initial level of the GPIO pin is high, if GPIO pins detect a falling edge, pin GPO15 will output a high-to-low level signal
    * Double edge interrupt: initial level of the GPIO pin is high, and the initial level switches once every time an interrupt occurs. If GPIO pins detect a falling edge or rising edge, pin GPO15 will output a high-to-low level signal.
    * Note: pin GPO15 is used to detect whether the pin level of GPIO0-GPIO7 change, default to output high level signal. If the status of one pin changes, GPO15 will continuously output a low level. And it will not output high level until the levels of all GPIO pins return to the initial level. If an interrupt occurs on a pin while the GPO is at a low level, the GPO pin signal will not change.
 4. Sleep mode: wake up in 2 ways:
    * GPIO pin level changes
    * I2C data communication with the module


![产品效果图](./resources/images/DFR0979.png)

## Product Link（[https://www.dfrobot.com](https://www.dfrobot.com)）
    SKU: DFR0979
    SKU: DFR0980


## Table of Contents

* [Summary](#summary)
* [Installation](#installation)
* [Methods](#methods)
* [Compatibility](#compatibility)
* [History](#history)
* [Credits](#credits)

## Summary
An Arduino library is provided for the CH423 I2C to I/O expansion module. It has the following functions:
1. Configure pin mode: GPIO pins can be configured as input or output mode, and GPO pins as push-pull or open-drain output mode;
   * Two types of pins: GPIO pins, GPO pins
   * Configure pin mode by type: pins of the same type can only be configured to the same mode at the same time, for example: all GPIO pins can only be configured to input or output mode at the same time. Ditto for GPO pins.
2. Set pin output status, supporting the following modes:
   * Set the output status of a pin;
   * Set the output status of every pin by group, for example: simultaneously set GPIO group(GPIO0 ~ GPIO7), GPO0_7 group(GPO0 ~ GPO7), GPO8_15 group(GPO8 ~ GPO15) and GPO group(GPO0 ~ GPO15) to output the same or different pin status
3. Read the level status of GPIO pins, supporting 2 reading ways:
   * Read the status of one GPIO pin
   * Read the status of the pins of GPIO0 ~ GPIO7 at the same time
4. GPIO external interrupt, including the following kinds of interrupts:
   * Low level interrupt: pin GPO15 will output a low level signal
   * High level interrupt: pin GPO15 will output a low level signal
   * Rising edge interrupt: pin GPO15 will output a high-to-low level signal
   * Falling edge interrupt: pin GPO15 will output a high-to-low level signal
   * Double edge interrupt: pin GPO15 will output a high-to-low level signal
   * Note: when pin GPO15 is at low level, if an interrupt occurs on other GPIO pins, the output level of pin GPO15 will not change
5. Sleep mode: this module supports sleep mode, and the power consumption will be reduced in the mode. There are 2 ways to wake up after entering sleep mode;
   * Changing GPIO pin level
   * I2C communication with the module

## Installation

There two methods: 
1. To use this library, first download the library file, paste it into the \Arduino\libraries directory, then open the examples folder and run the demo in the folder.
2. Search the DFRobot_CH423 library from the Arduino Software Library Manager and download it.

## Methods

```C++
  /**
   * @fn DFRobot_CH423
   * @brief Constructor
   * @param wire I2C bus pointer object. When calling the function, you may transfer a parameter into it. Defaule as Wire
   */
  DFRobot_CH423(TwoWire &wire = Wire);

  /**
   * @fn begin
   * @brief Initialize the module, this module has 2 groups of pins, one is bi-directional I/O pin GPIO0~GPIO7, which can be set as input or output mode at the same time,
   * @n the other is the GPO pins GPO0~GPO15, which can be set as open-drain output or push-pull output mode.
   * @param gpio set the mode of GPIO group pins, default to be eINPUT input mode, can be filled with parameter
   * @n     eINPUT       GPIO pin input mode, at high level when floating
   * @n     eOUTPUT      GPIO pin output mode, can output high and low level
   * @param gpo  Set the mode of GPO group pins, default to be ePUSH_PULL push-pull output mode, can be filled with parameter
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, the GPO pins only output low level or don't output in this mode
   * @n     ePUSH_PULL   GPO pin push-pull output mode, the GPO pins can output high or low level in this mode
   * @return Return 0 if initialization succeeds, otherwise return non-zero.
   */
  int begin(eMode_t gpio = eINPUT, eMode_t gpo = ePUSH_PULL);
  
  /**
   * @fn pinMode
   * @brief Set the mode of the pin groups, this module contains 2 groups of pins: GPIO(GPIO0~GPIO7) and GPO(GPO0~GPO15).
   * @note This module sets the mode by group, the pins in a group can only be set as one mode at the same time, and the pins in different groups can be set as different modes
   * @param group   Pin group parameter, ePinGroup_t enum variable member
   * @n     eGPIO                    Bi-directional I/O pin, GPIO0~GPIO7, which can be set as input(eINPUT) or output(eOUTPUT) mode, invalid when set to other modes.
   * @n     eGPO/eGPO0_7/eGPO8_15    The three parameters indicate the same meaning, set the mode of GPO group pins, select the parameter and GPO pins can only be configured as open-drain(eOPEN_DRAIN) or push-pull(ePUSH_PULL) output mode, other modes are invalid
   * @param mode    Group pin mode parameter, as member enum variable of eMode_t
   * @n     eINPUT       GPIO pin input mode, at high level when floating, this mode can only be used for eGPIO digital ports
   * @n     eOUTPUT      GPIO pin output mode, can output high and low levels, this mode can only be used for eGPIO digital ports
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, the GPO pins only output low level or don't output in this mode, it can only be used for eGPO group digital ports
   * @n     ePUSH_PULL   GPO pin push-pull output mode, the GPO pin can output high or low level in this mode, it can only be used for eGPO digital ports
   */
  void pinMode(ePinGroup_t group, eMode_t mode);
  void pinMode(int group, int mode);

  /**
   * @fn 
   * @brief Set the pin outputting high or low level
   * @param gpioPin   GPIO group pins, eGPIOPin_t enum variable member
   * @n     eGPIO0     Bi-directional I/O pin, GPIO0, indicates setting the output value of pin GPIO0 
   * @n     eGPIO1     Bi-directional I/O pin, GPIO1, indicates setting the output value of pin GPIO1
   * @n     eGPIO2     Bi-directional I/O pin, GPIO2, indicates setting the output value of pin GPIO2
   * @n     eGPIO3     Bi-directional I/O pin, GPIO3, indicates setting the output value of pin GPIO3
   * @n     eGPIO4     Bi-directional I/O pin, GPIO4, indicates setting the output value of pin GPIO4
   * @n     eGPIO5     Bi-directional I/O pin, GPIO5, indicates setting the output value of pin GPIO5
   * @n     eGPIO6     Bi-directional I/O pin, GPIO6, indicates setting the output value of pin GPIO6
   * @n     eGPIO7     Bi-directional I/O pin, GPIO7, indicates setting the output value of pin GPIO7
   * @n     eGPIOTotal sets the values of all GPIO group pins, when using this parameter, bit0-bit7 of parameter level are valid values, corresponding to the output values of pins GPIO0-GPIO7 respectively.
   * @param level    Output level
   * @n     HIGH or 1    Parameter level, bit0 in 8-bit data is valid, which indicates outputting high level
   * @n     LOW  or 0    Parameter level, bit0 in 8-bit data is valid, which indicates outputting low level
   * @n     0x00~0xFF    If parameter gpioPin is GPIOTotal, bit0~bit7 of parameter level are valid data, corresponding to GPIO0~GPIO7 pins respectively.
   */
  void  digitalWrite(eGPIOPin_t gpioPin, uint8_t level);

  /**
   * @fn digitalWrite
   * @brief Set the pin outputting high and low level, or control to output or stop (interrupt) low level
   * @param gpoPin   eGPOPin_t enum variable member
   * @n     eGPO0     GPO pin, GPO0, indicates setting the output value of pin GPO0
   * @n     eGPO1     GPO pin, GPO1, indicates setting the output value of pin GPO1
   * @n     eGPO2     GPO pin, GPO2, indicates setting the output value of pin GPO2
   * @n     eGPO3     GPO pin, GPO3, indicates setting the output value of pin GPO3
   * @n     eGPO4     GPO pin, GPO4, indicates setting the output value of pin GPO4
   * @n     eGPO5     GPO pin, GPO5, indicates setting the output value of pin GPO5
   * @n     eGPO6     GPO pin, GPO6, indicates setting the output value of pin GPO6
   * @n     eGPO7     GPO pin, GPO7, indicates setting the output value of pin GPO7
   * @n     eGPO8     GPO pin, GPO8, indicates setting the output value of pin GPO8
   * @n     eGPO9     GPO pin, GPO9, indicates setting the output value of pin GPO9
   * @n     eGPO10    GPO pin, GPO10, indicates setting the output value of pin GPO10
   * @n     eGPO11    GPO pin, GPO11, indicates setting the output value of pin GPO11
   * @n     eGPO12    GPO pin, GPO12, indicates setting the output value of pin GPO12
   * @n     eGPO13    GPO pin, GPO13, indicates setting the output value of pin GPO13
   * @n     eGPO14    GPO pin, GPO14, indicates setting the output value of pin GPO14
   * @n     eGPO15    GPO pin, GPO15, indicates setting the output value of pin GPO15
   * @n     eGPOTotal sets all the GPO group pins of 0~15, when using this parameter, the bit0~bit7 of the parameter 8-bit data level correspond to the output value of GPO0~GPO7 or GPO8~GPO15 pins respectively
   * @param level     Output level or output (stop) low level signal
   * @n     HIGH or 1    When GPO pin group is set to push-pull output mode, output high; for open-drain mode, output low level
   * @n     LOW  or 0    When GPO pin group is set to push-pull output mode, output low, for open-drain mode, no signal output
   * @n     0x00~0xFF    When gpoPin parameter is eGPOTotal, bit0~bit7 of level are all valid data, corresponding to pin GPO0~GPO7 or GPO8~GPO15
   */
  void  digitalWrite(eGPOPin_t gpoPin, uint8_t level);
  /**
   * @fn digitalWrite
   * @brief Set the output value of each group of CH423 IO pins by group
   * @param group   Group pin, ePinGroup_t enum variable member
   * @n     eGPIO    GPIO group pins 0~7, when setting the value, parameter level low 8bits valid, bit0~bit7 correspond to the output value of pin GPIO0-GPIO7 respectively, indicate setting the output value of GPIO group pins 0~7.
   * @n     eGPO     GPO pins 0~15, when setting the value, parameter level 16bits valid, bit0~bit15 correspond to the output value of pin GPO0~GPO15 respectively, indicate setting the output value of GPO group pins of 0~15.
   * @n     eGPO0_7  GPIO group pins 0~7, when setting the value, parameter level low 8bits valid, bit0~bit7 correspond to the output value of pin GPO0~GPO7 respectively, indicate setting the output value of GPO group pins 0~7.
   * @n     eGPO8_15 GPO group pins 8~15, when setting the value, parameter level high 8bits valid, bit8~bit15 correspond to the output value of pin GPO8~GPO15 respectively, indicate setting the output value of GPO group pins of 8~15.
   * @param level    16-bit data or uGroupValue_t union value combining with group parameter indicate the value of a group of pins, bit0~bit15 corresponds to GPIO0~GPIO7 (high 8bits invalid) or GPO0~GPO15
   * @n     0x0000~0xFFFF  16-bit data, bit0~bit15 represent different meanings respectively according to the value of the parameter group
   * @note uGroupValue_t union, users can use it to configure the value of each pin
   */
  void  digitalWrite(ePinGroup_t group, uGroupValue_t level);
  void  digitalWrite(ePinGroup_t group, uint16_t level);

  /**
   * @fn digitalRead
   * @brief Read the level status values of GPIO group pins
   * @param pin GPIO group pins, eGPIOPin_t enum variable member
   * @n     eGPIO0     Bi-directional I/O pin, GPIO0, indicates reading level status of pin GPIO0, 0 for low level, 1 for high level
   * @n     eGPIO1     Bi-directional I/O pin, GPIO1, indicates reading level status of pin GPIO1, 0 for low level, 1 for high level
   * @n     eGPIO2     Bi-directional I/O pin, GPIO2, indicates reading level status of pin GPIO2, 0 for low level, 1 for high level
   * @n     eGPIO3     Bi-directional I/O pin, GPIO3, indicates reading level status of pin GPIO3, 0 for low level, 1 for high level
   * @n     eGPIO4     Bi-directional I/O pin, GPIO4, indicates reading level status of pin GPIO4, 0 for low level, 1 for high level
   * @n     eGPIO5     Bi-directional I/O pin, GPIO5, indicates reading level status of pin GPIO5, 0 for low level, 1 for high level
   * @n     eGPIO6     Bi-directional I/O pin, GPIO6, indicates reading level status of pin GPIO6, 0 for low level, 1 for high level
   * @n     eGPIO7     Bi-directional I/O pin, GPIO7, indicates reading level status of pin GPIO7, 0 for low level, 1 for high level
   * @n     eGPIOTotal Bi-directional I/O pin, GPIO group pins GPIO0~GPIO7, indicate reading level status of all GPIO group pins, the returned value bit0~bit8 represents the level of GPIO0~GPIO7 pins respectively
   * @return Level status value
   */
  uint8_t digitalRead(eGPIOPin_t pin);
  
  /**
   * @fn attachInterrupt
   * @brief Set the external interrupt mode and interrupt service function of GPIO pins
   * @note Pin GPO15 of CH423 module is used to indicate whether an interrupt occurs on GPIO0-GPIO7 in interrupt mode, if an interrupt occurs on a pin, GPO15 will output a low level continuously, otherwise it will output a high level.
   * @n When an interrupt occurs on a pin, trigger continuously, if there is interrupt occurring on other pins, level of pin GPO15 keeps low without changing.
   * @param gpioPin   GPIO group pins, eGPIOPin_t enum variable member
   * @n     eGPIO0     Bi-directional I/O pin, GPIO0, indicates setting the external interrupt mode and interrupt service function of pin GPIO0
   * @n     eGPIO1     Bi-directional I/O pin, GPIO1, indicates setting the external interrupt mode and interrupt service function of pin GPIO1
   * @n     eGPIO2     Bi-directional I/O pin, GPIO2, indicates setting the external interrupt mode and interrupt service function of pin GPIO2
   * @n     eGPIO3     Bi-directional I/O pin, GPIO3, indicates setting the external interrupt mode and interrupt service function of pin GPIO3
   * @n     eGPIO4     Bi-directional I/O pin, GPIO4, indicates setting the external interrupt mode and interrupt service function of pin GPIO4
   * @n     eGPIO5     Bi-directional I/O pin, GPIO5, indicates setting the external interrupt mode and interrupt service function of pin GPIO5
   * @n     eGPIO6     Bi-directional I/O pin, GPIO6, indicates setting the external interrupt mode and interrupt service function of pin GPIO6
   * @n     eGPIO7     Bi-directional I/O pin, GPIO7, indicates setting the external interrupt mode and interrupt service function of pin GPIO7
   * @n     eGPIOTotal Set the values of all GPIO group pins, indicates setting GPIO0~GPIO7 to the same interrupt mode and interrupt service function
   * @param mode    Interrupt mode
   * @n     eLOW       Low level interrupt, initial level of the GPIO pin is high, if the GPIO pin detect a low level, pin GPO15 will output a low level signal
   * @n     eHIGH      High level interrupt, initial level of the GPIO pin is low, if the GPIO pin detect a high level, pin GPO15 will output a low level signal
   * @n     eRISING    Rising edge interrupt, initial level of the GPIO pin is low, if the GPIO pin detect a rising edge, pin GPO15 will output a high-to-low level signal (falling edge)
   * @n     eFALLING   Falling edge interrupt, initial level of the GPIO pin is high, if the GPIO pin detect a falling edge, pin GPO15 will output a high-to-low level signal (falling edge)
   * @n     eCHANGE    Double edge interrupt, initial level of the GPIO pin is high, every time an interrupt occurs, the initial level switches once. If the GPIO pin detect falling edge or rising edge, pin GPO15 will output a high-to-low level signal (falling edge)
   * @note If the level of any pin of GPIO0-GPIO7 changes, GPO15 will output a low level signal, and it will not output high level until the levels of GPIO0-GPIO7 pins return to the initial level.
   * @param cb  Point to interrupt service function
   */
  void attachInterrupt(eGPIOPin_t gpioPin, eInterruptMode_t mode, CH423_INT_CB cb);  
  
  /**
   * @fn enableInterrupt
   * @brief Enable GPIO external interrupt
   */
  void enableInterrupt();

  /**
   * @fn disableInterrupt
   * @brief Disable GPIO external interrupt
   */
  void disableInterrupt();
  
  /**
   * @fn pollInterrupts
   * @brief Poll GPIO interrupt event
   */
  void pollInterrupts();

  /**
   * @fn sleep
   * @brief Enter sleep mode
   * @note After entering sleep mode, wake up in 2 ways
   * @n 1. An external interrupt occurs on the GPIO pin
   * @n 2. Performing pin operation
   */
  void sleep();
  /**
   * @fn pinDescription
   * @brief Describe GPIO group pins
   * @param pin GPIO group pins, eGPIOPin_t enum variable member
   * @n     eGPIO0     Bi-directional I/O pin, GPIO0, indicates obtaining pin eGPIO0 description
   * @n     eGPIO1     Bi-directional I/O pin, GPIO1, indicates obtaining pin eGPIO1 description
   * @n     eGPIO2     Bi-directional I/O pin, GPIO2, indicates obtaining pin eGPIO2 description
   * @n     eGPIO3     Bi-directional I/O pin, GPIO3, indicates obtaining pin eGPIO3 description
   * @n     eGPIO4     Bi-directional I/O pin, GPIO4, indicates obtaining pin eGPIO4 description
   * @n     eGPIO5     Bi-directional I/O pin, GPIO5, indicates obtaining pin eGPIO5 description
   * @n     eGPIO6     Bi-directional I/O pin, GPIO6, indicates obtaining pin eGPIO6 description
   * @n     eGPIO7     Bi-directional I/O pin, GPIO7, indicates obtaining pin eGPIO7 description
   * @return Return pin description string
   * @n such as "GPIO0" "GPIO1" "GPIO2" "GPIO3" "GPIO4" "GPIO5" "GPIO6" "GPIO7"
   */
  String pinDescription(eGPIOPin_t pin);

  /**
   * @fn pinDescription
   * @brief Convert pin into string description 
   * @param pin  eGPOPin_t enum variable member
   * @n     eGPO0     GPO pin, GPO0, indicates obtaining pin eGPO0 description
   * @n     eGPO1     GPO pin, GPO1, indicates obtaining pin eGPO1 description
   * @n     eGPO2     GPO pin, GPO2, indicates obtaining pin eGPO2 description
   * @n     eGPO3     GPO pin, GPO3, indicates obtaining pin eGPO3 description
   * @n     eGPO4     GPO pin, GPO4, indicates obtaining pin eGPO4 description
   * @n     eGPO5     GPO pin, GPO5, indicates obtaining pin eGPO5 description
   * @n     eGPO6     GPO pin, GPO6, indicates obtaining pin eGPO6 description
   * @n     eGPO7     GPO pin, GPO7, indicates obtaining pin eGPO7 description
   * @n     eGPO8     GPO pin, GPO8, indicates obtaining pin eGPO8 description
   * @n     eGPO9     GPO pin, GPO9, indicates obtaining pin eGPO9 description
   * @n     eGPO10    GPO pin, GPO10, indicates obtaining pin eGPO10 description
   * @n     eGPO11    GPO pin, GPO11, indicates obtaining pin eGPO11 description
   * @n     eGPO12    GPO pin, GPO12, indicates obtaining pin eGPO12 description
   * @n     eGPO13    GPO pin, GPO13, indicates obtaining pin eGPO13 description
   * @n     eGPO14    GPO pin, GPO14, indicates obtaining pin eGPO14 description
   * @n     eGPO15    GPO pin, GPO15, indicates obtaining pin eGPO15 description
   * @return Return pin description string
   * @n such as "GPO0" "GPO1" "GPO2"  "GPO3"  "GPO4"  "GPO5"  "GPO6"  "GPO7"
   * @n         "GPO8" "GPO9" "GPO10" "GPO11" "GPO12" "GPO13" "GPO14" "GPO15"
   */
  String pinDescription(eGPOPin_t pin);
```

## Compatibility

MCU                |  Work Well    | Work Wrong   | Untested    | Remarks
------------------ | :----------: | :----------: | :---------: | -----
Arduino Uno        |       √       |              |             | 
Mega2560           |      √       |              |             | 
Leonardo           |      √       |              |             | The module interrupt function is not supported
ESP32              |      √       |              |             | 
ESP8266            |      √       |              |             | 
micro:bit          |      √       |              |             | The module low-level interrupt function is not supported
FireBeetle M0      |      √       |              |             | 

## History

- 2022/03/14 - Version 1.0.0 released.

## Credits

Written by Arya(xue.peng@dfrobot.com), 2022. (Welcome to our [website](https://www.dfrobot.com/))





