cd c:\workspace\codequery
md buildqt6
cd buildqt6
cmake -G "Ninja" -DBUILD_QT6=ON -DSQLITE_INCLUDE_DIR="%MinGW64_DIR%\x86_64-w64-mingw32\include" -DSQLITE_LIBRARY_RELEASE="%MinGW64_DIR%\bin\sqlite3.dll" ..
ninja
md output
copy *.exe output
copy "%MinGW64_DIR%\bin\sqlite3.dll" output
cd output
windeployqt --release codequery.exe
dir/b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension "c:\workspace\codequery\windows-install\win64\codequeryqt6.wxs"
light.exe -ext WixUIExtension -ext WixUtilExtension codequeryqt6.wixobj
