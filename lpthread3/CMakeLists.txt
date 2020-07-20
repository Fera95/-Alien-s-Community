cmake_minimum_required(VERSION 3.9)
project(LPthread C)

set(CMAKE_C_STANDARD 99)

add_executable(LPthread lpthread.c lpthread_t.h)