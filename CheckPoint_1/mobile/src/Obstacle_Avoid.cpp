#include "ros/ros.h"
#include <iostream>
#include <wiringPi.h>

# define ledPin 3

using namespace std;

int main(int argc, char **argv)
{
	// Pin init
	wiringPiSetup();
	pinMode(ledPin, INPUT);
	int read_data = analogRead(ledPin);

	// Ros init
	ros::init(argc, argv, "Obstacle Avoidance");
	ros::NodeHandle nh;

	ros::Rate loop_rate(10);
	
	while(ros::ok())
	{
		ROS_INFO("Value: %d", read_data);
		loop_rate.sleep();
	}

	ros::shutdown();
	return 0;
}
