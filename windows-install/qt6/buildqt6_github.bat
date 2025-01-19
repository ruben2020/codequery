aqt list-qt windows desktop
aqt list-qt windows desktop --arch 6.9.0
aqt list-qt windows desktop --modules 6.9.0 win64_mingw
set PATH=%Qt6_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
cmake -G Ninja -DGHAWIN=ON -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="c:\vcpkg\scripts\buildsystems\vcpkg.cmake" -S . -B buildqt6
cmake --build buildqt6
