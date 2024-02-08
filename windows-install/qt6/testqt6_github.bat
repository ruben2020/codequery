copy "c:\vcpkg\installed\x64-windows\bin\sqlite3.dll" buildqt6
copy "C:\mingw64\bin\libgcc_s_seh-1.dll" buildqt6
copy "C:\mingw64\bin\libstdc++-6.dll" buildqt6
copy "C:\mingw64\bin\libwinpthread-1.dll" buildqt6
"buildqt6\cqmakedb.exe" -v
"windows-install\wincommon\cscope.exe" -cbR
"windows-install\wincommon\ctags.exe" --fields=+i -nR
"buildqt6\cqmakedb.exe" -s cq.db -c cscope.out -t tags -p -d
"buildqt6\cqsearch.exe" -s cq.db -p 1 -t CODEQUERY_SW_VERSION -u
"windows-install\wincommon\cscope.exe" -bR -f cscope1.out
"buildqt6\cqmakedb.exe" -s cq1.db -c cscope1.out -t tags -p -d
"buildqt6\cqsearch.exe" -s cq1.db -p 1 -t CODEQUERY_SW_VERSION -u
