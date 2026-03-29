cmake_minimum_required(VERSION 3.16)
project(SmartFridgeDEVS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(smart_fridge
    main.cpp
)

target_include_directories(smart_fridge PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../external/cadmium/include
    ${CMAKE_SOURCE_DIR}/../external/DESTimes/include
    ${CMAKE_SOURCE_DIR}/../external/boost
)
