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

