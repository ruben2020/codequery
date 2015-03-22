[Files]
Source: "C:\workspace\codequery\build\cqmakedb.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "C:\workspace\codequery\build\cqsearch.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "C:\workspace\codequery\build\codequery.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "C:\workspace\codequery\windows-install\README.txt"; DestDir: "{app}"; Flags: overwritereadonly isreadme
Source: "C:\workspace\codequery\windows-install\LICENSE.txt"; DestDir: "{app}"; Flags: overwritereadonly
Source: "C:\workspace\codequery\windows-install\HOWTO-WINDOWS.txt"; DestDir: "{app}"; Flags: overwritereadonly
Source: "C:\workspace\codequery\windows-install\cscope.exe"; DestDir: "{app}"; Flags: 32bit
Source: "C:\workspace\codequery\windows-install\ctags.exe"; DestDir: "{app}"; Flags: 32bit
Source: "C:\MinGW\bin\libgcc_s_dw2-1.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\MinGW\bin\libstdc++-6.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\MinGW\bin\mingwm10.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\MinGW\bin\sqlite3.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\Qt\4.8.5\bin\QtCore4.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\Qt\4.8.5\bin\QtGui4.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\Qt\4.8.5\bin\QtXml4.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\Qt\4.8.5\bin\qscintilla2.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile

[Icons]
Name: "{group}\CodeQuery"; Filename: "{app}\codequery.exe"; WorkingDir: "{app}"; IconFilename: "{app}\codequery.exe"
Name: "{group}\README"; Filename: "{app}\README.txt"; WorkingDir: "{app}"
Name: "{group}\LICENSE"; Filename: "{app}\LICENSE.txt"; WorkingDir: "{app}"
Name: "{group}\How to Use CodeQuery"; Filename: "{app}\HOWTO-WINDOWS.txt"; WorkingDir: "{app}"
Name: "{group}\Updates Notification"; Filename: "{app}\updatesnoti.htm"; WorkingDir: "{app}"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"; WorkingDir: "{app}"

[Setup]
ShowLanguageDialog=auto
AppName=CodeQuery
AppVersion=0.15
AppCopyright=Copyright 2013-2014 (C) ruben2020
AppId={{D95A3CC0-CC79-4CF1-A92B-50E909B60EF1}
LicenseFile=C:\workspace\codequery\windows-install\LICENSE.txt
DefaultDirName={pf}\CodeQuery
AppPublisher=ruben2020
AppPublisherURL=https://github.com/ruben2020/codequery
AppSupportURL=https://github.com/ruben2020/codequery
AppUpdatesURL=https://github.com/ruben2020/codequery
DefaultGroupName=CodeQuery
ChangesEnvironment=true

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

