include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_subdirectory(protobuf)

add_executable(main test.cpp GameStateTracker.cpp Reader.cpp Drawer.cpp Writer.cpp)
target_link_libraries(main protobuf ${OpenCV_LIBS})
