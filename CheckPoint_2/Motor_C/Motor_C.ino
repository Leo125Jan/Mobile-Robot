#include <ros.h>
#include <util/atomic.h>
#include <std_msgs/Int64MultiArray.h>

// Define the motors
#define NMOTORS 2

const int ENCA[] = {4,12};
const int ENCB[]= {2,3};
const int pwmPin[] = {5,6};
const int inp1[] = {8,10};
const int inp2[] = {9,11};

// Variable
int pos_last[] = {0,0};
volatile int posi[] = {0};

float v[] = {0.0};
float v_last[] = {0.0};

float prevT = 0;
float integral[] = {0,0};

// Gain
float kp = 5.0;
float ki = 5.0;

//Ros init
ros::NodeHandle  nh;
int feedback[3];

void mcCb(const std_msgs::Int64MultiArray &msg)
{
  feedback[0] = msg.data[0];
  feedback[1] = msg.data[1];
  feedback[2] = msg.data[2];
}

ros::Subscriber<std_msgs::Int64MultiArray> mc_sub("/cmd", &mcCb);

void setup()
{
  nh.initNode();
  nh.subscribe(mc_sub);
  
  //Pin assign
  for (int i = 0; i < NMOTORS;i++)
  {
    pinMode(ENCA[i],INPUT);
    pinMode(ENCB[i],INPUT);
    pinMode(pwmPin[i],OUTPUT);
    pinMode(inp1[i],OUTPUT);
    pinMode(inp2[i],OUTPUT);
    
    analogWrite(pwmPin[i],0);
    digitalWrite(inp1[i],LOW);
    digitalWrite(inp2[i],LOW);
    
    attachInterrupt(i, readEncoder, RISING);
  }
}

void loop()
{
  nh.spinOnce();
  
  
  if (feedback[0] == 1)
  {
    for (int i = 0; i < NMOTORS;i++)
    {
      setMotor(feedback[i+1], pwmPin[i], inp1[i], inp2[i]);
    }
  }
  
  while (feedback[0] == 2)
  {
    nh.spinOnce();
    if (feedback[0] == 1)
    {
      break;
    }
    
    int pos[] = {0,0};
    noInterrupts();
    pos[0] = posi[0];
    pos[1] = posi[1];
    interrupts();
    
    float rpm[] = {((pos[0] - pos_last[0])/1920)*100, ((pos[1] - pos_last[1])/1920)*100};
    float e[] = {(rpm[0]-rpm[1]), (rpm[1]-rpm[0])};
    
    float currT = micros();
    float deltaT = ((float) (currT-prevT))/1.0e6;
    integral[0] = (integral[0] + e[0]*deltaT);
    integral[1] = (integral[1] + e[1]*deltaT);
    
    float u[] = {(kp*e[0]+ki*integral[0]), (kp*e[1]+ki*integral[1])};
    
    for (int i = 0; i < NMOTORS;i++)
    {
      setMotor(u[i], pwmPin[i], inp1[i], inp2[i]);
    }
    
    delay(10);
  }
  
  delay(10);
}

void setMotor(int pwmVal, int pwmPin, int in1, int in2)
{
  if (pwmVal > 0)
  {
    //Positive
    digitalWrite(in1,HIGH);
    digitalWrite(in2,LOW);
    analogWrite(pwmPin,pwmVal);
  }
  else if (pwmVal < 0)
  {
    //Negative
    digitalWrite(in1,LOW);
    digitalWrite(in2,HIGH);
    analogWrite(pwmPin,-pwmVal);
  }
  else
  {
    digitalWrite(in1,LOW);
    digitalWrite(in2,LOW);
  }  
}

void readEncoder()
{
  int b[] = {digitalRead(ENCB[1]), digitalRead(ENCB[0])};
  int c[] = {0};
  
  if (b[0] > 0)
  {
    c[0] = 1;
  }
  else
  {
    c[0] = -1;
  }
  
  if (b[1] > 0)
  {
    c[1] = 1;
  }
  else
  {
    c[1] = -1;
  }
  
  for (int i = 0; i < NMOTORS;i++)
  {
    posi[i] = posi[i] + c[i];
  }
}
