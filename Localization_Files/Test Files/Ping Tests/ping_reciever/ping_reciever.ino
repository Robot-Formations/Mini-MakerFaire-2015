const int ping1 = 7;
float duration;

void setup() {
Serial.begin(9600);
}

void loop() {  
  if (Serial.available())
  {
    if((char)Serial.read() == 'S')
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
          Serial.print(duration);
    } 
  }
}
