# Microsoft Developer Studio Project File - Name="BTDSys SampleGrid 2 Core" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BTDSys SampleGrid 2 Core - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys SampleGrid 2 Core.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys SampleGrid 2 Core.mak" CFG="BTDSys SampleGrid 2 Core - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BTDSys SampleGrid 2 Core - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid 2 Core - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BTDSys SampleGrid 2 Core - Win32 Release"

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
# ADD LINK32 ../../mdk/mdk.lib ../../dsplib/dsplib.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../_Buzz Test Copy_/Gear/BTDSys SampleGrid 2 Core BETA 061110.dat"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid 2 Core - Win32 Debug"

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
# ADD CPP /nologo /G5 /Gr /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../../mdk/mdk.lib ../../dsplib/dsplib.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../_Buzz Test Copy_/Gear/BTDSys SampleGrid 2 Core BETA 050716.dat" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BTDSys SampleGrid 2 Core - Win32 Release"
# Name "BTDSys SampleGrid 2 Core - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=".\BTDSys SampleGrid 2 Core.cpp"
# End Source File
# Begin Source File

SOURCE=".\BTDSys SampleGrid 2 Core.def"
# End Source File
# Begin Source File

SOURCE=".\BTDSys SampleGrid 2 Core.rc"
# End Source File
# Begin Source File

SOURCE=.\channel.cpp
# End Source File
# Begin Source File

SOURCE=.\CmdListDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\core.cpp
# End Source File
# Begin Source File

SOURCE=.\dbgprintf.cpp
# End Source File
# Begin Source File

SOURCE=.\drumkit.cpp
# End Source File
# Begin Source File

SOURCE=.\drumkit_battery.cpp
# End Source File
# Begin Source File

SOURCE=.\drumkit_psi.cpp
# End Source File
# Begin Source File

SOURCE=.\fuzzpilz_envelope.cpp
# End Source File
# Begin Source File

SOURCE=.\group.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupPerfDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\hermite.cpp
# End Source File
# Begin Source File

SOURCE=.\InertiaParam.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyCapBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\KitInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\KitSaveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoadStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoopDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\midi.cpp
# End Source File
# Begin Source File

SOURCE=.\paperwork.cpp
# End Source File
# Begin Source File

SOURCE=.\peerctrl.cpp

!IF  "$(CFG)" == "BTDSys SampleGrid 2 Core - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid 2 Core - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\QWERTYkeynames.cpp
# End Source File
# Begin Source File

SOURCE=.\sgrid.cpp
# End Source File
# Begin Source File

SOURCE=.\ShuffleDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\tempbodies.cpp
# End Source File
# Begin Source File

SOURCE=.\thread.cpp
# End Source File
# Begin Source File

SOURCE=.\track.cpp
# End Source File
# Begin Source File

SOURCE=.\transparency.cpp
# End Source File
# Begin Source File

SOURCE=.\trigger.cpp
# End Source File
# Begin Source File

SOURCE=.\WaveOffset.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\24bit.h
# End Source File
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\BandLimit.h
# End Source File
# Begin Source File

SOURCE=".\BTDSys SampleGrid 2 Core.h"
# End Source File
# Begin Source File

SOURCE=.\channel.h
# End Source File
# Begin Source File

SOURCE=.\CmdListDlg.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\core.h
# End Source File
# Begin Source File

SOURCE=.\drumkit.h
# End Source File
# Begin Source File

SOURCE=.\func_typedefs.h
# End Source File
# Begin Source File

SOURCE=.\fuzzpilz_envelope.h
# End Source File
# Begin Source File

SOURCE=.\fuzzpilz_globals.h
# End Source File
# Begin Source File

SOURCE=.\GroupDlg.h
# End Source File
# Begin Source File

SOURCE=.\GroupPerfDlg.h
# End Source File
# Begin Source File

SOURCE=.\hack.h
# End Source File
# Begin Source File

SOURCE=.\hermite.h
# End Source File
# Begin Source File

SOURCE=.\InertiaParam.h
# End Source File
# Begin Source File

SOURCE=.\KeyCapBtn.h
# End Source File
# Begin Source File

SOURCE=.\KitInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\KitSaveDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoadStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoopDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\mdk\MachineInterface.h
# End Source File
# Begin Source File

SOURCE=..\..\mdk\mdk.h
# End Source File
# Begin Source File

SOURCE=.\memdc.h
# End Source File
# Begin Source File

SOURCE=..\overloaderdata.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\peerctrl.h
# End Source File
# Begin Source File

SOURCE=.\QWERTYkeynames.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\sgrid.h
# End Source File
# Begin Source File

SOURCE=.\ShuffleDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\transparency.h
# End Source File
# Begin Source File

SOURCE=.\WaveOffset.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\BTDSys SampleGrid 2 Core.rc2"
# End Source File
# Begin Source File

SOURCE=.\commands.rtf
# End Source File
# Begin Source File

SOURCE=.\kitinfoi.bmp
# End Source File
# Begin Source File

SOURCE=.\separator.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\sgrid.asp
# End Source File
# Begin Source File

SOURCE=.\todo.txt
# End Source File
# End Target
# End Project
