#include <ros.h>
#include <util/atomic.h>
#include <std_msgs/Int64MultiArray.h>

// Define the motors
#define NMOTORS 2

class Motor
{
  public:
    Motor(int ENCA_, int ENCB_, int pwmPin_, int inp1_, int inp2_, int S);
    void setMotor(int pwmVal, int pwmPin, int in1, int in2);
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

void Motor::setMotor(int pwmVal, int pwmPin, int in1, int in2)
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
ros::NodeHandle  nh;
std_msgs::Int64MultiArray feedback;

void mcCb(const std_msgs::Int64MultiArray &msg)
{
  feedback= msg;
}

ros::Subscriber<std_msgs::Int64MultiArray> mc_sub("/cmd", &mcCb);

// Control Setup
Motor m[] = {Motor(4,2,6,10,11,0), Motor(12,3,5,8,9,1)};
volatile int pos[2];
int posp[2] = {0,0};
int pwm[2] = {85,100};
float e[2] = {0,0};
float u[2] = {0,0};
float eintegral[2] = {0,0};
float rpst[2] = {0,0};
float rpsp[2] = {0,0};
float rpsFilt[2] = {0,0};
long prevT = 0;
int post[2] = {0,0};

const float kpl = 3; //24
const float kpr = 2.3;

const float kil = 0.002;
const float kir = 0.0008;
const float kxl = 10;
const float kxr = 10;
int count = 0;

#define virtual_master_L 0.81 //0.81
#define virtual_master_R 0.68 //0.8

float t = 0;
int i = 0;
int j = 0;
double pos_m = 0;

void setup()
{
  nh.initNode();
  nh.subscribe(mc_sub);
  Serial.begin(9600);
}

void loop()
{
  m[0].setMotor(120, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].setMotor(120, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
/*
void loop()
{
  nh.spinOnce();
  
  
  if (feedback.data[0] == 1)
  {
    m[0].setMotor(feedback.data[1], m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].setMotor(feedback.data[2], m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
  }
  
  
  if (feedback.data[0] == 2)
  {
    pwm[0] = pwm[0] + floor(u[0]);
    pwm[1] = pwm[1] + floor(u[1]);
    m[0].setMotor(pwm[0], m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
    m[1].setMotor(pwm[1], m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
    
    noInterrupts();
    post[0] = pos[0];
    post[1] = pos[1];
    pos_m = (post[0]+post[i])/2;
    interrupts();
    long currT;
    
    if(i%5 == 0)
    {
      currT= micros(); 
      float deltaT = ((float) (currT-prevT))/1.0e6;
      
      rpst[0] = (post[0]-posp[0])/deltaT/969;
      rpst[1] = (post[1]-posp[1])/deltaT/967;
      
      e[0] = (virtual_master_L-rpst[0]);
      e[1] = (virtual_master_R-rpst[1]);
      eintegral[0] = (eintegral[0] + e[0]*deltaT);
      eintegral[1] = (eintegral[1] + e[1]*deltaT);
      posp[0] = post[0];
      posp[1] = post[1];
      
      t = t + deltaT;
      
      j++;
      if(j == 100)
      {
        j = 0;
        pos[0] = posp[0] = post[0] = 0;
        pos[1] = posp[1] = post[1] = 0;
  
      }
      
       prevT = currT;
    }
   
    if (t > 1)
    {
      u[0] = (kpl*e[0]+kil*eintegral[0]);
      u[1] = (kpr*e[1]+kir*eintegral[1]);
      //u[0] = (kpl*e[0]+kil*eintegral[0] + kxl*(pos_m - post[0])/pos_m);
      //u[1] = (kpr*e[1]+kir*eintegral[1] + kxr*(pos_m - post[1])/pos_m);
    }
    
    i++;
    Serial.print("r ");
    Serial.print(rpst[1]);
    Serial.print("l ");
    Serial.println(rpst[0]);
    
    
    delay(10);
    }
}
*/

void readEncoder_l()
{
  int b = digitalRead(m[0].getENCA());
  int c;
  
  if (b > 0)
  {
    pos[0]++;
  }
  else
  {
    pos[0]--;
  }
}

void readEncoder_r()
{
  int b = digitalRead(m[1].getENCA());
  int c;
  
  if (b > 0)
  {
    pos[1]--;
  }
  else
  {
    pos[1]++;
  }
}
