# Zserio Service gRPC backend

Sample implementation of Zserio Service gRPC backend in **C++**.

## Prerequisites

Install gRPC and protobuf according to the official instructions:
https://github.com/grpc/grpc/blob/master/BUILDING.md.

> Note that this sample use CMake and therefore it is necessary to install gRPC using CMake, otherwise
CMake's `find_package(gRPC)` will not be available.

> Since gRPC official instructions do not support properly older gRPC version using older CMake < 3.13,
you can check our [doc/BUILD_gRPC.md](doc/BUILD_gRPC.md).

> Note that Zserio prerequisites are automatically downloaded by CMake.

## Building

Make sure that `PREFIX_PATH` is set correctly to gRPC installation. For example,
if [doc/BUILD_gRPC.md](doc/BUILD_gRPC.md) has been used, set the following:
```bash
PREFIX_PATH="/opt/tools/gRPC/lib/cmake;/opt/tools/protobuf/lib/cmake"
PREFIX_PATH+=";/opt/tools/cares/lib/cmake;/opt/tools/abseil/lib/cmake"
```

Go to the ZserioServiceGrpc root directory.

Build ZserioServiceGrpc library:
```bash
mkdir build
pushd build
cmake -DCMAKE_PREFIX_PATH="${PREFIX_PATH}" ../src
cmake --build .
popd
```

> Note that for MSVC compiler, you might need to specify configuration by `cmake --build . --config Release`
or  `cmake --build . --config Debug`.

## Usage

See [Calculator Example](./examples/calculator/README.md).
