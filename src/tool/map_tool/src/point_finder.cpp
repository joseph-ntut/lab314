#include <ros/ros.h>
#include <fstream>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <yaml-cpp/yaml.h>
using namespace std;
string map_frame_,save_point_;
double pose_x,pose_y,pose_z,orientation_x,orientation_y,orientation_z,orientation_w,seq;
int number;

ros::Subscriber initial_pose;

void writeFile(const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& input)
{
    pose_x = input->pose.pose.position.x;
    pose_y = input->pose.pose.position.y;
    pose_z = input->pose.pose.position.z;
    orientation_x = input->pose.pose.orientation.x;
    orientation_y = input->pose.pose.orientation.y;
    orientation_z = input->pose.pose.orientation.z;
    orientation_w = input->pose.pose.orientation.w;
    seq = input->header.seq;

  YAML::Emitter out;
  out.SetOutputCharset(YAML::EscapeNonAscii);
  out.SetOutputCharset(YAML::EscapeNonAscii);
  out << YAML::BeginMap;
  out << YAML::Key << "map_frame" << YAML::Value << map_frame_;
  out << YAML::Key << "point_name"<< YAML::Value <<seq;
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
  out << YAML::EndMap;
  std::ofstream fout(save_point_);
  fout << out.c_str();
  fout.close();
}

int main (int arc , char **argv)
{
    ros::init(arc ,argv,"point_finder");

    system("rosparam dump example.yaml");

    ros::NodeHandle n ;
    ros::Subscriber initial_pose = n.subscribe("initialpose", 1000, writeFile);

    ros::spin();

    return 0;
}