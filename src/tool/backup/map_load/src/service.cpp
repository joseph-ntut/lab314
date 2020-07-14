#include <ros/ros.h>
#include <map_load/load.h>

std::string count(int x, int y)
{
    std::string var = std::to_string(x) + "x" + std::to_string(y) + ".pcd";

    return var;
}

bool load(map_load::load::Request &req, map_load::load::Response &res)
{

    int grid_x, grid_y;
    if (req.x >= 0.0)
    {
      grid_x = - ( req.x + req.dis ) / ( 2 * req.dis );
    }
    else if (req.x < 0.0)
    {
      grid_x = - ( req.x - req.dis ) / ( 2 * req.dis );
    }
    if (req.y >= 0.0)
    {
      grid_y = - ( req.y + req.dis ) / ( 2 * req.dis );
    }
    else if (req.y < 0.0)
    {
      grid_y = - ( req.y - req.dis ) / ( 2 * req.dis );
    }

    res.a = count(grid_x  , grid_y);
    res.b = count(grid_x+1, grid_y);
    res.c = count(grid_x-1, grid_y);
    res.d = count(grid_x  , grid_y+1);
    res.e = count(grid_x+1, grid_y+1);
    res.f = count(grid_x-1, grid_y+1);
    res.g = count(grid_x  , grid_y-1);
    res.h = count(grid_x+1, grid_y-1);
    res.i = count(grid_x-1, grid_y-1);

    return true;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "map_load");
    ros::NodeHandle n;

    ros::ServiceServer service = n.advertiseService("map_load", load);
    ROS_INFO("Ready");
    ros::spin();

    return 0;
}
