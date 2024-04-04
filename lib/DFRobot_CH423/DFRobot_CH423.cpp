/*!
 * @file DFRobot_CH423.cpp
 * @brief This is a I2C to IO expansion board based on the general remote IO chip of CH432 two-wire serial interface. It features:
 * @n 1. 8 bi-directional input/output pin: GPIO0 ~ GPIO7;
 * @n 2. 16 GPO pin: GPO0~GPO15;
 * @n 3. Support inputting level change interrupt, if the level of the GPIO pin is different from the initial level, GPO15 will output a low level signal;
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2022-03-07
 * @https://github.com/DFRobot/DFRobot_CH423
 */

#include <Arduino.h>
#include "DFRobot_CH423.h"
#include <string.h>

//Define DBG, change 0 to 1 open the DBG, 1 to 0 to close.  
#if 0
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
#define DBG(...)
#endif

#define CH423_CMD_SET_SYSTEM_ARGS   (0x48 >> 1) ///< Set system parameter command
#define CH423_CMD_SET_GPO_L         (0x44 >> 1) ///< Set low 8-bit GPO command
#define CH423_CMD_SET_GPO_H         (0x46 >> 1) ///< Set high 8-bit GPO command
#define CH423_CMD_SET_GPIO           0x30       ///< Set bi-directional input/output pin command
#define CH423_CMD_READ_GPIO         (0x4D >> 1) ///< Set bi-directional input/output pin command



DFRobot_CH423::sGPOPinDescription_t DFRobot_CH423::_gpoPinDescriptions[eGPOTotal] = 
{
  {eGPO0, "GPO0"},
  {eGPO1, "GPO1"},
  {eGPO2, "GPO2"},
  {eGPO3, "GPO3"},
  {eGPO4, "GPO4"},
  {eGPO5, "GPO5"},
  {eGPO6, "GPO6"},
  {eGPO7, "GPO7"},
  {eGPO8, "GPO8"},
  {eGPO9, "GPO9"},
  {eGPO10,"GPO10"},
  {eGPO11,"GPO11"},
  {eGPO12,"GPO12"},
  {eGPO13,"GPO13"},
  {eGPO14,"GPO14"},
  {eGPO15,"GPO15"}
};
DFRobot_CH423::sGPIOPinDescription_t DFRobot_CH423::_gpioPinDescriptions[eGPIOTotal] = {
  {eGPIO0, "GPIO0"},
  {eGPIO1, "GPIO1"},
  {eGPIO2, "GPIO2"},
  {eGPIO3, "GPIO3"},
  {eGPIO4, "GPIO4"},
  {eGPIO5, "GPIO5"},
  {eGPIO6, "GPIO6"},
  {eGPIO7, "GPIO7"},
};

DFRobot_CH423::DFRobot_CH423(TwoWire &wire)
{
  _pWire = &wire;
  _args.args = 0;
  memset(_cbs, 0, sizeof(_cbs));
  _intValue = 0;
  _gpo0_7 = 0;
  _gpo8_15 = 0;
}

int DFRobot_CH423::begin(eMode_t gpio, eMode_t gpo){
  if(_pWire == NULL){
    DBG("_pWire is null");
    return 1;
  }
  _pWire->begin();
  _args.args = 0;
  if(gpio < eOPEN_DRAIN){
    _args.ioEn = (gpio == eINPUT) ? 0 : 1;
  }
  if((gpo > eOUTPUT) && (gpo <= ePUSH_PULL)){
    _args.odEn = (gpo == eOPEN_DRAIN) ? 1 : 0;
  }
  setSystemArgs();
  _intValue = 0xFF;
  return 0;
}


void DFRobot_CH423::pinMode(ePinGroup_t group, eMode_t mode){
  if((group == eGPIO) && (mode < eOPEN_DRAIN)){
    _args.ioEn = (mode == eINPUT) ? 0 : 1;
    setSystemArgs();
  }else if((group > eGPIO) && (mode > eOUTPUT)){
    _args.odEn = (mode == eOPEN_DRAIN) ? 1 : 0;
    setSystemArgs();
  }
}

void DFRobot_CH423::pinMode(int group, int mode){
  if(group < 0 || group > (int)eGPO) return;
  if(mode < 0 || mode > (int)ePUSH_PULL) return;
  pinMode((ePinGroup_t)group, (eMode_t)mode);
}

void  DFRobot_CH423::digitalWrite(eGPIOPin_t gpioPin, uint8_t level){
  if(gpioPin < eGPIO0 || gpioPin > eGPIOTotal){
    DBG("pin out of range.");
    return;
  } 
  if(gpioPin == eGPIOTotal){
    _pWire->beginTransmission(CH423_CMD_SET_GPIO);
    _pWire->write(level);
    _pWire->endTransmission();
    return;
  }
  uGPIOState_t state;
  state.state = readGPIO();
  DBG(state.state, HEX);
  uint8_t cmd = CH423_CMD_SET_GPIO;// | (gpioPin & 0x0F);
  uint8_t cmd2 = gpioPin & 0x07;
  cmd2 = level ? (state.state | (1 << cmd2)) : (state.state & (~(1 << cmd2)));
  DBG(cmd, HEX);
  DBG(cmd2, HEX);
  _pWire->beginTransmission(cmd);
  _pWire->write(cmd2);
  _pWire->endTransmission();
}

void  DFRobot_CH423::digitalWrite(eGPOPin_t gpoPin, uint8_t level){
  if(gpoPin < eGPO0 || gpoPin > eGPOTotal) {
    DBG("pin out of range.");
    return;
  } 
  if(gpoPin == eGPOTotal){
    _gpo8_15 = level;
    _gpo0_7  = level;
    _pWire->beginTransmission(CH423_CMD_SET_GPO_H);
    _pWire->write(_gpo8_15);
    _pWire->endTransmission();
    _pWire->beginTransmission(CH423_CMD_SET_GPO_L);
    _pWire->write(_gpo0_7);
    _pWire->endTransmission();
    return;
  }
  if(gpoPin > eGPO7){
    _gpo8_15 = level ? (_gpo8_15 | (1 << (gpoPin - 8))) : (_gpo8_15 & (~(1 << (gpoPin - 8))));
    _pWire->beginTransmission(CH423_CMD_SET_GPO_H);
    _pWire->write(_gpo8_15);
    _pWire->endTransmission();
  }else{
    _gpo0_7 = level ? (_gpo0_7 | (1 << gpoPin)) : (_gpo0_7 & (~(1 << gpoPin)));
    _pWire->beginTransmission(CH423_CMD_SET_GPO_L);
    _pWire->write(_gpo0_7);
    _pWire->endTransmission();
  }
}

void  DFRobot_CH423::digitalWrite(ePinGroup_t group, uGroupValue_t level){
  switch(group){
    case eGPIO:
         _pWire->beginTransmission(CH423_CMD_SET_GPIO);
         _pWire->write(level.GPIO);
         _pWire->endTransmission();
         break;
    case eGPO:
         _gpo8_15 = level.GPO8_15;
         _gpo0_7  = level.GPO0_7;
         _pWire->beginTransmission(CH423_CMD_SET_GPO_H);
         _pWire->write(_gpo8_15);
         _pWire->endTransmission();
         _pWire->beginTransmission(CH423_CMD_SET_GPO_L);
         _pWire->write(_gpo0_7);
         _pWire->endTransmission();
         break;
    case eGPO0_7:
         _gpo0_7  = level.GPO0_7;
         _pWire->beginTransmission(CH423_CMD_SET_GPO_L);
         _pWire->write(_gpo0_7);
         _pWire->endTransmission();
         DBG(_gpo0_7, HEX);
         break;
    case eGPO8_15:
         _gpo8_15  = level.GPO8_15;
         _pWire->beginTransmission(CH423_CMD_SET_GPO_H);
         _pWire->write(_gpo8_15);
         _pWire->endTransmission();
         DBG(_gpo8_15, HEX);
         break;
  }
}
void  DFRobot_CH423::digitalWrite(ePinGroup_t group, uint16_t level){
  uGroupValue_t value;
  value.GPO0_15 = level;
  digitalWrite(group, value);
}

uint8_t DFRobot_CH423::digitalRead(eGPIOPin_t pin){
  if(pin < eGPIO0 || pin > eGPIOTotal) return -1;
  uGPIOState_t state;
  state.state = readGPIO();
  if(pin == eGPIOTotal) return state.state;
  return ((state.state >> pin) & 0x01);
}

void DFRobot_CH423::attachInterrupt(eGPIOPin_t gpioPin, eInterruptMode_t mode, CH423_INT_CB fun){
  if(gpioPin < eGPIO0 || gpioPin > eGPIOTotal) return;
  uint8_t bit;
  switch(mode){
    case eHIGH:
    case eRISING:
         bit = 0;
         break;
    case eLOW:
    case eFALLING:
    case eCHANGE:
         bit = 1;
         break;
  }
  
  if(gpioPin == eGPIOTotal){
    _intValue = bit ? 0xFF: 0x00;
    for(int i = 0; i < 8; i++){
      _cbs[i].mode = mode;
      _cbs[i].cb = fun;
    }
    return;
  }
  _intValue = bit ? (_intValue | (1 << (gpioPin & 0x07))): (_intValue & (~(1 << (gpioPin & 0x07))));
  _cbs[gpioPin].mode = mode;
  _cbs[gpioPin].cb = fun;
}

void DFRobot_CH423::enableInterrupt(){
  _args.intEn = 1;
  _args.decH = 0;
  DBG(_args.args, HEX);
  setSystemArgs();
  DBG(_intValue,HEX);
  digitalWrite(eGPIOTotal, _intValue);
}
void DFRobot_CH423::disableInterrupt(){
  _args.intEn = 0;
  setSystemArgs();
}
void DFRobot_CH423::pollInterrupts(){ 
  //disableInterrupt();
  uGPIOState_t state;
  state.state = 0;
  state.state = readGPIO();
  
  //digitalWrite(eGPIOTotal, state.state);//Clear the interrupt
  bool flag = false;
  DBG(_intValue, HEX);
  uint8_t temp = _intValue;
  DBG(state.state, HEX);

  for(int i = 0; i < 8; i++){
    uint8_t bit = (state.state >> i) & 1;
    uint8_t bit1 = (_intValue >> i) & 1;
    
    if(_cbs[i].cb && (bit != bit1)){
      if((((_cbs[i].mode == eHIGH) || (_cbs[i].mode == eRISING )) && (bit != 1)) || (((_cbs[i].mode == eLOW) || (_cbs[i].mode == eFALLING)) && (bit != 0))){
        continue;
      }
      if(_cbs[i].mode == eCHANGE){
        temp = bit ? (_intValue | (1 << i)): (_intValue & (~(1 << i)));
        flag = true;
      }
      _cbs[i].cb(i);
    }
  }
  //digitalWrite(eGPIOTotal, _intValue);//Disable the interrupt
  DBG(temp, HEX);
  //Serial.println();
  _intValue = temp;
  if(flag) digitalWrite(eGPIOTotal, _intValue);
}

void DFRobot_CH423::sleep(){
  _args.sleep = 1;
  setSystemArgs();
  _args.sleep = 0;
}

String DFRobot_CH423::pinDescription(eGPIOPin_t pin){
  for(int i=0; i< (int)(sizeof(_gpioPinDescriptions)/sizeof(_gpioPinDescriptions[i])); i++){
    if(pin == _gpioPinDescriptions[i].pin){
      return _gpioPinDescriptions[i].description;
    }
  }
  return "";
}


String DFRobot_CH423::pinDescription(eGPOPin_t pin){
  for(int i=0; i<(int)(sizeof(_gpoPinDescriptions)/sizeof(_gpoPinDescriptions[i])); i++){
    if(pin == _gpoPinDescriptions[i].pin){
      return _gpoPinDescriptions[i].description;
    }
  }
  return "";
}

void DFRobot_CH423::setSystemArgs(){
  _pWire->beginTransmission(CH423_CMD_SET_SYSTEM_ARGS);
  _pWire->write(_args.args);
  _pWire->endTransmission();
  
}

uint8_t DFRobot_CH423::readGPIO(){
  uint8_t val = 0;
  _pWire->requestFrom(CH423_CMD_READ_GPIO, 1);
  val = _pWire->read();
  return val;
}
