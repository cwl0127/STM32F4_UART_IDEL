#include <Arduino.h>
#include <HardwareSerial.h>
#include "ArduinoRS485.h"

// #define RS485_RE PG3
// #define RS485_RX_EN() do{delay(20); digitalWrite(RS485_RE, LOW); delay(20);}while(0)
// #define RS485_TX_EN() do{delay(20); digitalWrite(RS485_RE, HIGH); delay(20);}while(0)

RS485Class rs485(Serial6, PC6, PG3, PG3);

void setup()
{
  Serial.begin(9600);
  rs485.begin(9600);

  // enable transmission, can be disabled with: RS485.endTransmission();
  // rs485.beginTransmission();

  // enable reception, can be disabled with: RS485.noReceive();
  // rs485.receive();
}

void loop()
{
  rs485.receive();
  if (rs485.available())
  {
    String rcvBuf = rs485.readStringUntil('\n');
    Serial.write(rcvBuf.c_str());
  }
  // rs485.noReceive();

  if (Serial.available())
  {
    String rcvBuf = Serial.readStringUntil('\n');
    rs485.beginTransmission();
    rs485.write(rcvBuf.c_str());
    rs485.endTransmission();
  }
}
// void setup() {
//   //Serial.begin(9600);
//   pinMode(RS485_RE, OUTPUT);
//   RS485_RX_EN();
//   Serial6.begin(9600);
// }

// void loop() {
//   if(Serial6.available())
//   {
//     String rcvBuf = Serial6.readStringUntil('\n');
//     RS485_TX_EN();
//     Serial6.print(rcvBuf);
//     Serial6.println();
//     RS485_RX_EN();
//   }
// }