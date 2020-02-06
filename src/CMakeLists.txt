# Sample of Zserio Service gRPC implementation in C++
cmake_minimum_required(VERSION 2.8.12.2)

project(ZserioServiceGrpc)

set(PROJECT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE PATH "")
set(ZSERIO_INCLUDE_DIR "${PROJECT_ROOT}/3rdparty/runtime" CACHE PATH "")
option(BUILD_SHARED_LIBS "Whether to build shared libraries" ON)

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
)

add_library(${PROJECT_NAME}
    ZserioServiceGrpc.cpp
    ZserioServiceGrpc.cpp
    ${ZSERIO_SERVICE_GEN_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_BINARY_DIR}") # generated sources
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${ZSERIO_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} gRPC::grpc++)
