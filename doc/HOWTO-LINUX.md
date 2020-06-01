
HOWTO GUIDE
===========

This HOWTO guide applies to Linux only

## HOW TO USE CODEQUERY WITH C/C++ CODE?

1. Change directory to the base folder of your source code like this:
```bash
cd ~/projects/myproject/src
```
2. Create a cscope.files file with all the C/C++ source files listed in it. Files with [inline assembly code](http://en.wikipedia.org/wiki/Inline_assembler) should be excluded from this list.
```bash
find . -iname "*.c"    > ./cscope.files
find . -iname "*.cpp" >> ./cscope.files
find . -iname "*.cxx" >> ./cscope.files
find . -iname "*.cc " >> ./cscope.files
find . -iname "*.h"   >> ./cscope.files
find . -iname "*.hpp" >> ./cscope.files
find . -iname "*.hxx" >> ./cscope.files
find . -iname "*.hh " >> ./cscope.files
```
3. Create a cscope database like this (add `k`, if you don't want standard include paths like for stdio.h):
```bash
cscope -cb
```
4. Create a ctags database like this.
```bash
ctags --fields=+i -n -L ./cscope.files
```
5. Run cqmakedb to create a CodeQuery database out of the cscope and ctags databases, like this:
```bash
cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p
```
6. Open myproject.db using the CodeQuery GUI tool by running the following. Wild card search (* and ?) supported if Exact Match is switched off. Or use cqsearch, the CLI-version of CodeQuery (type `cqsearch -h` for more info).
```bash
codequery
```
Use `cqmakedb -h` to get help on cqmakedb command line arguments.



## HOW TO USE CODEQUERY WITH JAVA CODE?

1. Change directory to the base folder of your source code like this:
```bash
cd ~/projects/myproject/src
```
2. Create a cscope.files file with all the Java source files listed in it.
```bash
find . -iname "*.java" > ./cscope.files
```
3. Create a cscope database like this:
```bash
cscope -cb
```
4. Create a ctags database like this:
```bash
ctags --fields=+i -n -L ./cscope.files
```
5. Run cqmakedb to create a CodeQuery database out of the cscope and ctags databases, like this:
```bash
cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p
```
6. Open myproject.db using the CodeQuery GUI tool by running the following. Wild card search (* and ?) supported if Exact Match is switched off. Or use cqsearch, the CLI-version of CodeQuery (type `cqsearch -h` for more info).
```bash
codequery
```
Use `cqmakedb -h` to get help on cqmakedb command line arguments.



## HOW TO USE CODEQUERY WITH PYTHON CODE?

If you want to browse Python code, don't forget to install [pycscope](https://github.com/portante/pycscope). Information on how to install this tool is available on its github page.

1. Change directory to the base folder of your source code like this:
```bash
cd ~/projects/myproject/src
```
2. Create a cscope.files file with all the Python source files listed in it.
```bash
find . -iname "*.py"    > ./cscope.files
```
3. Create a cscope database like this:
```bash
pycscope -i ./cscope.files
```
4. Create a ctags database like this.
```bash
ctags --fields=+i -n -L ./cscope.files
```
5. Run cqmakedb to create a CodeQuery database out of the cscope and ctags databases, like this:
```bash
cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p
```
6. Open myproject.db using the CodeQuery GUI tool by running the following. Wild card search (* and ?) supported if Exact Match is switched off. Or use cqsearch, the CLI-version of CodeQuery (type `cqsearch -h` for more info).
```bash
codequery
```
Use `cqmakedb -h` to get help on cqmakedb command line arguments.



## HOW TO USE CODEQUERY WITH RUBY, GO AND JAVASCRIPT CODE?

If you want to browse Ruby, Go or Javascript code, don't forget to install [starscope](https://github.com/eapache/starscope). Information on how to install this tool is available on its github page.

1. Change directory to the base folder of your source code like this:
```bash
cd ~/projects/myproject/src
```
2. Create a cscope.files file with all the Ruby, Go or Javascript source files listed in it.
```bash
find . -iname "*.rb"    > ./cscope.files
find . -iname "*.go"    >> ./cscope.files
find . -iname "*.js"    >> ./cscope.files
```
3. Create a cscope database like this:
```bash
starscope -e cscope
```
4. Create a ctags database like this.
```bash
ctags --fields=+i -n -L ./cscope.files
```
5. Run cqmakedb to create a CodeQuery database out of the cscope and ctags databases, like this:
```bash
cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p
```
6. Open myproject.db using the CodeQuery GUI tool by running the following. Wild card search (* and ?) supported if Exact Match is switched off. Or use cqsearch, the CLI-version of CodeQuery (type `cqsearch -h` for more info).
```bash
codequery
```
Use `cqmakedb -h` to get help on cqmakedb command line arguments.

