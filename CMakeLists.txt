cmake_minimum_required(VERSION 3.3.2)
project(JServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug") 
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_definitions(-DS_MAC)

# Include path
include_directories("${PROJECT_SOURCE_DIR}/src")

# Binary file output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Static lib output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Dynamic lib output
# SET_TARGET_PROPERTIES(dllname PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)

# Will add xx.so xx.a to install
file(GLOB_RECURSE cpp src/*.cpp)
file(GLOB_RECURSE doth src/*.h)

add_executable(Test 
    ${cpp}
    ${doth}
)