#include <Servo.h>
#include <AniEyeball.h>
 
#define N_EYEBALL 4
#define N_ACTION  6
#define N_DANCE   2
 
AniEyeball ot108eyeball[N_EYEBALL];
AniEyeball *ptrEyeball;
int nRand = 0;
 
// 眼皮_PIN # i
// 眼球_PIN # i+1
const byte servoPin[N_EYEBALL*2] = {2,3,4,5,6,7,8,9};
 
void setup()
{
  Serial.begin(9600);
 
  for (int i=0; i<N_EYEBALL; i++)
  {
    ptrEyeball = &ot108eyeball[i];

    ptrEyeball->setPinP(servoPin[i*2]);
    ptrEyeball->setPinB(servoPin[i*2+1]);

    ptrEyeball->aebCResting();
  }

  randomSeed(analogRead(0));
}
 
void dance(int team, int act)
{
  AniEyeball *ptrTmp1, *ptrTmp2;

  if (team == 0)
  {
    ptrTmp1 = &ot108eyeball[0];
    ptrTmp2 = &ot108eyeball[2];
  }
  else
  {
    ptrTmp1 = &ot108eyeball[1];
    ptrTmp2 = &ot108eyeball[3];
  }

  int nTimes = 30;
  switch (act)
  {    
    case 0: // 眼皮, 閉|開
      while (nTimes)
      {
        ptrTmp1->setPPos(2000);
        delay(100);
        ptrTmp2->setPPos(2000);
        delay(1000);

        ptrTmp1->setPPos(1000);
        delay(100);
        ptrTmp2->setPPos(1000);
        delay(1000);

        nTimes--;
      }
      break;
    case 1: // 眼球, 左|右
      ptrTmp1->setPPos(1000);
      delay(100);
      ptrTmp2->setPPos(1000);
      delay(100);

      while (nTimes)
      {
        ptrTmp1->setBPos(1000);
        delay(100);
        ptrTmp2->setBPos(1000);
        delay(1000);

        ptrTmp1->setBPos(2000);
        delay(100);
        ptrTmp2->setBPos(2000);
        delay(1000);

        nTimes--;        
      }
      break;
    default:
      break;
  }

} 

void loop()
{
/*  ptrEyeball = &ot108eyeball[0];
  ptrEyeball->aebCGuarding();
  delay(3000);
  ptrEyeball->aebCSleepy();
  delay(3000);
  ptrEyeball->aebCTurnLeftSlowly();
  delay(3000);
  ptrEyeball->aebCTurnRightSlowly();
  delay(3000);
  ptrEyeball->aebCLeftBlinking();
  delay(3000);
  ptrEyeball->aebCRightBlinking();
  delay(3000);*/

  // 1-2-3-4
  for (int i=0; i<N_EYEBALL; i++)
  {
    ptrEyeball = &ot108eyeball[i];
    nRand = (int) random(N_ACTION);

    ptrEyeball->acting(nRand);
    delay(1000*5);
  }
 
  // 4-3-2-1 
  for (int i=N_EYEBALL-1; i>=0; i--)
  {
    ptrEyeball = &ot108eyeball[i];
    nRand = (int) random(N_ACTION);

    ptrEyeball->acting(nRand);
    delay(1000*5);    
  }
     
  // (1,3) 
  nRand = (int) random(N_DANCE);
  dance(0, nRand);
  delay(1000*5);    

  // (2,4)
  nRand = (int) random(N_DANCE);
  dance(1, nRand);
  delay(1000*5);    

  // all, close eye
  for (int i=0; i<N_EYEBALL; i++)
  {
    ptrEyeball = &ot108eyeball[i];
    ptrEyeball->aebCResting();
    delay(100);
  }

  delay(1000*20);   
}
