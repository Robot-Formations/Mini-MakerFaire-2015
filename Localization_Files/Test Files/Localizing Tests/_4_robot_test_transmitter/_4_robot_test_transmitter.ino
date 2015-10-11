#include "math.h"
#define NumOfRobots 2
//#define distancesF[x] pingDist[x].f
const int pingPin[4] = {6, 7, 9, 10};
const char doneKey[4] = {'l', 'm', 'n', 'o'};
typedef union {
  byte bytes[4];
  float f;
} bytesToFloat;

struct Robot {
  bytesToFloat pingDist[4];
  float lowestDist;
  boolean increasing;
  float lastVal;
  int increasingT;
  int sameNum;
  boolean done = false;
};
Robot robots[4];

boolean readFloat(bytesToFloat *PingToChange = NULL) {
  if (PingToChange == NULL)
    return false;
  long startTime = millis();
  while (!Serial1.available())
  {
    if (millis() - startTime > 2000)
      return false;
  }
  int pingNum = Serial1.read();
  Serial.println(pingNum);
  for (int i = 0; i < 4; i++) {
    startTime = millis();
    while (!Serial1.available()) {
      //Serial.println(millis()-startTime);
      if (millis() - startTime > 2000)
        return false;
    }
    (*PingToChange).bytes[i] = Serial1.read();
  }
  return true;
}

void sendPing(int num, int pingNum) {
  delay(100);
  Serial.print("number: ");
  Serial.println(num);
  delay(600);

  //char numChar = 48 + num;
  Serial1.print('s');
  //constant delay time
  delayMicroseconds(9175);
  pinMode(pingNum, OUTPUT);
  digitalWrite(pingNum, LOW);
  delayMicroseconds(2);
  digitalWrite(pingNum, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingNum, LOW);
}

void getDistances(float distances[4][4])
{
  for (int i = 3; i >= 0; i--) {
    for (int r  =0; r < 1; r++){
    robots[r].lowestDist = 1000.0;
    robots[r].increasing = false;
    robots[r].lastVal = 1000.0 ;
    robots[r].increasingT = 0;
    robots[r].sameNum = 0;
    robots[r].done = false;
    }
    Serial1.println('S');
    do {
      sendPing(i + 1 , pingPin[i]);
      for (int r  =0; r < 1; r++){  
      if (readFloat(&robots[r].pingDist[i])) {
        Serial.println(robots[r].pingDist[i].f);
        Serial.println(robots[r].lowestDist);
        robots[r].increasingT = robots[r].pingDist[i].f - robots[r].lowestDist > 20 ? ++robots[r].increasingT : 0;
        Serial.println(robots[r].increasingT);
        if (robots[r].increasingT > 2)
          robots[r].increasing = true;
        if ((abs(robots[r].pingDist[i].f - robots[r].lastVal) < 5) && robots[r].pingDist[i].f < 260)
          robots[r].sameNum++;
        else {
          robots[r].sameNum = 0;
        }
        robots[r].lowestDist = robots[r].pingDist[i].f < robots[r].lowestDist || (robots[r].sameNum > 0) ? robots[r].pingDist[i].f : robots[r].lowestDist;
        robots[r].lastVal = robots[r].pingDist[i].f;
        if (robots[r].increasing || robots[r].sameNum > 7) {
          Serial1.println('l');
          robots[r].done = true;
          distances[r][i] = robots[r].lowestDist;
        }
      }
      }
    } while (!robots[0].done);
  }
}
void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);

  Serial.println("starting");
  delay(5000);
  float distances[4][4];
  getDistances(distances);
  Serial.println("distances");
  for (int r = 0; r < 1; r++) {
    for (int i = 0; i < 4; i++)
      Serial.println(distances[r][i]);
  }
  /*
    float Xaverage=0;
    float Yaverage=0;
    for (int i = 0; i < 4; i++)
    {
      int nextInt = i == 3 ? 0 : i + 1;
      float DistOne = distances[i]+6;
      float DistTwo = distances[nextInt]+6;
      float DistThree = 190;
      float S = (DistThree + DistOne + DistTwo) / 2.0;
      float Y = (sqrt(S * (S - DistThree) * (S - DistOne) * (S - DistTwo)) / DistThree)*2;
      float X = sqrt(sq(DistOne) - sq(Y));
      Serial.println(X);
      Serial.println(Y);
      if(!isnan(X) || !isnan(Y))
      {
        switch(i)
        {
          case 0:
            Yaverage+= X;
            Xaverage+=Y;
            break;
          case 1:
            Yaverage+=190- Y;
            Xaverage+=X;
            break;
          case 2:
            Yaverage+= 190-X;
            Xaverage+=190-Y;
            break;
          case 3:
            Yaverage+= Y;
            Xaverage+=190-X;
            break;
        }
      }
      Serial.print("Yaverage");
      Serial.println(Yaverage);
      Serial.print("Xaverage");
      Serial.println(Xaverage);
    }
    Xaverage /= 4;
    Yaverage /= 4;
    Serial.println(Xaverage);
    Serial.println(Yaverage);*/
}
/*for (int i = 0; i < 4; i++){
sendPing(i+1 , pingPin[i]);
readFloat(&pingDist[i]);
Serial.println(pingDist[i].f);
}*/
/*float leastDist = 360;
do{
sendPing(0+1 , pingPin[0]);
readFloat(&pingDist[0]);
leastDist= leastDist>pingDist[0].f?pingDist[0].f:leastDist;
Serial.println(pingDist[0].f);
}while(leastDist - pingDist[0].f > -200);
Serial.println(leastDist);
Serial.println("DONE");*/

void loop() {

  //   for(int i = 3; i >= 0; i--){
  //     float lowestDist= 1000.0;
  //     boolean increasing = false;
  //     int decreasingT = 0;
  //     boolean decreasing = false;
  //     do{
  //       sendPing(i+1 , pingPin[i]);
  //       readFloat(&pingDist[i]);
  //       Serial.println(pingDist[i].f);
  //       if (!increasing && lowestDist - pingDist[i].f > 20)
  //       {
  //        increasing = true;
  //        decreasingT = 0;
  //       }
  //       else
  //       {
  //         decreasingT++;
  //       }
  //       decreasing = !decreasiing && (pingDist[i].f - lowestDist > 20) && decreasingT > 2 ? true: decreasing;
  //       lowestDist = pingDist[i].f < lowestDist?pingDist[i].f:lowestDist;
  //     }while(pingDist[i].f - lowestDist < 20 || (increasing && !decreasing));
  //   }
  /*float DistOne= pingDist[0].f;
  float DistTwo = pingDist[1].f;
  DistOne = (DistOne + DistTwo < 200)?DistOne+((200-(DistOne+DistTwo))/2):DistOne;
  DistTwo = (DistOne + DistTwo < 200)?DistTwo+((200-(DistOne+DistTwo))/2):DistTwo;
  float S = (200.0 + DistOne + DistTwo)/2;
  float Y = (sqrt(S*(S-200.0)*(S-DistOne)*(S-DistTwo)))/100;
  float X = sqrt(sq(DistOne)/sq(Y));
  Serial.print("(");
  Serial.println(X);
  Serial.print(",");
  Serial.print(Y);
  Serial.print(")");
  delay(20000);
  */
}

