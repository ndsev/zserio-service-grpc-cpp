# ZserioServiceGrpc library

Sample implementation of Zserio service gRPC backend in __C++__.

## Pre-requisities

Install gRPC and protobuf according to the official instructions: https://grpc.io/docs/quickstart/cpp/.

> Please note that this sample use CMake and therefore it is necessary to install gRPC using cmake, otherwise
CMake's `find_package(gRPC)` will not be available.

> Since gRPC official instructions are sometimes inaccurate, you can check our
[doc/BUILD_gRPC.md](doc/BUILD_gRPC.md).

## Building

> Note that the following instructions suppose that gRPC was installed according to
[doc/BUILD_gRPC.md](doc/BUILD_gRPC.md) and therefore it uses `-DCMAKE_PREFIX_PATH` to set correct path to gRPC
and its dependencies.

Make sure that protobuf compiler and gRPC plugin are in `PATH`:
```bash
export PATH=/opt/tools/protobuf/bin:/opt/tools/gRPC/bin:$PATH
```

Build ZserioServiceGrpc library:
```bash
mkdir build
pushd build
cmake -DCMAKE_PREFIX_PATH="/opt/tools/protobuf/lib/cmake;/opt/tools/gRPC/lib/cmake;/opt/tools/cares/lib/cmake" \
      ../src
make
popd
```

## Usage

See [Calculator Example](./examples/calculator/README.md).
