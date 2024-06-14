cmake_minimum_required(VERSION 3.22.1)

project("androidapp")

include_directories(librtmp libx264/include)

add_subdirectory(librtmp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/libx264/libs/${CMAKE_ANDROID_ARCH_ABI}")

file(GLOB src_file *.cpp)

add_library(${CMAKE_PROJECT_NAME} SHARED
        ${src_file})

target_link_libraries(${CMAKE_PROJECT_NAME}
        android
        rtmp
        x264
        log)