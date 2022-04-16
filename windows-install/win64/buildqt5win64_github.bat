set PATH=%Qt5_DIR%/bin;%Qt5_DIR%/../../Tools/mingw810_64/bin;%Qt5_DIR%/../../Tools/CMake_64/bin;%PATH%
md build
cd build
cmake -G "MinGW Makefiles" -DBUILD_QT5=ON -DSQLITE_INCLUDE_DIR="%Qt5_DIR%/../../Tools/mingw810_64/opt/include" -DSQLITE_LIBRARY_RELEASE="%Qt5_DIR%/../../Tools/mingw810_64/opt/lib/sqlite3.21.0/sqlite3210.dll" ..
mingw32-make
