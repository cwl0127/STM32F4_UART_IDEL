#include <Arduino.h>

#define OUT1 PD11
#define OUT2 PD12
#define BEEP PA6
#define DI1  PA3
#define DI2  PA4

void setup()
{
    Serial.begin(115200);
    pinMode(OUT1, OUTPUT);
    digitalWrite(OUT1, LOW);
    pinMode(OUT2, OUTPUT);
    digitalWrite(OUT2, LOW);
    pinMode(BEEP, OUTPUT);
    digitalWrite(BEEP, LOW);
    pinMode(DI1, INPUT);
    pinMode(DI2, INPUT);
}

void loop()
{
    if (digitalRead(DI1) == 0)
    {
        Serial.println("DI1");
    }
    if (digitalRead(DI2) == 0)
    {
        Serial.println("DI2");
    }
}