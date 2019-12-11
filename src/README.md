
roslaunch autolabor_pro1_driver driver.launch          (DRIVER)

roslaunch demo_pioneer pioneer-teleop.launch           (JOYSTICK)

roslaunch yocs_cmd_vel_mux standalone.launch           (PRIORITY_OF_MOVING)  
   
rosrun p3dx_navigation Neopixel                        (LED_STRIP)
 
rosrun rosserial_python serial_node.py /dev/ttyACM0    (ARDUINO)





