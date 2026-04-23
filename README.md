# ROS2 Drone Nodes

Training project toward AERIS Lab Robotics Research Engineer role.

## Packages

### drone_pkg
A two-node ROS2 system simulating drone telemetry.

- **drone_node** -- publishes battery level to '/battery_level' topic at 1hz
- **battery_monitor** -- subscribes to '/battery_level' and logs OK, LOW, and CRITICAL status

### flight_status_pkg
- **flight_status_node** -- Python node that subscribes to '/battery_level' and 
publishes human-readable flight status to '/flight_status' at 2hz


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

## Simulation Environment

- PX4 SITL with Gazebo -- X500 quadcopter simulation
- Flight tested: arm, takeoff, hold, land via PX4 shell
- Next step: connnecting ROS2 nodes to PX4  via micro-XRCE-DDS bridge
