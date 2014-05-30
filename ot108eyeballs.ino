#include <Servo.h>
#include <AniEyeball.h>

#define N_EYEBALL 4

AniEyeball ot108eyeball[N_EYEBALL];

// 眼皮_PIN # i
// 眼球_PIN # i+1
//const byte servoPin[N_EYEBALL*2] = {2,3,4,5,6,7,8,9,10,11,12,13};
const byte servoPin[N_EYEBALL*2] = {2,3,4,5,6,7,8,9};

void setup()
{
  Serial.begin(9600);

  for (int i=0; i<N_EYEBALL; i++)
  {
  	ot108eyeball[i].setPinP(servoPin[i*2]);
  	ot108eyeball[i].setPinB(servoPin[i*2+1]);
  }
}

void loop()
{
  for (int i=0; i<N_EYEBALL; i++)
    ot108eyeball[i].setPPos(1000); // 眼皮全開
  delay(500);

  for (int pos=1000; pos<=2000; pos+=250)
  {
    for (int i=0; i<N_EYEBALL; i++)
    {
      ot108eyeball[i].setBPos(pos); // 眼球 左->右
      delay(150);
    }
  }
  delay(500);
    
  for (int pos=2000; pos>=1000; pos-=250)
  {
    for (int i=N_EYEBALL-1; i>=0; i--)
    {
      ot108eyeball[i].setBPos(pos); // 眼球 右->左
      delay(150);
    }
  }
  delay(500);

  for (int pos=1000; pos<=2000; pos+=500)
  {
    for (int i=0; i<N_EYEBALL; i++)
    {
      ot108eyeball[i].setBPos(pos); // 眼球 左->右
      delay(150);
    }
  }
  delay(500);
    
  for (int pos=2000; pos>=1000; pos-=500)
  {
    for (int i=N_EYEBALL-1; i>=0; i--)
    {
      ot108eyeball[i].setBPos(pos); // 眼球 右->左
      delay(150);
    }
  }
  delay(500);

  for (int i=0; i<N_EYEBALL; i++)
    ot108eyeball[i].setPPos(2000); // 眼皮全閉
  delay(500);
}
