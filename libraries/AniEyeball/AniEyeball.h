/*
  AniEyeball.h - Library for Openlab Taipei Animatronic Eveball.
  Created by Ryan Tseng, May 27, 2014.
  Released into the public domain.
*/

#ifndef AniEyeball_h
#define AniEyeball_h

#include <Arduino.h>
#include <Servo.h>

#define FT_M   0
#define FT_L1 -1  
#define FT_L2 -2
#define FT_R1  1
#define FT_R2  2  

class AniEyeball
{
  public:
    AniEyeball();
    AniEyeball(int pinServoP, int pinServoB);
    void setPinP(int pinP);
    void setPinB(int pinB);
    void setPPos(int msec);
    void setBPos(int msec);
    void setFaceTrackingFlag(int flag);
    void faceTrackingMoving(int n);
    void acting(int n);
    void mix();
    // Eyelid 眼皮動作 
    void aebPClose();
    void aebPOpen();
    void aebPUp2Third();
    void aebPDown2Third();
    // Eyeball 眼球動作
    void aebBLeft();
    void aebBMiddle();
    void aebBRight();
    // 組合動作
    void aebCResting();
    void aebCTurnLeftSlowly();
    void aebCTurnRightSlowly();
    void aebCLeftBlinking();    
    void aebCRightBlinking();    
    void aebCGuarding();
    void aebCSleepy();
  private:
    Servo _servoP;
    Servo _servoB;
    int _pinServoP;
    int _pinServoB;
    int _faceTrackingFlag; // 大眼仔相對 webcam 的位置
                           //   左 ---  中 --- 右   
                           //   -2, -1, 0,  1, 2
};

#endif

