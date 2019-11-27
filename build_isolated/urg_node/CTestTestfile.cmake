# CMake generated Testfile for 
# Source directory: /home/ntut-mit/catkin_ws/src/autolabor/src/driver/lidar/urg_node
# Build directory: /home/ntut-mit/catkin_ws/build_isolated/urg_node
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(_ctest_urg_node_roslint_package "/home/ntut-mit/catkin_ws/build_isolated/urg_node/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/ntut-mit/catkin_ws/build_isolated/urg_node/test_results/urg_node/roslint-urg_node.xml" "--working-dir" "/home/ntut-mit/catkin_ws/build_isolated/urg_node" "--return-code" "/opt/ros/melodic/share/roslint/cmake/../../../lib/roslint/test_wrapper /home/ntut-mit/catkin_ws/build_isolated/urg_node/test_results/urg_node/roslint-urg_node.xml make roslint_urg_node")
add_test(_ctest_urg_node_roslaunch-check_launch_urg_lidar.launch "/home/ntut-mit/catkin_ws/build_isolated/urg_node/catkin_generated/env_cached.sh" "/usr/bin/python2" "/opt/ros/melodic/share/catkin/cmake/test/run_tests.py" "/home/ntut-mit/catkin_ws/build_isolated/urg_node/test_results/urg_node/roslaunch-check_launch_urg_lidar.launch.xml" "--return-code" "/usr/bin/cmake -E make_directory /home/ntut-mit/catkin_ws/build_isolated/urg_node/test_results/urg_node" "/opt/ros/melodic/share/roslaunch/cmake/../scripts/roslaunch-check -o '/home/ntut-mit/catkin_ws/build_isolated/urg_node/test_results/urg_node/roslaunch-check_launch_urg_lidar.launch.xml' '/home/ntut-mit/catkin_ws/src/autolabor/src/driver/lidar/urg_node/launch/urg_lidar.launch' ")
subdirs("gtest")
