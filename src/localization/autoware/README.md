# Autoware on PEV
## Mapping
Use ndt_mapping building Point Cloud map
```
roslaunch ndt_localizer ndt_mapping.launch
```
Save PointCloud2 topic to PCD file
```
roslaunch pev_map_server pointcloud_to_pcd.launch file_name:=myCloud
```