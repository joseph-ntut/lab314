# pev_localization
localization stack for pev
## Setup stepts
1. Install system dependencies
    ```
    sudo apt-get install libnlopt-dev freeglut3-dev qtbase5-dev libqt5opengl5-dev libssh2-1-dev libarmadillo-dev libpcap-dev gksu libgl1-mesa-dev libglew-dev python-wxgtk3.0 software-properties-common libmosquitto-dev libyaml-cpp-dev python-flask python-requests
    ```
2. Install ros dependencies
    ```
    sudo apt-get install ros-kinetic-jsk-recognition-msgs ros-kinetic-pcl-ros ros-kinetic-velodyne-pointcloud -y
    ```