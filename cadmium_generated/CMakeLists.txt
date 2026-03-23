cmake_minimum_required(VERSION 3.16)
project(SmartFridgeDEVS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CADMIUM_DIR "" CACHE PATH "Path to Cadmium root")
set(DESTIMES_DIR "" CACHE PATH "Path to DESTimes root")
set(BOOST_DIR "" CACHE PATH "Path to Boost root")

if(NOT CADMIUM_DIR)
    message(FATAL_ERROR "Please set CADMIUM_DIR to your Cadmium folder")
endif()

if(NOT DESTIMES_DIR)
    message(FATAL_ERROR "Please set DESTIMES_DIR to your DESTimes folder")
endif()

if(NOT BOOST_DIR)
    message(FATAL_ERROR "Please set BOOST_DIR to your Boost folder")
endif()

add_executable(smart_fridge
    main.cpp
)

target_include_directories(smart_fridge PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CADMIUM_DIR}/include
    ${DESTIMES_DIR}/include
    ${BOOST_DIR}
)