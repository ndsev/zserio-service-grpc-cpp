# Calculator example

This example demonstrates how to use __ZserioServiceGrpc library__ to use gRPC server-client communication.

## Pre-requisities

1. Pre-requisities for [ZserioServiceGrpc](../../README.md#prerequisities),
2. Zserio C++ runtime library,
3. Zseiro compiler (`zserio.jar`).

> Zserio prerequisities are included in this repo in 3rdparty folder.

## Building

```bash
mkdir build
pushd build
cmake -DCMAKE_PREFIX_PATH="/opt/tools/protobuf/lib/cmake;/opt/tools/gRPC/lib/cmake;/opt/tools/cares/lib/cmake" \
      ..
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
