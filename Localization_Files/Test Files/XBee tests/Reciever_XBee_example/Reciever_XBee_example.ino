void setup() {
  Serial.begin(9600);  
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {  
       //char data = Serial.read();
      Serial.print((char)Serial.read());
  }
}
