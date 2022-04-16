cd build
md output
copy *.exe output
copy "c:\vcpkg\installed\x64-windows\bin\sqlite3.dll" output
cd output
windeployqt codequery.exe
dir/b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension "..\..\windows-install\win64\codequery64.wxs"
light.exe -ext WixUIExtension -ext WixUtilExtension codequery64.wixobj
