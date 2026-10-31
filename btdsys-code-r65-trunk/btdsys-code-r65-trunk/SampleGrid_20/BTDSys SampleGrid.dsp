# Microsoft Developer Studio Project File - Name="BTDSys SampleGrid" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BTDSYS SAMPLEGRID - WIN32 RELEASE B16
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys SampleGrid.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys SampleGrid.mak" CFG="BTDSYS SAMPLEGRID - WIN32 RELEASE B16"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BTDSys SampleGrid - Win32 Release B4" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid - Win32 Release B8" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid - Win32 Release B16" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid - Win32 Release S8" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid - Win32 Release S16" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid - Win32 Release S32" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid - Win32 Semidebug B16" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid - Win32 Release B24" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B4"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Release_B4"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B4"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Release_B4"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B4"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=4 /D NUMTRIGGERS_STRING_HEX=_04_ /D NUMTRIGGERS_STRING_DEC=_4_ /D "TRIGGER_TYPE_BYTE" /D "B04" /D "GRID" /U "TRIGGER_TYPE_SWITCH" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../auxbus/auxbus.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - B04.dll"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B8"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Release_B8"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B8"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Release_B8"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B8"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=8 /D NUMTRIGGERS_STRING_HEX=_08_ /D NUMTRIGGERS_STRING_DEC=_8_ /D "TRIGGER_TYPE_BYTE" /D "B08" /D "GRID" /U "TRIGGER_TYPE_SWITCH" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../auxbus/auxbus.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - B08.dll"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B16"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Release_B16"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B16"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Release_B16"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B16"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=16 /D NUMTRIGGERS_STRING_HEX=_10_ /D NUMTRIGGERS_STRING_DEC=_16_ /D "TRIGGER_TYPE_BYTE" /D "B16" /D "GRID" /U "TRIGGER_TYPE_SWITCH" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../auxbus/auxbus.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - B16.dll"
# SUBTRACT LINK32 /profile /incremental:yes

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release S8"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Release_S8"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_S8"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Release_S8"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_S8"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=8 /D NUMTRIGGERS_STRING_HEX=_08_ /D NUMTRIGGERS_STRING_DEC=_8_ /D "TRIGGER_TYPE_SWITCH" /D "S08" /D "GRID" /U "TRIGGER_TYPE_BYTE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../auxbus/auxbus.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - S08.dll"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release S16"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Release_S16"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_S16"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Release_S16"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_S16"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=16 /D NUMTRIGGERS_STRING_HEX=_10_ /D NUMTRIGGERS_STRING_DEC=_16_ /D "TRIGGER_TYPE_SWITCH" /D "S16" /D "GRID" /U "TRIGGER_TYPE_BYTE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../auxbus/auxbus.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - S16.dll"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release S32"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Release_S32"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_S32"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Release_S32"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_S32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=32 /D NUMTRIGGERS_STRING_HEX=_20_ /D NUMTRIGGERS_STRING_DEC=_32_ /D "TRIGGER_TYPE_SWITCH" /D "S32" /D "GRID" /U "TRIGGER_TYPE_BYTE" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../auxbus/auxbus.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - S32.dll"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Semidebug B16"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Semidebug_B16"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Semidebug_B16"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Semidebug_B16"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Semidebug_B16"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=16 /D NUMTRIGGERS_STRING_HEX=_10_ /D NUMTRIGGERS_STRING_DEC=_16_ /D "TRIGGER_TYPE_BYTE" /D "B16" /D "GRID" /U "TRIGGER_TYPE_SWITCH" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /Zi /Od /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=16 /D NUMTRIGGERS_STRING_HEX=_10_ /D NUMTRIGGERS_STRING_DEC=_16_ /D "TRIGGER_TYPE_BYTE" /D "B16" /D "GRID" /U "TRIGGER_TYPE_SWITCH" /FAcs /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"Release/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /debug /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - B16.dll"
# SUBTRACT LINK32 /profile

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B24"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid___Win32_Release_B24"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B24"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "BTDSys_SampleGrid___Win32_Release_B24"
# PROP Intermediate_Dir "BTDSys_SampleGrid___Win32_Release_B24"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=16 /D NUMTRIGGERS_STRING_HEX=_10_ /D NUMTRIGGERS_STRING_DEC=_16_ /D "TRIGGER_TYPE_BYTE" /D "B16" /D "GRID" /U "TRIGGER_TYPE_SWITCH" /FR /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRID_EXPORTS" /D NUMTRIGGERS=24 /D NUMTRIGGERS_STRING_HEX=_18_ /D NUMTRIGGERS_STRING_DEC=_24_ /D "TRIGGER_TYPE_BYTE" /D "B24" /D "GRID" /U "TRIGGER_TYPE_SWITCH" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - B16.dll"
# SUBTRACT BASE LINK32 /profile /incremental:yes
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib ../dsplib/dsplib.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Generators/BTDSys SampleGrid 2 - B24.dll"
# SUBTRACT LINK32 /profile /incremental:yes

!ENDIF 

# Begin Target

# Name "BTDSys SampleGrid - Win32 Release B4"
# Name "BTDSys SampleGrid - Win32 Release B8"
# Name "BTDSys SampleGrid - Win32 Release B16"
# Name "BTDSys SampleGrid - Win32 Release S8"
# Name "BTDSys SampleGrid - Win32 Release S16"
# Name "BTDSys SampleGrid - Win32 Release S32"
# Name "BTDSys SampleGrid - Win32 Semidebug B16"
# Name "BTDSys SampleGrid - Win32 Release B24"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BandLimit.cpp
# End Source File
# Begin Source File

SOURCE=.\channel.cpp
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

SOURCE=.\hermite.cpp
# End Source File
# Begin Source File

SOURCE=.\InertiaParam.cpp
# End Source File
# Begin Source File

SOURCE=.\paperwork.cpp
# End Source File
# Begin Source File

SOURCE=.\SampleGrid.cpp
# End Source File
# Begin Source File

SOURCE=.\sgrid.rc

!IF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B4"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "B04" /d "GRID"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B8"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "B08" /d "GRID"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B16"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "B16" /d "GRID"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release S8"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "S08" /d "GRID"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release S16"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "S16" /d "GRID"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release S32"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "S32" /d "GRID"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Semidebug B16"

# ADD BASE RSC /l 0x809 /d "B16" /d "GRID"
# ADD RSC /l 0x809 /d "B16" /d "GRID"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid - Win32 Release B24"

# ADD BASE RSC /l 0x809 /d "B16" /d "GRID"
# ADD RSC /l 0x809 /d "B16" /d "GRID"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\track.cpp
# End Source File
# Begin Source File

SOURCE=.\work.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\24bit.h
# End Source File
# Begin Source File

SOURCE=.\attrs.h
# End Source File
# Begin Source File

SOURCE=.\BandLimit.h
# End Source File
# Begin Source File

SOURCE=.\channel.h
# End Source File
# Begin Source File

SOURCE=.\dbgprintf.h
# End Source File
# Begin Source File

SOURCE=.\drumkit.h
# End Source File
# Begin Source File

SOURCE=..\dsplib\dsplib.h
# End Source File
# Begin Source File

SOURCE=.\envelopes.h
# End Source File
# Begin Source File

SOURCE=.\envstuff.h
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

SOURCE=..\mdk\MachineInterface.h
# End Source File
# Begin Source File

SOURCE=..\mdk\mdk.h
# End Source File
# Begin Source File

SOURCE=.\midi.h
# End Source File
# Begin Source File

SOURCE=.\overloaderdata.h
# End Source File
# Begin Source File

SOURCE=.\params.h
# End Source File
# Begin Source File

SOURCE=.\sgrid.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\separator.bmp
# End Source File
# Begin Source File

SOURCE=".\skin b04.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin b08.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin b16 alpha.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin b16.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin b24.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin out.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin s08.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin s16.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin s32.bmp"
# End Source File
# Begin Source File

SOURCE=.\skin.bmp
# End Source File
# Begin Source File

SOURCE=.\skin_16.bmp
# End Source File
# Begin Source File

SOURCE=".\skinled_0,2 out.bmp"
# End Source File
# Begin Source File

SOURCE=".\skinled_0,2.bmp"
# End Source File
# Begin Source File

SOURCE=".\skinled_2,2.bmp"
# End Source File
# End Group
# Begin Source File

SOURCE=.\commands.htm
# End Source File
# Begin Source File

SOURCE=.\params.txt
# End Source File
# End Target
# End Project
