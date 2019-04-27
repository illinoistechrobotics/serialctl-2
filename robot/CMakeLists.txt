cmake_minimum_required(VERSION 3.8.2)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../Arduino-CMake-NG/cmake/Arduino-Toolchain.cmake")

project(serialctl)

add_arduino_library(serialctl
	./serialctl.cpp
	./serialctl.h)

target_include_directories(serialctl PUBLIC .)
