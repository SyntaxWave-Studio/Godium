cmake_minimum_required(VERSION 3.16)
project(GodiumIDE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(GodiumIDE 
    main.cpp 
    Windows/MainWindow.h
    Windows/VirtualWindow.cpp 
    Windows/VirtualWindow.h
    Windows/DragData.h
    Windows/WindowFactory.h
    Windows/Editor/EditorWindow.cpp
    Windows/Editor/EditorWindow.h
)

target_include_directories(GodiumIDE PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Windows
    ${CMAKE_CURRENT_SOURCE_DIR}/Windows/Editor
)

target_link_libraries(GodiumIDE PRIVATE Qt6::Widgets)
