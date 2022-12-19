#include "Motor.h"
#include <ros.h>
#include <std_msgs/Float64.h>

// Motor
Motor m[] = {Motor(4,2,6,10,11,0), Motor(12,3,5,8,9,1)};

void goBack(float ref, float deltaT);
void turnLeft(float ref, float deltaT);
void turnRight(float ref, float deltaT);
void goStraight(float ref, float deltaT);

// Control Effort
const float kd = 1;
const float kp = 200;
#define virtual_master 2.0

// Sensor Pin
#define phsPin A0
#define tPin_l A2
#define tPin_r A3
#define tPin_d A4
#define beacon_pin A5

// Varaible for touch sensor
byte t_l = 0;
byte t_r = 0;
byte t_d = 1;
byte t_l_prev = 1;
byte t_r_prev = 0;                              
int count = 0;
bool grap_ball = false;

// Varaible for photoresisitor
const float threshold_d = 45;
int phs_init = 0;
bool det = false;
bool ob = false;
bool dogo = true;

ros::NodeHandle  nh;
//std_msgs::Int64 feedback;
float ratio_;

//void obtCb(const std_msgs::Float64 &msg)
//{
  //feedback= msg;
//}
//ratio_ = feedback.data;

//ros::Subscriber<std_msgs::Float64> obt_sub("/Command", &obtCb);

void setup()
{
  Serial.begin(9600);

  pinMode(phsPin, INPUT);
  pinMode(tPin_l, INPUT);
  pinMode(tPin_r, INPUT);
  pinMode(tPin_d, INPUT);
  pinMode(beacon_pin, INPUT);
  
  //nh.initNode();
  //nh.subscribe(obt_sub);
  
  unsigned long localT = 0;
  while (micros() - localT < 0.3*1.0e6){};
}

unsigned long T = micros();
unsigned long local = micros();

void loop()
{
  /*
  if (micros()-T > 100000)
  {
    Ration_cal();
    T = micros();
  }
  */
  
  Ration_cal();
  /*
  if ( micros() - local > 0.012*1.0e6)
  {
    Stagation();
    goStraight(2.0);
    
    //Serial.print("L: ");
    //Serial.print(m[0].getRpst());
    //Serial.print(" R: ");
    //Serial.println(m[1].getRpst());
    
    local = micros(); 
  }
  */
  //beacon_detect();
  //obstacle_detect();
  
  //if (ob == true && grap_ball == false) light_detect();
  //if (grap_ball) beacon_detect();
  
}

//---------------------------Stagation---------------------------//
void Stagation(void)
{
  noInterrupts();
  m[0].setPost(m[0].getPos());
  m[1].setPost(m[1].getPos());
  interrupts();
}
//---------------------------------------------------------------//

//-----------------------Obstacle Avoidance----------------------//
// Timer
#define Interval_count 0.01*1.0e6

void obstacle_detect(void)
{
  t_l = digitalRead(tPin_l);
  t_r = digitalRead(tPin_r);

  if(t_l != t_r)
  {
    if ((t_l != t_l_prev) && (t_r != t_r_prev))  
    {                                       
      count++;
      t_l_prev = t_l;
      t_r_prev = t_r;

      if(count == 4)
      {
        t_l = 2;
        t_r = 2;
        count = 0;
      }
    }  
    else
    {
      count = 0;
    }
  } 
  
  unsigned long localT = 0;
  unsigned long localinT = 0;
  
  ////////////////////////////////ALL O
  if((t_l == 2) && (t_r == 2))
  {
    // Back up 1 second
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.8*1.0e6)
    {
      if ( micros() - localinT > 0.01*1.0e6)
      {
        Stagation();
        goBack(2.0);
        
        localinT = micros(); 
      }
    }

    // Turn left about 120 degrees
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.5*1.0e6)
    {
      if ( micros() - localinT > 0.015*1.0e6)
      {
        Stagation();
        turnLeft(1.5);
        
        localinT = micros(); 
      }
    }

    //Go Straight
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.95*1.0e6)
    {
      if ( micros() - localinT > 0.008*1.0e6)
      {
        Stagation();
        goStraight(1.5);
        
        localinT = micros(); 
      }
    }
    
    Flag();
  }///////////////////////////////////////////////
  else if(t_l == 0)////////////////////////////////Left O
  {
    // Back up 1 second
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.8*1.0e6)
    {
      if ( micros() - localinT > 0.01*1.0e6)
      {
        Stagation();
        goBack(2.0);
        
        localinT = micros(); 
      }
    }

    // Turn right about 60 degrees
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.5*1.0e6)
    {
      if ( micros() - localinT > 0.015*1.0e6)
      {
        Stagation();
        turnRight(1.5);
        
        localinT = micros(); 
      }
    }
    
    //Go Straight
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.95*1.0e6)
    {
      if ( micros() - localinT > 0.008*1.0e6)
      {
        Stagation();
        goStraight(1.5);
        
        localinT = micros(); 
      }
    }
    
    Flag();
  }///////////////////////////////////////////////////
  else if(t_r == 0)////////////////////////////////Right O
  {
    // Back up 1 second
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.8*1.0e6)
    {
      if ( micros() - localinT > 0.01*1.0e6)
      {
        Stagation();
        goBack(2.0);
        
        localinT = micros(); 
      }
    }

    //Turn left about 120 degrees
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.5*1.0e6)
    {
      if ( micros() - localinT > 0.015*1.0e6)
      {
        Stagation();
        turnLeft(1.5);
        
        localinT = micros(); 
      }
    }  
    
    //Go Straight
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.95*1.0e6)
    {
      if ( micros() - localinT > 0.008*1.0e6)
      {
        Stagation();
        goStraight(1.5);
        
        localinT = micros(); 
      }
    }
    Flag();
  }
  ////////////////////////////////////////////////////////////////
}
//--------------------------------------------------------------//

//---------------------obstacle_detect_4_beacon--------------------------------//
void obstacle_detect_4_beacon(void)
{
  t_l = digitalRead(tPin_l);
  t_r = digitalRead(tPin_r);
  
  unsigned long localT = 0;
  unsigned long localinT = 0;
  
  if(t_l == 0)////////////////////////////////Left O
  {
    // Back up 1 second
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.3*1.0e6)
    {
      if ( micros() - localinT > 0.01*1.0e6)
      {
        Stagation();
        goBack(2.0);
        
        localinT = micros(); 
      }
    }

    // Turn right about 60 degrees
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.2*1.0e6)
    {
      if ( micros() - localinT > 0.015*1.0e6)
      {
        Stagation();
        turnRight(1.2);
        
        localinT = micros(); 
      }
    }
  }///////////////////////////////////////////////////
  else if(t_r == 0)////////////////////////////////Right O
  {
    // Back up 1 second
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.3*1.0e6)
    {
      if ( micros() - localinT > 0.01*1.0e6)
      {
        Stagation();
        goBack(2.0);
        
        localinT = micros(); 
      }
    }

    //Turn left about 120 degrees
    localT = micros();
    localinT = micros();
    while(micros() - localT <= 0.2*1.0e6)
    {
      if ( micros() - localinT > 0.015*1.0e6)
      {
        Stagation();
        turnLeft(1.2);
        
        localinT = micros(); 
      }
    }
  }
}
//-----------------------------------------------------------------------------//

//-----------------------light_detect-----------------------------------------//
// Timer
#define Interval 0.01*1.0e6

void light_detect(void)
{
  phs_init = analogRead(phsPin);
  int small = phs_init;
  dogo = true;

  //---------------------Round 1---------------------//
  unsigned long currT = micros();
  unsigned long init_1 = micros();
  unsigned long localT = micros();
  while (micros() - init_1 <= 4.0*1.0e6)
  {
    if ( micros() - currT > 0.010*1.0e6)
    {
      Stagation();
      turnLeft(1.0);
      
      currT = micros();
    }
    
    phs_init = analogRead(phsPin);
    if ( phs_init < small )
    {
      small = phs_init;
    }
  }

  Serial.print("Small Value: ");
  Serial.print(small);
  Serial.println("");
  //-------------------------------------------------//
  
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
  
  // Delay for 1 sec
  Stop();
  localT = micros();
  while (micros() - localT <= 0.5*1.0e6){};
  Stop();
  
  //-------------------Round 2-----------------------// 
  currT = micros();
  unsigned long init_2 = micros();
  while (micros() - init_2 <= 6.0*1.0e6)
  {
    if ( micros() - currT > 0.010*1.0e6)
    {
      Stagation();
      turnRight(1.0);
      
      currT = micros();
    }
    
    //------------------Light Detect-------------------// 
    phs_init = analogRead(phsPin);
    if ( abs(phs_init - small) < threshold_d + 10)
    {
      Stop();
      localT = micros();
      while (micros() - localT < 0.5*1.0e6){};
      
      //------------------Revision-------------------//
      currT = micros();
      localT = micros();
      while (micros() - localT <= 0.03*1.0e6)
      {
        if ( micros() - currT > Interval)
        {
          Stagation();
          turnLeft(0.5);
          
          currT = micros();
        }
      }
      Stop();
      //-----------------------------------------//
      
      localT = micros();
      while (micros() - localT < 0.5*1.0e6){};
      Stop();

      currT = micros();
      unsigned long init_3 = micros();
      while (micros() - init_3 <= 3.0*1.0e6)
      {
        if ( micros() - currT > 0.012*1.0e6)
        {
          Stagation();
          goStraight(1.8);
          
          currT = micros();
        }
        
        obstacle_detect();
        dogo = false;
        
        
        t_d = digitalRead(tPin_d);
        if(t_d == 0)
        {
          Stop();
          grap_ball = true;
          
          localT = micros();
          while (micros() - localT < 0.5*1.0e6){};
        } 
      }

      break;
    }
    //---------------------------------------------------//
  }
  //--------------------------------------------------//
  
  //-----------------Round Straight-------------------//
  if (dogo)
  {
    currT = micros();
    unsigned long init_4 = micros();
    while (micros() - init_4 <= 0.5*1.0e6)
    {
      if ( micros() - currT > 0.012*1.0e6)
      {
        Stagation();
        goStraight(2.0);
        
        currT = micros();
      }
      obstacle_detect();
    }
  }
  //--------------------------------------------------//
}
//---------------------------------------------------------------------------//

//-----------------------------beacon_detect---------------------------------//
// Timer
#define Interval 0.01*1.0e6
#define Ratio_Interval 0.01*1.0e6
bool dog = true;
/*
void beacon_detect(void)
{
  byte val = 0;
  float max_ = 0.0;
  float ratio = 0.0;
  float count_0 = 0.0;
  float count_1 = 0.0;

  unsigned long currT = micros();
  unsigned long ratioT = micros();
  unsigned long localT = micros();

  //---------------------Round 1---------------------//
  unsigned long init_1 = micros();
  while ( micros() - init_1 <= 6.0*1.0e6)
  { 
    if ( micros() - currT > Interval)
    {
      Stagation();
      turnLeft(0.7);
      
      currT = micros();
    }
    
    val = digitalRead(beacon_pin);   
    if (val == 1) count_1++;
    if (val == 0) count_0++;
    
    if (micros() - ratioT > Ratio_Interval)
    {
      ratio = count_0/(count_0 + count_1);
  
      if (ratio >= max_)
      {
        max_ = ratio;
      }
      
      ratioT = micros();
    }
    
    Serial.print("val: ");
    Serial.print(val);
    Serial.println("");
  }
  
  Serial.print("max: ");
  Serial.print(max_);
  Serial.println("");
  //-----------------------------------------------//
  
  // Delay for 1 sec
  Stop();
  localT = micros();
  while (micros() - localT <= 0.5*1.0e6){};

  //-------------------Round 2-----------------------//
  currT = micros();
  ratioT = micros();
  unsigned long init_2 = micros();
  while ( micros() - init_2 <= 6.0*1.0e6)
  {
    if ( micros() - currT > Interval)
    { 
      Stagation();
      turnLeft(0.7);
      
      currT = micros();
    }
    
    val = digitalRead(beacon_pin);
    if (val == 1) count_1++;
    if (val == 0) count_0++;
    
    if (micros() - ratioT > Ratio_Interval)
    {
      ratio = count_0/(count_0 + count_1);
      
      ratioT = micros();
      Serial.print("ratio: ");
      Serial.print(ratio);
      Serial.println("");
    }
    
    Serial.print("val: ");
    Serial.print(val);
    Serial.println("");
    
    //--------------------Beacon Detect----------------//
    if ( ratio >= max_-0.03)
    {
      Stop();
      localT = micros();
      while (micros() - localT < 0.5*1.0e6){};
      
      //--------------------Revision----------------//
      
      currT = micros();
      localT = micros();
      while (micros() - localT <= 0.1*1.0e6)
      {
        if ( micros() - currT > Interval)
        {
          Stagation();
          turnRight(1.0);
          
          currT = micros();
        }
      }
      
      //------------------------------------------//
      
      localT = micros();
      while (micros() - localT <= 0.5*1.0e6){};
      Stop();
      
      dog = false;

      currT = micros();
      unsigned long init_3 = micros();
      while (micros()-init_3 <= 3.0*1.0e6)
      {
        if ( micros() - currT > 0.02*1.0e6)
        {
          Stagation();
          goStraight(2.0);
          
          currT = micros();
        }
        
        obstacle_detect_4_beacon();
      }
      
      Stop();
      exit(0);
    }
    //---------------------------------------------//
  }
  
  //-----------------Round Straight-------------------//
  if (dog)
  {
    currT = micros();
    unsigned long init_4 = micros();
    while (micros() - init_4 <= 1.0*1.0e6)
    {
      if ( micros() - currT > 0.02*1.0e6)
      {
        Stagation();
        goStraight(1.5);
        
        currT = micros();
      }
      obstacle_detect()4_beacon();
    }
  }
  
  //------------------------------------------//
    
  Serial.print("Ratio: ");
  Serial.print(ratio);
  Serial.println("");
}
*/
void beacon_detect(void)
{
  byte val = 0;
  float max_ = 0.0;
  float ratio = 0.0;
  float count_0 = 0.0;
  float count_1 = 0.0;

  unsigned long currT = micros();
  unsigned long ratioT = micros();
  unsigned long localT = micros();

  //---------------------Round 1---------------------//
  unsigned long init_1 = micros();
  while ( micros() - init_1 <= 6.0*1.0e6)
  { 
    if ( micros() - currT > 0.015*1.0e6)
    {
      Stagation();
      turnRight(0.9);
      
      currT = micros();
    }
    
    val = digitalRead(beacon_pin);
     
    if (val == 1) count_1++;
    if (val == 0) count_0++;
    
    if (micros() - ratioT > Ratio_Interval)
    {
      ratio = count_0/(count_0 + count_1);
  
      if (ratio >= max_)
      {
        max_ = ratio;
      }
      
      ratioT = micros();
    } 
    
    if (ratio == ratio_)
    {
      Stop();
      break;
    }
  }
  //-----------------------------------------------//
  
  // Delay for 1 sec
  Stop();
  localT = micros();
  while (micros() - localT <= 1*1.0e6){};
  
  //--------------------Revision----------------//
  currT = micros();
  localT = micros();
  while (micros() - localT <= 0.08*1.0e6)
  {
    if ( micros() - currT > Interval)
    {
      Stagation();
      turnRight(0.7);
      currT = micros();
    }
  }

  //-------------------Round 2-----------------------//
  currT = micros();
  unsigned long init_3 = micros();
  while (micros()-init_3 <= 20.0*1.0e6)
  {
    if ( micros() - currT > 0.012*1.0e6)
    {
      Stagation();
      goStraight(2.0);
      
      currT = micros();
    }
    
    obstacle_detect_4_beacon();
  }
  
  Stop();
  exit(0);
}

//-----------------------------------------------------------//

//----------------------------Ration_cal-------------------------------//
void Ration_cal(void)
{
    byte val = 0;
    float ratio = 0.0;
    float count_0 = 0.0;
    float count_1 = 0.0;

    unsigned long currT = micros(); 
    
    while ( micros() - currT < Ratio_Interval)
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
//-----------------------------------------------------------//

void Flag(void)
{
  ob = true;
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

void goStraight(float ref)
{
  float kp = 200;
  float kd = 1.0;
  m[0].PID_control(ref, kp, kd);
  m[1].PID_control(ref, kp, kd);
}

void goBack(float ref)
{
  float kp = 10000;
  float kd = 1.0;
  m[0].PID_control(-ref, kp, kd);
  m[1].PID_control(-ref, kp, kd);
}

void turnRight(float ref)
{
  float kp = 2000;
  float kd = 1.0;
  m[0].PID_control(ref, kp, kd);
  m[1].PID_control(-ref, kp, kd);
}

void turnLeft(float ref)
{
  float kp = 2000;
  float kd = 1.0;
  m[0].PID_control(-ref, kp, kd);
  m[1].PID_control(ref, kp, kd);
}

void Stop()
{
  m[0].setMotor(0, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].setMotor(0, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
