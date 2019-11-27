# -*- coding: utf-8 -*-
from __future__ import print_function
import argparse
import os
import stat
import sys

# find the import for catkin's python package - either from source space or from an installed underlay
if os.path.exists(os.path.join('/opt/ros/melodic/share/catkin/cmake', 'catkinConfig.cmake.in')):
    sys.path.insert(0, os.path.join('/opt/ros/melodic/share/catkin/cmake', '..', 'python'))
try:
    from catkin.environment_cache import generate_environment_script
except ImportError:
    # search for catkin package in all workspaces and prepend to path
    for workspace in "/home/ntut-mit/catkin_ws/devel_isolated/random_numbers;/home/ntut-mit/catkin_ws/devel_isolated/ps3joy;/home/ntut-mit/catkin_ws/devel_isolated/path_server;/home/ntut-mit/catkin_ws/devel_isolated/gmapping;/home/ntut-mit/catkin_ws/devel_isolated/openslam_gmapping;/home/ntut-mit/catkin_ws/devel_isolated/navigation;/home/ntut-mit/catkin_ws/devel_isolated/marvelmind;/home/ntut-mit/catkin_ws/devel_isolated/amcl;/home/ntut-mit/catkin_ws/devel_isolated/map_server;/home/ntut-mit/catkin_ws/devel_isolated/location_fusion;/home/ntut-mit/catkin_ws/devel_isolated/laser_proc;/home/ntut-mit/catkin_ws/devel_isolated/kinect2_viewer;/home/ntut-mit/catkin_ws/devel_isolated/kinect2_calibration;/home/ntut-mit/catkin_ws/devel_isolated/kinect2_bridge;/home/ntut-mit/catkin_ws/devel_isolated/kinect2_registration;/home/ntut-mit/catkin_ws/devel_isolated/joystick_drivers;/home/ntut-mit/catkin_ws/devel_isolated/joy_to_twist;/home/ntut-mit/catkin_ws/devel_isolated/joy;/home/ntut-mit/catkin_ws/devel_isolated/image_view;/home/ntut-mit/catkin_ws/devel_isolated/image_rotate;/home/ntut-mit/catkin_ws/devel_isolated/image_publisher;/home/ntut-mit/catkin_ws/devel_isolated/image_proc;/home/ntut-mit/catkin_ws/devel_isolated/image_pipeline;/home/ntut-mit/catkin_ws/devel_isolated/iai_kinect2;/home/ntut-mit/catkin_ws/devel_isolated/fake_localization;/home/ntut-mit/catkin_ws/devel_isolated/depth_image_proc;/home/ntut-mit/catkin_ws/devel_isolated/cartographer_rviz;/home/ntut-mit/catkin_ws/devel_isolated/cartographer_ros;/home/ntut-mit/catkin_ws/devel_isolated/cartographer_ros_msgs;/home/ntut-mit/catkin_ws/devel_isolated/camera_calibration;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_test_launch;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_simulation_object;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_simulation_stage;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_simulation_location;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_simulation_lidar;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_simulation_base;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_pro1_driver;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_navigation_launch;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_keyboard_control;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_description;/home/ntut-mit/catkin_ws/devel_isolated/autolabor_canbus_driver;/home/ntut-mit/catkin_ws/devel_isolated/ah100b;/opt/ros/melodic".split(';'):
        python_path = os.path.join(workspace, 'lib/python2.7/dist-packages')
        if os.path.isdir(os.path.join(python_path, 'catkin')):
            sys.path.insert(0, python_path)
            break
    from catkin.environment_cache import generate_environment_script

code = generate_environment_script('/home/ntut-mit/catkin_ws/devel_isolated/path_rviz_plugin/env.sh')

output_filename = '/home/ntut-mit/catkin_ws/build_isolated/path_rviz_plugin/catkin_generated/setup_cached.sh'
with open(output_filename, 'w') as f:
    #print('Generate script for cached setup "%s"' % output_filename)
    f.write('\n'.join(code))

mode = os.stat(output_filename).st_mode
os.chmod(output_filename, mode | stat.S_IXUSR)
