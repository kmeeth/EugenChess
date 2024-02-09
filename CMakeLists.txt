cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

project(EugenChess LANGUAGES CXX)
add_library(EugenChess
        h/Communicator.h
        h/Engine.h
        src/Engine.cpp
)

enable_testing()
add_subdirectory(test)