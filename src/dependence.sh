#!/bin/bash

echo "***********************************************************************"
echo "                      Installing jsk-recognition-msgs"
sudo apt-get install ros-melodic-jsk-recognition-msgs
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing ceres-solver"
sudo apt-get install ros-melodic-ceres-solver
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing nmea-msgs"
sudo apt-get install ros-melodic-nmea-msgs
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing hector-map-tools"
sudo apt-get install ros-melodic-hector-map-tools
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing qt4"
sudo apt-get install qt4-qmake
sudo apt-get install qt4-bin-dbg
sudo apt-get install qt4-default
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing random-numbers"
sudo apt-get install ros-melodic-random-numbers
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing jsk-rviz-plugins"
sudo apt-get install ros-melodic-jsk-rviz-plugins
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing map-server"
sudo apt-get install ros-melodic-map-server
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing  libpcap-dev"
sudo apt-get install libpcap-dev
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing velodyne-pointcloud"
sudo apt-get install ros-melodic-velodyne-pointcloud
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing tf2-sensor-msgs"
sudo apt-get install ros-melodic-tf2-sensor-msgs
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing costmap-converter"
sudo apt-get install ros-melodic-costmap-converter
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing ddynamic_reconfigure"
sudo apt-get install ros-melodic-ddynamic-reconfigure
echo "                      Installation complete"
echo "***********************************************************************"

'echo "***********************************************************************"
echo "                      Installing rtabmap"
sudo apt-get install ros-melodic-rtabmap-ros
sudo apt-get remove ros-melodic-rtabmap ros-melodic-rtabmap-ros
cd ~
git clone https://github.com/introlab/rtabmap.git rtabmap
cd rtabmap/build
cmake ..
make
sudo make install
cd ~/catkin_ws
git clone https://github.com/introlab/rtabmap_ros.git src/rtabmap_ros
catkin_make
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing imu_filter"
sudo apt-get install ros-melodic-imu-filter-madgwick
echo "                      Installation complete"
echo "***********************************************************************"

echo "***********************************************************************"
echo "                      Installing realsense_camera"
sudo apt-key adv --keyserver keys.gnupg.net --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE || sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE
sudo add-apt-repository "deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo bionic main" -u
sudo apt-get install librealsense2-dkms
sudo apt-get install librealsense2-utils
sudo apt-get install librealsense2-dev
sudo apt-get install librealsense2-dbg
echo "                      Installation complete"
echo "***********************************************************************"'














