cmake_minimum_required(VERSION 3.10)

project(NeuralNetworkTest)

aux_source_directory(source applicationSources)
aux_source_directory(source/NetworkStructure applicationSources)

list(APPEND applicationIncludeDirectories
    source
    source/NetworkStructure
)

list(APPEND externalSources
    external/lodepng/lodepng.cpp
)

list(APPEND externalIncludeDirectories
    external/lodepng
)

add_executable(NeuralNetworkTestApp
    ${applicationSources}
    ${externalSources}
)

include_directories(NeuralNetworkTestApp
    ${applicationIncludeDirectories}
    ${externalIncludeDirectories}
)

set_property(TARGET NeuralNetworkTestApp PROPERTY CXX_STANDARD 17)