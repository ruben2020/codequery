set PATH=%PATH%;"%WIX%\bin"
cd buildqt6
md output
copy *.exe output
copy "c:\vcpkg\installed\x64-windows\bin\sqlite3.dll" output
copy "C:\mingw64\bin\libgcc_s_seh-1.dll" output
copy "C:\mingw64\bin\libstdc++-6.dll" output
copy "C:\mingw64\bin\libwinpthread-1.dll" output
cd output
rem windeployqt --release --verbose 0 --list relative codequery.exe
windeployqt codequery.exe
dir /b/a/s
candle.exe -ext WixUIExtension -ext WixUtilExtension "..\..\windows-install\qt6\codequeryqt671.wxs"
light.exe -ext WixUIExtension -ext WixUtilExtension codequeryqt671.wixobj
dir /a/s *.msi

