import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32, String

class FlightStatusNode(Node):

    def __init__(self):
        super().__init__('flight_status_node')

        self.battery = 100.0

        # Subscribe to battery level from the C++ node
        self.battery_sub = self.create_subscription(
            Float32,
            '/battery_level',
            self.battery_callback,
            10)

        # Publish flight status
        self.status_pub = self.create_publisher(
            String, 
            '/flight_status',
            10)

        # Timer to publish status every 2 seconds
        self.timer = self.create_timer(2.0, self.publish_status)

        self.get_logger().info('Flight status node started!')

    def battery_callback(self, msg):
        self.battery = msg.data
##        self.get_logger().info(f'Received battery: {self.battery:.1f}%')

    def publish_status(self):
        status = String()

        if self.battery > 50.0:
            status.data = f'READY - Battery: {self.battery:.1f}%'
        elif self.battery > 20.0:
            status.data = f'CAUTION - Battery: {self.battery:.1f}%'
        else:
            status.data = f'LAND NOW - Battery: {self.battery:.1f}%'

        self.status_pub.publish(status)
        self.get_logger().info(f'Status: {status.data}')


def main(args=None):
    rclpy.init(args=args)
    node = FlightStatusNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()