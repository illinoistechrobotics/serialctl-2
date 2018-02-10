set(CMAKE_TOOLCHAIN_FILE ../robot/arduino-cmake/cmake/ArduinoToolchain.cmake)
cmake_minimum_required(VERSION 2.8)

project(example_robot CXX C)
set(ARDUINO_BOARD diecimila)
set(ARDUINO_BOARD_CPU atmega328)

add_subdirectory(../robot serialctl)

set(example_srcs
	main.cpp)

message("Using serial port ${ARDUINO_PORT}, board ${ARDUINO_BOARD}, and CPU ${ARDUINO_BOARD_CPU}")

generate_arduino_firmware(example_robot
	SRCS ${example_srcs}
	LIBS serialctl
	PORT ${ARDUINO_PORT}
	BOARD ${ARDUINO_BOARD}
	BOARD_CPU ${ARDUINO_BOARD_CPU})