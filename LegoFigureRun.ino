/*
 * LegoFigureRun.ino
 * 
 * Used to find the limits for servos when attached to a device.
 * 
 * Scott Gillis
 * 2021 August 22
 *
 */

#include <Arduino.h>

#include "ServoEasing.h"
#define VERSION_EXAMPLE "1.0"

const int HEAD_PIN = 0; 
const int HEAD_MAX = 170;
const int HEAD_MIN = 20;
const int HEAD_MID = 90;

const int RLEG_PIN = 1;
const int RLEG_MAX = 125;
const int RLEG_MIN = 40;
const int RLEG_MID = 100;

const int LLEG_PIN = 2;
const int LLEG_MAX = 125;
const int LLEG_MIN = 80;
const int LLEG_MID = 105;

const int RARM_PIN = 3;
const int RARM_MAX = 190;
const int RARM_MIN = 25;
const int RARM_MID = 75;

const int LARM_PIN = 4;
const int LARM_MAX = 165;
const int LARM_MIN = 30;
const int LARM_MID = 95;

const int button = D5; // digital pin for button
const int buttonLED = D3; // LED on button

/*
 * Constructor to specify the expander address (needed at least if you use more than one expander board)
 * and to specify the I2C implementation library.
 * This can be done for each servo separately, but you can not (yet) mix the 3 different
 * Servo implementation libraries (Arduino Servo, Lightweight Servo and I2C Expansion Board)
 */
ServoEasing servoHead(0x7F, &Wire);
ServoEasing servoRLeg(0x7F, &Wire);
ServoEasing servoLLeg(0x7F, &Wire);
ServoEasing servoRArm(0x7F, &Wire);
ServoEasing servoLArm(0x7F, &Wire);

void setup() {

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(buttonLED, OUTPUT);
    pinMode(button, INPUT);

    Serial.begin(115200);
    delay(500); // important to make sure serial gets started before proceeding
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ "\r\nVersion " VERSION_EXAMPLE " from " __DATE__));

    // Attach servo to pin
    Serial.println(F("Attach servos"));
    servoHead.attach(HEAD_PIN);
    servoRLeg.attach(RLEG_PIN);
    servoLLeg.attach(LLEG_PIN);
    servoRArm.attach(RARM_PIN);
    servoLArm.attach(LARM_PIN);
    Serial.println("Attached servos");

    resetPos();
}

void blinkLED() {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}

// put the figure in resting position
void restingPos() {
  servoHead.startEaseTo(HEAD_MID, 30);
  servoRLeg.startEaseTo(RLEG_MID, 30);
  servoLLeg.startEaseTo(LLEG_MID, 30);
  servoRArm.startEaseTo(RARM_MID, 30);
  servoLArm.easeTo(LARM_MID, 30);
  delay(500); // just to make sure they all made it
}

// put the figure in resting position
void resetPos() {
  servoHead.write(HEAD_MID);
  servoRLeg.write(RLEG_MID);
  servoLLeg.write(LLEG_MID);
  servoRArm.write(RARM_MID);
  servoLArm.write(LARM_MID);
  delay(500); // just to make sure they all made it
}


void waveRight() {
  restingPos(); // start from known position
  // wave right arm
  servoRArm.setEasingType(EASE_CUBIC_IN_OUT);
  servoRArm.startEaseToD(RARM_MAX, 500);
  while (servoRArm.isMovingAndCallYield()) {
      ; // no delays here to avoid break between forth and back movement
  }
  
  for(int ii = 0; ii < 2; ii++) {
    servoRArm.startEaseToD(RARM_MIN, 1000);
    while (servoRArm.isMovingAndCallYield()) {
        ; // no delays here to avoid break between forth and back movement
    }
    servoRArm.startEaseToD(RARM_MAX, 1000);
    while (servoRArm.isMovingAndCallYield()) {
        ; // no delays here to avoid break between forth and back movement
    }
  }
  servoRArm.startEaseToD(RARM_MID, 500);
  while (servoRArm.isMovingAndCallYield()) {
      ; // no delays here to avoid break between forth and back movement
  }
  servoRArm.setEasingType(EASE_LINEAR);
  restingPos();
}

void waveLeft() {
  restingPos(); // start from known position
  // wave right arm
  servoLArm.setEasingType(EASE_CUBIC_IN_OUT);
  servoLArm.startEaseToD(RARM_MAX, 500);
  while (servoLArm.isMovingAndCallYield()) {
      ; // no delays here to avoid break between forth and back movement
  }
  
  for(int ii = 0; ii < 2; ii++) {
    servoLArm.startEaseToD(RARM_MIN, 1000);
    while (servoLArm.isMovingAndCallYield()) {
        ; // no delays here to avoid break between forth and back movement
    }
    servoLArm.startEaseToD(RARM_MAX, 1000);
    while (servoLArm.isMovingAndCallYield()) {
        ; // no delays here to avoid break between forth and back movement
    }
  }
  servoLArm.startEaseToD(RARM_MID, 500);
  while (servoLArm.isMovingAndCallYield()) {
      ; // no delays here to avoid break between forth and back movement
  }
  servoLArm.setEasingType(EASE_LINEAR);
  restingPos();
}

void lookAround() {
  servoHead.easeTo(HEAD_MIN, 60);
  servoHead.easeTo(HEAD_MAX, 60);
  servoHead.easeTo(HEAD_MID, 60);
}

void bow() {
  servoLArm.startEaseTo(RARM_MAX, 30);
  servoRArm.startEaseTo(LARM_MIN, 30);
  servoLLeg.startEaseTo(LLEG_MAX, 15);
  servoRLeg.easeTo(RLEG_MIN, 15);
  delay(1000);
  restingPos();
}

void dance() {
  waveRight();
  waveLeft();
  lookAround();
  bow();
}


void loop() {

    if (Serial.available() > 0) {
      // get incoming byte:
      int inByte = Serial.read();
      switch(inByte) {
        case 'r': restingPos(); break;
        case 'd': dance(); break;
        default: break;
      }
    }
    if(!digitalRead(button)) {
      digitalWrite(buttonLED, HIGH);
      dance();
      digitalWrite(buttonLED, LOW);
    }
}
