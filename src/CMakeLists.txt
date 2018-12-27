include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(${PROTOBUF_INCLUDE_DIR})

add_subdirectory(protobuf)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

add_executable(robologs
        Robologs.cpp
        Player.cpp
        input/Reader.cpp
        game/GameInfoTracker.cpp
        game/GameStateTracker.cpp
        output/Writer.cpp
        interface/Interface.cpp
        interface/Field.cpp
        Utilities.h interface/Timeline.cpp interface/Timeline.h)
target_link_libraries(robologs protobuf ${OpenCV_LIBS} Qt5::Widgets Qt5::Gui)

#add_executable(ui Interface.cpp)
#target_link_libraries(ui Qt5::Widgets Qt5::Gui)