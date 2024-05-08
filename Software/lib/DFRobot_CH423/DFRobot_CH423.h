/*!
 * @file DFRobot_CH423.h
 * @brief This is a I2C to IO expansion board based on CH432, an universal remote I/O expansion chip with a two-wire serial interface. It features: 
 * @n 1. 8 Bi-directional input/output pin: GPIO0 ~ GPIO7;
 * @n 2. 16 general output pins: GPO0~GPO15;
 * @n 3. Support input level change interrupt: if GPIO pin level is not the same as the initial level, GPO15 will output a low level signal; 
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-07
 * @https://github.com/DFRobot/DFRobot_CH423
 */
#ifndef __DFROBOT_MCP23017_H
#define __DFROBOT_MCP23017_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Wire.h>


typedef void(*CH423_INT_CB)(int index);

class DFRobot_CH423{
public:
  typedef enum{
    eGPO0,   /**< General-purpose output pin, GPO0 */
    eGPO1,   /**< General-purpose output pin, GPO1 */
    eGPO2,   /**< General-purpose output pin, GPO2 */
    eGPO3,   /**< General-purpose output pin, GPO3 */
    eGPO4,   /**< General-purpose output pin, GPO4 */
    eGPO5,   /**< General-purpose output pin, GPO5 */
    eGPO6,   /**< General-purpose output pin, GPO6 */
    eGPO7,   /**< General-purpose output pin, GPO7 */
    eGPO8,   /**< General-purpose output pin, GPO8 */
    eGPO9,   /**< General-purpose output pin, GPO9 */
    eGPO10,  /**< General-purpose output pin, GPO10*/
    eGPO11,  /**< General-purpose output pin, GPO11*/
    eGPO12,  /**< General-purpose output pin, GPO12*/
    eGPO13,  /**< General-purpose output pin, GPO13*/
    eGPO14,  /**< General-purpose output pin, GPO14*/
    eGPO15,  /**< General-purpose output pin, GPO15*/
    eGPOTotal/**< Total number of general-purpose output pins  */
  }eGPOPin_t;

  typedef enum{
    eGPIO0,    /**< Bi-directional input/output pin, GPIO0*/
    eGPIO1,    /**< Bi-directional input/output pin, GPIO1*/
    eGPIO2,    /**< Bi-directional input/output pin, GPIO2*/
    eGPIO3,    /**< Bi-directional input/output pin, GPIO3*/
    eGPIO4,    /**< Bi-directional input/output pin, GPIO4*/
    eGPIO5,    /**< Bi-directional input/output pin, GPIO5*/
    eGPIO6,    /**< Bi-directional input/output pin, GPIO6*/
    eGPIO7,    /**< Bi-directional input/output pin, GPIO7*/
    eGPIOTotal /**< Total number of bi-directional input/output pins   */
  }eGPIOPin_t;

  typedef enum{
    eGPIO,   /**< Bi-directional input/output pin, GPIO0~GPIO7  */
    eGPO,    /**< General-purpose output pin, GPO0~GPO15       */
    eGPO0_7, /**< General-purpose output pin, GPO0~GPO7        */
    eGPO8_15 /**< General-purpose output pin, GPO8~GPO15       */
  }ePinGroup_t;

  typedef enum{
    eINPUT,       /**<GPIO pin input mode, at high level when floating*/
    eOUTPUT,      /**<GPIO pin output mode, can output high/low level*/
    eOPEN_DRAIN,  /**<GPO pin open-drain output mode, can only be used for eGPO0_7 and eGPO8_15 digital ports. And GPO can only output low level or do not output in this mode.*/
    ePUSH_PULL    /**<GPO pin push-pull output mode, can only be used for eGPO0_7 and eGPO8_15 digital ports. And GPO can output high or low level.*/
  }eMode_t;

  typedef enum{
    eLOW,            /**< configure pin interrupt, low level interrupt */
    eHIGH,           /**< configure pin interrupt, high level interrupt */
    eRISING,         /**< configure pin interrupt, rising edge interrupt */
    eFALLING,        /**< configure pin interrupt, falling edge interrupt */
    eCHANGE          /**< configure pin interrupt, double edge interrupt*/
  }eInterruptMode_t;

  typedef union{
    uint8_t GPIO;      /**< Level of GPIO pin 0~7*/
    uint16_t GPO0_15;      /**< Level of GPO pin 0~15*/
    struct{
      uint8_t GPO0_7;  /**< Level of GPO pin 0~7*/
      uint8_t GPO8_15; /**< Level of GPO pin 8~15*/
    };
    struct{
      uint16_t GPIO0:1; /**< Correspond to level of pin GPIO0 or GPO0*/
      uint16_t GPIO1:1; /**< Correspond to level of pin GPIO1 or GPO0*/
      uint16_t GPIO2:1; /**< Correspond to level of pin GPIO2 or GPO0*/
      uint16_t GPIO3:1; /**< Correspond to level of pin GPIO3 or GPO0*/
      uint16_t GPIO4:1; /**< Correspond to level of pin GPIO4 or GPO0*/
      uint16_t GPIO5:1; /**< Correspond to level of pin GPIO5 or GPO0*/
      uint16_t GPIO6:1; /**< Correspond to level of pin GPIO6 or GPO0*/
      uint16_t GPIO7:1; /**< Correspond to level of pin GPIO7 or GPO0*/
    };
    struct{
      uint16_t GPO0:1; /**< Correspond to level of pin GPIO0 or GPO0*/
      uint16_t GPO1:1; /**< Correspond to level of pin GPIO1 or GPO0*/
      uint16_t GPO2:1; /**< Correspond to level of pin GPIO2 or GPO0*/
      uint16_t GPO3:1; /**< Correspond to level of pin GPIO3 or GPO0*/
      uint16_t GPO4:1; /**< Correspond to level of pin GPIO4 or GPO0*/
      uint16_t GPO5:1; /**< Correspond to level of pin GPIO5 or GPO0*/
      uint16_t GPO6:1; /**< Correspond to level of pin GPIO6 or GPO0*/
      uint16_t GPO7:1; /**< Correspond to level of pin GPIO7 or GPO0*/
      uint16_t GPO8:1; /**< Correspond to level of pin GPO8*/
      uint16_t GPO9:1; /**< Correspond to level of pin GPO9*/
      uint16_t GPO10:1; /**< Correspond to level of pin GPO10*/
      uint16_t GPO11:1; /**< Correspond to level of pin GPO11*/
      uint16_t GPO12:1; /**< Correspond to level of pin GPO12*/
      uint16_t GPO13:1; /**< Correspond to level of pin GPO13*/
      uint16_t GPO14:1; /**< Correspond to level of pin GPO14*/
      uint16_t GPO15:1; /**< Correspond to level of pin GPO15*/
    };
  }uGroupValue_t;

public:
  /**
   * @fn DFRobot_CH423
   * @brief Constructor
   * @param wire I2C bus pointer object. When calling the function, you may transfer a parameter into it. Defaule as Wire
   */
  DFRobot_CH423(TwoWire &wire = Wire);

  /**
   * @fn begin
   * @brief Init module. The module has two groups of pins: bi-directional input/output GPIO0~GPIO7(can be set to input/output module simultaneously)  
   * @n and general-purpose output GPO0~GPO159(can be set to open-drain or push-pull output mode).
   * @param gpio Set GPIO pin mode, default to be eINPUT input mode, can be filled with parameter: 
   * @n     eINPUT       GPIO pin input mode, at high level when floating 
   * @n     eOUTPUT      GPIO pin output mode, can output high/low level 
   * @param gpo  Set gpo pin mode, default to be ePUSH_PULL push-pull output mode, can be filled with parameter: 
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, GPO only output low level or do not output in this mode. 
   * @n     ePUSH_PULL   GPO pin push-pull output mode, GPO can output high or low level in this mode.
   * @return Return 0 if initialization succeeds, otherwise return non-zero.
   */
  int begin(eMode_t gpio = eINPUT, eMode_t gpo = ePUSH_PULL);
  
  /**
   * @fn pinMode
   * @brief Set pin group mode. The module includes two groups of pins: GPIO(GPIO0~GPIO7) and GPO(GPO0~GPO15)
   * @note This module sets the mode by group, the pins in a group can only be set as one mode at the same time, and the pins in different groups can be set to different modes 
   * @param group   Pin group parameter, ePinGroup_t enum variable member 
   * @n     eGPIO                    Bi-directional input/output pin, GPIO0~GPIO7, this pin group can be set to input (eINPUT) or output(eOUTPUT) mode, invalid when set to other modes.
   * @n     eGPO/eGPO0_7/eGPO8_15    3 parameters has same meanings: set GPO group pin mode. They are for genernal-purpose output pin and can only be configured as open-drain(eOPEN_DRAIN) or push-pull(ePUSH_PULL)output mode, invalid when set to other modes.
   * @param mode    Group pin mode parameter, as member of eMode_t enum variable 
   * @n     eINPUT       GPIO pin input mode, at high level when floating, can only be used for eGPIO group digital port
   * @n     eOUTPUT      GPIO pin output mode, can output high or low level, can only be used for eGPIO digital port
   * @n     eOPEN_DRAIN  GPO pin open-drain output mode, GOP pin can only output low level or do not output in this mode. Only suitable for eGPO group digital port
   * @n     ePUSH_PULL   GPO pin push=pull output mode, GPO pin can output high or low level in this mode. Only suitable for eGPO group digital port
   */
  void pinMode(ePinGroup_t group, eMode_t mode);
  void pinMode(int group, int mode);

  /**
   * @fn 
   * @brief Set pin output level 
   * @param gpioPin   GPIO pins, eGPIOPin_t enum variable member 
   * @n     eGPIO0     Bi-directional input/output pin, GPIO0, set output value of pin GPIO0
   * @n     eGPIO1     Bi-directional input/output pin, GPIO1, set output value of pin GPIO1
   * @n     eGPIO2     Bi-directional input/output pin, GPIO2, set output value of pin GPIO2
   * @n     eGPIO3     Bi-directional input/output pin, GPIO3, set output value of pin GPIO3
   * @n     eGPIO4     Bi-directional input/output pin, GPIO4, set output value of pin GPIO4
   * @n     eGPIO5     Bi-directional input/output pin, GPIO5, set output value of pin GPIO5
   * @n     eGPIO6     Bi-directional input/output pin, GPIO6, set output value of pin GPIO6
   * @n     eGPIO7     Bi-directional input/output pin, GPIO7, set output value of pin GPIO7
   * @n     eGPIOTotal Set values of all pins in GPIO group. When using this parameter, bit0~bit7 of parameter level are valid value, corresponding to output of GPIO0~GPIO7
   * @param level    Output level 
   * @n     HIGH or 1    Parameter level, bit0 in 8-bit data is valid, which indicates outputting high level
   * @n     LOW  or 0    Parameter level, bit0 in 8-bit data is valid, which indicates outputting low level
   * @n     0x00~0xFF    If parameter gpioPin is GPIOTotal, bit0-bit7 of parameter level are valid data, corresponding to GPIO0-GPIO7 pins respectively.
   */
  void  digitalWrite(eGPIOPin_t gpioPin, uint8_t level);

  /**
   * @fn digitalWrite
   * @brief Set the pin to output high and low level, or control to output or stop (interrupt) low level
   * @param gpoPin   eGPOPin_t enum variable member 
   * @n     eGPO0     General-purpose output pin, GPO0, set output value of pin GPO0 
   * @n     eGPO1     General-purpose output pin, GPO1, set output value of pin GPO1
   * @n     eGPO2     General-purpose output pin, GPO2, set output value of pin GPO2
   * @n     eGPO3     General-purpose output pin, GPO3, set output value of pin GPO3
   * @n     eGPO4     General-purpose output pin, GPO4, set output value of pin GPO4
   * @n     eGPO5     General-purpose output pin, GPO5, set output value of pin GPO5
   * @n     eGPO6     General-purpose output pin, GPO6, set output value of pin GPO6
   * @n     eGPO7     General-purpose output pin, GPO7, set output value of pin GPO7
   * @n     eGPO8     General-purpose output pin, GPO8, set output value of pin GPO8
   * @n     eGPO9     General-purpose output pin, GPO9, set output value of pin GPO9
   * @n     eGPO10    General-purpose output pin, GPO10, set output value of pin GPO10
   * @n     eGPO11    General-purpose output pin, GPO11, set output value of pin GPO11
   * @n     eGPO12    General-purpose output pin, GPO12, set output value of pin GPO12
   * @n     eGPO13    General-purpose output pin, GPO13, set output value of pin GPO13
   * @n     eGPO14    General-purpose output pin, GPO14, set output value of pin GPO14
   * @n     eGPO15    General-purpose output pin, GPO15, set output value of pin GPO15
   * @n     eGPOTotal Set all pins in GPO0~15. When using this parameter,bit0~bit7 of parameter 8bit data bit0~bit7 corresponds to output value of GPO0~GPO7 or GPO8~GPO15
   * @param level     Output level, output low level or stop 
   * @n     HIGH or 1    When GPO pin group is set to push-pull output mode, output high; for open-drain mode, output low level 
   * @n     LOW  or 0    When GPO pin group is set to push-pull output mode, output low, for open-drain mode, no signal output 
   * @n     0x00~0xFF    When gpoPin parameter is eGPOTotal, bit0~bit7 of level are all valid data, corresponding to pin GPO0~GPO7 or GPO8~GPO15
   */
  void  digitalWrite(eGPOPin_t gpoPin, uint8_t level);
  /**
   * @fn digitalWrite
   * @brief Set IO output value by group 
   * @param group   Group pin, ePinGroup_t enum variable member 
   * @n     eGPIO    GPIO pin 0~7, when setting this value, parameter level low 8bits are valid, bit0~bit7 correspond to output of pin GPIO0~GPIO7, indicating setting output value of pin 0~7 in GPIO group. 
   * @n     eGPO     GPO pin 0~15, when setting this value, parameter level 16bits are valid, bit0~bit15 correspond to output of pin GPO0~GPIO15, indicating setting output value of pin 0~15 in GPO group.
   * @n     eGPO0_7  GPO pin 0~7, when setting this value, parameter level low 8bits are valid, bit0~bit7 correspond to output of pin GPO0~GPO7, indicating setting output value of pin 0~7 in GPO group. 
   * @n     eGPO8_15 GPO pin 8~15, when setting this value, parameter level high 8bits are valid, bit8~bit15 correspond to output of pin GPO8~GPO15, indicating setting output value of pin 8~15 in GPO group 
   * @param level    16bit data or uGroupValue_t union value. Combining with group parameter to represent the pin value of a group. bit0~bit15 correspond to GPIO0~GPIO7(high 8bits invalid) or GPO0~GPO15
   * @n     0x0000~0xFFFF  16bits data, bit0~bit15 have different meanings according to the value of parameter group. 
   * @note uGroupValue_t union, user can operate this to configure pin values.
   */
  void  digitalWrite(ePinGroup_t group, uGroupValue_t level);
  void  digitalWrite(ePinGroup_t group, uint16_t level);

  /**
   * @fn digitalRead
   * @brief Read pin level value of GPIO group 
   * @param pin GPIO pin, eGPIOPin_t enum variable member 
   * @n     eGPIO0     Bi-directional input/output pin, GPIO0, read level status of pin GPIO0, 0 for low level, 1 for high level 
   * @n     eGPIO1     Bi-directional input/output pin, GPIO1, read level status of pin GPIO1, 0 for low level, 1 for high level
   * @n     eGPIO2     Bi-directional input/output pin, GPIO2, read level status of pin GPIO2, 0 for low level, 1 for high level
   * @n     eGPIO3     Bi-directional input/output pin, GPIO3, read level status of pin GPIO3, 0 for low level, 1 for high level
   * @n     eGPIO4     Bi-directional input/output pin, GPIO4, read level status of pin GPIO4, 0 for low level, 1 for high level
   * @n     eGPIO5     Bi-directional input/output pin, GPIO5, read level status of pin GPIO5, 0 for low level, 1 for high level
   * @n     eGPIO6     Bi-directional input/output pin, GPIO6, read level status of pin GPIO6, 0 for low level, 1 for high level
   * @n     eGPIO7     Bi-directional input/output pin, GPIO7, read level status of pin GPIO7, 0 for low level, 1 for high level
   * @n     eGPIOTotal Bi-directional input/output pin, GPIO group GPIO0~GPIO7, indicating read level status of all pins in GPIO group, returning of bit0~bit8 represents the level value of pin GPIO0~GPIO7 respectively
   * @return Level status value 
   */
  uint8_t digitalRead(eGPIOPin_t pin);
  
  /**
   * @fn attachInterrupt
   * @brief Set the external interrupt mode and interrupt service function of GPIO pins
   * @note Module's pin GPO15 is used to indicate whether an interrupt occurs on GPIO0-GPIO7 in interrupt mode, if an interrupt occurs on a pin, GPO15 will output a low level continuously, otherwise it will output a high level.
   * @n When an interrupt occurs on a pin, trigger continuously, if there is interrupt occurring on other pins, level of pin GPO15 keeps low without changing. 
   * @param gpioPin   Pin in GPIO group, eGPIOPin_t enum variable member 
   * @n     eGPIO0     Bi-directional input/output pin, GPIO0, set pin GPIO0 external interrupt mode and interrupt service function
   * @n     eGPIO1     Bi-directional input/output pin, GPIO1, set pin GPIO1 external interrupt mode and interrupt service function
   * @n     eGPIO2     Bi-directional input/output pin, GPIO2, set pin GPIO2 external interrupt mode and interrupt service function
   * @n     eGPIO3     Bi-directional input/output pin, GPIO3, set pin GPIO3 external interrupt mode and interrupt service function
   * @n     eGPIO4     Bi-directional input/output pin, GPIO4, set pin GPIO4 external interrupt mode and interrupt service function
   * @n     eGPIO5     Bi-directional input/output pin, GPIO5, set pin GPIO5 external interrupt mode and interrupt service function
   * @n     eGPIO6     Bi-directional input/output pin, GPIO6, set pin GPIO6 external interrupt mode and interrupt service function
   * @n     eGPIO7     Bi-directional input/output pin, GPIO7, set pin GPIO7 external interrupt mode and interrupt service function
   * @n     eGPIOTotal Set the values of all GPIO pins, which indicates setting GPIO0-GPIO7 to the same interrupt mode and interrupt service function 
   * @param mode    Interrupt mode 
   * @n     eLOW       Low level interrupt, initial level of the GPIO pin is high, if the GPIO pin detect a low level, pin GPO15 will output a low level signal
   * @n     eHIGH      High level interrupt, initial level of the GPIO pin is low, if the GPIO pin detect a high level, pin GPO15 will output a low level signal
   * @n     eRISING    Rising edge interrupt, initial level of the GPIO pin is low, if the GPIO pin detect a rising edge, pin GPO15 will output a high-to-low level signal (falling edge)
   * @n     eFALLING   Falling edge interrupt, initial level of the GPIO pin is high, if the GPIO pin detect a falling edge, pin GPO15 will output a high-to-low level signal (falling edge)
   * @n     eCHANGE    Double edge interrupt, initial level of the GPIO pin is high. Initial level switches once every time an interrupt occurs. If the GPIO pin detects falling edge or rising edge, pin GPO15 will output a high-to-low level signal(alling edge).
   * @note If the any pin level from GPIO0-GPIO7 changes, GPO15 will output a low level signal, and it will not output high level until the levels of all GPIO0-GPIO7 pins return to the initial level.
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
   * @note It can be waken up in two ways: 
   * @n 1. An external interrupt occurs on the GPIO pin
   * @n 2. Execute pin operation
   */
  void sleep();
  /**
   * @fn pinDescription
   * @brief  Describe GPIO pins
   * @param pin Pins in GPIO group, eGPIOPin_t enum variable member
   * @n     eGPIO0     Bi-directional input/output pin, GPIO0, get description of pin eGPIO0
   * @n     eGPIO1     Bi-directional input/output pin, GPIO1, get description of pin eGPIO1
   * @n     eGPIO2     Bi-directional input/output pin, GPIO2, get description of pin eGPIO2
   * @n     eGPIO3     Bi-directional input/output pin, GPIO3, get description of pin eGPIO3
   * @n     eGPIO4     Bi-directional input/output pin, GPIO4, get description of pin eGPIO4
   * @n     eGPIO5     Bi-directional input/output pin, GPIO5, get description of pin eGPIO5
   * @n     eGPIO6     Bi-directional input/output pin, GPIO6, get description of pin eGPIO6
   * @n     eGPIO7     Bi-directional input/output pin, GPIO7, get description of pin eGPIO7
   * @return Return pin description string
   * @n such as "GPIO0" "GPIO1" "GPIO2" "GPIO3" "GPIO4" "GPIO5" "GPIO6" "GPIO7"
   */
  String pinDescription(eGPIOPin_t pin);

  /**
   * @fn pinDescription
   * @brief Convert pin into string description 
   * @param pin  eGPOPin_t enum variable member 
   * @n     eGPO0     General-purpose output pin, GPO0, get description of pin eGPIO0
   * @n     eGPO1     General-purpose output pin, GPO1, get description of pin eGPIO1
   * @n     eGPO2     General-purpose output pin, GPO2, get description of pin eGPIO2
   * @n     eGPO3     General-purpose output pin, GPO3, get description of pin eGPIO3
   * @n     eGPO4     General-purpose output pin, GPO4, get description of pin eGPIO4
   * @n     eGPO5     General-purpose output pin, GPO5, get description of pin eGPIO5
   * @n     eGPO6     General-purpose output pin, GPO6, get description of pin eGPIO6
   * @n     eGPO7     General-purpose output pin, GPO7, get description of pin eGPIO7
   * @n     eGPO8     General-purpose output pin, GPO8, get description of pin eGPIO8
   * @n     eGPO9     General-purpose output pin, GPO9, get description of pin eGPIO9
   * @n     eGPO10    General-purpose output pin, GPO10, get description of pin eGPIO10
   * @n     eGPO11    General-purpose output pin, GPO11, get description of pin eGPIO11
   * @n     eGPO12    General-purpose output pin, GPO12, get description of pin eGPIO12
   * @n     eGPO13    General-purpose output pin, GPO13, get description of pin eGPIO13
   * @n     eGPO14    General-purpose output pin, GPO14, get description of pin eGPIO14
   * @n     eGPO15    General-purpose output pin, GPO15, get description of pin eGPIO15
   * @return Return pin description string
   * @n such as "GPO0" "GPO1" "GPO2"  "GPO3"  "GPO4"  "GPO5"  "GPO6"  "GPO7"
   * @n         "GPO8" "GPO9" "GPO10" "GPO11" "GPO12" "GPO13" "GPO14" "GPO15"
   */
  String pinDescription(eGPOPin_t pin);

protected:
  void setSystemArgs();
  uint8_t readGPIO();

private:
  typedef union{
    struct{
      uint8_t ioEn:1;  /**< Control three-state output of bi-directional input/output pin GPIO7～IO0, 0 for output disabled, 1 for output enabled*/
      uint8_t decL:1;  /**< Control dynamic scanning disable/enable of output pin GPO7～GPO0*/
      uint8_t decH:1;  /**< Control dynamic scanning disable/enable of output pin GPO15～GPO8*/
      uint8_t intEn:1;  /**< Enable/disable input level change interrupt, disable when it's 0, allow outputting level change interrupt from GPO15 pin when it's 1 and decH is 0 */
      uint8_t odEn:1;  /**< Enable open-drain output of output pin GPO15～GPO0, 0: GPO15～GPO0 is set to push-pull output(can output high/low level), 1: GPO15～GPO0 is set to open-drain output(only output low level or do not output)*/
      uint8_t intens:1;  /**< Control the brightness dynamic display driver*/
      uint8_t sleep:1;  /**< Let CH423 enter low-power sleep state*/
      uint8_t rsv:1;
    };
    uint8_t args;
  }uSystemCmdArgs_t;

  typedef struct {
    eGPIOPin_t pin;              /**< digital pin, range 0~7 */
    const char * description;    /**< Bi-directional input/output pin string description: GPIO0 ~ GPIO7*/
  } __attribute__ ((packed)) sGPIOPinDescription_t;

  typedef struct {
    eGPOPin_t pin;              /**< digital pin, range 0~15 */
    const char * description;   /**< General-purpose pin string description, GPO0~GPO15*/
  } __attribute__ ((packed)) sGPOPinDescription_t;

  typedef union{
    struct{
      uint8_t GPIO0:1; /**< state bit0 for level of pin GPIO0*/
      uint8_t GPIO1:1; /**< state bit1 for level of pin GPIO1*/
      uint8_t GPIO2:1; /**< state bit2 for level of pin GPIO2*/
      uint8_t GPIO3:1; /**< state bit3 for level of pin GPIO3*/
      uint8_t GPIO4:1; /**< state bit4 for level of pin GPIO4*/
      uint8_t GPIO5:1; /**< state bit5 for level of pin GPIO5*/
      uint8_t GPIO6:1; /**< state bit6 for level of pin GPIO6*/
      uint8_t GPIO7:1; /**< state bit7 for level of pin GPIO7*/
    };
    uint8_t state; /**<Bi-directional input/output pin group, GPIO0~GPIO7 pin status*/
  }uGPIOState_t;

  typedef struct {
    eInterruptMode_t mode;  /**< eInterruptMode_t enum variable, record interrupt mode of pin configuration */
    CH423_INT_CB cb;   /**< Function-type variable, record function pointer */
  } __attribute__ ((packed)) sModeCB_t;

  TwoWire *_pWire;
  uSystemCmdArgs_t _args;
  uint8_t _intValue;
  uint8_t _gpo0_7;
  uint8_t _gpo8_15;
  sModeCB_t _cbs[eGPIOTotal];
  static sGPOPinDescription_t _gpoPinDescriptions[eGPOTotal];
  static sGPIOPinDescription_t _gpioPinDescriptions[eGPIOTotal];
};
#endif
