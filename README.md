# ROS2 Drone System

Training project toward AERIS Lab Robotics Research Engineer role at NC State University.

## System Overview

A multi-node ROS2 system that interfaces with PX4 SITL for simulated drone telemetry 
and control. Demonstrates topics, services, multi-language nodes, and live PX4 integration.

## Packages

### drone_pkg (C++)
- **drone_node** — publishes simulated battery telemetry to `/battery_level` at 1hz
- **battery_monitor** — subscribes to `/battery_level`, logs OK/LOW/CRITICAL status
- **arm_service** — ROS2 service on `/arm_drone` handling arm/disarm requests with 
battery safety checks
- **px4_monitor** — subscribes to live PX4 SITL topics, reads real position, 
battery, and attitude data from the flight controller

### flight_status_pkg (Python)
- **flight_status_node** — subscribes to `/battery_level`, publishes human-readable 
flight status to `/flight_status` at 2hz

## Simulation Environment

- PX4 Autopilot SITL with Gazebo — X500 quadcopter
- micro-XRCE-DDS bridge connecting PX4 to ROS2
- Live flight controller data flowing into ROS2 nodes including position, 
attitude, and battery status

## Building

```bash
cd ros2_ws
colcon build
source install/setup.bash
```

## Running

### Basic multi-node system
Terminal 1:
```bash
ros2 run drone_pkg drone_node
```
Terminal 2:
```bash
ros2 run drone_pkg battery_monitor
```
Terminal 3:
```bash
ros2 run flight_status_pkg flight_status_node
```

### PX4 SITL integration
Terminal 1:
```bash
cd ~/projects/PX4-Autopilot && make px4_sitl gz_x500
```
Terminal 2:
```bash
MicroXRCEAgent udp4 -p 8888
```
Terminal 3:
```bash
ros2 run drone_pkg px4_monitor
```

## Topics and Services

| Name | Type | Direction | Description |
|------|------|-----------|-------------|
| `/battery_level` | Float32 | Published | Simulated battery level |
| `/flight_status` | String | Published | Human readable flight status |
| `/arm_drone` | SetBool | Service | Arm or disarm the drone |
| `/fmu/out/vehicle_local_position_v1` | PX4 | Subscribed | Live position from PX4 |
| `/fmu/out/battery_status_v1` | PX4 | Subscribed | Live battery from PX4 |
| `/fmu/out/vehicle_attitude` | PX4 | Subscribed | Live attitude from PX4 |