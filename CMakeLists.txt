project(robologs)

cmake_minimum_required(VERSION 3.1)

#find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

#find_package(Boost 1.42.0 COMPONENTS program_options REQUIRED)
#find_package(ZLIB REQUIRED)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#set(OpenCV_DIR "../opencv/installation/OpenCV-3.4.4/share/OpenCV")
set(OpenCV_DIR "../opencv/installation/OpenCV-master/lib/cmake/opencv4")

find_package(OpenCV REQUIRED)

add_subdirectory(src)
