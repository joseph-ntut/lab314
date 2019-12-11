sudo chmod 777 -R /dev/ttyUSB0

sudo chmod a+rw /dev/ttyACM0

roslaunch autolabor_pro1_driver driver.launch

roslaunch demo_pioneer pioneer-teleop.launch

roslaunch yocs_cmd_vel_mux standalone.launch 

rosrun p3dx_navigation Neopixel

rosrun rosserial_python serial_node.py /dev/ttyACM0





