# Installation guide #

## Debian/Ubuntu
```sh
sudo apt-get install git cmake make g++ libclang-dev
```

```sh
git clone https://gitlab.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
sudo make install
```

## OS X with [Homebrew](http://brew.sh/)
```sh
brew install cmake --with-clang llvm
```

```sh
git clone https://gitlab.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
make install
```

##Windows with [MSYS2](https://msys2.github.io/)
Install dependencies (replace `x86_64` with `i686` for 32-bit MSYS2 installs):
```sh
pacman -S git mingw-w64-x86_64-cmake make mingw-w64-x86_64-toolchain mingw-w64-x86_64-clang
```

Compile and install (replace `mingw64` with `mingw32` for 32-bit MSYS2 installs):
```sh
git clone https://gitlab.com/cppit/libclangmm.git
cd libclangmm
cmake -G"MSYS Makefiles" -DCMAKE_INSTALL_PREFIX=/mingw64 .
make
make install
```

<!--
## Windows with Cygwin (https://www.cygwin.com/)
**Make sure the PATH environment variable does not include paths to non-Cygwin cmake, make and g++.**

Select and install the following packages from the Cygwin-installer:
```
git cmake make gcc-g++ libclang-devel
```
Then run the following in the Cygwin Terminal:
```sh
git clone https://gitlab.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
make install
```
-->
