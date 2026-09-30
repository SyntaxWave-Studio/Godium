cmake_minimum_required(VERSION 3.16) 
project(Godium CXX) 

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src") 

find_package(PkgConfig REQUIRED) 
pkg_check_modules(GLFW REQUIRED IMPORTED_TARGET glfw3) 
pkg_check_modules(CAIRO REQUIRED IMPORTED_TARGET cairo) 

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES}) 

target_include_directories(${PROJECT_NAME} PRIVATE "${SRC_DIR}") 

target_link_libraries(${PROJECT_NAME} PRIVATE 
    PkgConfig::GLFW 
    PkgConfig::CAIRO 
    OpenGL::GL 
    Threads::Threads
)
