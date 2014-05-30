/*
  AniEyeball.cpp - Library for Openlab Taipei Animatronic Eveball.
  Created by Ryan Tseng, May 27, 2014.
  Released into the public domain.
*/

#include <Arduino.h>
#include "AniEyeball.h"

int posP, posB;

// 眼皮: 
//   - 1000: 全開
//   - 1500: 半開
//   - 2000: 閉眼
// 眼球:
//   - 1000: 左
//   - 1500: 中
//   - 2000: 右

AniEyeball::AniEyeball()
{
}

AniEyeball::AniEyeball(int pinP, int pinB)
{
  _pinServoP = pinP;
  _pinServoB = pinB;
  _servoP.attach(_pinServoP);
  _servoB.attach(_pinServoB);  
}

void AniEyeball::setPinP(int pinP)
{
  _pinServoP = pinP;
  _servoP.attach(_pinServoP);
}

void AniEyeball::setPinB(int pinB)
{
  _pinServoB = pinB;
  _servoB.attach(_pinServoB);
}

void AniEyeball::setPPos(int msec)
{
  if (msec > 2000) posP = 2000;
  if (msec < 1000) posP = 1000;
  posP = msec;
  
  if (!_servoP.attached()) _servoP.attach(_pinServoP);  
  _servoP.writeMicroseconds(posP);
  delay(15);      

  //_servoP.detach();
}

void AniEyeball::setBPos(int msec)
{
  if (msec > 2000) posB = 2000;
  if (msec < 1000) posB = 1000;
  posB = msec;
  
  if (!_servoB.attached()) _servoB.attach(_pinServoB);  
  _servoB.writeMicroseconds(posB);
  delay(15);      

  //_servoB.detach();
}

void AniEyeball::acting(int n)
{
  switch (n)
  {
    case 1:
      turnLeftSlowly();
      break;
    case 2:
      turnRightSlowly();
      break;
    case 3:
      turnLeftBlinking();
      break;
    case 4:
      turnRightBlinking();
      break;
    case 5:
      guarding();
      break;
    case 6:
      sleepy();
      break;
    case 7:
      mix();
      break;
    default:
      break;
  }
}

void AniEyeball::center()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = posB = 1500;

  _servoP.writeMicroseconds(posP);  
  _servoB.writeMicroseconds(posB);
  delay(15);      

  _servoP.detach();
  _servoB.detach();
}

void AniEyeball::turnLeftSlowly()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = 1000;
  posB = 1800;
  
  _servoP.writeMicroseconds(posP);  
  _servoB.writeMicroseconds(posB);
  delay(1000);
  
  while (posB >= 1000)
  {
    _servoB.writeMicroseconds(posB);  
    posB -= 200;
    delay(100);
  } 

//  delay(3000);

  _servoP.detach();
  _servoB.detach();  
}

void AniEyeball::turnRightSlowly()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = 1000;
  posB = 1200;
  
  _servoP.writeMicroseconds(posP);  
  _servoB.writeMicroseconds(posB);
  delay(1000);
  
  while (posB <= 2000)
  {
    _servoB.writeMicroseconds(posB);  
    posB += 200;
    delay(100);
  } 

//  delay(3000);

  _servoP.detach();
  _servoB.detach();  
}

void AniEyeball::turnLeftBlinking()
{
  // 眼皮: 
  //   - 閉 -> 全開, 瞬開
  // 眼球
  //   - 中 -> 左, 慢慢
  // 眼皮:
  //   - 全開 -> 閉, 瞬閉

  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  _servoP.writeMicroseconds(2000);
  delay(1000);

  _servoP.writeMicroseconds(1000);
  delay(1000);

  posB = 1700;
  _servoB.writeMicroseconds(posB);
  delay(500);

  while (posB > 1000)
  {
    posB -= 50;
    _servoB.writeMicroseconds(posB);
    delay(30);
  }
  delay(3000);

  _servoP.writeMicroseconds(2000);
//  delay(1000);

  _servoP.detach();
  _servoB.detach();    
}

void AniEyeball::turnRightBlinking()
{
  // 眼皮: 
  //   - 閉 -> 全開, 瞬開
  // 眼球
  //   - 中 -> 右, 慢慢
  // 眼皮:
  //   - 全開 -> 閉, 瞬閉

  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  _servoP.writeMicroseconds(2000);
  delay(1000);

  _servoP.writeMicroseconds(1000);
  delay(1000);

  posB = 1300;
  _servoB.writeMicroseconds(posB);
  delay(500);

  while (posB < 2000)
  {
    posB += 50;
    _servoB.writeMicroseconds(posB);
    delay(30);
  }
  delay(3000);

  _servoP.writeMicroseconds(2000);
//  delay(1000);

  _servoP.detach();
  _servoB.detach();      
}

void AniEyeball::guarding()
{
  // 眼皮: 
  //   - 閉 -> 全開, 瞬開
  // 眼球
  //   - 中+ -> 左 -> 右, 瞬轉
  // 眼皮:
  //   - 全開 -> -半閉, 瞬閉
  //   - 重覆 2 遍

  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  _servoP.writeMicroseconds(2000);
  _servoB.writeMicroseconds(1800);
  delay(1000);

  _servoP.writeMicroseconds(1000);
  _servoB.writeMicroseconds(1000);
  delay(500);

  _servoB.writeMicroseconds(2000);

  for (int i=0; i<2; i++)
  {
    _servoP.writeMicroseconds(1800);
    delay(500);  

    _servoP.writeMicroseconds(1000);
    delay(500);
  }  
//  delay(1000);  

  _servoP.detach();
  _servoB.detach();        
}

void AniEyeball::sleepy()
{
  // 眼皮: 
  //   - 閉 -> +半開, 慢慢張開
  //   - +半開 -> 閉, 瞬閉
  //   - 重覆 3 遍

  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = 2000;

  for (int i=0; i<3; i++)
  {
    while (posP >= 1200)
    { 
      _servoP.writeMicroseconds(posP);
      posP -= 50;
      delay(100);
    }
    delay(1000);
    posP = 2000;
    _servoP.writeMicroseconds(posP);  
    delay(1000);
  }
 // delay(1000);

  _servoP.detach();
  _servoB.detach();          
}

void AniEyeball::mix()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = 2000;
  posB = 1500;

  _servoP.writeMicroseconds(posP);
  _servoB.writeMicroseconds(posB);  
  delay(15);

  // 眼皮: 
  //   - 閉 -> 半開, 慢慢張開
  while (posP >= 1300)
  {
    _servoP.writeMicroseconds(posP);
    posP -= 50;
    delay(100);
  }

  // 眼球: 
  //   - 中 -> 右, 慢慢轉
  //   - 右 -> 左, 快轉
  //   - 左 -> 中, 慢慢轉
  while (posB <= 2000)
  {
    _servoB.writeMicroseconds(posB);
    posB += 50;
    delay(100);
  }  

  while (posB >= 1000)
  {
    _servoB.writeMicroseconds(posB);
    posB -= 150;
    delay(30);
  }  

  while (posB <= 1500)
  {
    _servoB.writeMicroseconds(posB);
    posB += 50;
    delay(100);
  } 

  // 眼皮: 
  //   - 半開 -> 全開, 慢慢張開
  //   - 全開 -> 閉, 快快閉
  //   - 閉 -> 全開, 快快開
  posP = 1700;
  while (posP >= 1000)
  {
    _servoP.writeMicroseconds(posP);
    posP -= 50;
    delay(100);
  }  

  while (posP <= 2000)
  {
    _servoP.writeMicroseconds(posP);
    posP += 150;
    delay(30);
  } 

  while (posP >= 1000)
  {
    _servoP.writeMicroseconds(posP);
    posP -= 150;
    delay(30);
  }   

 // delay(1000);

  _servoP.detach();
  _servoB.detach();            
}
