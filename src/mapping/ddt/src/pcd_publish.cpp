#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl_conversions/pcl_conversions.h>

using namespace std;

string frame_id_, pcd_file_;
bool visualization_;
int outlier_mean_k_;
double outlier_thresh_, leaf_size_;
ros::Publisher cloud_pub_;
pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_publish;

void getParameters(ros::NodeHandle n_private)
{
  n_private.param<string>("frame_id", frame_id_, "map");
  n_private.param<string>("pcd_file", pcd_file_, ".pcd");
  n_private.param<bool>("visualization", visualization_, false);
  // for OutlierRemoval
  n_private.param<int>("outlier_mean_k", outlier_mean_k_, 50);
  n_private.param<double>("outlier_thresh", outlier_thresh_, 2.0);
  // for VoxelGrid filter
  n_private.param<double>("leaf_size", leaf_size_, 0.1);
}

void loadPcdFile(string &file_name, pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_filtered)
{
  pcl::PointCloud<pcl::PointXYZI>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZI>);
  pcl::PointCloud<pcl::PointXYZI>::Ptr outline_cloud (new pcl::PointCloud<pcl::PointXYZI>);
  // Fill in the cloud data
  pcl::PCDReader reader;
  // Replace the path below with the path where you saved your file
  reader.read<pcl::PointXYZI> ("/home/justin/pcd/1012vassar_st_5cm.pcd", *cloud);
  std::cerr << "Cloud before filtering: " << std::endl;
  std::cerr << *cloud << std::endl;
  if(outlier_mean_k_ > 0)
  {
    // OutlierRemoval
    pcl::StatisticalOutlierRemoval<pcl::PointXYZI> sor;
    sor.setInputCloud (cloud);
    sor.setMeanK (outlier_mean_k_);
    sor.setStddevMulThresh (outlier_thresh_);
    sor.filter (*outline_cloud);
  }
  if(leaf_size_ > 0)
  {
    // VoxelGrid filter
    pcl::VoxelGrid<pcl::PointXYZI> sor_v;
    sor_v.setInputCloud (outline_cloud);
    sor_v.setLeafSize (leaf_size_, leaf_size_, leaf_size_);
    sor_v.filter (*cloud_filtered);
  }
}

void publish_cloud(const ros::TimerEvent& event)
{
  static sensor_msgs::PointCloud2 ros_cloud;
  if(cloud_pub_.getNumSubscribers() == 0 || cloud_publish == NULL)
  {
    return;
  }
  pcl::toROSMsg(*cloud_publish, ros_cloud);
  ros_cloud.header.seq++;
  ros_cloud.header.stamp = ros::Time::now();
  ros_cloud.header.frame_id = frame_id_;
  cloud_pub_.publish(ros_cloud);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "pev_3d_map");
  ros::NodeHandle n;
  ros::NodeHandle n_private("~");
  pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZI>);
  ros::Timer timer = n.createTimer(ros::Duration(1.0), publish_cloud);
  cloud_pub_ = n.advertise<sensor_msgs::PointCloud2>("pcd_cloud", 10);
  getParameters(n_private);
  cerr << "Loading file ..." << endl;
  loadPcdFile(pcd_file_, cloud_filtered);
  cloud_publish = cloud_filtered;
  if(visualization_)
  {
    pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
    // Shaw result
    viewer.showCloud (cloud_filtered);
    while (!viewer.wasStopped ())
    {
      ros::spinOnce();
    }
  }
  else
  {
    ros::spin();
  }
  return 0;
}
