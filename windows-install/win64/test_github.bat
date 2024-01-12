copy "c:\vcpkg\installed\x64-windows\bin\sqlite3.dll" build
copy "C:\mingw64\bin\libgcc_s_seh-1.dll" build
copy "C:\mingw64\bin\libstdc++-6.dll" build
copy "C:\mingw64\bin\libwinpthread-1.dll" build
"build\cqmakedb.exe" -v
"windows-install\win64\cscope.exe" -cbR
"windows-install\win64\ctags.exe" --fields=+i -nR
"build\cqmakedb.exe" -s cq.db -c cscope.out -t tags -p -d
"build\cqsearch.exe" -s cq.db -p 1 -t CODEQUERY_SW_VERSION -u
"windows-install\win64\cscope.exe" -bR -f cscope1.out
"build\cqmakedb.exe" -s cq1.db -c cscope1.out -t tags -p -d
"build\cqsearch.exe" -s cq1.db -p 1 -t CODEQUERY_SW_VERSION -u
