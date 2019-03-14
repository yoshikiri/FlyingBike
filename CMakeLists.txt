cmake_minimum_required(VERSION 3.1)
project(FlyingBike CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread")

# copy resource file(image, sound) to build/resouce
file(COPY ${CMAKE_SOURCE_DIR}/resource/ DESTINATION ${PROJECT_BINARY_DIR}/resource/)
add_definitions(-DRESOURCE_DIR="${PROJECT_BINARY_DIR}/resource/")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
link_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

add_executable(FlyingBike main.cpp Mesh.cpp Model.cpp Camera.cpp Player.cpp Container.cpp Goal.cpp Play.cpp Result.cpp)

target_link_libraries(FlyingBike ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} assimp glfw3 openal Xrandr Xinerama Xcursor Xi Xxf86vm X11 rt m dl)
