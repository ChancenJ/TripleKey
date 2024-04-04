/*!
 * @file waterLamp.ino
 * @brief This demo is used to make RGB LED realize the effect of the water flowing effect
 * @note 8 LEDs need to be connected to GPIO group pins, GPO0~7 group pins, or GPO8~15 group pins, you can switch GROUP_FLAG to achieve the effect of connecting to the IO port of different groups
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-11
 * @url https://github.com/DFRobot/DFRobot_CH423
 */
#include "DFRobot_CH423.h"

#define GROUP_GPIO    0
#define GROUP_GPO0_7  1
#define GROUP_GPO8_15 2

//Control macro GROUP_FLAG to realize the connecting of the LED to the  pin group of the module
#define GROUP_FLAG  GROUP_GPIO
//#define GROUP_FLAG  GROUP_GPO0_7
//#define GROUP_FLAG  GROUP_GPO8_15

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
   * @n     eGPIO                    Bi-directional I/O pins, GPIO0~GPIO7, which can be set as input(eINPUT) or output(eOUTPUT) mode, invalid when set to other modes
   * @n     eGPO/eGPO0_7/eGPO8_15    The three parameters indicate the same meaning, set the mode of GPO group pins, select the parameter and GPO pins can only be configured as open-drain(eOPEN_DRAIN) or push-pull(ePUSH_PULL) output mode, other modes are invalid
   * @param mode    Group pin mode parameter, as member enum variable of eMode_t
   * @n     eINPUT       GPIO pin input mode, at high level when floating, this mode can only be used for eGPIO digital ports
   * @n     eOUTPUT      GPIO pin output mode, can output high and low levels, this mode can only be used for eGPIO digital ports
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, the GPO pins only output low level or don't output in this mode, it can only be used for eGPO group digital ports
   * @n     ePUSH_PULL   GPO pin push-pull output mode, the GPO pin can output high or low level in this mode, it can only be used for eGPO digital ports
   */
#if  (GROUP_FLAG == GROUP_GPIO)
  ch423.pinMode(ch423.eGPIO, ch423.eOUTPUT);
#else
  ch423.pinMode(ch423.eGPO, ch423.ePUSH_PULL);
#endif
}

void loop() {
#if  (GROUP_FLAG == GROUP_GPIO)

  DFRobot_CH423::eGPIOPin_t gpioBuf[]= {ch423.eGPIO0, ch423.eGPIO1, ch423.eGPIO2, ch423.eGPIO3, ch423.eGPIO4, ch423.eGPIO5, ch423.eGPIO6, ch423.eGPIO7};
  int on = 0, off;
  ch423.digitalWrite(gpioBuf[on], HIGH);
  for(on = 1, off = 0; on < sizeof(gpioBuf)/sizeof(DFRobot_CH423::eGPIOPin_t); on++, off++){
    ch423.digitalWrite(gpioBuf[on], HIGH);
    ch423.digitalWrite(gpioBuf[off], LOW);
    delay(200);
  }
  ch423.digitalWrite(gpioBuf[off], LOW);

#elif (GROUP_FLAG == GROUP_GPO0_7)
  DFRobot_CH423::eGPOPin_t gpoBuf[]= {ch423.eGPO0, ch423.eGPO1, ch423.eGPO2, ch423.eGPO3, ch423.eGPO4, ch423.eGPO5, ch423.eGPO6, ch423.eGPO7};
  int on = 0, off;
  ch423.digitalWrite(gpoBuf[on], HIGH);
  for(on = 1, off = 0; on < sizeof(gpoBuf)/sizeof(DFRobot_CH423::eGPOPin_t); on++, off++){
    ch423.digitalWrite(gpoBuf[on], HIGH);
    ch423.digitalWrite(gpoBuf[off], LOW);
    delay(200);
  }
  ch423.digitalWrite(gpoBuf[off], LOW);
#else
  DFRobot_CH423::eGPOPin_t gpoBuf[]= {ch423.eGPO8, ch423.eGPO9, ch423.eGPO10, ch423.eGPO11, ch423.eGPO12, ch423.eGPO13, ch423.eGPO14, ch423.eGPO15};
  int on = 0, off;
  ch423.digitalWrite(gpoBuf[on], HIGH);
  for(on = 1, off = 0; on < sizeof(gpoBuf)/sizeof(DFRobot_CH423::eGPOPin_t); on++, off++){
    ch423.digitalWrite(gpoBuf[on], HIGH);
    ch423.digitalWrite(gpoBuf[off], LOW);
    delay(200);
  }
  ch423.digitalWrite(gpoBuf[off], LOW);
#endif
  delay(200);
}
