#!/bin/sh
cd ..
rm -rf ./build
mkdir build
cd ./build
cmake -DCATCH_BUILD_EXAMPLES=ON ..
make -j4
./src/unit_testing/unit_testing