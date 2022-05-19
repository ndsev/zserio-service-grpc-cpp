# Sample of Zserio Service gRPC implementation in C++
cmake_minimum_required(VERSION 3.1.0)

project(ZserioServiceGrpc)

# please be aware that shared libraries are not recommended by GRPC (actually we observed Segmentation Fault)
# (https://github.com/grpc/grpc/blob/master/BUILDING.md#windows-a-note-on-building-shared-libs-dlls)
option(BUILD_SHARED_LIBS "Whether to build shared libraries" OFF)
option(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS "Whether to export all symbols on Windows" ON)

if(MSVC)
    add_definitions(-D_WIN32_WINNT=0x600)
endif()

find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# generate ZserioService using protobuf
set(PROTOC_EXECUTABLE $<TARGET_FILE:protobuf::protoc>)
set(GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)

get_filename_component(ZSERIO_SERVICE_PROTO "zserio_service.proto" ABSOLUTE)
get_filename_component(ZSERIO_SERVICE_PROTO_PATH "${ZSERIO_SERVICE_PROTO}" PATH)

set(ZSERIO_SERVICE_GEN_SOURCES
    "${CMAKE_CURRENT_BINARY_DIR}/zserio_service.pb.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/zserio_service.pb.h"
    "${CMAKE_CURRENT_BINARY_DIR}/zserio_service.grpc.pb.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/zserio_service.grpc.pb.h"
)

add_custom_command(
    OUTPUT ${ZSERIO_SERVICE_GEN_SOURCES}
    COMMAND ${PROTOC_EXECUTABLE}
    ARGS
        --grpc_out="${CMAKE_CURRENT_BINARY_DIR}" --cpp_out="${CMAKE_CURRENT_BINARY_DIR}"
        --plugin=protoc-gen-grpc="${GRPC_CPP_PLUGIN_EXECUTABLE}"
        -I "${ZSERIO_SERVICE_PROTO_PATH}" "${ZSERIO_SERVICE_PROTO}"
    DEPENDS "${ZSERIO_SERVICE_PROTO}"
    COMMENT "Generating Protobuf GRPC API"
)

# download the latest zserio together with runtime
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")
include(zserio_download)

download_zserio("${CMAKE_CURRENT_BINARY_DIR}/download" ZSERIO_JAR ZSERIO_RUNTIME_DIR)

# this is necessary for Windows to create runtime dll in the same directory as executables in the build tree
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_library(${PROJECT_NAME}
    ZserioServiceGrpc.cpp
    ${ZSERIO_SERVICE_GEN_SOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}") # generated sources
add_subdirectory("${ZSERIO_RUNTIME_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/ZserioCppRuntime")
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${ZSERIO_RUNTIME_DIR})
target_link_libraries(${PROJECT_NAME} gRPC::grpc++ ZserioCppRuntime)
