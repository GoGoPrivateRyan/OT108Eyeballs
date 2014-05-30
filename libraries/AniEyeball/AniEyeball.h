/*
  AniEyeball.h - Library for Openlab Taipei Animatronic Eveball.
  Created by Ryan Tseng, May 27, 2014.
  Released into the public domain.
*/

#ifndef AniEyeball_h
#define AniEyeball_h

#include <Arduino.h>
#include <Servo.h>

class AniEyeball
{
  public:
    AniEyeball();
    AniEyeball(int pinServoP, int pinServoB);
    void setPinP(int pinP);
    void setPinB(int pinB);
    void setPPos(int msec);
    void setBPos(int msec);
    void acting(int n);
    void mix();
    void center();
    void turnLeftSlowly();
    void turnRightSlowly();
    void turnLeftBlinking();    
    void turnRightBlinking();    
    void guarding();
    void sleepy();
  private:
    Servo _servoP;
    Servo _servoB;
    int _pinServoP;
    int _pinServoB;
};

#endif

