cmake_minimum_required(VERSION 3.10)

project(NeuralNetworkTest)

add_executable(NeuralNetworkTestApp
    external/Bitmap/bitmap.cpp
    source/main.cpp
)

include_directories(NeuralNetworkTestApp
    external/Bitmap
)