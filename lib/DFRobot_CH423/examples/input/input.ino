/*!
 * @file input.ino
 * @brief After pressing the button, the serial print button is pressed
 * @note Only the pins in GPIO group can be set to input mode for this module.
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-09
 * @url https://github.com/DFRobot/DFRobot_CH423
 */
#include "DFRobot_CH423.h"

DFRobot_CH423 ch423;

void setup() {
  Serial.begin(115200);

  while(!Serial){

  }
  ch423.begin();
  /**
   * @fn pinMode
   * @brief Set the mode of the pin groups, this module contains 2 groups of pins: GPIO(GPIO0~GPIO7) and GPO(GPO0~GPO15).
   * @note This module sets the mode by group, the pins in a group can only be set as one mode at the same time, and the pins in different groups can be set as different modes.
   * @param group   Pin group parameter, ePinGroup_t enum variable member
   * @n     eGPIO                    Bi-directional I/O pin, GPIO0-GPIO7, which can be set as input(eINPUT) or output(eOUTPUT) mode, setting to other modes is invalid
   * @n     eGPO/eGPO0_7/eGPO8_15    The three parameters indicate the same meaning, set the mode of GPO group pins, select the parameter and GPO pins can only be configured as open-drain(eOPEN_DRAIN) or push-pull(ePUSH_PULL) output mode, other modes are invalid
   * @param mode    Group pin mode parameter, as member enum variable of eMode_t
   * @n     eINPUT       GPIO pin input mode, at high level when floating, this mode can only be used for eGPIO digital ports
   * @n     eOUTPUT      GPIO pin output mode, can output high and low levels, this mode can only be used for eGPIO digital ports
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, the GPO pins only output low level or don't output in this mode, it can only be used for eGPO group digital ports
   * @n     ePUSH_PULL   GPO pin push-pull output mode, the GPO pin can output high or low level in this mode, it can only be used for eGPO digital ports
   */
  ch423.pinMode(ch423.eGPIO, ch423.eINPUT);
}

void loop() {
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
   * @n     eGPIOTotal Bi-directional I/O pin, GPIO group pins GPIO0~GPIO7 indicate reading level status of all GPIO group pins, the returned value bit0~bit8 represents the level of GPIO0~GPIO7 pins respectively
   * @return Level status value
   */
  uint8_t value = ch423.digitalRead(/*pin = */ch423.eGPIO0);
  if(value){
      Serial.println("Button press!");
      delay(200);
  }else{
      //Serial.println("Button release!");
  }
  
}
