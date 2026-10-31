# Microsoft Developer Studio Project File - Name="BTDSys PeerState" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BTDSys PeerState - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys PeerState.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys PeerState.mak" CFG="BTDSys PeerState - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BTDSys PeerState - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys PeerState - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys PeerState - Win32 Semidebug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BTDSys PeerState - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_buzz test copy_/gear/generators/BTDSys PeerState.dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "BTDSys PeerState - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MDd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /incremental:no /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "BTDSys PeerState - Win32 Semidebug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_PeerState___Win32_Semidebug"
# PROP BASE Intermediate_Dir "BTDSys_PeerState___Win32_Semidebug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_PeerState___Win32_Semidebug"
# PROP Intermediate_Dir "BTDSys_PeerState___Win32_Semidebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Semidebug/BTDSys PeerState.dll"

!ENDIF 

# Begin Target

# Name "BTDSys PeerState - Win32 Release"
# Name "BTDSys PeerState - Win32 Debug"
# Name "BTDSys PeerState - Win32 Semidebug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=".\BTDSys PeerState.cpp"
# End Source File
# Begin Source File

SOURCE=".\BTDSys PeerState.def"
# End Source File
# Begin Source File

SOURCE=".\BTDSys PeerState.rc"
# End Source File
# Begin Source File

SOURCE=.\EditDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\hack.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\MachineSelectDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\MyTreeCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\OtherDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\peerstate.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=".\BTDSys PeerState.h"
# End Source File
# Begin Source File

SOURCE=.\EditDialog.h
# End Source File
# Begin Source File

SOURCE=.\hack.h
# End Source File
# Begin Source File

SOURCE=..\mdk\MachineInterface.h
# End Source File
# Begin Source File

SOURCE=.\MachineSelectDlg.h
# End Source File
# Begin Source File

SOURCE=.\MainDialog.h
# End Source File
# Begin Source File

SOURCE=..\mdk\mdk.h
# End Source File
# Begin Source File

SOURCE=.\MyTreeCtrl.h
# End Source File
# Begin Source File

SOURCE=.\OtherDialog.h
# End Source File
# Begin Source File

SOURCE=.\peerstate.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\BTDSys PeerState.rc2"
# End Source File
# Begin Source File

SOURCE=.\next.ico
# End Source File
# Begin Source File

SOURCE=.\prev.ico
# End Source File
# Begin Source File

SOURCE=.\skin.bmp
# End Source File
# Begin Source File

SOURCE=".\skinled(0,0).bmp"
# End Source File
# Begin Source File

SOURCE=.\state_images.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
