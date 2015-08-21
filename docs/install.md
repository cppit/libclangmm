# Installation guide #

## Debian/Ubuntu
```
sudo apt-get install libclang-dev make cmake g++ git
```

```
git clone https://github.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
sudo make install
```

## OS X with Homebrew (http://brew.sh/)
```
brew install cmake --with-clang llvm
```

```
git clone https://github.com/cppit/libclangmm.git
cd libclangmm
cmake .
make
make install
```
