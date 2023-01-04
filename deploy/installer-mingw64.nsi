!include "x64.nsh"

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "Z-Score Peak Detection"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_PUBLISHER "Jason C"
!define PRODUCT_WEB_SITE "https://stackoverflow.com/a/74416006"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\ZScorePeakDetection.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_STARTMENU_REGVAL "NSIS:StartMenuDir"

SetCompressor /SOLID lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "..\LICENSE"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Start menu page
var ICONS_GROUP
!define MUI_STARTMENUPAGE_NODISABLE
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "Z-Score Peak Detection"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${PRODUCT_STARTMENU_REGVAL}"
!insertmacro MUI_PAGE_STARTMENU Application $ICONS_GROUP
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\ZScorePeakDetection.exe"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "setup-zspd-${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES64\Z-Score Peak Detection"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  SetRegView 64
  ${if} ${RunningX64}
  ${else}
     MessageBox MB_ICONSTOP|MB_TOPMOST "Unsupported platform: This installer is for 64-bit only."
     Abort
  ${endif}
FunctionEnd

Section "MainSection" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "ZScorePeakDetection.exe"
  File "Qt5Widgets.dll"
  File "Qt5Svg.dll"
  File "Qt5Gui.dll"
  File "Qt5Core.dll"
  File "Qt5Charts.dll"
  File "libwinpthread-1.dll"
  File "libstdc++-6.dll"
  File "libgcc_s_seh-1.dll"
  File /oname=LICENSE.txt "..\LICENSE"
  SetOutPath "$INSTDIR\iconengines"
  File /r "iconengines\"
  SetOutPath "$INSTDIR\imageformats"
  File /r "imageformats\"
  SetOutPath "$INSTDIR\platforms"
  File /r "platforms\"
  SetOutPath "$INSTDIR\styles"
  File /r "styles\"
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer

; Shortcuts
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$ICONS_GROUP"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Z-Score Peak Detection.lnk" "$INSTDIR\ZScorePeakDetection.exe"
  CreateShortCut "$DESKTOP\Z-Score Peak Detection.lnk" "$INSTDIR\ZScorePeakDetection.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -AdditionalIcons
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk" "$INSTDIR\uninst.exe"
  !insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\ZScorePeakDetection.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\ZScorePeakDetection.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Function un.onInit
  SetRegView 64
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER "Application" $ICONS_GROUP
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\LICENSE.txt"
  Delete "$INSTDIR\libgcc_s_seh-1.dll"
  Delete "$INSTDIR\libstdc++-6.dll"
  Delete "$INSTDIR\libwinpthread-1.dll"
  Delete "$INSTDIR\Qt5Charts.dll"
  Delete "$INSTDIR\Qt5Core.dll"
  Delete "$INSTDIR\Qt5Gui.dll"
  Delete "$INSTDIR\Qt5Svg.dll"
  Delete "$INSTDIR\Qt5Widgets.dll"
  Delete "$INSTDIR\ZScorePeakDetection.exe"

  Delete "$SMPROGRAMS\$ICONS_GROUP\Uninstall.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Website.lnk"
  Delete "$DESKTOP\Z-Score Peak Detection.lnk"
  Delete "$SMPROGRAMS\$ICONS_GROUP\Z-Score Peak Detection.lnk"

  RMDir "$SMPROGRAMS\$ICONS_GROUP"
  RMDir /r "$INSTDIR\styles"
  RMDir /r "$INSTDIR\platforms"
  RMDir /r "$INSTDIR\imageformats"
  RMDir /r "$INSTDIR\iconengines"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd