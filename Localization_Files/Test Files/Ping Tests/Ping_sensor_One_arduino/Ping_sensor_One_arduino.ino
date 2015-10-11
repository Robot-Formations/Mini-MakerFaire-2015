const int ping1 = 7;
const int ping2 = 8;

void setup() {
  Serial.begin(9600);
}

void loop()
{

  long duration, inches, cm;
  
  pinMode(ping1, OUTPUT);
  digitalWrite(ping1, LOW);
  delayMicroseconds(2);
  digitalWrite(ping1, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping1, LOW);


  pinMode(ping2, OUTPUT);
  digitalWrite(ping2, LOW);
  delayMicroseconds(2);
  digitalWrite(ping2, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping2, LOW);


  pinMode(ping1, INPUT);
  duration = pulseIn(ping1, HIGH);

  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  
  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  delay(100);
}

long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}

