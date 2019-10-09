; Inno setup script to generate Windows 32 bits and 64 bits installers
#ifdef x64
  #define Arch "x64"
  #define RegKey ""
  #define ArchMode "x64"
#else                     
  #define Arch "x86"
  #define RegKey "WOW6432Node\"
  #define ArchMode  ""
#endif

#define MyAppName "Sound Generator"
#define MyAppPublisher "Ben Pyton"
#define MyAppURL "https://github.com/BenPyton/SoundGenerator"
#define MyAppExeName "SoundGenerator.exe"
#define OutputDir "installs"  
#define RedistDir "vcredist" 

[Setup]
; NOTE: The value of AppId uniquely identifies this application. Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{4D1C544C-1EEA-4A60-9F23-EA61247E0224}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile={#BuildDir}\..\LICENSE
PrivilegesRequiredOverridesAllowed=dialog
OutputDir={#BuildDir}\{#OutputDir}
OutputBaseFilename=soundgenerator_{#MyAppVersion}_{#Arch}
Compression=lzma
SolidCompression=yes
WizardStyle=modern
ArchitecturesInstallIn64BitMode = {#ArchMode}
ArchitecturesAllowed={#ArchMode}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "french"; MessagesFile: "compiler:Languages\French.isl"
Name: "japanese"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "spanish"; MessagesFile: "compiler:Languages\Spanish.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#BuildDir }\{#Arch}\SoundGenerator.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\audio\*"; DestDir: "{app}\audio\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\bearer\*"; DestDir: "{app}\bearer\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\iconengines\*"; DestDir: "{app}\iconengines\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\imageformats\*"; DestDir: "{app}\imageformats\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\mediaservice\*"; DestDir: "{app}\mediaservice\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\platforms\*"; DestDir: "{app}\platforms\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\playlistformats\*"; DestDir: "{app}\playlistformats\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\styles\*"; DestDir: "{app}\styles\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\styles\*"; DestDir: "{app}\styles\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\translations\*"; DestDir: "{app}\translations\"; Flags: ignoreversion recursesubdirs createallsubdirs
Source: "{#BuildDir }\{#Arch}\D3Dcompiler_47.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\libEGL.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\libGLESV2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\opengl32sw.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\Qt5Core.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\Qt5Gui.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\Qt5Multimedia.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\Qt5Network.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\Qt5Svg.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\{#Arch}\Qt5Widgets.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\..\LICENSE"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\..\attributions.md"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BuildDir }\..\ReadMe.md"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files           
; VC++ redistributable runtime. Extracted by VC2017RedistNeedsInstall(), if needed.  
Source: "{#BuildDir}\{#RedistDir}\vc_redist.{#Arch}.exe"; DestDir: {tmp}; Flags: dontcopy

[CustomMessages]
InstallingVC2017redist=Installing Microsoft Visual C++ 2017 {#Arch} Redistributable...  

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]                                                                                                                                                                                                                                                                                                                                  
Filename: "{tmp}\vc_redist.{#Arch}.exe"; StatusMsg: "{cm:InstallingVC2017redist}"; Parameters: "/q /norestart"; Check: VC2017RedistNeedsInstall ; Flags: waituntilterminated
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[Code]
function VC2017RedistNeedsInstall: Boolean;
var 
  Version: String;
begin
  if (RegQueryStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE\{#RegKey}Microsoft\DevDiv\vc\Servicing\RuntimeMinimu', 'Version', Version)) then
  begin
    // Is the installed version at least 14.16 ? 
    Log('VC Redist Version check : found ' + Version);
    Result := (CompareStr(Version, 'v14.16.27027.1')<0);
  end
  else 
  begin
    // Not even an old version installed
    Result := True;
  end;
  if (Result) then
  begin
    ExtractTemporaryFile('vc_redist.{#Arch}.exe');
  end;
end;
