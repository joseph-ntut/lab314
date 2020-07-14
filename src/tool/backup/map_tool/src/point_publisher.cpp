#include <ros/ros.h>
#include <pwd.h>
#include <string>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <std_msgs/String.h>
#include <sensor_msgs/PointCloud2.h>
#include <geometry_msgs/PoseStamped.h>
using namespace std;

double pose_x,pose_y,pose_z,orientation_x,orientation_y,orientation_z,orientation_w;
string yaml_file_,load_file_,subscribe_topic_,pose_,frame;
string frame_id_,folder_path_,postion_,matching_pose_topic_;
string delay="done";
int   check = 1,seq = 0;
int wating_time_;
geometry_msgs::PoseStamped hold_local_pose_;
geometry_msgs::PoseWithCovarianceStamped hold_publish_pose_;
ros::Publisher pose_pub_;
ros::Subscriber matching_pose;
ros::Subscriber initial_pose;
ros::Subscriber map_name_;

void getParameters(ros::NodeHandle n_private)
{
  n_private.param<string>("map_frame", frame_id_, "map");
  n_private.param<string>("folder_path", folder_path_, "/home/joseph/param/");
  n_private.param<string>("subscribe_topic", subscribe_topic_, "");
  n_private.param<string>("matching_pose_topic", matching_pose_topic_, "ndt_pose");
  n_private.param<int>("wating_time", wating_time_, 2);
}
void pose_publish()
{
  ros::Time currentTime = ros::Time::now();
  geometry_msgs::PoseWithCovarianceStamped pose_publish;
  pose_publish.header.stamp = currentTime;
  pose_publish.header.seq = seq ;
  pose_publish.pose.pose.position.x = pose_x;
  pose_publish.pose.pose.position.y = pose_y;
  pose_publish.pose.pose.position.z = pose_z;
  pose_publish.pose.pose.orientation.x = orientation_x;
  pose_publish.pose.pose.orientation.y = orientation_y;
  pose_publish.pose.pose.orientation.z = orientation_z;
  pose_publish.pose.pose.orientation.w = orientation_w;
  pose_publish.header.frame_id = frame_id_;
  ROS_INFO("get pose from [%s] ", load_file_.c_str());
  pose_pub_.publish(pose_publish);
  ROS_INFO("Publish pose to initial pose finish ");
  seq += 1;
  check = 1;
}
void getfile (string get_path_,string map_name_)
{     
  load_file_= get_path_ ;
  load_file_+=map_name_;
  load_file_+=".yaml";
  YAML::Node data_pose = YAML::LoadFile(load_file_);
    try
    {
      pose_x = data_pose["pose"]["position"]["x"].as<double>() ;
      pose_y = data_pose["pose"]["position"]["y"].as<double>() ;
      pose_z = data_pose["pose"]["position"]["z"].as<double>() ;
      orientation_x = data_pose["pose"]["orientation"]["x"].as<double>() ;
      orientation_y = data_pose["pose"]["orientation"]["y"].as<double>() ;
      orientation_z = data_pose["pose"]["orientation"]["z"].as<double>() ;
      orientation_w = data_pose["pose"]["orientation"]["w"].as<double>() ;
      frame = data_pose["map_frame"].as<string>() ;
    }
    catch(YAML::InvalidScalar)
    {
      ROS_ERROR("YAML load paramerters failed");
      exit(-1);
    }
}
 void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
    ROS_INFO("Loading [%s] position", msg->data.c_str());
    postion_= msg->data.c_str();
    getfile(folder_path_,postion_);
    pose_publish();
}

void pose_checking(const geometry_msgs::PoseStamped&feedback ,const geometry_msgs::PoseWithCovarianceStamped&output)
{
  //cout<<feedback<<endl;
  //cout<<output<<endl;
  cout<<"in"<<endl;
  if(abs(feedback.pose.position.x - output.pose.pose.position.x)<=1)
  {
    if(abs(feedback.pose.position.y - output.pose.pose.position.y)<=1)
    {
      if(abs(feedback.pose.orientation.z - output.pose.pose.orientation.z)<=1)
        ROS_INFO("match");
    }  
  }
  else 
  {
    ROS_INFO("not match");
    pose_publish();
  }
}
void initial_pose_Scratchpad(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& input)
{
  hold_publish_pose_.pose.pose.position=input->pose.pose.position;
  hold_publish_pose_.pose.pose.orientation=input->pose.pose.orientation;
  sleep(wating_time_);
  if(check == 1)
  pose_checking(hold_local_pose_,hold_publish_pose_);
}
void pose_Scratchpad(const geometry_msgs::PoseStamped::ConstPtr&hold)
{
  //cout<<hold->pose.position<<endl;
  hold_local_pose_.pose.position = hold->pose.position;
  hold_local_pose_.pose.orientation = hold->pose.orientation;
}
int main (int argc,char **argv)
{
    ros::init(argc, argv, "point_publisher");
    
    ros::NodeHandle n;
    ros::NodeHandle n_private("~");   
    getParameters(n_private);
    //subscribe calling pose
    map_name_= n.subscribe(subscribe_topic_, 1000,chatterCallback);
    matching_pose= n.subscribe(matching_pose_topic_, 1000,pose_Scratchpad);
    initial_pose = n.subscribe("initialpose", 1000, initial_pose_Scratchpad);
    pose_pub_ = n.advertise<geometry_msgs::PoseWithCovarianceStamped >("initialpose", 1000, true);
    
    ros::spin(); 

    return 0;
}