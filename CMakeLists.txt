cmake_minimum_required(VERSION 3.8)
project(smile)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${OPENSSL_INCLUDE_DIR})

set(SOURCE_FILES output_packet.cpp output_packet.hpp literal.hpp access_output_packet.hpp access_output_packet.cpp service.hpp service_port_map.hpp service_port_map.cpp request.hpp access_request.hpp reply.hpp input_packet.hpp input_packet.cpp access_reply.hpp access_input_packet.hpp access_input_packet.cpp)
add_library(smile ${SOURCE_FILES})
target_link_libraries(smile ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} Threads::Threads)