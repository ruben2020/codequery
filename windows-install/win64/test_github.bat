cd
echo PATH=%PATH%
dir/a build\*.exe
"windows-install\win64\cscope.exe" -cbR
"windows-install\win64\ctags.exe" --fields=+i -nR
dir/a
"build\cqsearch.exe" -v
"build\cqmakedb.exe" -v
"build\cqmakedb.exe" -s cq.db -c cscope.out -t tags -p -d
"build\cqsearch.exe" -s cq.db -p 1 -t CODEQUERY_SW_VERSION -u
