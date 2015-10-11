#include <ZumoMotors.h>
#define RobotNum 0
#define myDoneKey 'l'
const int ping1 = 7;
float duration;
boolean done = false;

ZumoMotors motors;

typedef union{
   byte bytes[4];
   float f; 
}floatToBytes;

floatToBytes distance;

void flushFloatBuffer()
{
  for (int i = 0; i < 4; i++) {
    while (!Serial.available());
    Serial.read();
  }
}
void flushBuffer()
{
 while(Serial.available())
 {
    Serial.read(); 
    delay(15);
 }
}
void setup() {
Serial.begin(9600);
}

void loop() {  
  if (Serial.available())
  {
    char key = (char)Serial.read();
    if (key =='s' && !done)
    {
          pinMode(ping1, OUTPUT);
          digitalWrite(ping1, LOW);
          delayMicroseconds(2);
          digitalWrite(ping1, HIGH);
          delayMicroseconds(5);
          digitalWrite(ping1, LOW); 
          
          pinMode(ping1, INPUT);
          duration = pulseIn(ping1, HIGH);
          duration /= 29;
          distance.f = duration;
          delay(200);
          motors.setLeftSpeed(-100);
          motors.setRightSpeed(100);
          //motors.setSpeeds(50,-50);
          delay(100);
          motors.setSpeeds(0, 0);
          Serial.write(RobotNum);
          Serial.write(distance.bytes,4);
    }
    else if(key == 'S')// starting new ping sending
      done = false;
    else if(key ==myDoneKey)// done key has been sent meaning dont have to turn anymore
        done = true;
    else if(key >= '0' && key <'5')
        flushFloatBuffer();
  }
}
