//
// Created by Yang Shengjun on 24-12-13.
//

#ifndef USB_CAMERA_NODE_H
#define USB_CAMERA_NODE_H
// ROS
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "cv_bridge/cv_bridge.h"
// Opencv
#include <opencv2/opencv.hpp>

class UsbCameraNode : public rclcpp::Node {
public:
    UsbCameraNode() : Node("usb_camera_node") {

        // 初始化摄像头
        capture_ = cv::VideoCapture(0);

        while (!capture_.isOpened()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open camera.Trying to reopen camera...");
            capture_ = cv::VideoCapture(0);
        }

        RCLCPP_INFO(this->get_logger(), "SUccessfully Opened camera");

        // 创建发布者
        image_publisher_ = this->create_publisher<sensor_msgs::msg::Image>("/radar/image_raw", 10);

        // 设置定时器，以固定频率读取和发布图像
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),  // 100ms
            std::bind(&UsbCameraNode::capture_and_publish, this));
    }

private:
    void capture_and_publish() {
        while (!capture_.isOpened()) {
            RCLCPP_ERROR(this->get_logger(), "Failed to open camera.Trying to reopen camera...");
            capture_ = cv::VideoCapture(0);
        }

        cv::Mat frame;
        capture_ >> frame;  // 读取一帧图像

        if (frame.empty()) {
            RCLCPP_ERROR(this->get_logger(), "Received empty frame.");
            return;
        }

        // 将OpenCV图像转换为ROS消息
        auto msg = cv_bridge::CvImage(std_msgs::msg::Header(), "bgr8", frame).toImageMsg();

        // 发布图像消息
        image_publisher_->publish(*msg);
    }

    cv::VideoCapture capture_;
    rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr image_publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
};


#endif //USB_CAMERA_NODE_H
