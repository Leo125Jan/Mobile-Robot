#include <ros.h>
#include <util/atomic.h>
#include <std_msgs/Int64MultiArray.h>

// Define the motors
#define NMOTORS 2

class Motor
{
  public:
    Motor(int ENCA_, int ENCB_, int pwmPin_, int inp1_, int inp2_, int S);
    void maneuver(int pwmVal, int pwmPin, int in1, int in2);
    int getENCA(void);
    int getENCB(void);
    int getpwmPin(void);
    int getinp1(void);
    int getinp2(void);
    
  private:
     int ENCA;
     int ENCB;
     int pwmPin;
     int inp1;
     int inp2;
     int side;// 0: left, 1: right
};

Motor::Motor(int ENCA_, int ENCB_, int pwmPin_, int inp1_, int inp2_, int S)
{
  ENCA = ENCA_;
  ENCB = ENCB_;
  pwmPin = pwmPin_;
  inp1 = inp1_;
  inp2 = inp2_;
  side = S;
  
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  pinMode(pwmPin,OUTPUT);
  pinMode(inp1,OUTPUT);
  pinMode(inp2,OUTPUT);
    
  analogWrite(pwmPin,0);
  digitalWrite(inp1,LOW);
  digitalWrite(inp2,LOW);
}

void Motor::maneuver(int pwmVal, int pwmPin, int in1, int in2)
{
  if (pwmVal > 0)
  {
    //Positive
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(pwmPin,pwmVal);
  }
  else if (pwmVal < 0)
  {
    //Negative
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(pwmPin,-pwmVal);
  }
  else
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }
}

int Motor::getENCA(void)
{
  return ENCA;
}

int Motor::getENCB(void)
{
  return ENCB;
}

int Motor::getpwmPin(void)
{
  return pwmPin;
}

int Motor::getinp1(void)
{
  return inp1;
}

int Motor::getinp2(void)
{
  return inp2;
}

//Ros init
ros::NodeHandle nh;

//Motor Array
Motor m[] = {Motor(4,2,6,10,11,0), Motor(12,3,5,8,9,1)};

// AttachInterrupt for touch sensor
attachInterrupt(0, obstacle_detect, RISING);
attachInterrupt(1, obstacle_detect, RISING);

// Sensor Pin
#define phsPin A0
#define tPin_l A2
#define tPin_r A3

// Varaible for touch sensor
byte t_l = 0;
byte t_r = 0;
byte t_l_prev = 1;
byte t_r_prev = 0;                              
int count = 0;

// Varaible for photoresisitor
const float threshold_d = 0.25;
const float threshold_s = 2.0;
int phs_init = 0;
bool det = false;

// Setup Functuion
void setup()
{
  pinMode(phsPin, INPUT);
  pinMode(tPin_l, INPUT);
  pinMode(tPin_r, INPUT);

  Serial.begin(9600);

  delay(500);
  long initT = micros();

  while ( (micros()-initT)/1.0e6 <= 1.5)
  {
    m[0].maneuver(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
  }
}

// Main function
void loop()
{
  detect();

  long initT = micros();

  while ( (micros()-initT)/1.0e6 <= 1)
  {
    m[0].maneuver(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
  }
}

/*
-------------------------------------
Large value: Dark
Small value: Bright
-------------------------------------
*/

void detect(void)
{
  det = false;
  phs_init = analogRead(phsPin);

  // Detection region 0~180
  long initT = micros();

  if (volts(phs_init) < threshold_s)
  {
    while ( (micros()-initT)/1.0e6 <= 1)
    {
      m[0].maneuver(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
      m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    }
  }
  else
  {
    while ( (micros()-initT)/1.0e6 <= 0.6 && det == false)
    {
      m[0].maneuver(-50, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
      m[1].maneuver(+50, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());

      if ( abs(volts(analogRead(phsPin)) - volts(phs_init)) > threshold_d)
      {
        m[0].maneuver(0, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(0, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(50);
        m[0].maneuver(80, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(80, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(200);

        det = true;
        break;
      }
    }

    while ( (micros()-initT)/1.0e6 <= 0.6 && det == false)
    {
      m[0].maneuver(+50, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
      m[1].maneuver(-50, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());

      if ( abs(volts(analogRead(phsPin)) - volts(phs_init)) > threshold_d)
      {
        m[0].maneuver(0, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(0, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(50);
        m[0].maneuver(80, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(80, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(200);

        det = true;
        break;
      }
    }

    while ( (micros()-initT)/1.0e6 <= 0.6 && det == false)
    {
      m[0].maneuver(+50, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
      m[1].maneuver(-50, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());

      if ( abs(volts(analogRead(phsPin)) - volts(phs_init)) > threshold_d)
      {
        m[0].maneuver(0, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(0, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(50);
        m[0].maneuver(80, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(80, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(200);
        det = true;
        break;
      }
    }

    while ( (micros()-initT)/1.0e6 <= 0.6 && det == false)
    {
      m[0].maneuver(-50, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
      m[1].maneuver(+50, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());

      if ( abs(volts(analogRead(phsPin)) - volts(phs_init)) > threshold_d)
      {
        m[0].maneuver(0, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(0, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(50);
        m[0].maneuver(80, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
        m[1].maneuver(80, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
        delay(200);

        det = false;
        break;
      }
    }
  }
}

float volts(int ad)
{
  return (ad * 5.0 / 1024.0); 
}

void obstacle_detect(void)
{
  t_l = digitalRead(tPin_l);               // Copy left result to wLeft  
  t_r = digitalRead(tPin_r);              // Copy right result to wRight

  if(t_l != t_r)                        // One whisker pressed?
  {                                          // Alternate from last time?
    if ((t_l != t_l_prev) && (t_r != t_r_prev))  
    {                                       
      count++;                             // Increase count by one
      t_l_prev = t_l;                      // Record current for next rep
      t_r_prev = t_r;

      if(count == 4)                       // Stuck in a corner?
      {
        t_l = 2;                           // Set up for U-turn
        t_r = 2;
        count = 0;                         // Clear alternate corner count
      }
    }  
    else                                     // Not alternate from last time
    {
      count = 0;                           // Clear alternate corner count
    }
  } 

  if((t_l == 2) && (t_r == 2))          // If both whiskers contact
  {
    // Back up 1 second
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delayMicroseconds(1000*1000);

    // Turn left about 120 degrees
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delayMicroseconds(800*1000);                           
  }
  else if(t_l == 0)
  {
    // Back up 1 second
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delayMicroseconds(1000*1000);

    // Turn right about 60 degrees
    m[0].maneuver(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delayMicroseconds(400*1000);                          
  }
  else if(t_r == 0)
  {
    // Back up 1 second
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delayMicroseconds(1000*1000);

    // Turn left about 60 degrees
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delayMicroseconds(400*1000);                   
  }
}

