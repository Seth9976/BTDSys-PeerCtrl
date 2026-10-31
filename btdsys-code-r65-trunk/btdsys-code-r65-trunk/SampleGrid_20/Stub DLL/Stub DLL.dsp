# Microsoft Developer Studio Project File - Name="Stub DLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Stub DLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Stub DLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Stub DLL.mak" CFG="Stub DLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Stub DLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release B04" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release B08" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release B16" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release B32" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release S04" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release S08" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release S16" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Stub DLL - Win32 Release S32" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Stub DLL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG" /d "B16"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll" /pdbtype:sept

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B04"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_B04"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_B04"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_B04"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_B04"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B04" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "B04"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B04.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B08"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_B08"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_B08"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_B08"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_B08"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B08" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "B08"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B08.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B16"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_B16"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_B16"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_B16"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_B16"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "B16"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B32"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_B32"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_B32"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_B32"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_B32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B32" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "B32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B32.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S04"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_S04"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_S04"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_S04"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_S04"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "S04" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "S04"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - S04.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S08"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_S08"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_S08"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_S08"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_S08"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "S08" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "S08"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - S08.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S16"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_S16"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_S16"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_S16"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_S16"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "S16" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "S16"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - S16.dll"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S32"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Stub_DLL___Win32_Release_S32"
# PROP BASE Intermediate_Dir "Stub_DLL___Win32_Release_S32"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Stub_DLL___Win32_Release_S32"
# PROP Intermediate_Dir "Stub_DLL___Win32_Release_S32"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "B16" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "STUBDLL_EXPORTS" /D "S32" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG" /d "B16"
# ADD RSC /l 0x809 /d "NDEBUG" /d "S32"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - B16.dll"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386 /out:"../../_buzz test copy_/gear/generators/BTDSys SampleGrid 2 - S32.dll"

!ENDIF 

# Begin Target

# Name "Stub DLL - Win32 Release"
# Name "Stub DLL - Win32 Debug"
# Name "Stub DLL - Win32 Release B04"
# Name "Stub DLL - Win32 Release B08"
# Name "Stub DLL - Win32 Release B16"
# Name "Stub DLL - Win32 Release B32"
# Name "Stub DLL - Win32 Release S04"
# Name "Stub DLL - Win32 Release S08"
# Name "Stub DLL - Win32 Release S16"
# Name "Stub DLL - Win32 Release S32"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\stub.rc

!IF  "$(CFG)" == "Stub DLL - Win32 Release"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "B16"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Debug"

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B04"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B08"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B16"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release B32"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S04"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S08"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S16"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ELSEIF  "$(CFG)" == "Stub DLL - Win32 Release S32"

# ADD BASE RSC /l 0x809 /d "B16"
# ADD RSC /l 0x809

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\stubmain.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\skin b04.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin b08.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin b16.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin b32.bmp"
# End Source File
# Begin Source File

SOURCE=".\skin s04.bmp"
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

SOURCE=".\skinled_2,2.bmp"
# End Source File
# End Group
# End Target
# End Project
