# Installation guide #

## Debian/Ubuntu
```sh
sudo apt-get install libclang-dev make cmake g++ git
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
