cmake_minimum_required(VERSION 3.8)
project(smile)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES output_packet.cpp output_packet.hpp literal.hpp access_output_packet.hpp access_output_packet.cpp service.hpp service_port_map.hpp service_port_map.cpp)
add_library(smile ${SOURCE_FILES})