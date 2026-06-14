cmake_minimum_required(VERSION 3.10)
project(canny)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "~/build")

find_package(OpenCV REQUIRED)

add_executable(canny_edges canny_edges.cpp)
set_target_properties(canny_edges PROPERTIES OUTPUT_NAME "canny_edges.out")
target_link_libraries(canny_edges PRIVATE ${OpenCV_LIBS})

add_executable(canny_generate_inputs canny_generate_inputs.cpp)
set_target_properties(canny_generate_inputs PROPERTIES OUTPUT_NAME "canny_generate_inputs.out")
target_link_libraries(canny_generate_inputs PRIVATE ${OpenCV_LIBS})
