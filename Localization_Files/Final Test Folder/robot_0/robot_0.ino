#include <Wire.h>
#include <Zumo32U4.h>

const int ping1 = 11;
float duration;
boolean Turn = false;
Zumo32U4Motors motors;

typedef union{
   byte bytes[4];
   float f; 
}floatToBytes;

floatToBytes distance;
void setup() {
Serial1.begin(9600);
}

void loop() {  
  if (Serial1.available())
  {
    char key = (char)Serial1.read();
    Serial.print(key);
    if(key == 106)
    {
          Turn = true;
          ledRed(1);
          Serial.println("went in loop");
    }
    if(key== 'k')
          Turn = false;
    if(key> '0' && key< '5' && Turn)
    {
          ledRed(0);
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
          delay(70);
          motors.setSpeeds(0, 0);
          delay(50);
          Serial1.write(distance.bytes,4);
    } 
  }
}
