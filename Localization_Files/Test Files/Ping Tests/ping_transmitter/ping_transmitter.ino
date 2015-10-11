const int ping2 = 7;

void setup() {
 Serial1.begin(9600);
 
  Serial1.print('S');
  //constant delay time
  delayMicroseconds(11390);
  pinMode(ping2, OUTPUT);
  digitalWrite(ping2, LOW);
  delayMicroseconds(2);
  digitalWrite(ping2, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping2, LOW);
}

void loop(){
  delay(10000);
}
