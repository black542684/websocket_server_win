cmake_minimum_required(VERSION 3.16)
project(websocket_server_win)

set(CMAKE_CXX_STANDARD 17)
include_directories(includes)

add_executable(
        websocket_server_win
        main.cpp
        src/initSocket.cpp
        src/websocket.cpp
        src/utils.cpp
        src/Request.cpp
        lib/sha1.hpp)
