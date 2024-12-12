cmake_minimum_required(VERSION 3.8)
project(usb_camera)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)  # 确保添加了这一行
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(usb_camera_node src/usb_camera_node.cpp
        include/usb_camera/usb_camera_node.h)
target_include_directories(usb_camera_node PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(usb_camera_node PUBLIC c_std_99 cxx_std_17)  # Require C99 and C++17

ament_target_dependencies(usb_camera_node
        rclcpp
        sensor_msgs  # 确保添加了这一行
        cv_bridge
        OpenCV
)

install(TARGETS usb_camera_node
  DESTINATION lib/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
