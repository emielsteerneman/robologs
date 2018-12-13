include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_subdirectory(protobuf)
#add_subdirectory(qt)
#add_subdirectory(common)

#add_subdirectory(logconvert)
#add_subdirectory(logdaemon)
#add_subdirectory(logplayer)
#add_subdirectory(logrecorder)

#add_subdirectory(examples)

add_executable(tester test.cpp GameStateTracker.cpp Reader.cpp Drawer.cpp Writer.cpp Writer.h)
target_link_libraries(tester protobuf ${OpenCV_LIBS})

#add_executable(main ${SOURCES})
