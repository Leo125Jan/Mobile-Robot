# Mobile-Robot

## About The Project 
These are checkpoint practices of class Mobile Robots.  

## Built With
* Ubuntu mate 18.04
* Arduino

## Getting Started
### Configuration

1. Installing and setup ROS.
2. Installing Arduino IDE.

## Usage

### Check Point 1

* Purpose  
　　Setting the development environment for robot and using ROS system. First, you will need to produce the image for your raspberry pi and install ROS in it. Then use rosserial package to communicate between Raspberry pi and Arduino.

* Result

1. Check ID Address

<a><img src="https://user-images.githubusercontent.com/98295556/208383705-a7a209de-800a-4493-a942-3d1ad8d291dc.png" 
width="640" height="360" border="10" /></a>

2. Remote Connection By MobaXterm

<a><img src="https://user-images.githubusercontent.com/98295556/208384269-9a4a3088-87eb-42ba-9a5f-2facadeef601.png" 
width="640" height="360" border="10" /></a>

3. rqt_plot of node between Rpi and Arduino

<a><img src="https://user-images.githubusercontent.com/98295556/208384468-406d533e-f587-46f0-bf52-1d1b2f6e70e3.png" 
width="640" height="360" border="10" /></a>

4. Communication Result

<a><img src="https://user-images.githubusercontent.com/98295556/208384582-ad84d8b8-321d-49fd-9d16-13034f623efc.png" 
width="640" height="360" border="10" /></a>

### Check Point 2

* Purpose  
　　Motion Control of basic DC motors by encoder with Raspberry Pi and Arduino.

* Result

<a href="http://www.youtube.com/watch?feature=player_embedded&v=--dSvMMTLqc" target="_blank"><img src="http://img.youtube.com/vi/--dSvMMTLqc/0.jpg" 
width="480" height="270" border="10" /></a>

### Check Point 3

* Purpose  
　　The purpose of this checkpoint is to make sure you can control your robot to move in the arena. The mobile robot needs to detect an obstacle in front of it and take action to avoid the obstacle in order to continue its motion.  
　　Finally, your robot can find the assigned target. In this checkpoint, the target is a ring of LED lights.

* Result

<a href="http://www.youtube.com/watch?feature=player_embedded&v=mCQ8oAgoN5k" target="_blank"><img src="http://img.youtube.com/vi/mCQ8oAgoN5k/0.jpg" 
width="480" height="270" border="10" /></a>

### Check Point 4

* Purpose  
　　The purpose of this checkpoint has two goals. First, making sure that our robot can detect a beacon signal and move towards it. Second, combine all the function as Obstacle Avoidance, Hockey Seeking (Light-ball detection) and Goal Seeking (IR signal receiving and moving toward goal) together for robot hockey contest.  
　　For this assignment, two infrared diodes will be set up at opposite ends of an arena. Each diode will be emitting light modulated at 38 𝐾𝐻𝑧, but their pulse width are different when received by IR receiver module.
　　We need to demonstrate our robot’s capabilities under relaxed conditions with no other robots in the arena. The arena will be the actual contest arena.

* Result

<a href="http://www.youtube.com/watch?feature=player_embedded&v=NBi9Y4w6E1s" target="_blank"><img src="http://img.youtube.com/vi/NBi9Y4w6E1s/0.jpg" 
width="480" height="270" border="10" /></a>

### Term Project - Follower Robot with Obstacle Aovidance

* Abstract  
　　We make a follower robot with the functionality of obstacle avoidance. The robot will follow the target and avoid the obstacle on the road at the same time. Implemented by transforming the requirement of tracking and avoidance into the constraints of CBF.

  ![CBF_2](https://github.com/Leo125Jan/Mobile-Robot/assets/98295556/8be0cdce-25a5-4efa-a572-a2a62ad4c9e0)

  ![CBF_1](https://github.com/Leo125Jan/Mobile-Robot/assets/98295556/980cd5eb-6288-46fe-aece-2b1a890ebf07)

* Built with

    * D435 Camera
    * Rplidar

* Acknowledgement  
[AMNshaw/mobile_robot](https://github.com/AMNshaw/mobile_robot)

* Result  

* Follower Robot  

<a href="http://www.youtube.com/watch?feature=player_embedded&v=LJ-HknhwBtk" target="_blank"><img src="http://img.youtube.com/vi/LJ-HknhwBtk/0.jpg" 
width="480" height="270" border="10" /></a>

* Obstacle Avoidance    

<a href="http://www.youtube.com/watch?feature=player_embedded&v=0nmB6hvIK1I" target="_blank"><img src="http://img.youtube.com/vi/0nmB6hvIK1I/0.jpg" 
width="480" height="270" border="10" /></a>

* Follower Robot with Obstacle Avoidance  
    
<a href="http://www.youtube.com/watch?feature=player_embedded&v=hiY8583SR0E" target="_blank"><img src="http://img.youtube.com/vi/hiY8583SR0E/0.jpg" 
width="480" height="270" border="10" /></a>
