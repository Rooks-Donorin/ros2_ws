#include <rclcpp/rclcpp.hpp>
#include <std_srvs/srv/set_bool.hpp>

class ArmService : public rclcpp::Node {
private:
    bool is_armed;
    float battery;
    rclcpp::Service<std_srvs::srv::SetBool>::SharedPtr arm_server;

public:
    ArmService() : Node("arm_service") {
        is_armed = false;
        battery = 95.0;

        arm_server = this->create_service<std_srvs::srv::SetBool>(
            "/arm_drone",
            std::bind(&ArmService::handle_arm, this,
                std::placeholders::_1, 
                std::placeholders::_2)); 

        RCLCPP_INFO(this->get_logger(), "Arm service ready.");
    }

    void handle_arm(
        const std_srvs::srv::SetBool::Request::SharedPtr request,
        std_srvs::srv::SetBool::Response::SharedPtr response)
    {
        if (request->data == true) {
            if (battery < 20.0) {
                response->success = false;
                response->message = "Arming failed: battery critical";
                RCLCPP_ERROR(this->get_logger(), "Arming rejected - battery too low");
            } else {
                is_armed = true;
                response->success = true;
                response->message = "Drone armed!";
                RCLCPP_INFO(this->get_logger(), "Drone armed.");
            }
            }
        else {
            is_armed = false;
            response->success = true;
            response->message = "Drone disarmed!";
            RCLCPP_INFO(this->get_logger(), "Drone disarmed."); 
    }
}
};
int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ArmService>());
    rclcpp::shutdown();
    return 0;
}
