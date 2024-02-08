set PATH=%Qt5_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
md buildqt5
cd buildqt5
cmake -G Ninja -DBUILD_QT5=ON -DGHAWIN=ON -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="c:\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
ninja
cd ..
