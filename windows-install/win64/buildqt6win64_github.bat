aqt list-qt windows desktop
set PATH=%Qt6_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
md build
cd build
cmake -G Ninja -DBUILD_QT6=ON -DGHAWIN=ON -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="c:\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
ninja
cd ..
