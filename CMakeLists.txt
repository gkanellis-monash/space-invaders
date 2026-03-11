cmake_minimum_required(VERSION 3.21)

project(
    space_invaders
    LANGUAGES CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(glfw3 3.4 REQUIRED)
find_package(GLEW REQUIRED)

add_executable(space_invaders main.cpp)
set_target_properties(space_invaders PROPERTIES CXX_STANDARD 20)
target_link_libraries(space_invaders PRIVATE glfw)
target_link_libraries(space_invaders PRIVATE GLEW::GLEW)