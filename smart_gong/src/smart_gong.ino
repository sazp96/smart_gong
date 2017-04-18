/*
 * Project: smart_gong
 * Description: Alexa trigger gong
 * Author: Sergio Zamora
 * Date: Q2 2017
 */

#include "neopixel.h"

#define pinGong A0
#define pinServo D1
#define pinHandStrip D4
#define ledType WS2812B

Servo myServo;
int pos = 0;
Adafruit_NeoPixel handStrip(2, pinHandStrip, ledType);
int timeLastCelebration = 0; // seconds
int durationOfCelebration = 5; // seconds
int speed = 5; // milliseconds / degree
bool isInitialized = false;

void initialize () {
  handStrip.setPixelColor(1, 0, 0, 0);
  handStrip.setPixelColor(0, 0, 0, 0);
  handStrip.show();
  delay(500);

  myServo.write(5);
  isInitialized = true;
}

void setup() {
  Particle.publish("smart_gong","v1.0.0");
  Particle.variable("isIni", isInitialized);

  pinMode(pinGong, INPUT_PULLDOWN);
  pinMode(D7, OUTPUT);
  myServo.attach(pinServo);
  handStrip.begin();
  handStrip.show();
  initialize();
}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  if ((Time.now() - timeLastCelebration) < durationOfCelebration ) { // Celebrating
    for(pos = 5; pos < 180; pos += 1)
    {
      handStrip.setPixelColor(1, pos, 200 - pos, pos);
      handStrip.setPixelColor(0, 255 - pos, 100 + pos, 50 + pos);
      handStrip.show();
      myServo.write(pos);
      delay(speed);
    }
    for(pos = 175; pos>=1; pos-=1)
    {
      handStrip.setPixelColor(0, pos, 255 - pos, 255 - pos);
      handStrip.setPixelColor(1, 255 - pos, pos, pos);
      handStrip.show();
      myServo.write(pos);
      delay(speed);
    }
  } else if (digitalRead(pinGong) == LOW) { // Listiging for the Gong
    timeLastCelebration = Time.now();
    isInitialized = false;
    Particle.publish("smart_gong","Start");
    digitalWrite(D7, HIGH);
    delay (1000);
    digitalWrite(D7, LOW);
  } else if (!isInitialized) {
    Particle.publish("smart_gong","End");
    initialize();
  }
}
