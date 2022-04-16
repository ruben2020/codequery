set PATH=%Qt5_DIR%/bin;%PATH%
md build
cd build
cmake -G "MinGW Makefiles" -DBUILD_QT5=ON ..
mingw32-make
