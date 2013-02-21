[Files]
Source: "C:\workspace\CodeQuery\build\cqmakedb.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "C:\workspace\CodeQuery\build\codequery.exe"; DestDir: "{app}"; Flags: 32bit overwritereadonly
Source: "C:\workspace\CodeQuery\windows-install\README.txt"; DestDir: "{app}"; Flags: overwritereadonly isreadme
Source: "C:\workspace\CodeQuery\windows-install\LICENSE.txt"; DestDir: "{app}"; Flags: overwritereadonly
Source: "C:\workspace\CodeQuery\windows-install\HOWTO-WINDOWS.txt"; DestDir: "{app}"; Flags: overwritereadonly
Source: "C:\workspace\CodeQuery\windows-install\cscope.exe"; DestDir: "{app}"; Flags: 32bit
Source: "C:\workspace\CodeQuery\windows-install\ctags.exe"; DestDir: "{app}"; Flags: 32bit
Source: "C:\MinGW\bin\libgcc_s_dw2-1.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\MinGW\bin\libstdc++-6.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\MinGW\bin\mingwm10.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\MinGW\bin\sqlite3.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\Qt\4.8.4\bin\QtCore4.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\Qt\4.8.4\bin\QtGui4.dll"; DestDir: "{sys}"; Flags: 32bit sharedfile
Source: "C:\workspace\CodeQuery\gui\images\logo1616.ico"; DestDir: "{app}"; Flags: overwritereadonly

[Icons]
Name: "{group}\CodeQuery"; Filename: "{app}\codequery.exe"; WorkingDir: "{app}"; IconFilename: "{app}\logo1616.ico"
Name: "{group}\README"; Filename: "{app}\README.txt"; WorkingDir: "{app}"
Name: "{group}\LICENSE"; Filename: "{app}\LICENSE.txt"; WorkingDir: "{app}"
Name: "{group}\How to Use CodeQuery"; Filename: "{app}\HOWTO-WINDOWS.txt"; WorkingDir: "{app}"
Name: "{group}\Uninstall"; Filename: "{uninstallexe}"; WorkingDir: "{app}"

[Setup]
ShowLanguageDialog=auto
AppName=CodeQuery
AppVersion=0.03
AppCopyright=Copyright 2013 (C) ruben2020
AppId={{D95A3CC0-CC79-4CF1-A92B-50E909B60EF1}
LicenseFile=C:\workspace\CodeQuery\windows-install\LICENSE.txt
DefaultDirName={pf}\CodeQuery
AppPublisher=ruben2020
AppPublisherURL=https://github.com/ruben2020/codequery
AppSupportURL=https://github.com/ruben2020/codequery
AppUpdatesURL=https://github.com/ruben2020/codequery
DefaultGroupName=CodeQuery
ChangesEnvironment=true

[Tasks]
Name: "modifypath"; Description: "Add application directory to your environmental path"; Flags:

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
