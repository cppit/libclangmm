#!/bin/bash

if [ "${cmake_command}" == "" ]; then
  cmake_command="scan-build cmake .."
fi

if [ "${make_command}" == "" ]; then
  make_command="scan-build --status-bugs make -j 2"
fi

cd libclangmm || echo "Can't cd into libclangmm"
mkdir -p build && cd build || echo "Error making build directory"
sh -c "${cmake_command}" || echo "Cmake configuration failed"
exec sh -c "${make_command}"
