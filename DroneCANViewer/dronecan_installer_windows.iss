; Script for generating a Windows installer for DroneCANViewer application
; Uses the innosetup tool - https://www.jrsoftware.org/isinfo.php

#define MyAppName "DroneCAN Viewer"
#define MyAppVersion "0.0.1"
#define MyAppPublisher "DroneCAN"
#define MyAppURL "https://github.com/dronecan"
#define MyAppExeName "DroneCANViewer.exe"
#define MyBaseAppId "{82CE89F2-8010-1234-96AC-975253AF5FEB}"

[Setup]
AppId={code:GetAppId}     
AppName={#MyAppName}
AppVersion={#MyAppVersion}
VersionInfoVersion={#MyAppVersion}
VersionInfoCompany={#MyAppPublisher}
VersionInfoDescription={#MyAppName}
AppCopyright={#MyAppPublisher}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={code:GetDefaultDirName}
DisableProgramGroupPage=yes

; Uncomment the following line to run in non administrative install mode (install for current user only.)
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

; UsePreviousPrivileges must be no when AppId includes constants
UsePreviousPrivileges=no
OutputBaseFilename=Setup_DroneCANViewer_{#MyAppVersion}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
;SetupIconFile=resource\icon\dronecan.ico
; UsePreviousLanguage must be no when AppId includes constants
UsePreviousLanguage=no
;SignTool=key $f
;SignedUninstaller=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; Main application executable
Source: "wininstall\DroneCANViewer.exe"; DestDir: "{app}"; Flags: ignoreversion

; Other files required for DroneCAN install

Source: "wininstall\*.dll"; DestDir: "{app}";

Source: "resource\release_notes.txt"; DestDir: "{app}";

; Directories required for DroneCAN install
Source: "wininstall\bearer\*"; DestDir: "{app}\bearer"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "wininstall\canbus\*"; DestDir: "{app}\canbus"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "wininstall\iconengines\*"; DestDir: "{app}\iconengines"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "wininstall\imageformats\*"; DestDir: "{app}\imageformats"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "wininstall\platforms\*"; DestDir: "{app}\platforms"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "wininstall\printsupport\*"; DestDir: "{app}\printsupport"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "wininstall\styles\*"; DestDir: "{app}\styles"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "wininstall\translations\*"; DestDir: "{app}\translations"; Flags: ignoreversion recursesubdirs createallsubdirs

; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{autoprograms}\{#MyAppName} {#MyAppVersion}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName} {#MyAppVersion}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
//   The following code is to optionally allow multiple DroneCAN versions to be
// installed on the one computer.  If a previous DroneCAN installation is
// detected, a dialogue box asks the user whether they want to overwrite the
// existing installation, or install a new one.
var
  AppId: string;
  DefaultDirName: string;

// Getter for AppId  
function GetAppId(Param: string): string;
begin
  Result := AppId;
  Log('AppId = ' + Result);
end;

// Getter for DefaultDirName
function GetDefaultDirName(Param: string): string;
begin
  Result := DefaultDirName;
  Log('DefaultDirName = ' + Result);
end;

//   Impletement the event function InitializeSetup.  This is called by Inno
// Setup "during Setup's initialisation".
function InitializeSetup(): Boolean;
var
  PrevVersion: string;
  CurVersion: string;
  Message: string;
  R: Integer;

begin
  CurVersion := '{#MyAppVersion}';
  Log(Format('Installing "%s"', [CurVersion]));
          
  // Returning True means that installation will proceed
  Result := True;
     
  AppId := '{#MyBaseAppId}';
  DefaultDirName := ExpandConstant('{autopf}\DroneCAN\{#MyAppName}');

  // Look in the registry to see if DroneCAN is already installed      
  if RegQueryStringValue(HKLM, 'Software\Microsoft\Windows\CurrentVersion\Uninstall\{#MyBaseAppId}_is1', 'DisplayVersion', PrevVersion) or
     RegQueryStringValue(HKCU, 'SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\{#MyBaseAppId}_is1', 'DisplayVersion', PrevVersion) or
     RegQueryStringValue(HKLM, 'Software\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{#MyBaseAppId}_is1', 'DisplayVersion', PrevVersion) or
     RegQueryStringValue(HKCU, 'SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\{#MyBaseAppId}_is1', 'DisplayVersion', PrevVersion) then
  begin     
    Message :=
      Format(
        'Version is %s already installed. Do you want to upgrade to %s?'#13#10#13#10+
        'Press Yes, to replace %0:s with %1:s.'#13#10+
        'Press No, to keep %0:s and add separate installation of %1:s.'#13#10, [
        PrevVersion, CurVersion]);
    R := MsgBox(Message, mbConfirmation, MB_YESNOCANCEL);
    if R = IDYES then
    begin
      Log('User chose to replace previous installation');
    end
    else if R = IDNO then
    begin
      AppId := AppId + CurVersion;
      DefaultDirName := DefaultDirName + ' ' + CurVersion;
      Log('User chose to install new copy - using ID ' + AppId);
    end
    else
    begin
      Log('User chose to cancel installation');

      // Returning False will cancel the installation
      Result := False;
    end;
  end;
end;
