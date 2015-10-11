typedef union{
  byte byteRead[4];
  float f;
}u;

u bytesToFloat;

void setup() {
  Serial.begin(9600);  
}

void loop() {
  
  for(int i = 0;i<4;i++){
  while(!Serial.available());
  bytesToFloat.byteRead[i] = Serial.read();
  }
  Serial.println(bytesToFloat.f);
  delay(5000);
  /*for (int i =0;i<4;i++)
  u.byteRead[i]=Serial.read();*/
  
  /*for (int i =0; i< 4; i++)
    Serial.println(u.byteRead[i]);
  Serial.println(u.f);
  */
}
