# RT1 Assignment 2
The second Research Track 1 class project involves using a robot simulator in ROS (Robot Operating System) and building a very minimal user interface (User Interface).
## Installing and running
There is a requirement of ROS for this simulator which means there is a requirement of tools and libraries that help us in building robotic applications. This simulator smoothly runs on Noetic Release of ROS.<br />
In any case, this link has a list of all ROS releases.<br />
You should have created a workspace where you can develop your packages once you've installed ROS<br />
to run `ROS` on your pc.<br />
$ catkin_make<br />
to build the workspace. Then, to refresh the package list, run:<br />
$ rospack profile<br />
Once you have installed ROS and the package, run the following commands on different terminals:<br />
1 rosrun stage_ros stageros $(rospack find second_assignment)/world/my_world.world<br />
2 rosrun second_assignment controller_node<br />
3 rosrun second_assignment server_node<br />
4 rosrun second_assignment userinterface_node<br />
## Introduction
- The goal of the project was to get the robot to move inside a circuit (specifically, the Monza Circuit) without hitting any walls and to enable the user 
- control the robot's speed using a user interface.<br />
Following is the circuit. <br />
![image](https://user-images.githubusercontent.com/95746070/152133172-df0fcc6f-3949-43aa-9399-5e949c7125a3.png)<br />
That small dot at the bottom is the robot that is rotating clockwise. The lecturer had a lot of requests for the project:<br />
- Through the usage of a server, the node controlling the robot should additionally implement the ability to increase and decrease velocity.
- The Ul node should be waiting for human input at all times, which might be to increase or decrease the velocity, or to reset the robot to its initial condition.<br /> The commands are as follows:<br />
- 'w' to accelerate.
- 's' to decelerate.
- 'r' to reset the position of the robot.
## The logic behind the code
To meet the requirements, I decided to add three new nodes to the package, including one for the stage ros simulator. The following is the concept behind node communication:<br />
![image](https://user-images.githubusercontent.com/95746070/152137258-ebccd7b1-3b3a-4d85-8056-5aca66e3df77.png)<br />
As you've seen, it's not a difficult communication, but it does require some attention, particularly when using the server.<br />
## Briefly description
In a simplified way, the user will provide input to the Userinterface node that will either be good ('w' ,'s', 'r') or bad ('w' ,'s', 'r'), resulting in an error input on the console. This input will be processed by the server, which will release a float value that will be the acceleration value. The user interface will then send a specific message to the controller node, which will read the real acceleration value. The controller node will then communicate updated velocity information to the stage_ros node. Another responsibility assigned to the server node is to invoke a service from the ROS library that resets the robot's position.<br />
## Nodes and their logic
### Stage_ros node (Stage_ros package)
By using libstage, the stage_ros node covers the Stage 2-D multi-robot simulator. The stage_ros is a programme that replicates a world defined in a.world file. This file contains information about the world, including barriers (which are typically represented as a bitmap and utilised as a kind of background), robots, and other items. The following are the characteristics of the nodes:<br />
- Subscriptions<br />
            i. The node subscribes to the cmd_vel (geometry_msgs/Twist) topic, to manage the velocity of the robot.<br />
- Publishing<br />
    i. `odom` (nav_msgs/Odometry): odometry data of the model.<br />
    ii. `base_scan` (sensor_msgs/LaserScan): scans from the laser model. We will use this one.<br />
    iii. `base_pose_ground_truth` (nav_msgs/Odometry): the ground truth pose. We will use this one.<br />
    iv. `image` (sensor_msgs/Image): visual camera image.<br />
    v. `depth` (sensor_msgs/Image): depth camera image.<br />
    vi. `camera_info `(sensor_msgs/CameraInfo): camera calibration info.<br />
### Robotcontroller_node
The controller node has two primary functions: the first is to move the robot within the environment, and the second is to adjust the robot's velocity. The callback function and the node's main are used to execute these duties. The `main()` function uses the `init()` function to set up the node and subscribes to several topics.<br />
- Subscriptions<br />
i. base_scan (sensor_msgs/Laser_scan) which provides data about the laser that scans the surroundings of the robot. I will develop the laser code later.<br />
ii. usermessage (second_assignment/Usermessage)  provides the amount to add to the base velocity. <br />
- Publishing<br />
i. `cmd_vel geometry_msgs/Twist` which is the topic to change the linear and angular velocity's values of the robot.<br />
As previously stated, the controller node's primary duty is to move the robot about the environment. This is accomplished by subscribing to `base scan`, which allows the `ControllerCallback()` method to loop with `ros::spin()` inside the `main()` function.<br />
 I devised a simple method consisting of three if statements after making the ControllerCallback() function loop. This approach is very similar to the main() function in Assignment 1. <br />

Well here is the concept behind the movement; I used the same python algorithm that I used for the first assignment, where I first get the distance information from an array `ranges[i]` and divided them into threee arrays to get the minimum value of distance values of front, left and right. <br />


With these informations, the algorithm can work perfectly. Here in code (this is all inside the ControllerCallback()):<br />
```ruby
    if(minfront<1.5){
        if(minleft>minright){
            myvel.angular.z = 1;
            myvel.linear.x = 0.1;
        }
        else if(minright>minleft){
            myvel.angular.z = -1;
            myvel.linear.x = 0.1;
        }
    }
    else{
        myvel.linear.x = 1 + change;
        myvel.angular.z = 0;
    }
```
However, to better understand how ROS works, I preferred to focus on strengthening the node relationships rather than the algorithm itself.
Another crucial function is SpeedchangeCallback(), which reads the amount of velocity we want to add from the custom message.<br />
 ```ruby
void SpeedchangeCallback(const second_assignment::Usermessage::ConstPtr& speed){
    change = speed->uimsg;
}
```
Where `change` is a global variable that we use in the ControllerCallback() function.   
### Server node (second assignment package)
This node is required for the use of the Userinterface node and, as a result, the User Interface commands. As previously stated, the user has the option of pressing:<br />
    • 'w' to accelerate.<br />
    • 's' to decelerate.<br />
    • 'r' to reset the robot to its original place within the circuit <br />
      
     ` $ rosrun second_assignment server_node`<br />

When a user inserts a variable into the server via the Uiserinterface node, the server reads it and, depending on the input, sends something back as an output in the response. The SRV file is organised as follows:<br />
char input<br />
float 32 output<br />

As can be seen, the request contains a char and the response has a val. The value is obviously determined by the input and is assigned within the VelocityCallback() method. When the server service receives a request, it spins on the main. Here's the code:<br />
```ruby

bool VelocityCallback(second_assignment::Accelerator::Request &req, second_assignment::Accelerator::Response &res){
	if(req.input == 'w'){
		stuff += 0.1;
		req.input = 'x';
	}
	if(req.input == 's'){
		stuff -= 0.1;
		req.input = 'x';
	}
	if(req.input == 'r'){
		ros::service::call("/reset_positions", res_server);
	}
	res.output = acceleration;
	return true;
	}
```
  The char 'x' is only a placeholder to prevent the function from incrementing the value indefinitely. As you can see, when we get the request 'r,' we call the res server service /reset positions to reset the robot's position
 ### Userinterface node (second_assignment package)
The Userinterface node as the name can suggest is the core of the User Interface and it communicates with both the nodes, server and controller. It gets the input by the terminal and sends a request to the server, which will respond still to the Userinterface node. This happens all inside the InputCallback() function.<br />
  The node publishes to the Usermessage (second_assignment/Usermessage) topic that Robotcontroller.cpp will read. <br />
  As a result, the Userinterface node receives the input, sends a request to the server, waits for a response, and then transmits the acceleration value to the controller node.<br />
  
  ## Conclusion and Improvements
  It was a wonderful experience to use ROS. It helped us to have a better understanding about the nodes and communicate between the nodes.<br />
  
  Various enhancements can be made.<br />
    • First, the algorithm controlling the robot's direction and speed may be improved because, as you can see when the robot is running, it has difficulty turning in the circuit. <br />
    • Second, the user interface offers simple outputs and graphics. It would be easier to use and appreciate the UI node if it had more aesthetic values.<br />
    
    
    
