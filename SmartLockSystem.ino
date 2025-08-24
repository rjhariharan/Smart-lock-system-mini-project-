#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

const int lockPin = 2;        
String Data;                  

String UnlockPassword = "1234";  // Unlock password
String LockPassword   = "0000";  // Lock password

void setup() {
  SerialBT.begin("ESP32-Bluetooth");
  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin, LOW); // Start locked
}

void loop() {
  if (SerialBT.available()) {
    Data = "";
    while (SerialBT.available()) {
      char c = SerialBT.read();
      Data += c;
      delay(10);
    }

    if (Data == UnlockPassword) {
      SerialBT.println("Access Granted");
      digitalWrite(lockPin, HIGH);   // Unlock
      unsigned long startTime = millis();

      // Wait up to 10 seconds for lock password
      while (millis() - startTime < 10000) {
        if (SerialBT.available()) {
          String temp = "";
          while (SerialBT.available()) {
            char c = SerialBT.read();
            temp += c;
            delay(10);
          }
          if (temp == LockPassword) {
            SerialBT.println("Locked Early");
            digitalWrite(lockPin, LOW);  // Lock immediately
            return;
          }
        }
      }
      // Auto lock if no lock password received
      SerialBT.println("Auto Locked");
      digitalWrite(lockPin, LOW);
    } 
    
    else {
      SerialBT.println("Wrong Password");
    }
  }
}