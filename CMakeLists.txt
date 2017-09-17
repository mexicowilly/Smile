cmake_minimum_required(VERSION 3.8)
project(smile)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)
find_package(OpenSSL REQUIRED)
find_package(ICU REQUIRED COMPONENTS io i18n uc data)
find_path(CHUCHO_INCLUDE_DIR chucho/log.hpp)
if (NOT CHUCHO_INCLUDE_DIR)
    message(FATAL_ERROR "Could not find chucho/log.hpp")
endif()
find_library(CHUCHO_LIBRARY chucho)
if(NOT CHUCHO_LIBRARY)
    message(FATAL_ERROR "Could not find libchucho")
endif()

include_directories(include
                    ${Boost_INCLUDE_DIRS}
                    ${OPENSSL_INCLUDE_DIR}
                    ${ICU_INCLUDE_DIRS}
                    ${CHUCHO_INCLUDE_DIR})

set(SMILE_PUBLIC_HEADERS
    include/smile/as400.hpp
    include/smile/os400_version.hpp)
set(SMILE_PRIVATE_HEADERS
    access_input_packet.hpp
    access_output_packet.hpp
    access_reply.hpp
    access_request.hpp
    auth_type.hpp
    end_signon_service_request.hpp
    icu_exception.hpp
    input_packet.hpp
    literal.hpp
    output_packet.hpp
    reply.hpp
    request.hpp
    service.hpp
    service_port_map.hpp
    signon_info_reply.hpp
    signon_info_request.hpp
    signon_service.hpp
    signon_text.hpp
    text_conversion.hpp)
set(SMILE_SOURCES
    access_input_packet.cpp
    access_output_packet.cpp
    access_request.cpp
    as400.cpp
    icu_exception.cpp
    input_packet.cpp
    os400_version.cpp
    output_packet.cpp
    service.cpp
    service_port_map.cpp
    signon_info_reply.cpp
    signon_info_request.cpp
    signon_service.cpp
    signon_text.cpp
    text_conversion.cpp)

add_library(smile
    ${SMILE_PUBLIC_HEADERS}
    ${SMILE_PRIVATE_HEADERS}
    ${SMILE_SOURCES})
target_link_libraries(smile
    ${Boost_LIBRARIES}
    ${ICU_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${CHUCHO_LIBRARY}
    Threads::Threads)