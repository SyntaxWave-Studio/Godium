cmake_minimum_required(VERSION 3.16)
project(GodiumIDE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

file(GLOB_RECURSE ALL_FILES "*.cpp" "*.h" "*.hpp" "*.ui" "*.qrc")

set(CLEAN_SOURCES "")
foreach(FILE ${ALL_FILES})
    if(NOT FILE MATCHES "\\.git" AND NOT FILE MATCHES "build" AND NOT FILE MATCHES "moc_")
        list(APPEND CLEAN_SOURCES ${FILE})
    endif()
endforeach()

add_executable(GodiumIDE ${CLEAN_SOURCES})

file(GLOB_RECURSE ALL_DIRS LIST_DIRECTORIES true "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(dir ${ALL_DIRS})
    if(IS_DIRECTORY ${dir} AND NOT dir MATCHES "\\.git" AND NOT dir MATCHES "build")
        target_include_directories(GodiumIDE PRIVATE ${dir})
    endif()
endforeach()
target_include_directories(GodiumIDE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GodiumIDE PRIVATE Qt6::Widgets)
