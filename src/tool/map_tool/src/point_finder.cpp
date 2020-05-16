#include <ros/ros.h>
#include <fstream>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <yaml-cpp/yaml.h>
using namespace std;
string map_frame_,save_path_,file_name_,num,folder_name_;
double pose_x,pose_y,pose_z,orientation_x,orientation_y,orientation_z,orientation_w;
ros::Subscriber initial_pose;

void getParameters(ros::NodeHandle n_private)
{
  n_private.param<string>("path", save_path_, "/home/joseph/param/");
  
}
void writeFile(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& input)
{
     ROS_INFO("get pose and saving ");
    pose_x = input->pose.pose.position.x;
    pose_y = input->pose.pose.position.y;
    pose_z = input->pose.pose.position.z;
    orientation_x = input->pose.pose.orientation.x;
    orientation_y = input->pose.pose.orientation.y;
    orientation_z = input->pose.pose.orientation.z;
    orientation_w = input->pose.pose.orientation.w;
    folder_name_=save_path_;
    num= to_string(input->header.seq);
    file_name_=num + ".yaml";
    folder_name_ += file_name_;
  std::ofstream fout(folder_name_.c_str());
  YAML::Emitter out(fout);
  out << YAML::BeginMap;
  out << YAML::Key << "map_frame" << YAML::Value <<input->header.frame_id;
  out << YAML::Key << "point_name"<< YAML::Value <<input->header.seq;
  out << YAML::EndMap;
    out << YAML::BeginMap;
      out << YAML::Key << "pose";
      out <<YAML::BeginMap;
        out << YAML::Key << "position";
        out << YAML::BeginMap;
          out << YAML::Key << "x"<< YAML::Value <<pose_x;
          out << YAML::Key << "y"<< YAML::Value <<pose_y;
          out << YAML::Key << "z"<< YAML::Value <<pose_z;
        out << YAML::EndMap;
        out << YAML::Key << "orientation";
          out << YAML::BeginMap;
            out << YAML::Key << "x"<< YAML::Value <<orientation_x;
            out << YAML::Key << "y"<< YAML::Value <<orientation_y;
            out << YAML::Key << "z"<< YAML::Value <<orientation_z;
            out << YAML::Key << "w"<< YAML::Value <<orientation_w;
          out << YAML::EndMap;
        out << YAML::EndMap;
    out << YAML::EndMap;
    ROS_INFO("save pose data to %s  ", folder_name_.c_str());

}

int main (int arc , char **argv)
{
    ros::init(arc ,argv,"point_finder");

    ros::NodeHandle n ;
    ros::NodeHandle nh_private("~");
    getParameters(nh_private);
    ros::Subscriber initial_pose = n.subscribe("initialpose", 1000, writeFile);

    ros::spin();

    return 0;
}