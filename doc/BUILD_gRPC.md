# Building gRPC

First install prerequisites listed in official instructions:
https://github.com/grpc/grpc/blob/master/BUILDING.md.

And clone the repository with all submodules:
```bash
git clone -b v1.27.0 https://github.com/grpc/grpc --recurse-submodules
```

> Note that the following instructions are valid for gRPC v1.27.0. Different steps may be needed for other
versions. It is always a good idea to check gRPC distribution tests for CMake mentioned below to see what
have to be done.

> :warning: gRPC release v1.26.0 has a bug which cause a deadlock when trying to build a server or connect
to a channel. As a workaround it is recommended to use v1.26.x branch or switch to v1.27.0 or newer.

## Ubuntu

The following steps are inspired by gRPC distribution tests for CMake:
https://github.com/grpc/grpc/blob/master/test/distrib/cpp/run_distrib_test_cmake.sh.

> Note that we use `-DCMAKE_INSTALL_PREFIX=/opt/tools/${TOOL_NAME}` to make it easier to uninstall gRPC and its
dependencies. Then, we have to use `-DCMAKE_PREFIX_PATH` properly to find the tools by CMake.

> Note that you should use `-DBUILD_SHARED_LIBS=ON` only if you need to build shared libraries.

Go to the gRPC root directory.

Install OpenSSL and Zlib from Ubuntu repositories:
```bash
sudo apt install libssl-dev libz-dev
```

Build and install abseil which comes as a submodule with gRPC:
```bash
mkdir -p third_party/abseil-cpp/cmake/build
pushd third_party/abseil-cpp/cmake/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/tools/abseil \
      -DCMAKE_POSITION_INDEPENDENT_CODE=TRUE -DBUILD_SHARED_LIBS=ON ../..
make
sudo make install
popd
```

Build and install c-ares which comes as a submodule with gRPC:
```bash
mkdir -p third_party/cares/cares/cmake/build
pushd third_party/cares/cares/cmake/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/tools/cares -DBUILD_SHARED_LIBS=ON ../..
make
sudo make install
popd
```

Build and install protobuf which comes as a submodule with gRPC:
```bash
mkdir -p third_party/protobuf/cmake/build
pushd third_party/protobuf/cmake/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/tools/protobuf -DBUILD_SHARED_LIBS=ON \
      -Dprotobuf_BUILD_TESTS=OFF ..
make
sudo make install
popd
```

Build and install gRPC itself:
```bash
mkdir -p cmake/build
pushd cmake/build
INSTALL_PREFIX=/opt/tools/gRPC
PREFIX_PATH="/opt/tools/protobuf/lib/cmake;/opt/tools/cares/lib/cmake;/opt/tools/abseil/lib/cmake"
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=${INSTALL_PREFIX} -DBUILD_SHARED_LIBS=ON \
      -DCMAKE_INSTALL_RPATH="${INSTALL_PREFIX}/lib;/opt/tools/protobuf/lib" -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON \
      -DCMAKE_PREFIX_PATH="${PREFIX_PATH}" -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF \
      -DgRPC_ABSL_PROVIDER=package -DgRPC_CARES_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package \
      -DgRPC_SSL_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package \
      -DProtobuf_INCLUDE_DIR=/opt/tools/protobuf/include \
      -DProtobuf_LIBRARY=/opt/tools/protobuf/lib/libprotobuf.so \
      -DProtobuf_PROTOC_LIBRARY=/opt/tools/protobuf/lib/libprotoc.so \
      -DProtobuf_PROTOC_EXECUTABLE=/opt/tools/protobuf/bin/protoc \
      ../..
make
sudo make install
popd
```

> For CMake older than 3.13, try to change `-DgRPC_PROTOBUF_PROVIDER=package` to
`-DgRPC_PROTOBUF_PROVIDER=module` and remove `-DProtobuf_INCLUDE_DIR`, `-DProtobuf_LIBRARY`,
`-DProtobuf_PROTOC_LIBRARY` and `-DProtobuf_PROTOC_EXECUTABLE`.

> Since we install gRPC to custom location, we have to use `-DCMAKE_INSTALL_RPATH=${CMAKE_INSTALL_PREFIX}/lib`
and `-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON` to ensure that binaries find their dependencies.

### Known Problems

#### FindProtobuf.cmake

Current gRPC CMake configuration uses `find_package(protobuf ...)`, but it uses MODULE mode by default. Then it
expects legacy variable names like `Protobuf_INCLUDE_DIR`, `Protobuf_LIBRARY`, `Protobuf_PROTOC_LIBRARY` and
`Protobuf_PROTOC_EXECUTABLE`. Unfortunately older CMake (tested with 3.10) comes with its own
`FindProtobuf.cmake` which is incompatible with the protobuf library packed as a third_party submodule in gRPC
repository.

1. With CMake >= 3.15, just add `-DCMAKE_FIND_PACKAGE_PREFER_CONFIG=TRUE` to the gRPC cmake configuration.
2. With CMake < 3.15:
    1. Either temporarily disable (e.g. rename) `FindProtobuf.cmake` during gRPC cmake configuration. The
    `FindProtobuf.cmake` is usually located in `/usr/share/cmake-?.??/Modules`. This causes that CMake use
    CONFIG mode as a fallback and finds Protobuf config in the specified `CMAKE_PREFIX_PATH`.
    2. Or specify all `Protobuf_` variables manually, like we do above.
