#include <ros/ros.h>
#include <std_msgs/Int64MultiArray.h>

using namespace std;

int main(int argc, char **argv)
{
	// Node initialization// Pub & Sub
	ros::init(argc, argv, "MC");
	ros::NodeHandle nh;

	// Pub
	ros::Publisher mc_pub = nh.advertise<std_msgs::Int64MultiArray>("/cmd",100);

	//Variable
	std_msgs::Int64MultiArray msg;
	ros::Rate loop_rate(1);
	int temp = 0;

	ROS_INFO("Start Task");

	while(mc_pub.getNumSubscribers() == 0)
	{
		loop_rate.sleep();

		msg.data.push_back(0);
		msg.data.push_back(0);
		msg.data.push_back(0);
		
		loop_rate.sleep();
		mc_pub.publish(msg);
		msg.data.clear();
	}

	while (ros::ok())
	{
		cout << "User's control mode(1: manual, 2: auto) is: ";
		cin >> temp;
		msg.data.push_back(temp);

		cout << "User's pwm for left wheel: ";
		cin >> temp;
		msg.data.push_back(temp);

		cout << "User's pwm for right wheel: ";
		cin >> temp;
		msg.data.push_back(temp);

		loop_rate.sleep();
		mc_pub.publish(msg);
		loop_rate.sleep();

		msg.data.clear();
	}

	ros::shutdown();
	return 0;
}
