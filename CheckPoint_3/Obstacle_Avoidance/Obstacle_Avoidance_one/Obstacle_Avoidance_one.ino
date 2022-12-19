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
Motor m[] = {Motor(4,2,6,10,11,0), Motor(12,3,5,8,9,1)};

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

void turnLeft() //800
{
  m[0].maneuver(-100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
void turnRight() //800
{
  m[0].maneuver(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(-100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}

void turnLeft(int pwm) //800
{
  m[0].maneuver(-pwm, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(pwm, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
void turnRight(int pwm) //800
{
  m[0].maneuver(pwm, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(-pwm, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}

void goStraight()
{
  m[0].maneuver(103, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(106, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
void goStraight_s()
{
  m[0].maneuver(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(103, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
void goBack() //600
{
  m[0].maneuver(-120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(-120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
void Stop()
{
  m[0].maneuver(0, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].maneuver(0, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}

//Ros init
ros::NodeHandle nh;

// Sensor Pin
#define phsPin A0
#define tPin_l 3
#define tPin_r 2
#define tPin_d A5

// Varaible for touch sensor
byte t_l = 0;
byte t_r = 0;
byte t_d = 1;
byte t_l_prev = 1;
byte t_r_prev = 0;                              
int count = 0;

// Varaible for photoresisitor
const float threshold_d = 45;
int phs_init = 0;
bool det = false;
bool ob = false;
bool flag = false;
bool dogo = true;

// Setup Functuion
void setup()
{
  pinMode(phsPin, INPUT);
  //pinMode(tPin_l, INPUT_PULLUP);
  //pinMode(tPin_r, INPUT_PULLUP);
  pinMode(tPin_d, INPUT);
  
  // AttachInterrupt for touch sensor
  attachInterrupt(tPin_l-2, Flag, FALLING);
  attachInterrupt(tPin_r-2, Flag, FALLING);

  Serial.begin(9600);
}

// Main function
void loop()
{
  t_d = digitalRead(tPin_d);
  goStraight();
  // delay time real
  
  //if (flag)
  obstacle_detect();
  
  if (ob)
    detect();

  if(t_d == 0)
  {
    Stop();
    delay(1);
    exit(0);
  }
  
}

void obstacle_detect(void)
{
  delay(200);
  noInterrupts();
  t_l = digitalRead(tPin_l);               // Copy left result to wLeft  
  t_r = digitalRead(tPin_r);              // Copy right result to wRight
  interrupts();

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
    goBack();
    delay(1000);

    // Turn left about 120 degrees
    turnLeft();
    delay(800);

    goStraight();
    delay(400);    
  }
  else if(t_l == 0)
  {
    // Back up 1 second
    goBack();
    delay(1000);

    // Turn right about 60 degrees
    turnRight();
    delay(800);
    
    goStraight();
    delay(1000); 
  }
  else if(t_r == 0)
  {
    // Back up 1 second
    goBack();
    delay(1000);

    // Turn left about 60 degrees
    turnLeft();
    delay(800);  
    
    goStraight();
    delay(1000); 
  }
  flag = false;
}

/*
-------------------------------------
Large value: Dark
Small value: Bright
-------------------------------------
*/

void detect(void)
{
  phs_init = analogRead(phsPin);
  int small = phs_init;
  dogo = true;

  //------------------------------------------
  // Round 1
  // Detection region 0~180
  long init_1 = micros();
  
  while ( (micros()-init_1)/1.0e6 <= 0.8*5)
  {
    phs_init = analogRead(phsPin);
    turnLeft(100);
    
    if ( phs_init < small )
    {
      small = phs_init;
    }
  }
  Serial.print("Small Value: ");
  Serial.print(small);
  Serial.println("");
  //------------------------------------------
  
  /*
  Serial.print("Small Value: ");
  Serial.print(small);
  Serial.println("");
  Stop();
  delay(1000);
  phs_init = analogRead(phsPin);
  
  while(abs(phs_init - small) > threshold_d)
  {
    phs_init = analogRead(phsPin);
    Serial.print("PHS: ");
    Serial.print(phs_init);
    Serial.print("   Diff: ");
    Serial.print(abs(phs_init - small));
    Serial.println("");
    turnLeft();
  }
  */
  turnRight(100);
  delay(500);
  
  //------------------------------------------
  // Round 2
  long init_2 = micros();
  while ( (micros()-init_2)/1.0e6 <= 0.8*5)
  {
    turnLeft(100);
    phs_init = analogRead(phsPin);
    Serial.print("PHS: ");
    Serial.print(phs_init);
    Serial.print("   Diff: ");
    Serial.print(abs(phs_init - small));
    Serial.println("");
    
    if ( abs(phs_init - small) < threshold_d)
    {
      Stop();
      delay(100);
      turnRight();
      delay(200);
      long init_3 = micros();
      Stop();
      
      while ( (micros()-init_3)/1.0e6 <= 2)
      {
        goStraight_s();
        obstacle_detect();
        t_d = digitalRead(tPin_d);
        
        dogo = false;
        
        if(t_d == 0)
        {
          Stop();
          delay(1);
          exit(0);
        }
      }
    }
  }
  //------------------------------------------
  
  //------------------------------------------
  // Round Straight
  if (dogo)
  {
    long init_4 = micros();
    while ( (micros()-init_4)/1.0e6 <= 2)
    {
      goStraight();
      obstacle_detect();
    }
  }
  //------------------------------------------
}

float volts(int ad)
{
  return (ad * 5.0 / 1024.0); 
}

void Flag(void)
{
  flag = true;
  ob = true;
}
