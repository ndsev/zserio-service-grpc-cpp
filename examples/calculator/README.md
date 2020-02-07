# Calculator example

This example demonstrates how to use **ZserioServiceGrpc library** to use gRPC server-client communication.

## Prerequisites

1. Prerequisites for [ZserioServiceGrpc](../../README.md#prerequisites),
2. Zserio C++ runtime library,
3. Zserio compiler (`zserio.jar`).

> Zserio prerequisites are included in this repo in 3rdparty folder.

## Building

```bash
mkdir build
pushd build
PREFIX_PATH="/opt/tools/gRPC/lib/cmake;/opt/tools/protobuf/lib/cmake"
PREFIX_PATH+=";/opt/tools/cares/lib/cmake;/opt/tools/abseil/lib/cmake"
cmake -DCMAKE_PREFIX_PATH="${PREFIX_PATH}" ..
make
popd
```

## Testing

```bash
cd build
./CalculatorServer &
./CalculatorClient
# follow client's instructions
# ...
# pres q + ENTER to quit client
fg # and pres Ctrl+C to quit server
```
