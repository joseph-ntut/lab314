#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>
#include <stdio.h>


main (int argc, char **argv)
{
    ros::init (argc, argv, "map_combine");
    ros::NodeHandle nh;
    ros::Publisher point_pub = nh.advertise<sensor_msgs::PointCloud2>("point2", 100);
    pcl::PointCloud<pcl::PointXYZ> cloud_1, cloud_2, cloud_3, cloud_4, cloud_5;
    pcl::PointCloud<pcl::PointXYZ> cloud;
    sensor_msgs::PointCloud2 out;

    int x, y; //current grid
    x = y = 0;
    char file_1[50], file_2[50], file_3[50], file_4[50], file_5[50];
    sprintf(file_1,"/home/eric/Desktop/%dx%d.pcd",x ,y);
    sprintf(file_2,"/home/eric/Desktop/%dx%d.pcd",x ,y+1);
    sprintf(file_3,"/home/eric/Desktop/%dx%d.pcd",x ,y-1);
    sprintf(file_4,"/home/eric/Desktop/%dx%d.pcd",x+1 ,y);
    sprintf(file_5,"/home/eric/Desktop/%dx%d.pcd",x-1 ,y);

    ros::Rate r(100);
    while (nh.ok())
    {
        ros::spinOnce();
        pcl::io::loadPCDFile(file_1,cloud_1);
        pcl::io::loadPCDFile(file_2,cloud_2);
        pcl::io::loadPCDFile(file_3,cloud_3);
        pcl::io::loadPCDFile(file_4,cloud_4);
        pcl::io::loadPCDFile(file_5,cloud_5);
        cloud = cloud_1 + cloud_2;
        cloud += cloud_3;
        cloud += cloud_4;
        cloud += cloud_5;
        pcl::toROSMsg(cloud,out);
        out.header.stamp = ros::Time::now();
        out.header.frame_id = "odom";
        point_pub.publish(out);
        r.sleep();
    }
    
}