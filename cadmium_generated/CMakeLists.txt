cmake_minimum_required(VERSION 3.16)
project(SmartFridgeDEVS)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths relative to cadmium_generated/
set(CADMIUM_INCLUDE "${CMAKE_SOURCE_DIR}/../../cadmium/include")
set(DESTIMES_INCLUDE "${CMAKE_SOURCE_DIR}/../../cadmium/DESTimes/include")

add_executable(smart_fridge
    main.cpp
)

target_include_directories(smart_fridge PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CADMIUM_INCLUDE}
    ${DESTIMES_INCLUDE}
)