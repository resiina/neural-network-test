cmake_minimum_required(VERSION 3.10)

project(NeuralNetworkTest)

aux_source_directory(source applicationSources)
aux_source_directory(source/NetworkStructure applicationSources)

list(APPEND applicationIncludeDirectories
    source
    source/NetworkStructure
)

list(APPEND externalSources
    external/Bitmap/bitmap.cpp
)

list(APPEND externalIncludeDirectories
    external/Bitmap
)

add_executable(NeuralNetworkTestApp
    ${applicationSources}
    ${externalSources}
)

include_directories(NeuralNetworkTestApp
    ${applicationIncludeDirectories}
    ${externalIncludeDirectories}
)