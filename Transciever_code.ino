/*
* Arduino Wireless Communication Example
* Transmitter Code for Arduino Nano Every
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <NRFLite.h>
#include <RF24.h>

#define led 12        // LED pin (optional, for debugging if needed)
RF24 radio(7, 8);    // CE, CSN - adjust as needed for Nano Every

const byte addresses[][6] = {"00001", "00002"};
int targetAngle = 90; // Set target angle to send (adjust as needed)

void setup() {
  pinMode(led, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
  Serial.begin(9600);
  Serial.println("Transmitter started");
}

void loop() {
  delay(5);

  // Transmit target angle to receiver
  radio.stopListening();
  radio.write(&targetAngle, sizeof(targetAngle));

  delay(5);

  // Optionally, listen for feedback from receiver
  radio.startListening();
  if (radio.available()) {
    int actualAngle;
    radio.read(&actualAngle, sizeof(actualAngle));
    Serial.print("Received actual angle from receiver: ");
    Serial.println(actualAngle);
  }
}

