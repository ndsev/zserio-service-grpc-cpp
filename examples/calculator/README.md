# Calculator example

This example demonstrates how to use **ZserioServiceGrpc library** to use gRPC server-client communication.

## Prerequisites

1. Prerequisites for [ZserioServiceGrpc](../../README.md#prerequisites),

> Note that Zserio prerequisites are automatically downloaded by CMake.

## Building

Go to the calculator example directory.

```bash
mkdir build
pushd build
PREFIX_PATH="/opt/tools/gRPC/lib/cmake;/opt/tools/protobuf/lib/cmake"
PREFIX_PATH+=";/opt/tools/cares/lib/cmake;/opt/tools/abseil/lib/cmake"
cmake -DCMAKE_PREFIX_PATH="${PREFIX_PATH}" ..
cmake --build .
popd
```

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
