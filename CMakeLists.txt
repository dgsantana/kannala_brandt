cmake_minimum_required (VERSION 2.8.9)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC")
project (KannalaBrandt)
# find_package(OpenCV)
# include_directories(${OpenCV_INCLUDE_DIRS})
# message(${OpenCV_INCLUDE_DIRS}
# ${OpenCV_LIBRARIES})
include_directories(.)
add_library(judgeKB SHARED KannalaBrandt.cpp)
add_executable(demo demo.cpp )
target_link_libraries(demo judgeKB )
# target_link_libraries(demo judgeKB ${OpenCV_LIBRARIES})

