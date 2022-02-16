cmake_minimum_required(VERSION 3.5)
project(support_lib)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

set( msg_files
  "msg/Current.msg"
  "msg/Voltage.msg"
  "msg/Power.msg"
  "msg/ServoPos.msg"
  "msg/Temperature.msg"
  "msg/Id.msg"
)

rosidl_generate_interfaces( msgs
  ${msg_files}
)

add_library(${PROJECT_NAME} src/support_lib.cpp)

rosidl_target_interfaces( ${PROJECT_NAME}
  msgs "rosidl_typesupport_cpp"
)

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

ament_target_dependencies(${PROJECT_NAME} rclcpp std_msgs )

ament_export_targets(my_libraryTargets HAS_LIBRARY_TARGET)
ament_export_dependencies(some_dependency)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT my_libraryTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

include_directories(include)

add_executable(test1 src/test1.cpp)
ament_target_dependencies(test1 rclcpp std_msgs)

rosidl_target_interfaces( test1
  msgs "rosidl_typesupport_cpp"
)

install(TARGETS
  test1
  DESTINATION lib/${PROJECT_NAME})

ament_package()
