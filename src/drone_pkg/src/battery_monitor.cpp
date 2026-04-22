#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>

class BatteryMonitor : public rclcpp::Node {
private:
  rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr battery_sub;

public:
  BatteryMonitor() : Node("battery_monitor") {
    battery_sub = this->create_subscription<std_msgs::msg::Float32>(
      "/battery_level", 10,
      std::bind(&BatteryMonitor::battery_callback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "Battery monitor started!");
    }

    void battery_callback(const std_msgs::msg::Float32::SharedPtr msg) {
        float level = msg->data;

        if (level > 50.0) {
            RCLCPP_INFO(this->get_logger(), "Battery OK: %.1f%%", level);
        } else if (level > 20.0) {
            RCLCPP_WARN(this->get_logger(), "Battery LOW: %.1f%%", level);
        } else {
            RCLCPP_ERROR(this->get_logger(), "CRITICAL: Return to base! %.1f%%", level);
        }
    }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<BatteryMonitor>());
  rclcpp::shutdown();
  return 0;
}
        