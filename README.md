# TBD
# Autonomous Driving: Lidar Mapping and Positioning

This ROS 2 Package was developed as part of the Capstone Project at Carleton University for:
- Aymaan Newaz
- Basel Syed
- Mohammad Jemal
- Mohamed Selim

The project was supervised by:
- Professor Chao Shen
- Professor Richard Dansereau

## Acknowledgments:
We, the students, would like to take this chance to acknowledge the efforts of our supervising professors in guiding us throughout the development process of this project. Their mentorship and support have helped us overcome many obstacles that came up throughout the eight months of our work on this project. We are sincerely grateful for their patience and efforts that have benefited our professional growth. 

## Introduction:
This package is configured to work with ROS 2 Foxy.
It has two main components:
- Simulation
- Physical Implementation

Our project encompasses three key objectives: mapping, localization, and automation. 

In terms of mapping, our robot is capable of mapping its surroundings with quantifiable precision and speed, and in parallel with the movement of the robot. To achieve this, our system can visualize the surrounding environment using sensor data from a RPLidar A3.

For localization, our aim is to fuse data from two distinct sources. LiDAR will contribute to object detection and the creation of environment maps, while wheel encoders will provide velocity and distance information. The integration of these data sources allows the system to keep track of its current state and calculate its next course of action.
In line with the concept of automation, the robot is able to navigate through its surroundings while avoiding any obstacles. After being provided with a goal pose, it determines the best path to follow and updates that path dynamically as new information is made available to it. 

Video:


https://github.com/mselim6022/slambot_pttype/assets/97752834/dc365cc0-437e-4817-8234-7dcd0e9261c9


## Installation:
1- Download this package and extract it into your workspace folder

2- Run ``` colcon build --symlink-install ``` to add the package to your ROS 2 installation

## Dependencies:
TBD
Nav2:

SLAM Toolbox:

GMapping:

Twist Mux:

Teleop Twist Keyboard:

Gazebo ROS:

ROS 2 Control:

DiffDriveArduino:

RPLidar ROS:

Serial:


## Bringup:
### Simulation:
TBD
### Physical Robot Implementation:

1- In order to start up the robot state publisher:
``` ros2 launch slambot_pttype launch_robot.launch.py ```
This includes operating the physical LiDAR and the Twist Mux package

2- In order to begin mapping, start one of the two supported packages:
KartoSLAM (from SLAM Toolbox):
``` ros2 launch slam_toolbox online_async_launch.py params_file:=/path/to/workspace/dev_ws/src/slambot_pttype/config/mapper_params_online_async.yaml use_sim_time:=false ```
GMapping:
``` ros2 launch slam_gmapping slam_gmapping.launch.py use_sim_time:=false ```

3- To run the navigation stack, use:
``` ros2 launch nav2_bringup navigation_launch.py use_sim_time:=false ```
