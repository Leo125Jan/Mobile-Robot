#include <unistd.h>
#include <ros/ros.h>
#include <std_msgs/Int64.h>

using namespace std;

// Callback
/*
void ardnumCallback(const std_msgs::Float64::ConstPtr& msg)
{
	float temp = msg->data;
	ROS_INFO("Message from Arduino is %f", temp);
}
*/
int main(int argc, char **argv)
{
	// Node initialization
	ros::init(argc, argv, "RPI");
	ros::NodeHandle nh;

	// Pub & Sub
	ros::Publisher rpi_pub = nh.advertise<std_msgs::Int64>("/Command", 100);
	//ros::Subscriber rpi_sub = nh.subscribe("/ard_number", 1, ardnumCallback);

	// Variable
	float count = 0.0;
	std_msgs::Int64 input;
	ros::Rate loop_rate(10);

	ROS_INFO("Start Task");
	
	while(rpi_pub.getNumSubscribers() == 0)
	{
		ROS_INFO("No sub");
		loop_rate.sleep();
	}
    
	// Main function
	while (ros::ok())
	{
		//ROS_INFO("User's input is %d", count);
		cout << "Input is: ";
		cin >> input.data;

		//input.data = count;

		loop_rate.sleep();
		rpi_pub.publish(input);
		loop_rate.sleep();
		ros::spinOnce();
		loop_rate.sleep();
		
		//count++;
	}
	ros::shutdown();
	return 0;
}
