# juCi++
## Installation guide ##
## Linux
```sh
# Libraries 
$ sudo apt-get install libboost-log-dev libclang-dev
# Programs
$sudo apt-get install make cmake gcc

```
Compile
```sh
# When git clone
$ cd path-to-cloned-from-folder/jucipp/juci
# When download zipped file, extraxt it to a folder of your choice
$ cd path-to-folder-extraxted-into/jucipp-master/juci
# In both cases above you can choose remove the jucipp folder, but remeber to apply changes to cd command as well.
$ cmake .
$ make
```

## Run
```sh
$ ./bin/juci
```
