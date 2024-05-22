HOW TO INSTALL CODEQUERY IN LINUX
=================================

This INSTALL guide applies to Linux and Mac only

(For Windows, a SETUP EXE package will be provided)


## HOW TO INSTALL IN LINUX?

Step 1: Install CMake (>=3.16), sqlite3, Qt5 (>=5.12) or Qt6 (>=6.4) for Qt5 or Qt6, cscope (15.8a or higher), ctags. If you have Ubuntu, Linux Mint, Debian or Fedora installed, most of these should be obtainable through the package managers. Note that the cscope version on the Ubuntu repositories may not be the latest one. It's better to have the latest version of cscope installed.   
[CMake](http://www.cmake.org/)   
[sqlite3](http://www.sqlite.org/)   
[Qt5 or Qt6](http://qt-project.org/)   
[cscope](http://cscope.sourceforge.net/) for C/C++/Java   
[Exuberant ctags](http://ctags.sourceforge.net/)    
[Universal ctags](https://github.com/universal-ctags/ctags/)    
[pycscope](https://github.com/portante/pycscope) for Python    
[starscope](https://github.com/eapache/starscope) for Ruby, Go and Javascript    

In Ubuntu or Linux Mint or Debian, do the following first:    
```bash
sudo apt-get update
sudo apt-get install build-essential g++ git cmake ninja-build sqlite3 libsqlite3-dev cscope exuberant-ctags
```

In Ubuntu or Linux Mint or Debian, do the following next for Qt5:    
```bash
sudo apt-get install qtcreator qtbase5-dev qt5-qmake qttools5-dev-tools qttools5-dev
```

In Ubuntu or Linux Mint or Debian, do the following next for Qt6:    
```bash
sudo apt-get install libglx-dev libgl1-mesa-dev libvulkan-dev libxkbcommon-dev
sudo apt-get install qt6-base-dev qt6-base-dev-tools qt6-tools-dev qt6-tools-dev-tools
sudo apt-get install libqt6core5compat6-dev qt6-l10n-tools qt6-wayland
```

If you want to browse Python code, don't forget to install [pycscope](https://github.com/portante/pycscope) at this point. Information on how to install this tool is available on its github page.

If you want to browse Ruby, Go or Javascript code, don't forget to install [starscope](https://github.com/eapache/starscope) at this point. Information on how to install this tool is available on its github page.


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

Step 4: Run cmake (first line for Qt6 OR second line for Qt5 OR third line for no GUI).     
```bash
cmake -G Ninja -S . -B build
cmake -G Ninja -DBUILD_QT5=ON -S . -B build
cmake -G Ninja -DNO_GUI=ON -S . -B build
```

Step 5: Run make and make install.       
```bash
cmake --build build
sudo cmake --install build
```

If you want to install to an alternative directory instead of the default one, use the following:     
```bash
cmake -DCMAKE_INSTALL_PREFIX="/home/johndoe/tools/" -G Ninja -DBUILD_QT5=ON -S . -B build
cmake --build build
sudo cmake --install build
```


Step 6: Please read [HOWTO-LINUX](HOWTO-LINUX.md) to learn how to use this software.

If the theming of the codequery GUI app looks strange in environments other than KDE Plasma, please refer to [this page](https://wiki.archlinux.org/title/Qt#Configuration_of_Qt_5/6_applications_under_environments_other_than_KDE_Plasma) for hints.

