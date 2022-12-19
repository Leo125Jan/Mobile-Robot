#include <ros.h>
#include <std_msgs/Int64.h>

#include "Motor.h"

// Motor
Motor m[] = {Motor(4,2,6,10,11,0), Motor(12,3,5,8,9,1)};

ros::NodeHandle  nh;
std_msgs::Int64 feedback;

void obtCb(const std_msgs::Int64 &msg)
{
  feedback= msg;
}

ros::Subscriber<std_msgs::Int64> obt_sub("/Command", &obtCb);

void setup()
{
  nh.initNode();
  nh.subscribe(obt_sub);
  
  Stop();
}

void loop()
{
  nh.spinOnce();
  
  if (feedback.data == 1)
  {
    Forward();
  }
  else if (feedback.data == 0)
  {
    Stop();
  }
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

void Forward()
{
  m[0].setMotor(100, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].setMotor(100, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}

void Stop()
{
  m[0].setMotor(0, m[0].getpwmPin(), m[0].getinp1(), m[0].getinp2());
  m[1].setMotor(0, m[1].getpwmPin(), m[1].getinp1(), m[1].getinp2());
}
