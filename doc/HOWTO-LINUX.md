
HOWTO GUIDE
===========

## HOW TO USE CODEQUERY WITH C/C++ CODE?

This HOWTO guide applies to Linux only

1. Change directory to the base folder of your source code like this:

     cd ~/projects/myproject/src


2. Create a cscope.files file with all the C/C++ source
   files listed in it.

     find -iname "*.c"    > ./cscope.files
     find -iname "*.cpp" >> ./cscope.files
     find -iname "*.cxx" >> ./cscope.files
     find -iname "*.h"   >> ./cscope.files
     find -iname "*.hpp" >> ./cscope.files
     find -iname "*.hxx" >> ./cscope.files


3. Create a cscope database like this:

     cscope -cbR


4. Create a ctags database like this:

     ctags --fields=+i -n -R -L ./cscope.files


5. Run cqmakedb to create a CodeQuery database out of the
   cscope and ctags databases, like this:

     cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p


6. Open myproject.db using the CodeQuery GUI tool by running:

     codequery

Use cqmakedb -h to get help on cqmakedb command line arguments.



## HOW TO USE CODEQUERY WITH JAVA CODE?

1. Change directory to the base folder of your source code like this:

     cd ~/projects/myproject/src


2. Create a cscope.files file with all the Java source
   files listed in it.

     find -iname "*.java" > ./cscope.files


3. Create a cscope database like this:

     cscope -cbR


4. Create a ctags database like this:

     ctags --fields=+i -n -R -L ./cscope.files


5. Run cqmakedb to create a CodeQuery database out of the
   cscope and ctags databases, like this:

     cqmakedb -s ./myproject.db -c ./cscope.out -t ./tags -p


6. Open myproject.db using the CodeQuery GUI tool by running:

     codequery

Use cqmakedb -h to get help on cqmakedb command line arguments.

