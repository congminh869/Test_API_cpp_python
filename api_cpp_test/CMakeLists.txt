cmake_minimum_required(VERSION 2.6)

project(cppAPI)

add_definitions(-std=c++11)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -g -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(cppAPI api_cpp_minh.cpp)

target_link_libraries(cppAPI curl)
target_link_libraries(cppAPI ${OpenCV_LIBS})
