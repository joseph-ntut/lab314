# **driver launch**
roslaunch autolabor_pro1_driver driver.launch          
# **joystick launch**
roslaunch demo_pioneer pioneer-teleop.launch          
# **priority of moving**
roslaunch yocs_cmd_vel_mux standalone.launch           
# **led strip**   
rosrun p3dx_navigation Neopixel                        
# **connect to arduino** 
rosrun rosserial_python serial_node.py /dev/ttyACM0   





