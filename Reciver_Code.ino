/*
* Arduino Wireless Communication Example
* Receiver Code for Arduino Nano Every
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define feedbackPin A1 // Feedback pin to read the actual servo angle

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
Servo myServo;

void setup() {
  myServo.attach(5); // Attach servo to pin 5
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}

void loop() {
  delay(5);
  
  // Listen for incoming data
  radio.startListening();
  if (radio.available()) {
    while (radio.available()) {
      int targetAngle = 0;
      radio.read(&targetAngle, sizeof(targetAngle));
      myServo.write(targetAngle); // Move servo to target angle
    }
    
    delay(5);
    radio.stopListening();
    
    // Read actual servo position
    int feedbackValue = analogRead(feedbackPin);               // Read feedback pin
    int actualAngle = map(feedbackValue, 0, 1023, 0, 180);     // Map to 0-180 degrees

    // Send actual servo position back to transmitter
    radio.write(&actualAngle, sizeof(actualAngle));
  }
}
