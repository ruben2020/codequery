
HOW TO INSTALL CODEQUERY IN LINUX
=================================

This INSTALL guide applies to Linux only

(For Windows, a SETUP EXE package will be provided)


## HOW TO INSTALL IN LINUX?

1. Install CMake (>2.7), sqlite3, Qt4 (>4.7), cscope (15.8a or higher), ctags. If you have Ubuntu installed, most of these should be obtainable through the Synaptic Package Manager. Note that the cscope version on the Ubuntu repositories may not be the latest one. It's better to have the latest version of cscope installed.   
[CMake](http://www.cmake.org/)   
[sqlite3](http://www.sqlite.org/)   
[Qt4](http://qt-project.org/)   
[cscope](http://cscope.sourceforge.net/)   
[ctags](http://ctags.sourceforge.net/)


2. Download the repository as a ZIP file from github:
[codequery@github](https://github.com/ruben2020/codequery)


3. Unzip to a directory and change to that directory.
```bash
cd ~/workspace/codequery
```

4. Create a directory called build and change to it.
```bash
mkdir build
cd build
```

5. Run cmake, make and make install.
```bash
cmake ..
make
sudo make install
```

6. Please read [HOWTO-LINUX](HOWTO-LINUX.md) to learn how to use this software.

