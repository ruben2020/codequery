
HOWTO GUIDE
===========

This HOWTO guide applies to Linux only

## HOW TO USE CODEQUERY WITH C/C++ CODE?

1. Change directory to the base folder of your source code like this:
```bash
cd ~/projects/myproject/src
```

2. Create a cscope.files file with all the C/C++ source files listed in it.
```bash
find -iname "*.c"    > ./cscope.files
find -iname "*.cpp" >> ./cscope.files
find -iname "*.cxx" >> ./cscope.files
find -iname "*.h"   >> ./cscope.files
find -iname "*.hpp" >> ./cscope.files
find -iname "*.hxx" >> ./cscope.files
```

3. Create a cscope database like this:
```bash
cscope -cbR
```

4. Create a ctags database like this.
```bash
ctags --fields=+i -n -R -L ./cscope.files
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
find -iname "*.java" > ./cscope.files
```

3. Create a cscope database like this:
```bash
cscope -cbR
```

4. Create a ctags database like this:
```bash
ctags --fields=+i -n -R -L ./cscope.files
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

1. Change directory to the base folder of your source code like this:
```bash
cd ~/projects/myproject/src
```

2. Create a cscope.files file with all the Python source files listed in it.
```bash
find -iname "*.py"    > ./cscope.files
```

3. Create a cscope database like this:
```bash
pycscope -i ./cscope.files
```

4. Create a ctags database like this.
```bash
ctags --fields=+i -n -R -L ./cscope.files
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



## HOW TO USE CODEQUERY WITH RUBY AND GO CODE?

1. Change directory to the base folder of your source code like this:
```bash
cd ~/projects/myproject/src
```

2. Create a cscope.files file with all the Ruby or Go source files listed in it.
```bash
find -iname "*.rb"    > ./cscope.files
find -iname "*.go"    > ./cscope.files
```

3. Create a cscope database like this:
```bash
starscope -e cscope
```

4. Create a ctags database like this.
```bash
ctags --fields=+i -n -R -L ./cscope.files
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

