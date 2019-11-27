# CMake generated Testfile for 
# Source directory: /home/ntut-mit/catkin_ws/src/autolabor/src/driver/joystick/joystick_drivers/wiimote
# Build directory: /home/ntut-mit/catkin_ws/build_isolated/wiimote
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_wiimote_roslint_package "/home/ntut-mit/catkin_ws/build_isolated/wiimote/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/ntut-mit/catkin_ws/build_isolated/wiimote/test_results/wiimote/roslint-wiimote.xml" "--working-dir" "/home/ntut-mit/catkin_ws/build_isolated/wiimote" "--return-code" "/opt/ros/melodic/share/roslint/cmake/../../../lib/roslint/test_wrapper /home/ntut-mit/catkin_ws/build_isolated/wiimote/test_results/wiimote/roslint-wiimote.xml make roslint_wiimote")
subdirs("gtest")
