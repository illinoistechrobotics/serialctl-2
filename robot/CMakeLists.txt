set(CMAKE_TOOLCHAIN_FILE arduino-cmake/cmake/ArduinoToolchain.cmake)
cmake_minimum_required(VERSION 2.8)

project(serialctl2 CXX C)

file(GLOB possible_ports /dev/ttyUSB* /dev/ttyACM* /dev/tty.usbserial-*)
if(possible_ports)
	list(GET possible_ports 0 ARDUINO_PORT)
	set(ARDUINO_PORT ${ARDUINO_PORT} PARENT_SCOPE)
	message("Found arduino at ${ARDUINO_PORT}")
else(possible_ports)
	set(ARDUINO_PORT /dev/ttyUSB0 PARENT_SCOPE)
	message("Didn't find arduino, defaulting to ${ARDUINO_PORT}")
endif(possible_ports)

message("Port: ${ARDUINO_PORT}, Board: ${ARDUINO_BOARD}, CPU: ${ARDUINO_BOARD_CPU}")

set(serialctl_srcs
	serialctl.cpp)

set(serialctl_headers
	serialctl.h)

generate_arduino_library(serialctl
	SRCS ${serialctl_srcs}
	HDRS ${serialctl_headers}
	BOARD ${ARDUINO_BOARD}
	BOARD_CPU ${ARDUINO_BOARD_CPU})