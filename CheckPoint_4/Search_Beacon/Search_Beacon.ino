#include "Motor.h"

#define beacon_pin A5
int period = 0.1*1.0e6; //62900
// Timer
#define Interval_count 10000
unsigned long currT = 0;
float deltaT = 0.0;

// Control Effort
#define virtual_master 2.0
const float kp = 200;
const float kd = 1;

Motor m[] = {Motor(4,2,6,10,11,0), Motor(12,3,5,8,9,1)};

void goStraight(float ref, float deltaT);
void goBack(float ref, float deltaT);
void turnLeft(float ref, float deltaT);
void turnRight(float ref, float deltaT);


void setup()
{
  Serial.begin(9600);
  
  currT = micros();

  for(int i = 0; i < 2; i++)
  {
      m[i].setMotor(0, m[i].getpwmPin(), m[i].getinp1(), m[i].getinp2());
      m[i].setPID_params(virtual_master, kp, kd);
  }  
}

unsigned long T = micros();

void loop()
{
  
  if (micros()-T > 100000)
  {
    Ration_cal();
    T = micros();
  }
  
  /*
  if ( micros() - currT > Interval_count)
  {
    noInterrupts();
    m[0].setPost(m[0].getPos());
    m[1].setPost(m[1].getPos());
    interrupts();

    deltaT = ((float) Interval_count)/1.0e6;

    //m[0].PID_control(deltaT);
    //m[1].PID_control(deltaT);
    turnRight(2.0, deltaT);
    
    Serial.print("R: ");
    Serial.print(m[1].getRpst());
    Serial.print(" L: ");
    Serial.print(m[0].getPos());
    Serial.println("");
    
    
    currT = micros(); 
  }
  */
}

void Ration_cal(void)
{
    byte val = 0;
    float ratio = 0.0;
    float count_0 = 0.0;
    float count_1 = 0.0;

    unsigned long currT = micros(); 
    
    while ( micros() - currT < period)
    { 
      val = digitalRead(beacon_pin);
      
      if (val == 1) count_1++;
      if (val == 0) count_0++;
    }

    ratio = count_0/(count_0 + count_1);
    
    Serial.print("Ratio: ");
    Serial.print(ratio);
    Serial.println("");
}

void readEncoder_l()
{
  int b = digitalRead(m[0].getENCA());

  if (b > 0)
  {
    m[0].setPos(m[0].getPos()+1);
  }
  else
  {
    m[0].setPos(m[0].getPos()-1);
  }
}

void readEncoder_r()
{
  int b = digitalRead(m[1].getENCA());
  
  if (b > 0)
  {
    m[1].setPos(m[1].getPos()-1);
  }
  else
  {
    m[1].setPos(m[1].getPos()+1);
  }
}

void goStraight(float ref, float deltaT)
{
  float kp = 200;
  float kd = 1.0;
  m[0].PID_control(ref, deltaT, kp, kd);
  m[1].PID_control(ref, deltaT, kp, kd);
}

void goBack(float ref, float deltaT)
{
  float kp = 10000;
  float kd = 1.0;
  m[0].PID_control(-ref, deltaT, kp, kd);
  m[1].PID_control(-ref, deltaT, kp, kd);
}

void turnRight(float ref, float deltaT)
{
  float kp = 2000;
  float kd = 1.0;
  m[0].PID_control(ref, deltaT, kp, kd);
  m[1].PID_control(-ref, deltaT, kp, kd);
}

void turnLeft(float ref, float deltaT)
{
  float kp = 2000;
  float kd = 1.0;
  m[0].PID_control(-ref, deltaT, kp, kd);
  m[1].PID_control(ref, deltaT, kp, kd);
}

