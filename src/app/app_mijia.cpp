#include "app_mijia.h"

void app_mijia_control(uint8_t pin, uint8_t type)
{
    if (type == 1)
    {
        app_mijia_short(pin);
    }
    else if (type == 2)
    {
        app_mijia_double(pin);
    }
    else if (type == 3)
    {
        app_mijia_long(pin);
    }
}

void app_mijia_short(uint8_t pin)
{
    Serial.println("米家单击");
    // digitalWrite(pin, LOW);
    // delay(100);
    // digitalWrite(pin, HIGH);
    // delay(100);
#ifdef SUPPORT_PCF8574
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    Serial.println(~(1 << pin), BIN);
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
#endif
#ifdef SUPPORT_CH423S
    DFRobot_CH423::uGroupValue_t gpo;

  gpo.GPO0  = 0; //Set pin GPO0 outputting 0
  gpo.GPO1  = 0; //Set pin GPO1 outputting 0
  gpo.GPO2  = 0; //Set pin GPO2 outputting 0
  gpo.GPO3  = 0; //Set pin GPO3 outputting 0
  gpo.GPO4  = 1; //Set pin GPO4 outputting 1
  gpo.GPO5  = 1; //Set pin GPO5 outputting 1
  gpo.GPO6  = 1; //Set pin GPO6 outputting 1
  gpo.GPO7  = 1; //Set pin GPO7 outputting 1
  gpo.GPO8  = 0; //Set pin GPO0 outputting 1
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

  //ch423.digitalWrite(ch423.eGPO0_7, 0xF0);          //only set GPO0~GPO7 outputting 0xF0
  //ch423.digitalWrite(ch423.eGPO8_15, (0x55 << 8));  //only set GPO8~GPO15 outputting 0x55
  ch423.digitalWrite(ch423.eGPO, gpo);        //set pin GPO15~GPO0 outputting 0x55F0
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
void app_mijia_double(uint8_t pin)
{
    Serial.println("米家双击");
    // digitalWrite(pin, LOW);
    // delay(100);
    // digitalWrite(pin, HIGH);
    // delay(100);
    // digitalWrite(pin, LOW);
    // delay(100);
    // digitalWrite(pin, HIGH);
    // delay(100);
#ifdef SUPPORT_PCF8574
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    Serial.println(~(1 << pin), BIN);
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    delay(100);
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
#endif
}
void app_mijia_long(uint8_t pin)
{
    Serial.println("米家长按");
    // digitalWrite(pin, LOW);
    // uint16_t pressStartTime = (uint16_t)millis(); // 记录按下操作的开始时间，并转换为uint16_t
    // while ((uint16_t)millis() - pressStartTime < 2000) {}
    // digitalWrite(pin, HIGH);
    // delay(100);
#ifdef SUPPORT_PCF8574
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(~(1 << pin));
    Wire.endTransmission();
    Serial.println(~(1 << pin), BIN);
    uint16_t pressStartTime = (uint16_t)millis(); // 记录按下操作的开始时间，并转换为uint16_t
    while ((uint16_t)millis() - pressStartTime < 2000)
    {
    }
    Wire.beginTransmission(PCF8574_ADDRESS);
    Wire.write(0xff);
    Wire.endTransmission();
#endif
}

uint8_t app_mijia_get(uint8_t pin)
{
    uint8_t value;
    Wire.requestFrom(PCF8574_ADDRESS, 1); // 请求从PCF8574读取一个字节
    if (Wire.available())
    {
        value = Wire.read(); // 读取引脚状态
    }
    uint8_t state = (value & (1 << pin)) >> pin;
    Serial.printf("pin%d:%d\n", pin, state);
    return state;
}