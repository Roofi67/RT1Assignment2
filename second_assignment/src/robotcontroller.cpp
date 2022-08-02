#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Twist.h"
#include "turtlesim/Spawn.h"
#include "second_assignment/Accelerator.h"
#include "second_assignment/Usermessage.h"
#include "std_srvs/Empty.h"



ros::Publisher pub;
geometry_msgs::Twist myvel;
float change = 0;
float speed = 0;

void ControllerCallback(const sensor_msgs::LaserScan::ConstPtr& position)
{
	float scan[721];
	for(int i=0; i<721; i++){
	scan[i]=position -> ranges[i];
	
	}
	
	
	//for minvalue of  right 
	float minfront, minright, minleft;
	
	minright=100;
	for(int j = 0; j < 100; j++){
	if(scan[j]<=minright){
	minright = scan[j];
	}
	}
	
	
	//for minvalue of front
	minfront=100;
	for(int k = 310; k < 410; k++){
	if(scan[k]<=minfront){
	minfront = scan[k];
	}
	}
	
	
	//for minvalue of left
	minleft=100;
	for(int l = 620; l < 720; l++){
	if(scan[l]<=minleft){
	minleft = scan[l];
	}
	}
	
	
	if(minfront<1.5){
	if(minright>minleft){
	myvel.angular.z=-1;
	myvel.linear.x=0.3;
	}
	
	else if(minleft>minright){
	myvel.angular.z=1;
	myvel.linear.x=0.3;
	}
	}
	
	
	else{
	myvel.linear.x = 1 + change;
	myvel.angular.z=0;
	}
	
	
	pub.publish(myvel);
	
}

void SpeedchangeCallback(const second_assignment::Usermessage::ConstPtr& speed){
	change = speed->uimsg;
	}


int main( int argc, char **argv){
	ros::init(argc, argv, "robotcontroller");
	ros::NodeHandle nh;
	
	pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",1);
	
	ros::Subscriber sub = nh.subscribe("/base_scan",1,ControllerCallback);
	ros::Subscriber sub2 = nh.subscribe("/message", 1, SpeedchangeCallback);
	
	ros::spin();
	return 0;
}
