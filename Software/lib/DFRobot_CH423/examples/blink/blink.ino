/*!
 * @file blink.ino
 * @brief Turns an LED on for one second, then off for one second, repeatedly.
 * @note This module implements the blink function in 2 ways:
 * @n 1. Set GPIO group pins to output mode and connect the LED lamp to one of the GPIO pins (GPIO0~GPIO7), and control the pin to output high and low levels;
 * @n 2. Set GPO group pins as push-pull output mode and connect the LED lamp to one of the GP0 pins (GPO0~GPO15), and control the pin to output high and low levels;
 * @n Hardware connection: connect the LED pin to the corresponding output pin
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-09
 * @url https://github.com/DFRobot/DFRobot_CH423
 */
#include "DFRobot_CH423.h"

//Disable the macro, and the blink function will shift to the pin GPO0 of GPO pins, otherwise, it will be implemented by the pin GPIO0 of GPIO pins
#define GPIO_ENABLE

DFRobot_CH423 ch423;

void setup() {
  Serial.begin(115200);

  while(!Serial){

  }
  ch423.begin();
  /**
   * @fn pinMode
   * @brief Set the mode of the pin group, this module contains 2 groups of pins: GPIO(GPIO0~GPIO7) and GPO(GPO0~GPO15).
   * @note This module sets the mode by group, the pins in a group can only be set as one mode at the same time, and the pins in different groups can be set as different modes
   * @param group   Pin group parameter, ePinGroup_t enum variable member
   * @n     eGPIO                    Bi-directional I/O pin, GPIO0-GPIO7, which can be set as input(eINPUT) or output(eOUTPUT) mode, setting to other modes is invalid
   * @n     eGPO/eGPO0_7/eGPO8_15    The three parameters indicate the same meaning, set the mode of GPO group pins, select the parameter and GPO pins can only be configured as open-drain(eOPEN_DRAIN) or push-pull(ePUSH_PULL) output mode, other modes are invalid
   * @param mode    Group pin mode parameter, as member enum variable of eMode_t
   * @n     eINPUT       GPIO pin input mode, at high level when floating, this mode can only be used for eGPIO digital ports
   * @n     eOUTPUT      GPIO pin output mode, can output high and low levels, this mode can only be used for eGPIO digital ports
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, the GPO pins only output low level or don't output in this mode, it can only be used for eGPO group digital ports
   * @n     ePUSH_PULL   GPO pin push-pull output mode, the GPO pin can output high or low level in this mode, it can only be used for eGPO digital ports
   */
#ifdef GPIO_ENABLE
  ch423.pinMode(ch423.eGPIO, ch423.eOUTPUT);
#else
  ch423.pinMode(ch423.eGPO, ch423.ePUSH_PULL);
#endif
}

void loop() {
#ifdef GPIO_ENABLE
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
   * @n     eGPIOTotal sets the values of all GPIO group pins, when using this parameter, bit0~bit7 of parameter level are valid values, corresponding to the output values of pins GPIO0~GPIO7 respectively.
   * @param level    Output level
   * @n     HIGH or 1    Parameter level, bit0 in 8-bit data is valid, which indicates outputting high level
   * @n     LOW  or 0    Parameter level, bit0 in 8-bit data is valid, which indicates outputting low level
   * @n     0x00~0xFF    If parameter gpioPin is GPIOTotal, bit0~bit7 of parameter level are valid data, corresponding to GPIO0~GPIO7 pins respectively.
   */
  ch423.digitalWrite(/* gpio= */ch423.eGPIO0, /* level= */HIGH);       // turn the LED on (HIGH is the voltage level)
  //ch423.digitalWrite(/* gpio= */ch423.eGPIOTotal, /* level= */0xFF); // set all pins in GPIO group(GPIO0~GPIO7) outputting high level
  delay(1000);                                                         // wait for a second
  ch423.digitalWrite(/* gpio= */ch423.eGPIO0, /* level= */LOW);        // turn the LED off by making the voltage LOW
  //ch423.digitalWrite(/* gpio= */ch423.eGPIOTotal, /* level= */0x00); // set all pins in GPIO group(GPIO0~GPIO7) outputting low level
  delay(1000);                                                         // wait for a second
#else
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
   * @n     eGPOTotal sets all the GPO pins of 0~15, when using this parameter, the bit0~bit7 of the parameter 8-bit data level correspond to the output value of GPO0~GPO7 or GPO8~GPO15 pins respectively
   * @param level     Output level or output (stop) low level signal
   * @n     HIGH or 1    When GPO pin group is set to push-pull output mode, output high; for open-drain mode, output low level
   * @n     LOW  or 0    When GPO pin group is set to push-pull output mode, output low, for open-drain mode, no signal output
   * @n     0x00~0xFF    When gpoPin parameter is eGPOTotal, bit0~bit7 of level are all valid data, corresponding to pin GPO0~GPO7 or GPO8~GPO15
   */
  ch423.digitalWrite(/* gpo= */ch423.eGPO0, /* level= */HIGH);       // turn the LED on (HIGH is the voltage level)
  //ch423.digitalWrite(/* gpo= */ch423.eGPOTotal, /* level= */0xFF); // set all pins in GPO group(GPO0~GPO15) outputting high level
  delay(1000);                                                       // wait for a second
  ch423.digitalWrite(/* gpio= */ch423.eGPO0, /* level= */LOW);       // turn the LED off by making the voltage LOW
  //ch423.digitalWrite(/* gpo= */ch423.eGPOTotal, /* level= */0x00); // set all pins in GPO group(GPO0~GPO15) outputting low level
  delay(1000);                           // wait for a second
#endif
  
}
