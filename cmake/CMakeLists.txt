cmake_minimum_required(VERSION 3.16)
project(Godium)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE SOURCES 
    "../src/*.cpp" 
    "../src/*.h" 
)

add_executable(Godium ${SOURCES})

file(GLOB_RECURSE ALL_DIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/../src/*")
list(FILTER ALL_DIRS EXCLUDE REGEX "\\.[a-zA-Z0-9]+$")

target_include_directories(Godium PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../src" ${ALL_DIRS})

target_link_libraries(Godium PRIVATE Qt6::Widgets)

if(WIN32)
    if(MSVC)
        target_compile_options(Godium PRIVATE /O1 /GR-)
    endif()
elseif(APPLE)
    target_compile_options(Godium PRIVATE -Os -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -fvisibility=hidden)
    target_link_options(Godium PRIVATE -Wl,-dead_strip)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_options(Godium PRIVATE -Os -fno-exceptions -fno-rtti -ffunction-sections -fdata-sections -fvisibility=hidden)
    target_link_options(Godium PRIVATE -Wl,--gc-sections -Wl,-as-needed)
endif()