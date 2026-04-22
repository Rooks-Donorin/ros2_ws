# ROS2 Drone Nodes

Training project toward AERIS Lab Robotics Research Engineer role.

## Packages

### drone_pkg
A two-node ROS2 system simulating drone telemetry.

- **drone_node** -- publishes battery level to '/battery_level' topic at 1hz
- **battery_monitor** -- subscribes to '/battery_level' and logs OK, LOW, and CRITICAL status

## Building

'''bash
cd ros2_ws
colcon build
source install/setup.bash
'''

## Running

Terminal 1:
'''bash
ros2 run drone_pkg drone_node
''' 

Terminal 2:
'''bash
ros2 run drone_pkg battery_monitor
