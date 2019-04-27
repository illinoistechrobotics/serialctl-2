cmake_minimum_required(VERSION 3.8.2)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../Arduino-CMake-NG/cmake/Arduino-Toolchain.cmake")
set(AUTO_SET_SKETCHBOOK_PATH ON CACHE BOOL "Whether Arduino IDE's Sketchbook Location should be automatically found")

project(example_robot VERSION 0.1.0)

set(BOARD_NAME uno CACHE STRING "The name of the arduino (e.g. \"uno\")")
set(BOARD_CPU "" CACHE STRING "The arduino's CPU")
set(UPLOAD_PORT /dev/ttyACM0 CACHE STRING "The serial device to upload to")

if ("${BOARD_CPU}" STREQUAL "")
	arduino_cmake_project(example_robot BOARD_NAME "${BOARD_NAME}")
else ()
	arduino_cmake_project(example_robot BOARD_NAME "${BOARD_NAME}" BOARD_CPU "${BOARD_CPU}")
endif ()

add_arduino_executable(example_robot main.cpp)

set_target_upload_port(example_robot "${UPLOAD_PORT}")