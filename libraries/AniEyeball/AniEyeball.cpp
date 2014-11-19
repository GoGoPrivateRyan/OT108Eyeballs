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

void AniEyeball::setFaceTrackingFlag(int flag)
{
  // 大眼仔相對 webcam 的位置 
  //   左 ---  中 --- 右   
  //   -2, -1, 0,  1, 2  
  _faceTrackingFlag = flag;
}

void AniEyeball::faceTrackingMoving(int n)
{
  // 根據大眼仔的相對位置做轉動 
  //   (Move) -2, -1, 0, 1, 2
  // (Flag)
  //   -2     0,  1,  2,  2, 2
  //   -1    -1,  0,  1,  2, 2
  //    0    -2, -1,  0,  1, 2
  //    1    -2, -2, -1,  0, 1
  //    2    -2, -2, -2, -1, 0

  setPPos(1000);
  delay(15);
  
  switch (_faceTrackingFlag) // 大眼仔的相對位置
  {
    case FT_L2:
      switch (n) // 人臉位置
      {
        case FT_L2:
          setBPos(1500); // 眼珠轉中間
          delay(15);
          break;
        case FT_L1:
          setBPos(1750); // 眼珠轉右一
          delay(15);
          break;
        default:
          setBPos(2000); // 眼珠轉右二
          delay(15);        
          break;
      }
      break;
    case FT_L1:
      switch (n) // 人臉位置
      {
        case FT_L2:
          setBPos(1250); // 眼珠轉左一
          delay(15);
          break;
        case FT_L1:
          setBPos(1500); // 眼珠轉中
          delay(15);
          break;
        case FT_M:
          setBPos(1750); // 眼珠轉右一
          delay(15);        
          break;
        default:
          setBPos(2000); // 眼珠轉右二
          delay(15);        
          break;
      }    
      break;
    case FT_M:
      switch (n)
      {
        case FT_L2:
          setBPos(1000); // 眼珠轉左二
          delay(15);
          break;
        case FT_L1:
          setBPos(1250); // 眼珠轉左一
          delay(15);
          break;
        case FT_M:
          setBPos(1500); // 眼珠轉中
          delay(15);        
          break;
        case FT_R1:
          setBPos(1750); // 眼珠轉右一
          delay(15);                
          break;
        default:
          setBPos(2000); // 眼珠轉右二
          delay(15);        
          break;
      }        
      break;
    case FT_R1:
      switch (n)
      {
        case FT_M:
          setBPos(1250); // 眼珠轉左一
          delay(15);        
          break;
        case FT_R1:
          setBPos(1500); // 眼珠轉中
          delay(15);                
          break;
        case FT_R2:
          setBPos(1750); // 眼珠轉右一
          delay(15);        
          break;
        default:
          setBPos(1000); // 眼珠轉左二
          delay(15);        
          break;
      }            
      break;
    case FT_R2:
      switch (n)
      {
        case FT_R1:
          setBPos(1250); // 眼珠轉左一
          delay(15);                
          break;
        case FT_R2:
          setBPos(1500); // 眼珠轉中
          delay(15);        
          break;
        default:
          setBPos(1000); //眼珠轉左二 
          delay(15);        
          break;
      }                
      break;
    default:
      break;
  }
}

void AniEyeball::acting(int n)
{
  switch (n)
  {
    case 0:
      aebCSleepy();
      break;
    case 1:
      aebCTurnLeftSlowly();
      break;      
    case 2:
      aebCTurnRightSlowly();
      break;
    case 3:
      aebCLeftBlinking();
      break;
    case 4:
      aebCRightBlinking();
      break;
    case 5:
      aebCGuarding();
      break;
    case 6:
      mix();
      break;
    default:
      break;
  }
}

// Eyelid 眼皮動作
void AniEyeball::aebPClose()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  
  posP = 2000;
  _servoP.writeMicroseconds(posP); 
  delay(15);  

  _servoP.detach();  
}

void AniEyeball::aebPOpen()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  
  posP = 1000;
  _servoP.writeMicroseconds(posP); 
  delay(15);  

  _servoP.detach();  
}

void AniEyeball::aebPUp2Third()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  
  posP = 1300;
  _servoP.writeMicroseconds(posP); 
  delay(15);  

  _servoP.detach();  
}

void AniEyeball::aebPDown2Third()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  
  posP = 1700;
  _servoP.writeMicroseconds(posP); 
  delay(15);  

  _servoP.detach();  
}

// Eyeball 眼球動作
void AniEyeball::aebBLeft()
{
  if (!_servoB.attached()) _servoB.attach(_pinServoB);
  
  posB = 1000;
  _servoB.writeMicroseconds(posB); 
  delay(15);  

  _servoB.detach();  
}

void AniEyeball::aebBMiddle()
{
  if (!_servoB.attached()) _servoB.attach(_pinServoB);
  
  posB = 1500;
  _servoB.writeMicroseconds(posB); 
  delay(15);  

  _servoB.detach();  
}

void AniEyeball::aebBRight()
{
  if (!_servoB.attached()) _servoB.attach(_pinServoB);
  
  posB = 2000;
  _servoB.writeMicroseconds(posB); 
  delay(15);  

  _servoB.detach();  
}

// 組合動作
void AniEyeball::aebCResting()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = 2000;
  posB = 1500;

  _servoP.writeMicroseconds(posP);
  delay(150);
  _servoB.writeMicroseconds(posB);
  delay(100);

  _servoP.detach();
  _servoB.detach();  
}

void AniEyeball::aebCTurnLeftSlowly()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = 1000;
  posB = 1800;
  
  _servoP.writeMicroseconds(posP);  
  delay(100);
  _servoB.writeMicroseconds(posB);
  delay(1000);
  
  while (posB >= 1000)
  {
    _servoB.writeMicroseconds(posB);  
    posB -= 50;
    delay(100);
  } 

  delay(500);

  _servoP.detach();
  _servoB.detach();  
}

void AniEyeball::aebCTurnRightSlowly()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  posP = 1000;
  posB = 1200;
  
  _servoP.writeMicroseconds(posP); 
  delay(100); 
  _servoB.writeMicroseconds(posB);
  delay(1000);
  
  while (posB <= 2000)
  {
    _servoB.writeMicroseconds(posB);  
    posB += 50;
    delay(100);
  } 

  delay(500);

  _servoP.detach();
  _servoB.detach();  
}

void AniEyeball::aebCLeftBlinking()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  _servoP.writeMicroseconds(1000);
  delay(1000);

  _servoB.writeMicroseconds(1200);
  delay(500);

  for (int i=0; i<10; i++)
  {
    _servoP.writeMicroseconds(1500);
    delay(150);
    _servoP.writeMicroseconds(1000);
    delay(150);
  }

  delay(500);

  _servoP.detach();
  _servoB.detach();    
}

void AniEyeball::aebCRightBlinking()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  _servoP.writeMicroseconds(1000);
  delay(1000);

  _servoB.writeMicroseconds(1800);
  delay(500);

  for (int i=0; i<10; i++)
  {
    _servoP.writeMicroseconds(1500);
    delay(150);
    _servoP.writeMicroseconds(1000);
    delay(150);    
  }
  
  delay(500);

  _servoP.detach();
  _servoB.detach();      
}

void AniEyeball::aebCGuarding()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  // 眼皮, 閉
  _servoP.writeMicroseconds(2000);
  delay(1000);
 
  // 眼皮, 瞬開
  _servoP.writeMicroseconds(1000);
  delay(500);

  // 眼球, 左右反覆 2 遍
  for (int i=0; i<8; i++)
  {
    _servoB.writeMicroseconds(1000);
    delay(500);  
    _servoB.writeMicroseconds(2000);
    delay(500);
  }  

  delay(500);  

  _servoP.detach();
  _servoB.detach();        
}

void AniEyeball::aebCSleepy()
{
  if (!_servoP.attached()) _servoP.attach(_pinServoP);
  if (!_servoB.attached()) _servoB.attach(_pinServoB);

  _servoB.writeMicroseconds(1500);
  delay(1000);

  posP = 2000;

  for (int i=0; i<8; i++)
  {
    while (posP >= 1200)
    { 
      _servoP.writeMicroseconds(posP);
      posP -= 50;
      delay(100);
    }
    delay(2000);
    posP = 2000;
    _servoP.writeMicroseconds(posP);  
    delay(1000);
  }

  delay(500);

  _servoB.detach();      
  _servoP.detach();      
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
