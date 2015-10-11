const int ping2 = 10;
float duration;

void setup() {
 Serial.begin(9600);
}

void loop() {
  pinMode(ping2, OUTPUT);
  digitalWrite(ping2, LOW);
  delayMicroseconds(2);
  digitalWrite(ping2, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping2, LOW);
  pinMode(ping2, INPUT);
  duration = pulseIn(ping2, HIGH);
  duration = duration / 29/2;
  Serial.println(duration);
}
