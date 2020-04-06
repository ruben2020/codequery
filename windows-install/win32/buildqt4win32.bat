cd c:\workspace\codequery
md build
cd build
cmake -G "MinGW Makefiles" -DBUILD_QT5=OFF ..
mingw32-make
md output
copy *.exe output
copy C:\mingw32\bin\sqlite3.dll output
copy "C:\mingw32\bin\libgcc_s_dw2-1.dll" output
copy "C:\mingw32\bin\libstdc++-6.dll" output
copy C:\mingw32\bin\libwinpthread-1.dll output
copy C:\Qt\4.8.7\bin\QtCore4.dll output
copy C:\Qt\4.8.7\bin\QtGui4.dll output
copy C:\Qt\4.8.7\bin\QtXml4.dll output
cd output
dir/b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension c:\workspace\codequery\windows-install\win32\codequery32.wxs
light.exe -ext WixUIExtension -ext WixUtilExtension codequery32.wixobj
