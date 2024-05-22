cd c:\workspace\codequery
cmake -G "Ninja" -DSQLITE_INCLUDE_DIR="%MinGW64_DIR%\x86_64-w64-mingw32\include" -DSQLITE_LIBRARY_RELEASE="%MinGW64_DIR%\bin\sqlite3.dll" -S . -B buildqt6
cmake --build buildqt6
cd buildqt6
md output
copy *.exe output
copy "%MinGW64_DIR%\bin\sqlite3.dll" output
cd output
windeployqt --release codequery.exe
dir/b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension "c:\workspace\codequery\windows-install\qt6\codequeryqt671.wxs"
light.exe -ext WixUIExtension -ext WixUtilExtension codequeryqt671.wixobj
