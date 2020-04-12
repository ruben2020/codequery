cd c:\workspace\codequery
md build
cd build
cmake -G "MinGW Makefiles" -DBUILD_QT5=ON -DSQLITE_INCLUDE_DIR=c:\qt\Tools\mingw730_64\x86_64-w64-mingw32\include -DSQLITE_LIBRARY_RELEASE=c:\qt\Tools\mingw730_64\bin\sqlite3.dll ..
mingw32-make
strip.exe .\*.exe
md output
copy *.exe output
copy c:\Qt\tools\mingw730_64\bin\sqlite3.dll output
cd output
windeployqt --release codequery.exe
dir/b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension c:\workspace\codequery\windows-install\win64\codequery64.wxs
light.exe -ext WixUIExtension -ext WixUtilExtension codequery64.wixobj
