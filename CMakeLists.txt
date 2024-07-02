cmake_minimum_required(VERSION 3.0.0)
project(KewellMidware VERSION 1.0.0)

set(ARM ON) 
set(LINUXBUILD ON)

if(${ARM})
    set(ARM_CROSS /home/yli/Downloads/toolchain/gcc-linaro-7.3.1-2018.05-x86_64_aarch64-linux-gnu/bin)
    set(CMAKE_CXX_COMPILER ${ARM_CROSS}/aarch64-linux-gnu-g++)
endif()

if(${LINUXBUILD})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -std=c++11 -lpthread")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O2 -Wall -std=c++11 -lpthread -lcurl -lssl -lcrypto -framework CoreFoundation -framework SystemConfiguration")
endif()

add_subdirectory(app)
