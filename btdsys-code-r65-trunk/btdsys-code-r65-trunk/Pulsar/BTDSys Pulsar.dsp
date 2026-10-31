# Microsoft Developer Studio Project File - Name="BTDSys Pulsar" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BTDSys Pulsar - Win32 FX Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys Pulsar.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys Pulsar.mak" CFG="BTDSys Pulsar - Win32 FX Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BTDSys Pulsar - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Pulsar - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Pulsar - Win32 FX Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Pulsar - Win32 FX Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Pulsar - Win32 Lite Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys Pulsar - Win32 Profiled" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BTDSys Pulsar - Win32 Release"

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
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARGEN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar.dll"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARGEN" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /profile /debug /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar._ll"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_Pulsar___Win32_FX_Release"
# PROP BASE Intermediate_Dir "BTDSys_Pulsar___Win32_FX_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_Pulsar___Win32_FX_Release"
# PROP Intermediate_Dir "BTDSys_Pulsar___Win32_FX_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARFX" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar.dll"
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Effects/BTDSys PulsarFX.dll"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BTDSys_Pulsar___Win32_FX_Debug"
# PROP BASE Intermediate_Dir "BTDSys_Pulsar___Win32_FX_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BTDSys_Pulsar___Win32_FX_Debug"
# PROP Intermediate_Dir "BTDSys_Pulsar___Win32_FX_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARFX" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar.dll" /pdbtype:sept
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../_Buzz Test Copy_/Gear/Effects/BTDSys PulsarFX.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Lite Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_Pulsar___Win32_Lite_Release"
# PROP BASE Intermediate_Dir "BTDSys_Pulsar___Win32_Lite_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_Pulsar___Win32_Lite_Release"
# PROP Intermediate_Dir "BTDSys_Pulsar___Win32_Lite_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARGEN" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARGEN" /D "PULSARLITE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar.dll"
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar lite.dll"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Profiled"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_Pulsar___Win32_Profiled"
# PROP BASE Intermediate_Dir "BTDSys_Pulsar___Win32_Profiled"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_Pulsar___Win32_Profiled"
# PROP Intermediate_Dir "BTDSys_Pulsar___Win32_Profiled"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARGEN" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob0 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "PULSARGEN" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x809 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar.dll"
# ADD LINK32 ../mdk/mdk.lib /nologo /subsystem:windows /dll /profile /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys Pulsar.dll"

!ENDIF 

# Begin Target

# Name "BTDSys Pulsar - Win32 Release"
# Name "BTDSys Pulsar - Win32 Debug"
# Name "BTDSys Pulsar - Win32 FX Release"
# Name "BTDSys Pulsar - Win32 FX Debug"
# Name "BTDSys Pulsar - Win32 Lite Release"
# Name "BTDSys Pulsar - Win32 Profiled"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BandLimit.cpp

!IF  "$(CFG)" == "BTDSys Pulsar - Win32 Release"

# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Release"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Debug"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Lite Release"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Profiled"

# ADD BASE CPP /Yu
# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\BTDSys Pulsar.cpp"
# End Source File
# Begin Source File

SOURCE=".\BTDSys Pulsar.def"
# End Source File
# Begin Source File

SOURCE=".\BTDSys Pulsar.rc"

!IF  "$(CFG)" == "BTDSys Pulsar - Win32 Release"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "PULSARGEN"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Debug"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Release"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "PULSARFX"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Debug"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Lite Release"

# ADD BASE RSC /l 0x809 /d "PULSARGEN"
# ADD RSC /l 0x809 /d "PULSARGEN"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Profiled"

# ADD BASE RSC /l 0x809 /d "PULSARGEN"
# ADD RSC /l 0x809 /d "PULSARGEN"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\describevalue.cpp
# End Source File
# Begin Source File

SOURCE=.\hermite.cpp
# End Source File
# Begin Source File

SOURCE=.\InertiaParam.cpp
# End Source File
# Begin Source File

SOURCE=.\iwaves.cpp

!IF  "$(CFG)" == "BTDSys Pulsar - Win32 Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Debug"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Release"

# PROP Exclude_From_Build 1
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Debug"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Lite Release"

# PROP Exclude_From_Build 1
# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Profiled"

# SUBTRACT BASE CPP /YX /Yc /Yu
# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\iwaves_lite.cpp

!IF  "$(CFG)" == "BTDSys Pulsar - Win32 Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Debug"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Release"

# PROP Exclude_From_Build 1

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 FX Debug"

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Lite Release"

# SUBTRACT CPP /YX /Yc /Yu

!ELSEIF  "$(CFG)" == "BTDSys Pulsar - Win32 Profiled"

# PROP BASE Exclude_From_Build 1
# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\lfoenv.cpp
# End Source File
# Begin Source File

SOURCE=.\OscilloscopeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\param.cpp
# End Source File
# Begin Source File

SOURCE=.\pulsar.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\tick.cpp
# End Source File
# Begin Source File

SOURCE=.\work.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BandLimit.h
# End Source File
# Begin Source File

SOURCE=".\BTDSys Pulsar.h"
# End Source File
# Begin Source File

SOURCE=.\frand.h
# End Source File
# Begin Source File

SOURCE=.\hermite.h
# End Source File
# Begin Source File

SOURCE=.\InertiaParam.h
# End Source File
# Begin Source File

SOURCE=.\iwaves.h
# End Source File
# Begin Source File

SOURCE=..\mdk\MachineInterface.h
# End Source File
# Begin Source File

SOURCE=..\mdk\mdk.h
# End Source File
# Begin Source File

SOURCE=.\memdc.h
# End Source File
# Begin Source File

SOURCE=.\OscilloscopeDlg.h
# End Source File
# Begin Source File

SOURCE=.\overloaderdata.h
# End Source File
# Begin Source File

SOURCE=.\pulsar.h
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

SOURCE=".\res\BTDSys Pulsar.rc2"
# End Source File
# Begin Source File

SOURCE=.\fxseparator.bmp
# End Source File
# Begin Source File

SOURCE=.\fxskin.bmp
# End Source File
# Begin Source File

SOURCE=.\separator.bmp
# End Source File
# Begin Source File

SOURCE=.\skin.bmp
# End Source File
# Begin Source File

SOURCE=".\skinled (2,2).bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\profile.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
