HOW TO INSTALL CODEQUERY IN LINUX
=================================

This INSTALL guide applies to Linux and Mac only

(For Windows, a SETUP EXE package will be provided)


## HOW TO INSTALL IN LINUX?

Step 1: Install CMake (>2.7), sqlite3, Qt4 (>4.7) or Qt5 (>5.7) for Qt4 or Qt5, cscope (15.8a or higher), ctags. If you have Ubuntu, Linux Mint, Debian or Fedora installed, most of these should be obtainable through the package managers. Note that the cscope version on the Ubuntu repositories may not be the latest one. It's better to have the latest version of cscope installed.   
[CMake](http://www.cmake.org/)   
[sqlite3](http://www.sqlite.org/)   
[Qt4 or Qt5](http://qt-project.org/)   
[cscope](http://cscope.sourceforge.net/)   
[ctags](http://ctags.sourceforge.net/)    
[pycscope](https://github.com/portante/pycscope)    

In Ubuntu or Linux Mint or Debian, do the following (first line for Qt5 OR second line for Qt4):    
```bash
sudo apt-get install g++ git cmake sqlite3 libsqlite3-dev qt5-default qttools5-dev-tools cscope exuberant-ctags
sudo apt-get install g++ git cmake sqlite3 libsqlite3-dev qt4-dev-tools cscope exuberant-ctags
```

In Arch Linux or Manjaro or Apricity, do the following (first line for Qt5 OR second line for Qt4):    
```bash
sudo pacman -S gcc git cmake sqlite qt5-base qt5-tools cscope ctags
sudo pacman -S gcc git cmake sqlite qt4 cscope ctags
```


Step 2: Download the repository as a ZIP file from github or clone git repository:     
[codequery@github](https://github.com/ruben2020/codequery)     
```bash
cd ~/myrepos
git clone https://github.com/ruben2020/codequery.git
```

Step 3: Unzip to a directory and change to that directory.     
```bash
cd ~/myrepos/codequery
```

Step 4: Create a directory called build and change to it.     
```bash
mkdir build
cd build
```

Step 5: Run cmake (first line for Qt5 OR second line for Qt4).     
```bash
cmake -G "Unix Makefiles" -DBUILD_QT5=ON ..
cmake -G "Unix Makefiles" -DBUILD_QT5=OFF ..
```

Step 6: Run make and make install.     
```bash
make
sudo make install
```

If you want to install to an alternative directory instead of the default one, use the following:     
```bash
cmake -DCMAKE_INSTALL_PREFIX="/home/johndoe/tools/" -G "Unix Makefiles" -DBUILD_QT5=ON ..
make
make install
```

Step 7: Please read [HOWTO-LINUX](HOWTO-LINUX.md) to learn how to use this software.

