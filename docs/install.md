# Installation guide #

## Debian/Ubuntu
```sh
sudo apt-get install git cmake make g++ libclang-dev
```

```sh
git clone https://github.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
sudo make install
```

## OS X with Homebrew (http://brew.sh/)
```sh
brew install cmake --with-clang llvm
```

```sh
git clone https://github.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
make install
```

## Windows with Cygwin (https://www.cygwin.com/)
**Make sure the PATH environment variable does not include paths to non-Cygwin cmake, make and g++.**

Select and install the following packages from the Cygwin-installer:
```
git cmake make gcc-g++ libclang-devel
```
Then run the following in the Cygwin Terminal:
```sh
git clone https://github.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
make install
```
