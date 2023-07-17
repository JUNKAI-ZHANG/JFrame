cmake_minimum_required(VERSION 3.3.2)
project(JServer)

set(CMAKE_CXX_STANDARD 11)

# Binary file output
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Static lib output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Dynamic lib output
# SET_TARGET_PROPERTIES(dllname PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)

# Will add xx.so xx.a to install
file(GLOB_RECURSE cpp_My src/Helper/*.cpp)
file(GLOB_RECURSE cpp_ThirdParty src/ThirdParty/*.cpp)
file(GLOB_RECURSE hs_My src/*.h)
file(GLOB_RECURSE hs_ThirdParty src/ThirdParty/*.h)

add_executable(Test 
    ${cpp_My}
    ${hs_My}
    ${cpp_ThirdParty}
    ${hs_ThirdParty}
)