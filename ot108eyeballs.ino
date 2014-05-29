#include <Servo.h>
#include <AniEyeball.h>

#define N_EYEBALL 3

AniEyeball ot108eyeball[N_EYEBALL];

// 眼皮_PIN # i
// 眼球_PIN # i+1
//const byte servoPin[N_EYEBALL*2] = {2,3,4,5,6,7,8,9,10,11,12,13};
const byte servoPin[N_EYEBALL*2] = {2,3,4,5,6,7};

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
  {
  	long randNumber = random(0, 7);
  	if (randNumber <= 1)
      ot108eyeball[i].acting(1);
    else if (randNumber <= 2)
      ot108eyeball[i].acting(2);
    else if (randNumber <= 3)
      ot108eyeball[i].acting(3);
    else if (randNumber <= 4)
      ot108eyeball[i].acting(4);
    else if (randNumber <= 5)
      ot108eyeball[i].acting(5);
    else if (randNumber <= 6)
      ot108eyeball[i].acting(6);
    else
      ot108eyeball[i].acting(7);
  }  
  delay(1000);
}
