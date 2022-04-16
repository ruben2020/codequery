set PATH=%Qt5_DIR%/bin;%PATH%
cmake --version
gcc -v
g++ -v
md build
cd build
cmake -G "MinGW Makefiles" -DBUILD_QT5=ON -DGHAWIN=ON -DVCPKG_TARGET_TRIPLET=x64-windows -DCMAKE_TOOLCHAIN_FILE="c:\vcpkg\scripts\buildsystems\vcpkg.cmake" ..
mingw32-make
cd ..
build\cqmakedb.exe -v
"windows-install\win64\cscope.exe" -cbR
"windows-install\win64\ctags.exe" --fields=+i -nR
build\cqmakedb.exe -s cq.db -c cscope.out -t tags -p -d
build\cqsearch.exe -s cq.db -p 1 -t CODEQUERY_SW_VERSION -u

