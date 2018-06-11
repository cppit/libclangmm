**_This project has moved to https://gitlab.com/cppit/libclangmm._**

# libclangmm

###### an easy-to-use C++ wrapper for libclang

## About ##
Provides C++ bindings and class structure to the [libclang](http://www.llvm.org) C library.

Developed for [juCi++](https://gitlab.com/cppit/jucipp), a lightweight, platform-independent C++ IDE.

## Dependencies ##
* libclang

## Installation ##
See [installation guide](https://gitlab.com/cppit/libclangmm/blob/master/docs/install.md)

# Tests #
To run the unit tests:
```sh
mkdir build && cd build
cmake -DBUILD_TESTING=1 ..
make
make test
```
