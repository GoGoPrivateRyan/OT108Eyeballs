//
// OT108Eyeballs_i2c_m.ino
// - Use I2C bus to control multiple devices
// - Master device
//

#include <Wire.h>
#include <Servo.h>
#include <AniEyeball.h>

#define N_SLAVE   2
#define N_EYEBALL 2

AniEyeball ot108eyeball[N_EYEBALL];

// 眼皮_PIN # i
// 眼球_PIN # i+1
const byte servoPin[N_EYEBALL*2] = {2,3,4,5};

// for command string handling (from serial port)
char inData[80];
byte index = 0;
byte count = 0;

#define NO_PREFIX       0
#define S_PREFIX        1
#define P_PREFIX        1
#define E_PREFIX        1

#define TOKEN_S         "s"
#define TOKEN_P         "p"
#define TOKEN_E         "e"

byte pfxS = NO_PREFIX;
byte pfxP = NO_PREFIX; 
byte pfxE = NO_PREFIX; 
byte sAll = 0; // all slaves mode
byte sNum = 0; // slave-#
byte pNum = 0; // pattern-#
byte eNum = 0; // eyeball-#
byte fFinish = 0;

void setup()
{
  Wire.begin();
  
  Serial.begin(9600);

  for (int i=0; i<N_EYEBALL; i++)
  {
  	ot108eyeball[i].setPinP(servoPin[i*2]);
  	ot108eyeball[i].setPinB(servoPin[i*2+1]);
  }
}

void loop()
{
  while (Serial.available() > 0)
  {
    char aChar = Serial.read();
    if (aChar == '\n')
    {
      // End of string detected. Time to parse
      char *p = inData; //assign the string to *p
      int counter = 0;  //initialise the counter
      String str;
            
      str = strtok(p, ",");
      while (str != NULL)
      {
        Serial.println(str);
        ParseCommand(str);
        counter++;
        str = strtok(NULL, ",");
      }

      index = 0;
      inData[index] = NULL;      
      Serial.write("done\n");
    }
    else
    {
      inData[index] = aChar;
      index++;
      inData[index] = '\0'; // Keep the string NULL terminated
    }
  }

  if (fFinish)
  {
    // 全部命令解析完成, 來操控 slave 吧
    if (sAll)
    {
      for (int i=0; i<N_SLAVE; i++)
      {   
        //
        // 暫且忽略 e 命令吧, 以後有空再補上
        //
        Wire.beginTransmission(i+1);
        Wire.write(pNum);
        Wire.endTransmission();        
      }
    }
    else
    {
      // 特定 salves
      Wire.beginTransmission(sNum);      
      Wire.write(pNum);   
      Wire.endTransmission();
    }

    // 不光是 slave 要做事, master 自己也要做啊.
    for (int i=0; i<N_EYEBALL; i++)
    {
      ot108eyeball[i].acting(pNum);
    } 
  
    resetFlags();    
  }

  // if no inputs from remote, how about letting master device's eyeballs moving 
  // tweeting();
}

void ParseCommand(String str)
{
  // the input commands formats:
  //   * s
  //      - all: all slaves
  //      - (num): slave-#(num)   
  //   * e
  //      - (num): eyeball-#(num)
  //   * p
  //      - (num): pattern-#(num)  
  //
  // the command example:
  //   s,all,p,1    -> all slaves, run pattern-1
  //   s,5,e,1,p,3  -> slave-5, eyeball-1, run pattern-3
  //   s,5,p,3      -> slave-5, all eyeballs, run pattern-3   

   if (str == TOKEN_S && pfxS == NO_PREFIX) // 開始 s 命令
   {
      pfxS = S_PREFIX; 
   }
   else if (str == "all" && pfxS == S_PREFIX) // s 命令, all
   {                               
      sAll = 1;
      pfxS = NO_PREFIX; // s 命令結束      
   }
   else if (pfxP == NO_PREFIX && pfxE == NO_PREFIX && pfxS == S_PREFIX) // 仍在解析 s 命令, 尚未走到 p 或 e 命令
   {
      sNum = str.toInt();
      pfxS = NO_PREFIX; // s 命令結束
   }
   else if (str == TOKEN_E && pfxE == NO_PREFIX && pfxP == NO_PREFIX && pfxS == NO_PREFIX) // 開始 e 命令
   {
      pfxE = E_PREFIX;
   }
   else if (str == TOKEN_P && pfxP == NO_PREFIX && pfxE == NO_PREFIX && pfxS == NO_PREFIX) // 開始 p 命令
   {
      pfxP = P_PREFIX;
   }
   else if (pfxE == E_PREFIX && pfxP == NO_PREFIX && pfxS == NO_PREFIX) // 解析 e 命令的參數
   {
     eNum = str.toInt();
     pfxE = NO_PREFIX; // e 命令結束
   }
   else if (pfxP == P_PREFIX && pfxE == NO_PREFIX && pfxS == NO_PREFIX) // 解析 p 命令的參數
   {
      pNum = str.toInt();
      pfxP = NO_PREFIX; // p 命令結束
      fFinish = 1;
   }
}

void resetFlags()
{
  pfxS = NO_PREFIX;
  pfxP = NO_PREFIX; 
  pfxE = NO_PREFIX; 
  sAll = 0;
  sNum = 0;
  pNum = 0;
  eNum = 0;
  fFinish = 0;
}
