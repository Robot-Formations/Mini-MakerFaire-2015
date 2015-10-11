#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <Wire.h>
#include <LSM303.h>

#define SPEED           200 // Maximum motor speed when going straight; variable speed when turning
#define TURN_BASE_SPEED 100 // Base speed when turning (added to variable speed)

#define CALIBRATION_SAMPLES 70  // Number of compass readings to take when calibrating
#define CRB_REG_M_2_5GAUSS 0x60 // CRB_REG_M value for magnetometer +/-2.5 gauss full scale
#define CRA_REG_M_220HZ    0x1C // CRA_REG_M value for magnetometer 220 Hz update rate

#define DEVIATION_THRESHOLD 5

ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);
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

  button.waitForButton();
}

void loop()
{
  float heading, relative_heading;
  int speed;
  static float target_heading;
  float x = 0.0;
  float y = 0.0;
  float newx = 3.0;
  float newy = 4.0;

  float angle = atan2(newy - y, newx - x) * (180 / PI);
  
  if (newx - x > 0)
  {
    if(newy - y > 0)
      target_heading = fmod(angle + 90+ averageHeading(), 360);
    if(newy - y < 0)
      target_heading = fmod(angle + averageHeading(),360);    
  }
  else if (newx - x < 0)
  {
    if (newy - y > 0)
      target_heading = fmod(angle + 180 + averageHeading(), 360);
    if (newy - y < 0)
      target_heading = fmod(angle + 270 + averageHeading(), 360);
  }

  float dist = sqrt(sq(newy - y) + sq(newx - x));

  while (abs(relative_heading) > DEVIATION_THRESHOLD) {

    // This gives us the relative heading with respect to the target angle
    relative_heading = relativeHeading(heading, target_heading);

    Serial.print("Target heading: ");
    Serial.print(target_heading);
    Serial.print("    Actual heading: ");
    Serial.print(heading);
    Serial.print("    Difference: ");
    Serial.print(relative_heading);

    speed = SPEED * relative_heading / 180;

    if (speed < 0)
      speed -= TURN_BASE_SPEED;
    else
      speed += TURN_BASE_SPEED;

    motors.setSpeeds(-speed, speed);

    Serial.println("   Turn");
    
    heading = averageHeading();  
  }
  motors.setSpeeds(0,0);
  while(true);
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

