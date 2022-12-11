cmake_minimum_required(VERSION 3.23)
project(C_PROGRAMMING C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C_PROGRAMMING
        client.c
        server.c
        server_macOS_test.c file_reader.h DynamicMemoryAllocationTest.h utility.h)
