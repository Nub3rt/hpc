cmake_minimum_required(VERSION 3.10)
project(hough_transform)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "~/build")

find_package(OpenCV REQUIRED)

find_package(MPI REQUIRED)

add_executable(hough_transform main.cpp)

set_target_properties(hough_transform PROPERTIES OUTPUT_NAME "main.out")

target_link_libraries(hough_transform PRIVATE ${OpenCV_LIBS} MPI::MPI_CXX)
