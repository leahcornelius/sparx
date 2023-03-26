# Sparx
A ROS 2 package for my robot, Sparx. The sparx_core package runs on a raspberry pi 4 on the robot, designed to run on Ubuntu 22.04 64 bit with ROS 2 Humble. To be able to use a picamera with rpi ubuntu, i had to modify /boot/firmware/config.txt (using usercfg.txt didn't work) and add the following lines:
```bash
start_x=1
gpu_mem=128
```
The picamera should now be available as /dev/video0 (or /dev/video1) and can be used like a usual V4L camera, i use RosUsbCam to publish the camera feed to ROS and cv_bridge to convert the ROS image frames into open cv BGR images for processing. Ensure both `ros_humble_usb_cam` and `ros_image_transport_plugins` are installed before building, which will enable compression of the images.

## Build
# Linux
Assumming ROS 2 Humble is installed along with above prerequisites, clone this repo into the src folder of your workspace and build the packages using colcon: (replace ~/ros2_ws with the path to your workspace)
```bash
source /opt/ros/humble/setup.bash
cd ~/ros2_ws/src
git clone https://github.com/leocornelius/sparx.git
cd ~/ros2_ws
colcon build --symlink-install
```

# Non-Linux
No clue, should probably work with enough vodka and patience but i value my sanity too much to fuck around with windows. Dangerous shit. GLHF if you try, let me know how it goes in the issues.

# Run
Open another terminal and source both the ROS 2 setup script and the overlay setup script:
```bash
source /opt/ros/humble/setup.bash # Only needed if you haven't already sourced it/ is not in your .bashrc
cd ~/ros2_ws/
source install/setup.bash
```

Then run the launch file:
```bash
ros2 launch sparx_core robot.launch.py
```


