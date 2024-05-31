cmake_minimum_required(VERSION 3.5.0)
project(bassishell VERSION 0.1.0 LANGUAGES C CXX)

include_directories(include)

file(GLOB SOURCES "src/*.cpp")
add_executable(bassishell ${SOURCES})

include(CTest)
enable_testing()
