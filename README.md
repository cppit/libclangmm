# libclangmm [![Build Status](https://travis-ci.org/cppit/libclangmm.svg?branch=master)](https://travis-ci.org/cppit/libclangmm) [![Build status](https://ci.appveyor.com/api/projects/status/bqcva8fovxu31yp3/branch/master?svg=true)](https://ci.appveyor.com/project/zalox/libclangmm/branch/master)

###### an easy-to-use C++ wrapper for libclang

## About ##
Provides C++ bindings and class structure to the [libclang](http://www.llvm.org) C library.

Developed for [juCi++](https://github.com/cppit/jucipp), a lightweight, platform-independent C++ IDE.

## Dependencies ##
* libclang

## Installation ##
See [installation guide](https://github.com/cppit/libclangmm/blob/master/docs/install.md)

# Tests #
To run the unit tests, first enable the CMake option `LIBCLANGMM_BUILD_TESTS`:
```sh
cmake -DLIBCLANGMM_BUILD_TESTS=ON .
```
Then, simply call:
```sh
make
ctest
```
You may also run the test binary directly to get more details:
```sh
make
cd tests
./clangmm_tests --log_level=all
```
For more options, see the [documentation of boost’s unit testing framework](http://www.boost.org/doc/libs/1_58_0/libs/test/doc/html/utf/user-guide/runtime-config/reference.html).
