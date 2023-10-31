cmake_minimum_required(VERSION 3.26)
project(socket C)

set(CMAKE_C_STANDARD 11)

add_executable(socket
        socket.c
        sender.c)
