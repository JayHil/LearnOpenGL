cmake_minimum_required(VERSION 3.27.1)

project(HelloTriangle)

#dont need to build these to use glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#sets required verson of cpp to 11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#links the target executable HelloWindow to the source files hellowindow and glad.c
add_executable(HelloTriangle glad.c stbInclude.c hellotriangle.cxx)

#adds the glfw directory to the project to ensure it can find the includes/headers in this subdirectory
add_subdirectory(glfw-3.4)

#include directories to find the library headers
target_include_directories(HelloTriangle PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4")
target_include_directories(HelloTriangle PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4/deps")
target_include_directories(HelloTriangle PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/glfw-3.4/include")
target_include_directories(HelloTriangle PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(HelloTriangle glfw ${GLFW_LIBRARIES})