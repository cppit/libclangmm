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
To run the unit tests:
```sh
mkdir build && cd build
cmake -DBUILD_TESTING=1 ..
make
make test
```
