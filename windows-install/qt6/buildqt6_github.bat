aqt list-qt windows desktop
aqt list-qt windows desktop --arch 6.6.1
aqt list-qt windows desktop --modules 6.6.1 win64_mingw
set PATH=%Qt6_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
md buildqt6
cd buildqt6
cmake -G Ninja -DBUILD_QT6=ON -DGHAWIN=ON -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="c:\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
ninja
cd ..
