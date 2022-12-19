#include <unistd.h>
#include <ros/ros.h>
#include <std_msgs/Int64.h>

using namespace std;

// Callback
std_msgs::Int64 feedback;

void rpinumCallback(const std_msgs::Int64::ConstPtr& msg)
{
  feedback.data = 2*msg->data;
  int temp = msg->data;
  ROS_INFO("Message from RPI is %d", temp);
}

int main(int argc, char **argv)
{
  // Node initialization
  ros::init(argc, argv, "Arduino");
  ros::NodeHandle nh;

  // Pub & Sub
  ros::Publisher ard_pub = nh.advertise<std_msgs::Int64>("/ard_number", 100);
  ros::Subscriber ard_sub = nh.subscribe("/rpi_number", 100, rpinumCallback);

  // Variable
  ros::Rate loop_rate(10);
  feedback.data = 0;

  // Main function
  while (ros::ok())
  {
    ros::spinOnce();

    ard_pub.publish(feedback);

    loop_rate.sleep();
  }
}