#include <ros/ros.h>
#include <pwd.h>
#include <string>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl/io/pcd_io.h>
#include <pcl/console/print.h>
#include <pcl/console/parse.h>
#include <pcl/console/time.h>
#include <pcl/common/transforms.h>
#include <cmath>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <geometry_msgs/PoseStamped.h>


using namespace std;
using namespace pcl;
using namespace pcl::io;
using namespace pcl::console;
Eigen::Matrix4f tform; 
//tform.setIdentity ();
ros::Subscriber initial_pose;
geometry_msgs::PoseStamped map_coordinate;
geometry_msgs::PoseStamped world_coordinate;

string world_frame_name_,map_frame_file_,output_name_;



void getParameters(ros::NodeHandle n_private)
{
  n_private.param<string>("world_frame_name", world_frame_name_, "");
  n_private.param<string>("map_frame_file", map_frame_file_, ".pcd");
  n_private.param<string>("output_name", output_name_, ".pcd");

}

void 
printElapsedTimeAndNumberOfPoints (double t, int w, int h = 1)
{
  print_info ("[done, "); print_value ("%g", t); print_info (" ms : "); 
  print_value ("%d", w*h); print_info (" points]\n");
}

bool
loadCloud (const std::string &filename, pcl::PCLPointCloud2 &cloud)
{
  TicToc tt;
  print_highlight ("Loading "); print_value ("%s ", filename.c_str ());

  tt.tic ();
  if (loadPCDFile (filename, cloud) < 0)
    return (false);

  printElapsedTimeAndNumberOfPoints (tt.toc (), cloud.width, cloud.height);

  print_info ("Available dimensions: "); print_value ("%s\n", pcl::getFieldsList (cloud).c_str ());

  return (true);
}

template <typename PointT> void
transformPointCloudHelper (PointCloud<PointT> & input, PointCloud<PointT> & output,
Eigen::Matrix4f &tform)
{
  transformPointCloud (input, output, tform);
}

template <> void
transformPointCloudHelper (PointCloud<PointNormal> & input, PointCloud<PointNormal> & output, 
                           Eigen::Matrix4f &tform)
{
  transformPointCloudWithNormals (input, output, tform);
}

template <> void
transformPointCloudHelper<PointXYZRGBNormal> (PointCloud<PointXYZRGBNormal> & input, 
                                              PointCloud<PointXYZRGBNormal> & output, 
                                              Eigen::Matrix4f &tform)
{
  transformPointCloudWithNormals (input, output, tform);
}


template <typename PointT> void
transformPointCloud2AsType (const pcl::PCLPointCloud2 &input, pcl::PCLPointCloud2 &output,
                            Eigen::Matrix4f &tform)
{
  PointCloud<PointT> cloud;
  fromPCLPointCloud2 (input, cloud);
  transformPointCloudHelper (cloud, cloud, tform);
  toPCLPointCloud2 (cloud, output);
}

void
transformPointCloud2 (const pcl::PCLPointCloud2 &input, pcl::PCLPointCloud2 &output,
                      Eigen::Matrix4f &tform)
{
  // Check for 'rgb' and 'normals' fields
  bool has_rgb = false;
  bool has_normals = false;
  for (const auto &field : input.fields)
  {
    if (field.name.find("rgb") != std::string::npos)
      has_rgb = true;
    if (field.name == "normal_x")
      has_normals = true;
  }

  // Handle the following four point types differently: PointXYZ, PointXYZRGB, PointNormal, PointXYZRGBNormal
  if (!has_rgb && !has_normals)
    transformPointCloud2AsType<PointXYZ> (input, output, tform);
  else if (has_rgb && !has_normals)
    transformPointCloud2AsType<PointXYZRGB> (input, output, tform);
  else if (!has_rgb && has_normals)
    transformPointCloud2AsType<PointNormal> (input, output, tform);
  else // (has_rgb && has_normals)
    transformPointCloud2AsType<PointXYZRGBNormal> (input, output, tform);
}

void
compute (const pcl::PCLPointCloud2::ConstPtr &input, pcl::PCLPointCloud2 &output,
         Eigen::Matrix4f &tform)
{
  TicToc tt;
  tt.tic ();
  
  print_highlight ("Transforming ");

  transformPointCloud2 (*input, output, tform);

  printElapsedTimeAndNumberOfPoints (tt.toc (), output.width, output.height);
}

void
saveCloud (const std::string &filename, const pcl::PCLPointCloud2 &output)
{
  TicToc tt;
  tt.tic ();

  print_highlight ("Saving "); print_value ("%s ", filename.c_str ());

  PCDWriter w;
  w.writeBinaryCompressed (filename, output);
  
  printElapsedTimeAndNumberOfPoints (tt.toc (), output.width, output.height);
}

void coordinate_calculate (const geometry_msgs::PoseStamped& data ,const geometry_msgs::PoseStamped& input)
{
  float dx, dy, dz;

  dx = input.pose.position.x - data.pose.position.x;
  dy = input.pose.position.y - data.pose.position.y;
  dz = input.pose.position.z - data.pose.position.z;
  tform (0, 3) = dx;
  tform (1, 3) = dy;
  tform (2, 3) = dz;

  const float& x = input.pose.orientation.x - data.pose.orientation.x;
  const float& y = input.pose.orientation.y - data.pose.orientation.y;
  const float& z = input.pose.orientation.z - data.pose.orientation.z;
  const float& w = input.pose.orientation.w - data.pose.orientation.w;
  tform.topLeftCorner (3, 3) = Eigen::Matrix3f (Eigen::Quaternionf (w, x, y, z));  
}

void position_get_ (const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& input)
{
  if (input->header.frame_id == "world")
  {
    world_coordinate.pose.position = input->pose.pose.position;
    world_coordinate.pose.orientation = input->pose.pose.orientation;
  }
  if(input->header.frame_id == "map")
  {
    map_coordinate.pose.position = input->pose.pose.position;
    map_coordinate.pose.orientation = input->pose.pose.orientation;
  }
  coordinate_calculate(map_coordinate,world_coordinate);
}



int main (int argc, char** argv)
{

  ros::init(argc, argv, "map_align");
    
  ros::NodeHandle n;
  ros::NodeHandle n_private("~");   
  getParameters(n_private);
  //subscribe calling pose
  initial_pose = n.subscribe("initialpose", 1000, position_get_);

  std::vector<int> p_file_indices;
  p_file_indices = parse_file_extension_argument (argc, argv, ".pcd");
  // Load the first file
  pcl::PCLPointCloud2::Ptr cloud (new pcl::PCLPointCloud2);
  if (!loadCloud (map_frame_file_, *cloud)) 
    return (-1);

  // Apply the transform
  pcl::PCLPointCloud2 output;
  compute (cloud, output, tform);

  // Save into the second file
  saveCloud (output_name_, output);

  ros::spin(); 

  return 0;
}
