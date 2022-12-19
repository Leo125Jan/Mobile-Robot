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
  
  if (ENCB-2 == 0)
  {
    attachInterrupt(ENCB-2, readEncoder_l, RISING);
  }
  else
  {
    attachInterrupt(ENCB-2, readEncoder_r, RISING);
  }
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

// Sensor Pin
#define phsPin_l A0
#define phsPin_r A1
#define tPin_l A2
#define tPin_r A3

// Varaible for touch sensor
byte t_l = 0;
byte t_r = 0;
byte t_l_prev = 0;
byte t_r_prev = 1;                              
int count = 0;

// Varaible for photoresisitor
int phs_l = 0;
int phs_r = 0;
float shade = 0.0;
int pwmLeft = 120;
int pwmRight = 120;

// Setup Functuion
void setup()
{
  pinMode(phsPin_l, INPUT);
  pinMode(phsPin_r, INPUT);
  pinMode(tPin_l, INPUT);
  pinMode(tPin_r, INPUT);

  Serial.begin(9600);
}

// Main function
void loop()
{
  obstacle_detect();
  Light_Navigation();
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

  if((t_l == 0) && (t_r == 0))          // If both whiskers contact
  {
    // Back up 1 second
    m[0].maneuver(-120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(1000);

    // Turn left about 120 degrees
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(10);                           
  }
  else if(t_l == 0)
  {
    // Back up 1 second
    m[0].maneuver(-120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(1000);

    // Turn right about 60 degrees
    m[0].maneuver(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(10);                          
  }
  else if(t_r == 0)
  {
    // Back up 1 second
    m[0].maneuver(-120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(1000);

    // Turn left about 60 degrees
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(10);                   
  }
  else if((t_l == 2) && (t_r == 2))
  {
    // Back up 1 second
    m[0].maneuver(-120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(-120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(1000);

    // Turn left about 60 degrees
    m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(100);                   
  }
  else
  {
    // Forward 1/50 of a second
    m[0].maneuver(120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].maneuver(120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(10);
  }
}

/*
-------------------------------------
Large value: Dark
Small value: Bright
-------------------------------------
*/

void Light_Navigation(void)
{
  phs_l = analogRead(phsPin_l);
  phs_r = analogRead(phsPin_r);
  shade = volts(phs_l)/( volts(phs_l) + volts(phs_r) ) - 0.5;

  if ( (abs(shade) < 0.1) && (phs_l < 50) && (phs_r < 50) )
  {
    pwmLeft = 120;
    pwmRight = 120;
  }
  else if ( (abs(shade) < 0.1) && (phs_l > 200) && (phs_r > 200) )
  {
    pwmLeft = 120;
    pwmRight = 120;
    //m[0].maneuver(120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    //m[1].maneuver(-120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    delay(500);
  }
  else if (shade < 0.0)                         // Shade on right?
  {                                          // Slow down left wheel
    pwmLeft = int(120.0 + (shade * 800.0));
    pwmLeft = constrain(pwmLeft, -200, 200);
    pwmRight = 120;                        // Full speed right wheel
  } 
  else if (shade > 0.0)                                       // Shade on Left?
  {                                          // Slow down right wheel
    pwmRight = int(120 - (shade * 800.0));
    pwmRight = constrain(pwmRight, -200, 200);
    pwmLeft = 120;                         // Full speed left wheel
  }
  
  Serial.print("Left PWM: ");
  Serial.print(pwmLeft);
  Serial.print("   ");

  Serial.print("ZNED: ");
  Serial.print(shade);
  Serial.print(" volts   ");

  Serial.print("Right PWM: ");
  Serial.print(pwmRight);
  Serial.println("");

  m[0].maneuver(pwmLeft, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(pwmRight, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
  delay(20);
}

float volts(int ad)
{
  return (ad * 5.0 / 1024.0); 
}

void readEncoder_l()
{
}

void readEncoder_r()
{
}
