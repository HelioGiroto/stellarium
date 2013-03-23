; Stellarium installer
; Run "make install" first to generate binary and translation files.
; @ISS_AUTOGENERATED_WARNING@

[Setup]
@ISS_ARCHITECTURE_SPECIFIC@
DisableStartupPrompt=yes
WizardSmallImageFile=data\icon.bmp
WizardImageFile=data\splash.bmp
WizardImageStretch=no
WizardImageBackColor=clBlack
AppName=Stellarium
AppVersion=@PACKAGE_VERSION@
AppVerName=Stellarium @PACKAGE_VERSION@
AppPublisher=Stellarium team
AppPublisherURL=http://www.stellarium.org/
OutputBaseFilename=stellarium-@PACKAGE_VERSION@-@ISS_PACKAGE_PLATFORM@
OutputDir=installers
; In 64-bit mode, {pf} is equivalent to {pf64},
; see http://www.jrsoftware.org/ishelp/index.php?topic=32vs64bitinstalls
DefaultDirName={pf}\Stellarium
DefaultGroupName=Stellarium
UninstallDisplayIcon={app}\data\stellarium.ico
LicenseFile=COPYING
Compression=zip/9

[Files]
Source: "@CMAKE_INSTALL_PREFIX@\bin\stellarium.exe"; DestDir: "{app}"
Source: "@CMAKE_INSTALL_PREFIX@\lib\libstelMain.dll"; DestDir: "{app}"
Source: "stellarium.url"; DestDir: "{app}"
Source: "README"; DestDir: "{app}"; Flags: isreadme; DestName: "README.rtf"
Source: "INSTALL"; DestDir: "{app}"; DestName: "INSTALL.rtf"
Source: "COPYING"; DestDir: "{app}"; DestName: "GPL.rtf"
Source: "AUTHORS"; DestDir: "{app}"; DestName: "AUTHORS.rtf"
Source: "ChangeLog"; DestDir: "{app}"; DestName: "ChangeLog.rtf"
Source: "@ICONV_INCLUDE_DIR@/../bin/libiconv*.dll"; DestDir: "{app}";
Source: "@INTL_INCLUDE_DIR@/../bin/libintl*.dll"; DestDir: "{app}";
Source: "@ZLIB_INCLUDE_DIR@/../bin/zlib1.dll"; DestDir: "{app}";
@ISS_ARCH_SPECIFIC_MINGW_LIBS@
Source: "@QT_BINARY_DIR@\phonon4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtSql4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtSvg4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtCore4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtGui4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtOpenGL4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtNetwork4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtScript4.dll"; DestDir: "{app}";
Source: "@QT_BINARY_DIR@\QtXml4.dll"; DestDir: "{app}";
;Source: "@QT_PLUGINS_DIR@\sqldrivers\qsqlite4.dll"; DestDir: "{app}\sqldrivers\";
Source: "@CMAKE_INSTALL_PREFIX@\share\stellarium\*"; DestDir: "{app}\"; Flags: recursesubdirs
; Locales
Source: "@CMAKE_INSTALL_PREFIX@\share\locale\*"; DestDir: "{app}\locale\"; Flags: recursesubdirs

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"
Name: desktopicon\common; Description: "For all users"; GroupDescription: "Additional icons:"; Flags: exclusive
Name: desktopicon\user; Description: "For the current user only"; GroupDescription: "Additional icons:"; Flags: exclusive unchecked
Name: removeconfig; Description: "Remove the main configuration file"; GroupDescription: "Remove files left over from a previous installation:"; Flags: unchecked
Name: removeplugins; Description: "Remove plug-ins' configuration files"; GroupDescription: "Remove files left over from a previous installation:"; Flags: unchecked
Name: removesolar; Description: "Remove Solar System file"; GroupDescription: "Remove files left over from a previous installation:"; Flags: unchecked
Name: removelandscapes; Description: "Remove user-installed landscapes"; GroupDescription: "Remove files left over from a previous installation:"; Flags: unchecked
Name: removeshortcuts; Description: "Remove shortcut's configuration file"; GroupDescription: "Remove files left over from a previous installation:"; Flags: unchecked

[Run]
;An option to start Stellarium after setup has finished
Filename: "{app}\stellarium.exe"; Description: "Start Stellarium"; Flags: postinstall nowait skipifsilent unchecked

[InstallDelete]
;The old log file in all cases
Type: files; Name: "{userappdata}\Stellarium\log.txt"
Type: files; Name: "{userappdata}\Stellarium\config.ini"; Tasks: removeconfig
Type: files; Name: "{userappdata}\Stellarium\data\ssystem.ini"; Tasks: removesolar
Type: filesandordirs; Name: "{userappdata}\Stellarium\modules"; Tasks: removeplugins
Type: filesandordirs; Name: "{userappdata}\Stellarium\landscapes"; Tasks: removelandscapes
Type: files; Name: "{userappdata}\Stellarium\data\shortcuts.json"; Tasks: removeshortcuts

[UninstallDelete]

[Icons]
Name: "{group}\Visit Website of Stellarium"; Filename: "{app}\stellarium.url"; IconFilename: "{app}\data\stellarium.ico"
Name: "{group}\Stellarium"; Filename: "{app}\stellarium.exe"; WorkingDir: "{app}"; IconFilename: "{app}\data\stellarium.ico"
Name: "{group}\Stellarium (fallback mode)"; Filename: "{app}\stellarium.exe"; Parameters: "--safe-mode"; WorkingDir: "{app}"; IconFilename: "{app}\data\stellarium.ico"
Name: "{group}\Uninstall Stellarium"; Filename: "{uninstallexe}"
Name: "{group}\config.ini"; Filename: "{userappdata}\Stellarium\config.ini"
Name: "{group}\Last run log"; Filename: "{userappdata}\Stellarium\log.txt"
Name: "{group}\Change log"; Filename: "{app}\ChangeLog.rtf"
Name: "{commondesktop}\Stellarium"; Filename: "{app}\stellarium.exe"; WorkingDir: "{app}"; IconFilename: "{app}\data\stellarium.ico"; Tasks: desktopicon\common
Name: "{userdesktop}\Stellarium"; Filename: "{app}\stellarium.exe"; WorkingDir: "{app}"; IconFilename: "{app}\data\stellarium.ico"; Tasks: desktopicon\user

[Languages]
; Official translations of GUI of Inno Setup
; Recommended use InnoSetup 5.5.3+ (Unicode)
Name: "en"; MessagesFile: "compiler:Default.isl"
Name: "ca"; MessagesFile: "compiler:Languages\Catalan.isl"
Name: "co"; MessagesFile: "compiler:Languages\Corsican.isl"
Name: "cs"; MessagesFile: "compiler:Languages\Czech.isl"
Name: "da"; MessagesFile: "compiler:Languages\Danish.isl"
Name: "nl"; MessagesFile: "compiler:Languages\Dutch.isl"
Name: "fi"; MessagesFile: "compiler:Languages\Finnish.isl"
Name: "fr"; MessagesFile: "compiler:Languages\French.isl"
Name: "de"; MessagesFile: "compiler:Languages\German.isl"
Name: "el"; MessagesFile: "compiler:Languages\Greek.isl"
Name: "he"; MessagesFile: "compiler:Languages\Hebrew.isl"
Name: "hu"; MessagesFile: "compiler:Languages\Hungarian.isl"
Name: "it"; MessagesFile: "compiler:Languages\Italian.isl"
Name: "ja"; MessagesFile: "compiler:Languages\Japanese.isl"
Name: "no"; MessagesFile: "compiler:Languages\Norwegian.isl"
Name: "pl"; MessagesFile: "compiler:Languages\Polish.isl"
Name: "pt_BR"; MessagesFile: "compiler:Languages\BrazilianPortuguese.isl"
Name: "pt"; MessagesFile: "compiler:Languages\Portuguese.isl"
Name: "ru"; MessagesFile: "compiler:Languages\Russian.isl"
Name: "sr"; MessagesFile: "compiler:Languages\SerbianCyrillic.isl"
Name: "sl"; MessagesFile: "compiler:Languages\Slovenian.isl"
Name: "es"; MessagesFile: "compiler:Languages\Spanish.isl"
Name: "uk"; MessagesFile: "compiler:Languages\Ukrainian.isl"
; Unofficial translations of GUI of Inno Setup
Name: "gla"; MessagesFile: "util\ISL\ScotsGaelic.isl"

