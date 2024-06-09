cmake_minimum_required(VERSION 3.3.2)
project(JServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug") 
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# Include path
include_directories("${PROJECT_SOURCE_DIR}/src")

# Binary file output
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


# Dynamic lib output
# SET_TARGET_PROPERTIES(dllname PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../bin)

# JFrame/src/3rd
file(GLOB_RECURSE src_3rd_rapidjson src/3rd/rapidjson/*.cpp src/3rd/rapidjson/*.h src/3rd/rapidjson/*.hpp src/3rd/rapidjson/*.cc)
file(GLOB_RECURSE src_3rd_spdlog    src/3rd/spdlog/*.cpp    src/3rd/spdlog/*.h    src/3rd/spdlog/*.hpp    src/3rd/spdlog/*.cc)

# JFrame/src/lib
file(GLOB_RECURSE src_lib_log src/lib/log/*.cpp src/lib/log/*.h src/lib/log/*.hpp src/lib/log/*.cc)
file(GLOB_RECURSE src_lib_net src/lib/net/*.cpp src/lib/net/*.h src/lib/net/*.hpp src/lib/net/*.cc)

# JFrame/src/proto
file(GLOB_RECURSE src_proto_ss src/proto/msg_ss*.* )
file(GLOB_RECURSE src_proto_cs src/proto/msg_cs*.* )
file(GLOB_RECURSE src_proto_cm src/proto/msg_id*.* ) # 可继续扩展

# JFrame/src/libserver
file(GLOB_RECURSE src_libserver src/libserver/*.cpp src/libserver/*.h src/libserver/*.hpp src/libserver/*.cc)

######################################## StaticLib ########################################
# Static lib output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/lib)

add_library(_json STATIC ${src_3rd_rapidjson})
set_target_properties(_json PROPERTIES LINKER_LANGUAGE CXX)

######################################## LoginServer #######################################


######################################## GateServer ########################################
file(GLOB_RECURSE gate_server src/server/GateServer/*.h src/server/GateServer/*.cpp src/server/GateServer/*.hpp src/server/GateServer/*.cc)
add_executable(GateServer
    ${gate_server}
    ${src_3rd_spdlog}
    ${src_lib_log}
    ${src_lib_net}
    ${src_proto_ss}
    ${src_proto_cs}
    ${src_proto_cm}
    ${src_libserver}
)

target_link_libraries(GateServer
    pthread
    protobuf

    _json
)
#############################################################################################