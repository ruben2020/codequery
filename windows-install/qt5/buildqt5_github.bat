set PATH=%Qt5_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
cmake -G Ninja -DBUILD_QT5=ON -DGHAWIN=ON -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="c:\vcpkg\scripts\buildsystems\vcpkg.cmake" -S . -B buildqt5
cmake --build buildqt5

