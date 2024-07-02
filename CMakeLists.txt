cmake_minimum_required(VERSION 3.0.0)
project(KewellDaemon VERSION 1.0.0)

set(ARM ON)

if(${ARM})
    set(ARM_CROSS /tool/gcc_linaro/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin)
    set(CMAKE_CXX_COMPILER ${ARM_CROSS}/aarch64-linux-gnu-g++)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0 -Wall -std=c++17 -lpthread -ldl -lrt")

add_subdirectory(app)
