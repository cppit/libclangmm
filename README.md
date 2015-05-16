# libclangmm - An easy to use C++-wrapper for libclang #

## About ##
This project is written by *cppit* as part of an bachelor thesis

# Install #
This section describes how to install this library on your system. The section below is tested in on unix based systems and in windows. If there are any problems please create an issue and we will look into it.
## Dependencies ##
Please install these dependencies on your system.

* libclang
* cmake
* make
* clang or gcc (compiler)

## Download the source ##
There are two ways of downloading the source

### Download the zip ###
You can download the zip [here](https://github.com/cppit/libclangmm/archive/master.zip).

### Cloning the repository ###

```sh
$ git clone https://github.com/cppit/libclangmm.git

```


## Installation ##

```sh
$ cmake .
$ make install
```
**Notice:** *make install* needs root privileges

# Usage #
One quick start:

```cpp
  // lets say it is empty
  std::string path("your file here");

  clang::Index index(0, 0);
  clang::TranslationUnit tu(&index, path);

  // ReparseTranslationUnit takes a map with filepath as key
  // and buffer as value
  std::map<std::string, std::string> buffers;
  
  // create buffer (this would normally be a gtk/qt-buffer or something)
  std::string file;
  file.append("#include <iostream>");
  file.append("int main(int argc, char *argv[]) {\n");
  file.append("std::cout << \"Hello World!\" << std::endl;\n");
  file.append("return 0\n");
  file.append("}");

  buffers[path] = file;

  // after you can use various methods to get locations 
  // for tokens for syntax highlighting of refactoring

  // the buffer map should contain all open files. I.e in an text editor
  // you could have more than one file open at the same time. Putting the
  // files in this std::map will make the translationunit be reparsed
  // from memory instead of from file.
  tu.ReparseTranslationUnit(path, buffers);

  // zero is the start of the buffer
  clang::SourceLocation start(&tu, path, 0);

  // the 129 value is arbitrary, you must set it to the size of your
  // buffer (the number of chars in the buffer)
  clang::SourceLocation end(&tu, path, 129);

  clang::SourceRange range(&start, &end);
  clang::Tokens tokens(&tu, &range);

  // now tokens are stored in "tokens" we can extract ranges that are
  // comments for instance
  std::vector<clang::SourceRange> ranges;
  for (auto &t : tokens.tokens()) {
    if (t.kind() == clang::Token_Comment) {
      ranges.emplace_back(&tu, &t);
    }
  }
```
For more examples see tests/

# Tests #
To run tests simply do the following:
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


