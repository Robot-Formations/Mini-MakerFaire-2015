//#define distancesF[x] pingDist[x].f
const int pingPin[4] = {6,7,9,10};

typedef union{
   byte bytes[4];
   float f; 
}bytesToFloat;

bytesToFloat pingDist[4];

void readFloat(bytesToFloat *PingToChange = NULL){
  if(PingToChange == NULL)
    return;
  for(int i = 0;i<4;i++){
    while(!Serial1.available());
    (*PingToChange).bytes[i] = Serial1.read();
  }  
}

void sendPing(int num,int pingNum){
  delay(5000);
  Serial.print("number: ");
  Serial.println(num);
  delay(1000);
  
  char numChar = 48+num;
  Serial1.print(numChar);
  //constant delay time
  delayMicroseconds(9175);
  pinMode(pingNum, OUTPUT);
  digitalWrite(pingNum, LOW);
  delayMicroseconds(2);
  digitalWrite(pingNum, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingNum, LOW);  
}

void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);

  Serial.println("starting");
 
  for (int i = 0; i < 4; i++){
  sendPing(i+1 , pingPin[i]);
  readFloat(&pingDist[i]);
  Serial.println(pingDist[i].f);
  }

}

void loop() {
 
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

