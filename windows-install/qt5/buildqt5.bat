cd c:\workspace\codequery
cmake -G "Ninja" -DBUILD_QT5=ON -DSQLITE_INCLUDE_DIR="%MinGW64_DIR%\x86_64-w64-mingw32\include" -DSQLITE_LIBRARY_RELEASE="%MinGW64_DIR%\bin\sqlite3.dll" -S . -B buildqt5
cmake --build buildqt5
cd buildqt5
md output
copy *.exe output
copy "%MinGW64_DIR%\bin\sqlite3.dll" output
cd output
windeployqt --release codequery.exe
dir/b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension "c:\workspace\codequery\windows-install\qt5\codequeryqt5.wxs"
light.exe -ext WixUIExtension -ext WixUtilExtension codequeryqt5.wixobj
