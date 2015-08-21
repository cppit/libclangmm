# libclangmm - An easy to use C++-wrapper for libclang #

## About ##
This is a highlevel wrapper for [libclang](http://www.llvm.org). It
provides highlevel C++11 access to complicated C code.

## Dependencies ##
* libclang

## Installation ##
See [installation guide](https://github.com/cppit/libclangmm/blob/master/docs/install.md)

# Tests #
The compilation of the tests are disabled due to ease of installation. Simply enter CMakeList.txt and uncomment the last two lines in the file to enable testing.

```sh
# enable_testing()
# add_subdirectory(tests)
```
Then simply: 
```sh
$ cmake .
$ make
$ ctest
```
If you want an more detailed look at the tests run the binary instead
```sh
$ cmake .
$ make
$ cd tests
$ ./clangmm_tests --log_level=all
```
To see more log parameters see [here](http://www.boost.org/doc/libs/1_58_0/libs/test/doc/html/utf/user-guide/runtime-config/reference.html).


