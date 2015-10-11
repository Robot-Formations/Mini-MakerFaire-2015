#include <Wire.h>
#include <Zumo32U4.h>
#include <LSM303.h>

#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C

Zumo32U4Motors motors;
LSM303 compass;
  
void setup()
{
  LSM303::vector<int16_t> running_min = {32767, 32767, 32767}, running_max = { -32767, -32767, -32767};
  unsigned char index;

  Serial.begin(9600);
  Wire.begin();
  compass.init(compass.device_DLM, compass.sa0_auto);

  compass.enableDefault();

  compass.writeReg(LSM303::CRB_REG_M, CRB_REG_M_2_5GAUSS); // +/- 2.5 gauss sensitivity to hopefully avoid overflow problems
  compass.writeReg(LSM303::CRA_REG_M, CRA_REG_M_220HZ);    // 220 Hz compass update rate

  //compass.m_min = (LSM303::vector<int16_t>){ -672,   -889,   -638};
  //compass.m_max = (LSM303::vector<int16_t>){  +455,   +369,   +431};

  compass.m_max.x = 94;
  compass.m_max.y = 47;
  compass.m_min.x = -180;
  compass.m_min.y = -246;
}

void loop()
{
  float heading = averageHeading();
  Serial.println(heading);
}


template <typename T> float heading(LSM303::vector<T> v)
{
  float x_scaled =  2.0 * (float)(v.x - compass.m_min.x) / ( compass.m_max.x - compass.m_min.x) - 1.0;
  float y_scaled =  2.0 * (float)(v.y - compass.m_min.y) / (compass.m_max.y - compass.m_min.y) - 1.0;

  float angle = atan2(y_scaled, x_scaled) * 180 / M_PI;
  if (angle < 0)
    angle += 360;
  return angle;
}

float relativeHeading(float heading_from, float heading_to)
{
  float relative_heading = heading_to - heading_from;

  // constrain to -180 to 180 degree range
  if (relative_heading > 180)
    relative_heading -= 360;
  if (relative_heading < -180)
    relative_heading += 360;

  return relative_heading;
}

float averageHeading()
{
  LSM303::vector<int32_t> avg = {0, 0, 0};

  for (int i = 0; i < 10; i ++)
  {
    compass.read();
    avg.x += compass.m.x;
    avg.y += compass.m.y;
  }
  avg.x /= 10.0;
  avg.y /= 10.0;
  return heading(avg);
}
