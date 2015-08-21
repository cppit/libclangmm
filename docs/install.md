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
Install https://github.com/transcode-open/apt-cyg:
```sh
lynx -source rawgit.com/transcode-open/apt-cyg/master/apt-cyg > apt-cyg
install apt-cyg /bin
```

```sh
apt-cyg install git cmake make gcc-g++ libclang
```

```sh
git clone https://github.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
make install
```
