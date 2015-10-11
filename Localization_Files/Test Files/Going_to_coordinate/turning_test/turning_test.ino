#include <Wire.h>
#include <LSM303.h>

LSM303 compass;
float startHeading;
float degOff;
void setup() {
  Serial.begin(9600);
  Wire.begin();
  compass.init();
  compass.enableDefault();
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  //min: { -6482,  -5366,  +6519}    max: { +1834,   +290, +13947}

  compass.m_min = (LSM303::vector<int16_t>){ -6482,  -5366,  +6519};
  compass.m_max = (LSM303::vector<int16_t>){ +1834,   +290, +13947};
  
  compass.read();
  startHeading = compass.heading();
}

void loop() {
  compass.read();
  float heading = compass.heading();
  degOff = abs(heading-startHeading > 180)?abs(360-(heading-startHeading)):abs(heading-startHeading);   
  
  Serial.println(heading);
  
  //Serial.println(degOff);
  delay(100);

}
