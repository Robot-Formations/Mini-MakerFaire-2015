void setup() {
  Serial.begin(9600);
}

void loop() {
  float x = 0.0;
  float y = 0.0;
  float newx=3.0;
  float newy = 4.0;
  
  float angle = atan2(newy-y,newx-x)*(180/PI);
  float target_heading = angle;
  //angle+=heading;
  if(newx -x >0 && newy-y >0)
      target_heading = fmod(angle + 90, 360);
  else if (newx-x < 0)
  {
    if(newy-y>0)
      target_heading = fmod(angle+180 + 90, 360);
    if(newy-y<0)
      target_heading = fmod(angle + 270, 360);
  }
  float dist = sqrt(sq(newy-y)+sq(newx-x));
  Serial.println(target_heading);
  Serial.println(dist);

}
