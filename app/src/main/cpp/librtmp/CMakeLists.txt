cmake_minimum_required(VERSION 3.22.1)

file(GLOB rtmp_src *.c)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DNO_CRYPTO")

add_library(rtmp STATIC
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${rtmp_src})