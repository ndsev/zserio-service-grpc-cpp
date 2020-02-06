# Buliding gRPC

First listed prereuqisities listed in official instaructions: https://grpc.io/docs/quickstart/cpp/.

And clone the repository with all submodules:
```bash
git clone -b $(curl -sL https://grpc.io/release) https://github.com/grpc/grpc --recurse-submodules
cd grpc
```

## Ubuntu

The following steps are inspired by gRPC's CMake distribution test:
https://github.com/grpc/grpc/blob/master/test/distrib/cpp/run_distrib_test_cmake.sh.

> Note that we use `-DCMAKE_INSTALL_PREFIX=/opt/tools/${TOOL_NAME}` to make it easier to uninstall gRPC and its
dependencies. Then we have to use `-DCMAKE_PREFIX_PATH` properly to find the tools by CMake.

> Note that you should use `-DBUILD_SHARED_LIBS=ON` only if you need to build shared libraries.

Install OpenSSL and Zlib from Ubuntu repositories:
```bash
sudo apt install libssl-dev libz-dev
```

Build and install c-ares which comes as a sumboldule with gRPC:
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
```
mkdir -p cmake/build
pushd cmake/build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/opt/tools/gRPC -DBUILD_SHARED_LIBS=ON \
      -DCMAKE_INSTALL_RPATH=/opt/tools/gRPC/lib -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON \
      -DCMAKE_PREFIX_PATH="/opt/tools/protobuf/lib/cmake;/opt/tools/cares/lib/cmake" \
      -DgRPC_INSTALL=ON -DgRPC_BUILD_TESTS=OFF -DgRPC_CARES_PROVIDER=package -DgRPC_PROTOBUF_PROVIDER=package \
      -DgRPC_SSL_PROVIDER=package -DgRPC_ZLIB_PROVIDER=package ../..
make
sudo make intall
popd
```

> Since we install gRPC to custom location, we have to use `-DCMAKE_INSTALL_RPATH=${CMAKE_INSTALL_PREFIX}/lib`
and `-DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON` to ensure that binaries find their dependencies.
