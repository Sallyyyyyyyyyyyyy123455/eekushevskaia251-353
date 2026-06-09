cmake_minimum_required(VERSION 3.10)
project(TaMP_Client)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network)

qt_standard_project_setup()
qt_add_executable(TaMP_Client
    src/main.cpp
    src/mainwindow.cpp
)

target_link_libraries(TaMP_Client Qt6::Core Qt6::Widgets Qt6::Network)
