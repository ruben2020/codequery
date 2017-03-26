[Files]
Source: "\workspace\codequery\build\cqmakedb.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\workspace\codequery\build\cqsearch.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\workspace\codequery\build\codequery.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\workspace\codequery\windows-install\README.txt"; DestDir: "{app}"; Flags: overwritereadonly isreadme
Source: "\workspace\codequery\windows-install\LICENSE.txt"; DestDir: "{app}"; Flags: overwritereadonly
Source: "\workspace\codequery\windows-install\HOWTO-WINDOWS.txt"; DestDir: "{app}"; Flags: overwritereadonly
Source: "\workspace\codequery\windows-install\cscope.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\workspace\codequery\windows-install\ctags.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\Qt\4.8.6\bin\qscintilla2.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\Qt\4.8.6\bin\QtCore4.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\Qt\4.8.6\bin\QtGui4.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\Qt\4.8.6\bin\QtXml4.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\mingw32\bin\sqlite3.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\mingw32\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\mingw32\bin\libstdc++-6.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "\mingw32\bin\libwinpthread-1.dll"; DestDir: "{app}"; Flags: 32bit overwritereadonly

[Icons]
Name: "{group}\CodeQuery"; Filename: "{app}\codequery.exe"; WorkingDir: "{app}"; IconFilename: "{app}\codequery.exe"
Name: "{group}\README"; Filename: "{app}\README.txt"; WorkingDir: "{app}"
Name: "{group}\LICENSE"; Filename: "{app}\LICENSE.txt"; WorkingDir: "{app}"
Name: "{group}\How to Use CodeQuery"; Filename: "{app}\HOWTO-WINDOWS.txt"; WorkingDir: "{app}"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"; WorkingDir: "{app}"

[Setup]
ShowLanguageDialog=auto
AppName=CodeQuery
AppVersion=0.19.0
AppCopyright=Copyright 2013-2017 (C) ruben2020
AppId={{D95A3CC0-CC79-4CF1-A92B-50E909B60EF1}
LicenseFile=\workspace\codequery\windows-install\LICENSE.txt
DefaultDirName={pf}\CodeQuery
AppPublisher=ruben2020
AppPublisherURL=https://github.com/ruben2020/codequery
AppSupportURL=https://github.com/ruben2020/codequery
AppUpdatesURL=https://github.com/ruben2020/codequery
DefaultGroupName=CodeQuery
ChangesEnvironment=true
DisableStartupPrompt=False

[Tasks]
Name: "modifypath"; Description: "Add application directory to your environmental path"; Flags:

[Run]
Filename: "{app}\codequery.exe"; Description: "Launch application"; Flags: postinstall nowait skipifsilent unchecked

[Code]
const
    ModPathName = 'modifypath';
    ModPathType = 'user';

function ModPathDir(): TArrayOfString;
begin
    setArrayLength(Result, 1)
    Result[0] := ExpandConstant('{app}');
end;
#include "modpath.iss"

