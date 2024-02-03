cd c:\workspace\codequery
md build
cd build
cmake -G "Ninja" -DBUILD_QT6=ON -DSQLITE_INCLUDE_DIR="%MinGW64_DIR%\x86_64-w64-mingw32\include" -DSQLITE_LIBRARY_RELEASE="%MinGW64_DIR%\bin\sqlite3.dll" ..
ninja
rem md output
rem copy *.exe output
rem copy "%MinGW64_DIR%\bin\sqlite3.dll" output
rem cd output
rem windeployqt --release codequery.exe
rem dir/b/a/s
rem candle.exe -ext WixUIExtension -ext WixUtilExtension "c:\workspace\codequery\windows-install\win64\codequery64.wxs"
rem light.exe -ext WixUIExtension -ext WixUtilExtension codequery64.wixobj
