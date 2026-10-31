# Microsoft Developer Studio Project File - Name="BTDSys Groupie" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BTDSys Groupie - Win32 Semi Debug Gen
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys Groupie.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys Groupie.mak" CFG="BTDSys Groupie - Win32 Semi Debug Gen"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BTDSys Groupie - Win32 Semi Debug Gen" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Groupie - Win32 Release Gen" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Groupie - Win32 Release Eff" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Groupie - Win32 Semi Debug Eff" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BTDSys Groupie - Win32 Semi Debug Gen"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_Groupie___Win32_Semi_Debug_Gen"
# PROP BASE Intermediate_Dir "BTDSys_Groupie___Win32_Semi_Debug_Gen"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_Groupie___Win32_Semi_Debug_Gen"
# PROP Intermediate_Dir "BTDSys_Groupie___Win32_Semi_Debug_Gen"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /Zi /O2 /Ob2 /D "_GROUPIE_GEN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../_buzz test copy_/gear/generators/BTDSys Groupie (Gen).dll"

!ELSEIF  "$(CFG)" == "BTDSys Groupie - Win32 Release Gen"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_Groupie___Win32_Release_Gen"
# PROP BASE Intermediate_Dir "BTDSys_Groupie___Win32_Release_Gen"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_Groupie___Win32_Release_Gen"
# PROP Intermediate_Dir "BTDSys_Groupie___Win32_Release_Gen"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "_GROUPIE_GEN" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
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
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_buzz test copy_/gear/generators/BTDSys Groupie (Gen).dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "BTDSys Groupie - Win32 Release Eff"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_Groupie___Win32_Release_Eff"
# PROP BASE Intermediate_Dir "BTDSys_Groupie___Win32_Release_Eff"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_Groupie___Win32_Release_Eff"
# PROP Intermediate_Dir "BTDSys_Groupie___Win32_Release_Eff"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "_GROUPIE_EFF" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
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
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_buzz test copy_/gear/effects/BTDSys Groupie (Eff).dll"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "BTDSys Groupie - Win32 Semi Debug Eff"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_Groupie___Win32_Semi_Debug_Eff"
# PROP BASE Intermediate_Dir "BTDSys_Groupie___Win32_Semi_Debug_Eff"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_Groupie___Win32_Semi_Debug_Eff"
# PROP Intermediate_Dir "BTDSys_Groupie___Win32_Semi_Debug_Eff"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /Zi /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /Zi /O2 /Ob2 /D "_GROUPIE_EFF" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /debug /machine:I386
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../_buzz test copy_/gear/effects/BTDSys Groupie (Eff).dll"

!ENDIF 

# Begin Target

# Name "BTDSys Groupie - Win32 Semi Debug Gen"
# Name "BTDSys Groupie - Win32 Release Gen"
# Name "BTDSys Groupie - Win32 Release Eff"
# Name "BTDSys Groupie - Win32 Semi Debug Eff"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AddMachineDlg.cpp
# End Source File
# Begin Source File

SOURCE=".\BTDSys Groupie.cpp"
# End Source File
# Begin Source File

SOURCE=".\BTDSys Groupie.def"
# End Source File
# Begin Source File

SOURCE=".\BTDSys Groupie.rc"
# End Source File
# Begin Source File

SOURCE=.\BuzzMI.cpp
# End Source File
# Begin Source File

SOURCE=.\BuzzMI_InternalCallbacks.cpp
# End Source File
# Begin Source File

SOURCE=.\BuzzMI_LoadMachine.cpp
# End Source File
# Begin Source File

SOURCE=.\BuzzMI_Work.cpp
# End Source File
# Begin Source File

SOURCE=.\dbgprintf.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=.\DSPStuff.cpp
# End Source File
# Begin Source File

SOURCE=.\groupie.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyboardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MacView.cpp
# End Source File
# Begin Source File

SOURCE=.\MacView_Mouse.cpp
# End Source File
# Begin Source File

SOURCE=.\MacView_Paint.cpp
# End Source File
# Begin Source File

SOURCE=.\MacViewFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ParamDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\params.cpp
# End Source File
# Begin Source File

SOURCE=.\RenameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\routing.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\VisualPrefsDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AddMachineDlg.h
# End Source File
# Begin Source File

SOURCE=".\BTDSys Groupie.h"
# End Source File
# Begin Source File

SOURCE=.\BuzzMI.h
# End Source File
# Begin Source File

SOURCE=.\dbgprintf.h
# End Source File
# Begin Source File

SOURCE=.\DSPStuff.h
# End Source File
# Begin Source File

SOURCE=.\groupie.h
# End Source File
# Begin Source File

SOURCE=.\KeyboardDlg.h
# End Source File
# Begin Source File

SOURCE=..\mdk\MachineInterface.h
# End Source File
# Begin Source File

SOURCE=.\MacView.h
# End Source File
# Begin Source File

SOURCE=.\MacViewFrame.h
# End Source File
# Begin Source File

SOURCE=..\mdk\mdk.h
# End Source File
# Begin Source File

SOURCE=.\memdc.h
# End Source File
# Begin Source File

SOURCE=.\ParamDlg.h
# End Source File
# Begin Source File

SOURCE=.\RenameDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\VisualPrefsDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\BTDSys Groupie.rc2"
# End Source File
# Begin Source File

SOURCE=.\toolbar1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\betatesters.txt
# End Source File
# Begin Source File

SOURCE=.\changelog.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\todo.txt
# End Source File
# End Target
# End Project
