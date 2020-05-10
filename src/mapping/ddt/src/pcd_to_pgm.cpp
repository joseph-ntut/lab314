#include <ros/ros.h>
#include <nav_msgs/OccupancyGrid.h>

// PCL
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>
#include <pcl/common/transforms.h>

using namespace std;

string pcd_file_;

void get_param(ros::NodeHandle n_private){
    n_private.param<string>("pcd_file" , pcd_file_ , "mycloud.pcd");

}

void cloud_to_map(  pcl::PointCloud<pcl::PointXYZ>::Ptr main_cloud, nav_msgs::OccupancyGrid &map){

    double max_x, max_y, mini_x, mini_y;

    max_x = mini_x = main_cloud->points[0].x;
    max_y = mini_y = main_cloud->points[0].y;

    for(int i = 1; i < main_cloud->points.size(); i++){

        if (main_cloud->points[i].x > max_x){
            max_x = main_cloud->points[i].x;
        }else if (main_cloud->points[i].x < mini_x){
            mini_x = main_cloud->points[i].x;
        }
        if (main_cloud->points[i].y > max_y){
            max_y = main_cloud->points[i].y;
        }else if (main_cloud->points[i].y < mini_y){
            mini_y = main_cloud->points[i].y;
        }
    }
    cout << "Size in meter: " << max_x-mini_x << " x " << max_y-mini_y << endl;

    map.header.stamp = ros::Time::now();
    map.header.frame_id = "map";
    map.info.resolution = 0.05;
    map.info.map_load_time = ros::Time::now();
    map.info.width = (int)(abs(max_x-mini_x) / map.info.resolution);
    map.info.height = (int)(abs(max_y-mini_y) / map.info.resolution);
    map.info.origin.position.x = -(max_x-mini_x) / 2.0;
    map.info.origin.position.y = -(max_y-mini_y) / 2.0;
    map.info.origin.position.z = 0;
    map.info.origin.orientation.w = 1;

    char data[map.info.height][map.info.width];

    for(int i = 0; i < map.info.height; i++){
        for(int j = 0; j < map.info.width; j++){
            data[i][j] = -1;
        }
    }
    cout << "Initialed map" << endl;

    Eigen::Matrix4f transform_1 = Eigen::Matrix4f::Identity();
    transform_1 (0,3) = -(max_x + mini_x) / 2.0;
    transform_1 (1,3) = -(max_y + mini_y) / 2.0;
    pcl::transformPointCloud (*main_cloud, *main_cloud, transform_1);

    int x, y, drop_count;
    drop_count = 0;

    for (int i = 0; i < main_cloud->points.size(); i++){
        x = (int)(main_cloud->points[i].x / map.info.resolution + map.info.width / 2);
        y = (int)(main_cloud->points[i].y / map.info.resolution + map.info.height / 2);
        if(y >= 0 && y <  map.info.height && x >=0 && x < map.info.width){
            data[y][x] = 100;
        }else{
            drop_count++;
        }
    }

    cout << "Put obstacel on map and droped " << drop_count << " points" << endl;

    for(int i = 0; i < map.info.height; i++){
        for(int j = 0; j < map.info.width; j++){
            if(data[i][j] == -1){
                data[i][j] = 0;
            }
        }
    }

    for(int i = 0; i < map.info.height; i++){
        for(int j = 0; j < map.info.width; j++){
            map.data.push_back(data[i][j]);
        }
    }

}
int main(int argc, char **argv){

    ros::init(argc, argv, "visual_3d_map");
    nav_msgs::OccupancyGrid map_msg;
    ros::NodeHandle n;
    ros::NodeHandle n_private("~");
    ros::Publisher map_pub;
    map_pub = n.advertise<nav_msgs::OccupancyGrid>("map", 1, true);
    get_param(n_private);


    
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    std::cout << "Start Loading PointCloud ..." << '\n';

    double Start = ros::Time::now().toSec();

    if (pcl::io::loadPCDFile<pcl::PointXYZ> (pcd_file_, *cloud) == -1){
        ROS_ERROR("can not open file: %s", pcd_file_.c_str());
    return (-1);
    }

    std::cout << "Use: " << (ros::Time::now().toSec() - Start)*1000 << " ms" << '\n';
    cloud_to_map(cloud, map_msg);
    map_pub.publish( map_msg );
    ros::spin();

}

