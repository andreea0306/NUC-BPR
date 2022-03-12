cmake_minimum_required(VERSION 3.20)
project(NUCv2)

set(CMAKE_CXX_STANDARD 20)

add_executable(NUCv2 main.cpp frame.cpp frame.h BP.cpp BP.h)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( NUCv2 ${OpenCV_LIBS} gomp)