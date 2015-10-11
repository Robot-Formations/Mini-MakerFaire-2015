const int ping1 = 6;
const int ping2 = 7;
int distances[4];
void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);

  Serial.println("starting");
  delay(5000);
  Serial1.print('1');
  //constant delay time
  delayMicroseconds(9175);
  pinMode(ping1, OUTPUT);
  digitalWrite(ping1, LOW);
  delayMicroseconds(2);
  digitalWrite(ping1, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping1, LOW);
  
  while (!Serial1.available());
  Serial.print((int)Serial1.read());
  delay(5000);
  Serial.println("number 2");
  delay(1000);

  Serial1.print('2');
  //constant delay time
  delayMicroseconds(9175);
  pinMode(ping2, OUTPUT);
  digitalWrite(ping2, LOW);
  delayMicroseconds(2);
  digitalWrite(ping2, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping2, LOW);

  while (!Serial1.available());
  distance = (int)Serial1.read();
  
  Serial.println(distances[0]);
  delay(5000);
  Serial.println(distance);
  delay(5000);

}

void loop() {
}

