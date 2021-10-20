# Calculator example

This example demonstrates how to use **ZserioServiceGrpc library** to use gRPC server-client communication.

## Prerequisites

1. Prerequisites for [ZserioServiceGrpc](../../README.md#prerequisites),

> Note that Zserio prerequisites are automatically downloaded by CMake.

## Building

Make sure that `PREFIX_PATH` is set correctly to gRPC installation. For example,
if [doc/BUILD_gRPC.md](doc/BUILD_gRPC.md) has been used, set the following:
```bash
PREFIX_PATH="/opt/tools/gRPC/lib/cmake;/opt/tools/protobuf/lib/cmake"
PREFIX_PATH+=";/opt/tools/cares/lib/cmake;/opt/tools/abseil/lib/cmake"
```

Go to the calculator example directory.

```bash
mkdir build
pushd build
cmake -DCMAKE_PREFIX_PATH="${PREFIX_PATH}" ..
cmake --build .
popd
```

> Note that for MSVC compiler, you might need to specify configuration by `cmake --build . --config Release`
or  `cmake --build . --config Debug`.

## Testing

Go to the build directory where executables are located.

```bash
./CalculatorServer &
./CalculatorClient
# follow client's instructions
# ...
# press q + ENTER to quit the client
fg # and press Ctrl+C to quit the server
```
