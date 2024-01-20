cd c:\workspace\codequery
md build
cd build
cmake -G "Ninja" -DBUILD_QT5=ON -DSQLITE_INCLUDE_DIR="%MinGW64_DIR%\x86_64-w64-mingw32\include" -DSQLITE_LIBRARY_RELEASE="%MinGW64_DIR%\bin\sqlite3.dll" ..
ninja
md output
copy *.exe output
copy "%MinGW64_DIR%\bin\sqlite3.dll" output
cd output
windeployqt --release codequery.exe
dir/b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension "c:\workspace\codequery\windows-install\win64\codequery64.wxs"
light.exe -ext WixUIExtension -ext WixUtilExtension codequery64.wixobj
