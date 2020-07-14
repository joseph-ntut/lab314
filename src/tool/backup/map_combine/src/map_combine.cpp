#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PoseStamped.h>
#include <std_msgs/String.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <stdio.h>
#include <map_load/load.h>

float dis;
float pose_x, pose_y;
std::string floors;
ros::Subscriber floor_sub;
ros::Subscriber sub;
ros::Publisher point_pub;
ros::ServiceClient client;

void floor_callback(const std_msgs::String& f)
{
    floors = f.data;
}

void callback(const geometry_msgs::PoseStamped& p)
{
    pose_x = p.pose.position.x;
    pose_y = p.pose.position.y;
}

main (int argc, char **argv)
{
    ros::init (argc, argv, "map_combine");
    ros::NodeHandle nh;
    floor_sub = nh.subscribe("floor", 100, floor_callback);
    sub = nh.subscribe("ndt_pose", 100, callback);
    point_pub = nh.advertise<sensor_msgs::PointCloud2>("points_map", 100);
    client = nh.serviceClient<map_load::load>("map_load");
    pcl::PointCloud<pcl::PointXYZI> cloud_1, cloud_2, cloud_3, cloud_4, cloud_5;
    pcl::PointCloud<pcl::PointXYZI> cloud_6, cloud_7, cloud_8, cloud_9;
    pcl::PointCloud<pcl::PointXYZI> cloud;
    sensor_msgs::PointCloud2 out;
    map_load::load srv;
    
    dis = 7.0;

    ros::Rate r(100);
    while (nh.ok())
    {
        ros::spinOnce();
        srv.request.x = pose_x;
        srv.request.y = pose_y;
        srv.request.dis = dis;

        client.call(srv);

        std::string str[9] = {srv.response.a, srv.response.b, srv.response.c, 
        srv.response.d, srv.response.e, srv.response.f, 
        srv.response.g, srv.response.h, srv.response.i};
        char cstr[9][50];
        
        sprintf(cstr[0],"/home/joseph/map2/%s",(floors + "/" +str[0]).c_str());
        sprintf(cstr[1],"/home/joseph/map2/%s",(floors + "/" +str[1]).c_str());
        sprintf(cstr[2],"/home/joseph/map2/%s",(floors + "/" +str[2]).c_str());
        sprintf(cstr[3],"/home/joseph/map2/%s",(floors + "/" +str[3]).c_str());
        sprintf(cstr[4],"/home/joseph/map2/%s",(floors + "/" +str[4]).c_str());
        sprintf(cstr[5],"/home/joseph/map2/%s",(floors + "/" +str[5]).c_str());
        sprintf(cstr[6],"/home/joseph/map2/%s",(floors + "/" +str[6]).c_str());
        sprintf(cstr[7],"/home/joseph/map2/%s",(floors + "/" +str[7]).c_str());
        sprintf(cstr[8],"/home/joseph/map2/%s",(floors + "/" +str[8]).c_str());

        pcl::io::loadPCDFile(cstr[0],cloud_1);
        pcl::io::loadPCDFile(cstr[1],cloud_2);
        pcl::io::loadPCDFile(cstr[2],cloud_3);
        pcl::io::loadPCDFile(cstr[3],cloud_4);
        pcl::io::loadPCDFile(cstr[4],cloud_5);
        pcl::io::loadPCDFile(cstr[5],cloud_6);
        pcl::io::loadPCDFile(cstr[6],cloud_7);
        pcl::io::loadPCDFile(cstr[7],cloud_8);
        pcl::io::loadPCDFile(cstr[8],cloud_9);        
        cloud = cloud_1 + cloud_2;
        cloud += cloud_3;
        cloud += cloud_4;
        cloud += cloud_5;
        cloud += cloud_6;
        cloud += cloud_7;
        cloud += cloud_8;
        cloud += cloud_9;

        pcl::toROSMsg(cloud,out);
        out.header.stamp = ros::Time::now();
        out.header.frame_id = "map";
        point_pub.publish(out);
        r.sleep();
    }
    
}
