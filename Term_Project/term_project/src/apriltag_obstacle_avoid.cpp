#include <unistd.h>
#include <iostream>
#include <ros/ros.h>
#include <wiringPi.h>
#include <std_msgs/Int64.h>

using namespace std;

//Ultrasonic Sensor Pin
#define l_tr 7
#define l_ec 0
#define r_tr 2
#define r_ec 3

void send_trigger_pulse(void)
{
	digitalWrite(l_tr, LOW);
	ros::Duration(5.0/1.0e6).sleep();
	digitalWrite(l_tr, HIGH);
	ros::Duration(10.0/1.0e6).sleep();
	digitalWrite(l_tr, LOW);
}

void wait_for_echo(bool value, int count)
{
	int count_ = count;

	while(digitalRead(l_ec) != value && count_ > 0)
	{
		count_--;
	}
}

float get_distance(void)
{
	send_trigger_pulse();
	
	wait_for_echo(1, 5000*4);
	ros::Time begin = ros::Time::now();
	wait_for_echo(0, 5000*4);
	ros::Time finish = ros::Time::now();

	/*
	while (digitalRead(l_ec) == 0)
	{
		begin = ros::Time::now();
	}

	while (digitalRead(l_ec) == 1)
	{
		finish = ros::Time::now();
	}
	*/

	ros::Duration duration = finish - begin;
	float cm = (duration.toSec()*0.5)*340*100;

	return cm;
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "apriltag_obstacle_avoid");
	ros::NodeHandle nh;

	//std_msgs::Int64 input;
	//ros::Publisher mt_pub = nh.advertise<std_msgs::Int64>("/Command",1000);

	// WiringPi setup
	wiringPiSetup();

	pinMode(l_tr, OUTPUT);
	pinMode(l_ec, INPUT);
	pinMode(r_tr, OUTPUT);
	pinMode(r_ec, INPUT);

	ROS_INFO("Start Distance Detect");
	/*
	while (mt_pub.getNumSubscribers() == 0)
	{
		ros::Duration(1/1000).sleep();
	}
	*/
	while(ros::ok())
	{
		float d = get_distance();

		if (d > 15)
		{
			ROS_INFO("Distance: %f", d);

			//input.data = 1;
		}
		else if (d < 15)
		{
			ROS_INFO("Distance: %f", d);
			
			//input.data = 0;
		}
		
		//mt_pub.publish(input);
		ros::Duration(100/1000).sleep();
	}
}
