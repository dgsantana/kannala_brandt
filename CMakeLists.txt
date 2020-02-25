cmake_minimum_required (VERSION 2.8.9)
project (KannalaBrandt)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3 -fPIC")

find_package(OpenCV)
if(OpenCV_FOUND)
    message("found opencv")
    add_definitions("-DUSE_OpenCV")
    include_directories(${OpenCV_INCLUDE_DIRS})
endif()

include_directories(.)
add_library(judgeKB SHARED KannalaBrandt.cpp)
add_executable(demo demo.cpp )

if(OpenCV_FOUND)
    target_link_libraries(judgeKB ${OpenCV_LIBRARIES})
    target_link_libraries(demo judgeKB )
else()
    target_link_libraries(demo judgeKB )
endif()

