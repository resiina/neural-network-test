cmake_minimum_required(VERSION 3.10)

project(NeuralNetworkTest)

aux_source_directory(source applicationSources)
aux_source_directory(source/UserInterface applicationSources)
aux_source_directory(source/NetworkStructure applicationSources)
aux_source_directory(source/DataCollection applicationSources)

list(APPEND applicationIncludeDirectories
    source

    source/UserInterface

    source/NetworkStructure
    source/DataCollection
)

list(APPEND externalSources
    external/lodepng/lodepng.cpp
)

list(APPEND externalIncludeDirectories
    external/lodepng
    external/rang/include
)

add_executable(NeuralNetworkTestApp
    ${applicationSources}
    ${externalSources}
)

include_directories(NeuralNetworkTestApp
    ${applicationIncludeDirectories}
    ${externalIncludeDirectories}
)

target_link_libraries(NeuralNetworkTestApp
    pthread
)

set_property(TARGET NeuralNetworkTestApp PROPERTY CXX_STANDARD 17)