set PATH=%Qt5_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
dir /b/a/s c:\vcpkg\vcpkg.cmake
dir /b/a/s c:\vcpkg\*sqlite*.*
md build
cd build
cmake -G "MinGW Makefiles" -DBUILD_QT5=ON -DGHAWIN=ON ..
mingw32-make
