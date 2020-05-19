#include <ros/ros.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <yaml-cpp/yaml.h>
#include <fstream>
#include <iostream>
#include <std_msgs/String.h>
#include <geometry_msgs/PoseStamped.h>
using namespace std;

double pose_x,pose_y,pose_z,orientation_x,orientation_y,orientation_z,orientation_w;
string frame_id_,yaml_file_,folder_path_,load_file_,subscribe_topic_,pose_,postion_,frame,matching_pose_topic_;
int start(1),send(1),wating_time_;
geometry_msgs::PoseStamped::ConstPtr hold_pose_;

void getParameters(ros::NodeHandle n_private)
{
  n_private.param<string>("map_frame", frame_id_, "map");
  n_private.param<string>("folder_path", folder_path_, "/home/joseph/param/");
  n_private.param<string>("subscribe_topic", postion_, "");
  n_private.param<string>("matching_pose_topic", matching_pose_topic_, "");
  n_private.param<int>("wating_time", wating_time_, 1500);

}
 void chatterCallback(const std_msgs::String::ConstPtr& msg)
  {
    ROS_INFO("Loading [%s] position", msg->data.c_str());
    postion_= msg->data.c_str();
  }

void getfile (string get_path_,string map_name_)
{     
    load_file_= get_path_ ;
    load_file_+=map_name_;
    load_file_+=".yaml";
    YAML::Node data_pose = YAML::LoadFile(load_file_);
    try
    {
      data_pose["pose"]["position"]["x"] = pose_x;
      data_pose["pose"]["position"]["y"] = pose_y;
      data_pose["pose"]["position"]["z"] = pose_z;
      data_pose["pose"]["orientation"]["x"] = orientation_x;
      data_pose["pose"]["orientation"]["y"] = orientation_y;
      data_pose["pose"]["orientation"]["z"] = orientation_z;
      data_pose["pose"]["orientation"]["w"] = orientation_w;
      data_pose["map_frame"] = frame;
    }
    catch(YAML::InvalidScalar)
    {
      ROS_ERROR("YAML load paramerters failed");
      exit(-1);
    }
    send = 0;
}
void pose_publish(ros::Publisher &pub )
{
  cout<<frame<<endl;
  cout<<pose_x<<endl;
  cout<<pose_y<<endl;
  cout<<pose_z<<endl;
  geometry_msgs::PoseWithCovarianceStamped pose_publish;
  ros::Time currentTime = ros::Time::now();
  pose_publish.header.stamp = currentTime;
  pose_publish.header.seq = start;
  pose_publish.pose.pose.position.x = pose_x;
  pose_publish.pose.pose.position.y = pose_y;
  pose_publish.pose.pose.position.z = pose_z;
  pose_publish.pose.pose.orientation.x = orientation_x;
  pose_publish.pose.pose.orientation.y = orientation_y;
  pose_publish.pose.pose.orientation.z = orientation_z;
  pose_publish.pose.pose.orientation.w = orientation_w;
  pose_publish.header.frame_id = frame_id_;
  ROS_INFO("get pose from [%s] ", load_file_.c_str());
  pub.publish(pose_publish);
  ROS_INFO("Publish pose to initial pose finish ");
  ROS_INFO("get pose from %d ", pose_x);
  start += 1; 
  pose_checking(hold_pose_,pose_publish);
}
void pose_checking(const geometry_msgs::PoseStamped::ConstPtr&feedback ,geometry_msgs::PoseWithCovarianceStamped&output)
{
  geometry_msgs::PoseStamped::ConstPtr feedback_1;
  while(sleep(wating_time_))
  {
    feedback_1 = hold_pose_;
  }
  if(abs(feedback->pose.position.x - feedback_1->pose.position.x)<=5)
    if(abs(feedback->pose.position.y - feedback_1->pose.position.y)<=5)
    {
      {
        send = 1;
      }
    }
}
void pose_Scratchpad(const geometry_msgs::PoseStamped::ConstPtr&hold)
{
  hold_pose_ = hold;
}
int main (int argc,char **argv)
{
    ros::init(argc, argv, "point_publisher");
    
    ros::NodeHandle n;
    ros::NodeHandle n_private("~");   
    getParameters(n_private);
    //subscribe calling pose
    //ros::Subscriber map_name_= n.subscribe(subscribe_topic_, 1000,chatterCallback);
    getfile(folder_path_,postion_);
    ros::Subscriber matching_pose= n.subscribe(matching_pose_topic_, 1000,pose_Scratchpad);

    //publish pose to initial pose
    
    ros::Publisher pose_pub_ = n.advertise<geometry_msgs::PoseWithCovarianceStamped >("initialpose", 1000, true);
    while (send == 1)
    {
      pose_publish(pose_pub_);
    }
    
    ros::spin();

    return 0;
}