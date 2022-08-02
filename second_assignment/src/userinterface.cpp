#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "second_assignment/Accelerator.h"
#include "second_assignment/Usermessage.h"
#include "std_srvs/Empty.h"

ros::ServiceClient client1;
ros::Publisher pub;


void InputCallback(){

	second_assignment::Accelerator srv;
	
	char press;
	printf("Press W to accelerate\n Press S to deaccelerate\n Press R to reset");
	std::cin>>press;
	
	srv.request.input = press;
	
	client1.waitForExistence();
	client1.call(srv);
	
	second_assignment::Usermessage m;
	m.uimsg = srv.response.output;
	pub.publish(m);
	}
	

int main (int argc, char **argv){

	ros::init(argc, argv, "Userinterface");
	ros::NodeHandle nh;
	
	pub = nh.advertise<second_assignment::Usermessage>("/message",10);
	
	client1 = nh.serviceClient<second_assignment::Accelerator>("/Accelerator");
	
	while(ros::ok()){
	InputCallback();
	ros::spinOnce();
	}
	return 0;
	}
