#!/usr/bin/env bash

set -ex

rm -rf build

vcpkg install
cmake --preset=vcpkg
cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build --preset=vcpkg
ctest --preset=vcpkg
