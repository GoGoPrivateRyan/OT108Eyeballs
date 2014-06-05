//
// OT108Eyeballs_i2c_s.ino
// - Use I2C bus to control multiple devices
// - Slave device
//

#include <Wire.h>
#include <Servo.h>
#include <AniEyeball.h>


#define N_EYEBALL 1

AniEyeball ot108eyeball[N_EYEBALL];

// 眼皮_PIN # i
// 眼球_PIN # i+1
const byte servoPin[N_EYEBALL*2] = {2,3};

const int SLAVE_ADDRESS = 2;
char inByte = 0;

void setup()
{
  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
  Wire.onReceive(receiveEvent); // register event
  
  Serial.begin(9600);

  for (int i=0; i<N_EYEBALL; i++)
  {
  	ot108eyeball[i].setPinP(servoPin[i*2]);
  	ot108eyeball[i].setPinB(servoPin[i*2+1]);
  }
}

void loop()
{
  // 可能要有緩衝, 以免灌進太多指令, 來不及處理
  if (inByte)
  {
    for (int i=0; i<N_EYEBALL; i++)
    {
      ot108eyeball[i].acting(inByte);
    }    
    inByte = 0;    
  }
  else
  {
    //ot108eyeball[i].tweeting();
  }

}

void receiveEvent(int howMany)
{
  // master 傳來的命令格式為一數值, 代表 pattern-#.
  while (Wire.available()) 
  {
    inByte = Wire.read();
  }
}
