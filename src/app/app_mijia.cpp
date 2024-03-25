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
    digitalWrite(pin, LOW);
    delay(100);
    digitalWrite(pin, HIGH);
    delay(100);
}
void app_mijia_double(uint8_t pin)
{
    Serial.println("米家双击");
    digitalWrite(pin, LOW);
    delay(100);
    digitalWrite(pin, HIGH);
    delay(100);
    digitalWrite(pin, LOW);
    delay(100);
    digitalWrite(pin, HIGH);
    delay(100);
}
void app_mijia_long(uint8_t pin)
{
    Serial.println("米家长按");
    digitalWrite(pin, LOW);
    delay(2000);
    digitalWrite(pin, HIGH);
    delay(100);
}