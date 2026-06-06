cmake_minimum_required(VERSION 3.22)
project(Godium)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

file(GLOB_RECURSE SOURCES
    "../../src/*.cpp"
    "../../src/*.h"
)

qt_add_executable(Godium ${SOURCES})

if(NOT DEFINED CONFIG_PATH)
    set(CONFIG_PATH "config")
endif()
target_compile_definitions(Godium PRIVATE CONFIG_PATH="${CONFIG_PATH}")

file(GLOB_RECURSE ALL_DIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/../../src/*")
list(FILTER ALL_DIRS EXCLUDE REGEX "\\.[a-zA-Z0-9]+$")

target_include_directories(Godium PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../src" ${ALL_DIRS})

target_link_libraries(Godium PRIVATE Qt6::Widgets)

target_compile_options(Godium PRIVATE
    -s   
    -O3
    -fno-exceptions
    -fno-rtti
    -ffunction-sections
    -fdata-sections
    -fvisibility=hidden
)