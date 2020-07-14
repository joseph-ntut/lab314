#include <ros/ros.h>
#include <map_load/load.h>
#include <cstdlib>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "map_load_client");
  if (argc != 4)
  {
    ROS_INFO("usage: map_load_client X Y DIS");
    return 1;
  }

  ros::NodeHandle n;
  ros::ServiceClient client = n.serviceClient<map_load::load>("map_load");
  map_load::load srv;
  srv.request.x = atoll(argv[1]);
  srv.request.y = atoll(argv[2]);
  srv.request.dis = atoi(argv[3]);
  if (client.call(srv))
  {
    std::string str = srv.response.a;
    const char *cstr = str.c_str();
    ROS_INFO("%s", cstr);
  }
  else
  {
    ROS_ERROR("Failed to call service map_load");
    return 1;
  }

  return 0;
}