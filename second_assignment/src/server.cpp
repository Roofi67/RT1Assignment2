#include "ros/ros.h"
#include "second_assignment/Accelerator.h"
#include "std_srvs/Empty.h"

float acceleration=0;
std_srvs::Empty restart;


 bool VelocityCallback(second_assignment::Accelerator::Request &req, second_assignment::Accelerator::Response &res){

	if(req.input == 'w'){
	acceleration += 1;
	req.input = 'x';
	}
	
	if(req.input == 's'){
	acceleration -= 1;
	req.input = 'x';
	}
	
	if(req.input == 'r'){
		ros::service::call("/reset_positions", restart);
	}
	
	res.output = acceleration;
	return true;
	}

int main(int argc, char **argv){
	ros::init(argc, argv, "server");
	ros::NodeHandle nh;
	
	ros::ServiceServer service = nh.advertiseService("/Accelerator",VelocityCallback);
	
	ros::spin();
	return 0;
	}
