#include <ros.h>
#include <std_msgs/Float64.h>

ros::NodeHandle  nh;

std_msgs::Float64 feedback;

void rpinumCb(const std_msgs::Float64& msg)
{
  feedback.data = 2*(msg.data);
}

ros::Publisher ard_pub("/ard_number", &feedback);
ros::Subscriber<std_msgs::Float64> ard_sub("/rpi_number", &rpinumCb);

void setup()
{
  nh.initNode();
  nh.advertise(ard_pub);
  nh.subscribe(ard_sub);
}

void loop()
{
  nh.spinOnce();
  ard_pub.publish(&feedback);
  delay(100);
}
