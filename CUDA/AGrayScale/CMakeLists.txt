cmake_minimum_required(VERSION 3.5)
project(ImageFilter)

find_package(OpenCV REQUIRED)
find_package(CUDA 7.5 REQUIRED)

set(CMAKE_CXX_STANDARD 11)

file( GLOB  cu  *.cu)
cuda_add_executable(Image main.cpp grayscale.cu)

target_link_libraries(Image ${OpenCV_LIBS} )
