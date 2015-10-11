typedef union {
   byte bytes[4];
   float f; 
}bf;
bf floatToBytes;
void setup() {
  Serial1.begin(9600);
  Serial.begin(9600);
  
  floatToBytes.bytes[0]=0;
  floatToBytes.bytes[1]=0;
  floatToBytes.bytes[2]=152;
  floatToBytes.bytes[3]=65;
}
void loop() {

  //floatToBytes.f = 1.5;
  Serial.println(floatToBytes.f);
  
  delay(15000);
}
