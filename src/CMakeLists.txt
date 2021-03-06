cmake_minimum_required(VERSION 3.1)
project(TrackMaker CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread")

# copy resource file to build/resouce
file(COPY ${CMAKE_SOURCE_DIR}/../resource/ DESTINATION ${PROJECT_BINARY_DIR}/resource/)
add_definitions(-DRESOURCE_DIR="${PROJECT_BINARY_DIR}/resource/")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
link_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

add_executable(TrackMaker main.cpp Camera.cpp Player.cpp Container.cpp Goal.cpp Title.cpp Play.cpp Result.cpp)

target_link_libraries(TrackMaker ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} IrrKlang glfw3 Xrandr Xinerama Xcursor Xi Xxf86vm X11 rt m dl)
