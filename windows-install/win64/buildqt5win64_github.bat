set PATH=%Qt5_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
md build
cd build
cmake -G "MinGW Makefiles" -DBUILD_QT5=ON -DGHAWIN=ON -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="c:\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
mingw32-make
cd ..

