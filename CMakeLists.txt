cmake_minimum_required(VERSION 3.27)
project(compression_library)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
find_package(benchmark REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(compression_library main.cpp
        BinaryStdIn.h
        BinaryStdOut.h
        RunLengthEncoding.h
        HuffmanEncoding.h
        LZW.h
        TST.h
        DCT.h
        ../test/Benchmarking.h)

target_link_libraries(compression_library ${OpenCV_LIBS})
target_link_libraries(compression_library benchmark::benchmark)
