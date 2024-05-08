/*!
 * @file group.ino
 * @brief This demo is mainly used to demonstrate how to operate the digital ports of a group at a time. This module has 2 groups of digital pins, GPIO group pins and GPO pins, and GPO pins are divided into GPO0_7 group pins and GPO8_15 group pins.
 * @n GPIO group pins: can be used as both input pins and output pins
 * @n GPO group pins: can only be used as output pins, there are 2 output modes, push-pull and open-drain output mode
 * @n Users can operate the pins by group according to the API function provided
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-14
 * @url https://github.com/DFRobot/DFRobot_CH423
 */
#include "DFRobot_CH423.h"

#define GROUP_GPIO_INPUT   0
#define GROUP_GPIO_OUTPUT  1
#define GROUP_GPO          2

//Demo function switch
#define DEMO_FUN_SWITCH    GROUP_GPIO_INPUT
//#define DEMO_FUN_SWITCH    GROUP_GPIO_OUTPUT
//#define DEMO_FUN_SWITCH    GROUP_GPO

DFRobot_CH423 ch423;

void setup() {
  Serial.begin(115200);

  while(!Serial){

  }
  ch423.begin();
  /**
   * @fn pinMode
   * @brief Set the mode of the pin groups, this module contains 2 groups of pins: GPIO(GPIO0~GPIO7) and GPO(GPO0~GPO15).
   * @note This module sets the mode by group, the pins in a group can only be set as one mode at the same time, and the pins in different groups can be set as different modes
   * @param group   Pin group parameter, ePinGroup_t enum variable member
   * @n     eGPIO                    Bi-directional I/O pin, GPIO0~GPIO7, which can be set as input(eINPUT) or output(eOUTPUT) mode, setting to other modes is invalid
   * @n     eGPO/eGPO0_7/eGPO8_15    The three parameters indicate the same meaning, set the mode of GPO group pins, select the parameter and GPO pins can only be configured as open-drain(eOPEN_DRAIN) or push-pull(ePUSH_PULL) output mode, other modes are invalid
   * @param mode    Group pin mode parameter, enum variable member for eMode_t
   * @n     eINPUT       GPIO pin input mode, at high level when floating, this mode can only be used for eGPIO digital ports
   * @n     eOUTPUT      GPIO pin output mode, can output high and low levels, this mode can only be used for eGPIO digital ports
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, the GPO pins only output low level or don't output in this mode, it can only be used for eGPO group digital ports
   * @n     ePUSH_PULL   GPO pin push-pull output mode, the GPO pin can output high or low level in this mode, it can only be used for eGPO digital ports
   */
#if(DEMO_FUN_SWITCH == GROUP_GPIO_INPUT)
  ch423.pinMode(ch423.eGPIO, ch423.eINPUT);
#elif(DEMO_FUN_SWITCH == GROUP_GPIO_OUTPUT) 
  ch423.pinMode(ch423.eGPIO, ch423.eOUTPUT);

  //Set GPIO group pin outputting 0xF0, bit0~bit7 of 0xF0 correspond to GPIO0~GPIO7 of GPIO group respectively
  DFRobot_CH423::uGroupValue_t gpio;
  
  gpio.GPO0  = 0; //Set pin GPIO0 outputting 0
  gpio.GPO1  = 0; //Set pin GPIO1 outputting 0
  gpio.GPO2  = 0; //Set pin GPIO2 outputting 0
  gpio.GPO3  = 0; //Set pin GPIO3 outputting 0
  gpio.GPO4  = 1; //Set pin GPIO4 outputting 1
  gpio.GPO5  = 1; //Set pin GPIO5 outputting 1
  gpio.GPO6  = 1; //Set pin GPIO6 outputting 1
  gpio.GPO7  = 1; //Set pin GPIO7 outputting 1
  //gpio.GPIO = 0xF0

  ch423.digitalWrite(ch423.eGPIO, gpio);        //Set GPIO0~GPIO3 outputting 0, GPIO4~GPIO7 outputting 1
  //ch423.digitalWrite(ch423.eGPIO, 0xF0);      //Set GPIO0~GPIO3 outputting 0, GPIO4~GPIO7 outputting 1
  //ch423.digitalWrite(ch423.eGPIOTotal, 0xF0); //Set GPIO0~GPIO3 outputting 0, GPIO4~GPIO7 outputting 1

  Serial.print("GPIO0  OUPUT: ");Serial.println(gpio.GPIO0);
  Serial.print("GPIO1  OUPUT: ");Serial.println(gpio.GPIO1);
  Serial.print("GPIO2  OUPUT: ");Serial.println(gpio.GPIO2);
  Serial.print("GPIO3  OUPUT: ");Serial.println(gpio.GPIO3);
  Serial.print("GPIO4  OUPUT: ");Serial.println(gpio.GPIO4);
  Serial.print("GPIO5  OUPUT: ");Serial.println(gpio.GPIO5);
  Serial.print("GPIO6  OUPUT: ");Serial.println(gpio.GPIO6);
  Serial.print("GPIO7  OUPUT: ");Serial.println(gpio.GPIO7);
#else
  ch423.pinMode(ch423.eGPO, ch423.ePUSH_PULL);

  //Set GPO group pins outputting 0x55F0, bit0~bit15 of 0x55F0 correspond to GPO0~GPIO15 of GPO group respectively
  DFRobot_CH423::uGroupValue_t gpo;

  gpo.GPO0  = 0; //Set pin GPO0 outputting 0
  gpo.GPO1  = 0; //Set pin GPO1 outputting 0
  gpo.GPO2  = 0; //Set pin GPO2 outputting 0
  gpo.GPO3  = 0; //Set pin GPO3 outputting 0
  gpo.GPO4  = 1; //Set pin GPO4 outputting 1
  gpo.GPO5  = 1; //Set pin GPO5 outputting 1
  gpo.GPO6  = 1; //Set pin GPO6 outputting 1
  gpo.GPO7  = 1; //Set pin GPO7 outputting 1
  gpo.GPO8  = 1; //Set pin GPO0 outputting 1
  gpo.GPO9  = 0; //Set pin GPO1 outputting 0
  gpo.GPO10 = 1; //Set pin GPO2 outputting 1
  gpo.GPO11 = 0; //Set pin GPO3 outputting 0
  gpo.GPO12 = 1; //Set pin GPO4 outputting 1
  gpo.GPO13 = 0; //Set pin GPO5 outputting 0
  gpo.GPO14 = 1; //Set pin GPO6 outputting 1
  gpo.GPO15 = 0; //Set pin GPO7 outputting 0
  //gpo.GPO0_7  = 0xF0; 
  //gpo.GPO8_15 = 0x55; 
  //gpo.GPO = 0x55F0;  

  ch423.digitalWrite(ch423.eGPO0_7, 0xF0);          //only set GPO0~GPO7 outputting 0xF0
  ch423.digitalWrite(ch423.eGPO8_15, (0x55 << 8));  //only set GPO8~GPO15 outputting 0x55
  //ch423.digitalWrite(ch423.eGPO, gpo);        //set pin GPO15~GPO0 outputting 0x55F0
  //ch423.digitalWrite(ch423.eGPO, 0x55F0);   //set pin GPO15~GPO0 outputting 0x55F0
  Serial.print("GPO0  OUPUT: ");Serial.println(gpo.GPO0);
  Serial.print("GPO1  OUPUT: ");Serial.println(gpo.GPO1);
  Serial.print("GPO2  OUPUT: ");Serial.println(gpo.GPO2);
  Serial.print("GPO3  OUPUT: ");Serial.println(gpo.GPO3);
  Serial.print("GPO4  OUPUT: ");Serial.println(gpo.GPO4);
  Serial.print("GPO5  OUPUT: ");Serial.println(gpo.GPO5);
  Serial.print("GPO6  OUPUT: ");Serial.println(gpo.GPO6);
  Serial.print("GPO7  OUPUT: ");Serial.println(gpo.GPO7);
  Serial.print("GPO8  OUPUT: ");Serial.println(gpo.GPO8);
  Serial.print("GPO9  OUPUT: ");Serial.println(gpo.GPO9);
  Serial.print("GPO10 OUPUT: ");Serial.println(gpo.GPO10);
  Serial.print("GPO11 OUPUT: ");Serial.println(gpo.GPO11);
  Serial.print("GPO12 OUPUT: ");Serial.println(gpo.GPO12);
  Serial.print("GPO13 OUPUT: ");Serial.println(gpo.GPO13);
  Serial.print("GPO14 OUPUT: ");Serial.println(gpo.GPO14);
  Serial.print("GPO15 OUPUT: ");Serial.println(gpo.GPO15);
#endif
}

void loop() {
#if(DEMO_FUN_SWITCH == GROUP_GPIO_INPUT)
  //Query the value of each GPIO input pin (GPIO0~GPIO7) once in 1s
  //0~7 bit of value correspond to the pin status of pins GPIO0~GPIO7 respectively
  uint8_t value = ch423.digitalRead(ch423.eGPIOTotal);
  Serial.print("The group of GPIO's pins state are 0x");
  Serial.println(value, HEX);
  delay(1000);
#endif
  
}
