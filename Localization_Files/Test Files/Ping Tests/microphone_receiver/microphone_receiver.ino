int analogPin = 5;
int StartTime;
int val = 0;
int duration;
void setup()
{
  Serial.begin(9600);   
}

void loop()
{
  
  if (Serial.available())
  {  
    StartTime=millis();
    while(true){ 
    if (analogRead(analogPin)<900){
       duration = millis()-StartTime;
       Serial.print(duration); 
       delay(10000); 
    }
    }
  }
}
