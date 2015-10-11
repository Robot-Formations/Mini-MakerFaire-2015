#include <ZumoMotors.h>
#define myDoneKey 'l'
const int ping1 = 7;
float duration;

ZumoMotors motors;

typedef union{
   byte bytes[4];
   float f; 
}floatToBytes;

bool done=false;
floatToBytes distance;
void setup() {
Serial.begin(9600);
}

void loop() {  
  if (Serial.available())
  {
    char key = (char)Serial.read();
    if(key == 's' && !done)
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
          delay(50);
          motors.setLeftSpeed(-100);
          motors.setRightSpeed(100);
          //motors.setSpeeds(50,-50);
          delay(150);
          motors.setSpeeds(0, 0);
          delay(50);
          Serial.write(1);
          Serial.write(distance.bytes,4);
    }
    else if (key == 'l')
      done = true;
    else if (key == 'S')
      done = false;
  }
}
