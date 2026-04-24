#include <rclcpp/rclcpp.hpp>
#include <px4_msgs/msg/vehicle_local_position.hpp>
#include <px4_msgs/msg/battery_status.hpp>
#include <px4_msgs/msg/vehicle_attitude.hpp>
#include <rmw/types.h>

class PX4Monitor : public rclcpp::Node {
    rclcpp::QoS px4_qos() {
    rclcpp::QoS qos(10);
    qos.reliability(RMW_QOS_POLICY_RELIABILITY_BEST_EFFORT);
    qos.durability(RMW_QOS_POLICY_DURABILITY_TRANSIENT_LOCAL);
    return qos;
}
private:
    rclcpp::Subscription<px4_msgs::msg::VehicleLocalPosition>::SharedPtr local_position_sub;
    rclcpp::Subscription<px4_msgs::msg::BatteryStatus>::SharedPtr battery_sub;
    rclcpp::Subscription<px4_msgs::msg::VehicleAttitude>::SharedPtr attitude_sub;

    float x, y, z;
    float battery;
    float q[4];

public:
    PX4Monitor() : Node("px4_monitor") {
        x = y = z = 0.0;
        battery = 0.0;

        local_position_sub = this->create_subscription<px4_msgs::msg::VehicleLocalPosition>(
            "/fmu/out/vehicle_local_position_v1", px4_qos(),
            std::bind(&PX4Monitor::position_callback, this, std::placeholders::_1)
        );
        battery_sub = this->create_subscription<px4_msgs::msg::BatteryStatus>(
            "/fmu/out/battery_status_v1", px4_qos(),
            std::bind(&PX4Monitor::battery_callback, this, std::placeholders::_1)
        );
        attitude_sub = this->create_subscription<px4_msgs::msg::VehicleAttitude>(
            "/fmu/out/vehicle_attitude", px4_qos(),
            std::bind(&PX4Monitor::attitude_callback, this, std::placeholders::_1)
        );

        RCLCPP_INFO(this->get_logger(), "PX4 Monitor started - reading live flight data!");
}

void position_callback(const px4_msgs::msg::VehicleLocalPosition::SharedPtr msg) {
        x = msg->x;
        y = msg->y;
        z = msg->z;
        RCLCPP_INFO(this->get_logger(), 
        "Position - x: %.2f  y: %.2f  z: %.2f  altitude: %.2fm",
         x, y, z, -z);
}
void battery_callback(const px4_msgs::msg::BatteryStatus::SharedPtr msg) {
        battery = msg->remaining * 100.0; 
        RCLCPP_INFO(this->get_logger(),
        "Battery: %.1f%% voltage: %.2fV",
         battery, msg->voltage_v);
}
void attitude_callback(const px4_msgs::msg::VehicleAttitude::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(),
        "Attitude quaternion - w: %.2f x: %.2f y: %.2f z: %.2f",
         msg->q[0], msg->q[1], msg->q[2], msg->q[3]); 
}
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PX4Monitor>());
    rclcpp::shutdown();
    return 0;
}
