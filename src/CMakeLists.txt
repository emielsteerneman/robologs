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

add_executable(test test.cpp GameStateTracker.cpp Reader.cpp)
target_link_libraries(test protobuf)

#add_executable(main ${SOURCES})
