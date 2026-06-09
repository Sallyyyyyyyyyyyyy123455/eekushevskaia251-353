cmake_minimum_required(VERSION 3.10)
project(TaMP_Server)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenSSL REQUIRED)
find_package(SQLite3 REQUIRED)

add_executable(server src/main.cpp src/server.cpp src/database.cpp src/vigenere.cpp)

target_link_libraries(server SQLite::SQLite3 OpenSSL::SSL OpenSSL::Crypto)

if(WIN32)
    target_link_libraries(server ws2_32)
endif()
