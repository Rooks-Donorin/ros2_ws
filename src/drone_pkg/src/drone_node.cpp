#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/float32.hpp>

class DroneNode : public rclcpp::Node {
private:
  float battery;
  float altitude;
  rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr battery_pub;
  rclcpp::TimerBase::SharedPtr timer;

public:
  DroneNode() : Node("drone_node") {
    battery = 95.0;
    altitude = 0.0;

    // Create a publisher on the /battery_level topic
    battery_pub = this->create_publisher<std_msgs::msg::Float32>("/battery_level", 10);

    // create a timer that calls update() every second
    timer = this->create_wall_timer(
      std::chrono::seconds(1),
      std::bind(&DroneNode::update, this));

    RCLCPP_INFO(this->get_logger(), "Drone node started!");
}

  void update() {
    // Drain battery slightly each second
    battery -= 0.5;

    // Publish battery level
    auto msg = std_msgs::msg::Float32();
    msg.data = battery;
    battery_pub->publish(msg);

    RCLCPP_INFO(this->get_logger(), "Battery: %.1f%%", battery);
  }
};

int main(int argc, char **argv) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<DroneNode>());
  rclcpp::shutdown();
  return 0;
}