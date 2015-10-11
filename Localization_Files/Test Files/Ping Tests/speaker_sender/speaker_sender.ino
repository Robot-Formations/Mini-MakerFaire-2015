int speakerPin = 9;
char sendOver = 'A';
 
void setup()
{
  Serial.begin(9600);
}
 
void loop()
{
  Serial.println(sendOver);
  delay(100);
  while(true){
  tone(speakerPin, 400, 1000);
  }
}
