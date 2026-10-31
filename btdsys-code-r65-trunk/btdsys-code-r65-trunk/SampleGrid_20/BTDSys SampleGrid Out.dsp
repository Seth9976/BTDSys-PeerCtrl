# Microsoft Developer Studio Project File - Name="BTDSys SampleGrid Out" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=BTDSys SampleGrid Out - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys SampleGrid Out.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "BTDSys SampleGrid Out.mak" CFG="BTDSys SampleGrid Out - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "BTDSys SampleGrid Out - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "BTDSys SampleGrid Out - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "BTDSys SampleGrid Out - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "BTDSys_SampleGrid_Out___Win32_Release"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid_Out___Win32_Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Out_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRIDOUT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /G5 /Gr /Zp4 /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRIDOUT_EXPORTS" /D "OUTMAC" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ../mdk/mdk.lib /nologo /dll /machine:I386 /out:"../_Buzz Test Copy_/Gear/Effects/BTDSys SampleGrid Out.dll"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid Out - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "BTDSys_SampleGrid_Out___Win32_Debug"
# PROP BASE Intermediate_Dir "BTDSys_SampleGrid_Out___Win32_Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "BTDSys_SampleGrid_Out___Win32_Debug"
# PROP Intermediate_Dir "BTDSys_SampleGrid_Out___Win32_Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRIDOUT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "BTDSYSSAMPLEGRIDOUT_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "BTDSys SampleGrid Out - Win32 Release"
# Name "BTDSys SampleGrid Out - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\sgrid.rc

!IF  "$(CFG)" == "BTDSys SampleGrid Out - Win32 Release"

# ADD BASE RSC /l 0x809
# ADD RSC /l 0x809 /d "OUTMAC"

!ELSEIF  "$(CFG)" == "BTDSys SampleGrid Out - Win32 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\sgridout.cpp
# End Source File
# Begin Source File

SOURCE=.\sgridout.def
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
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

SOURCE=".\skinled_0,2 out.bmp"
# End Source File
# Begin Source File

SOURCE=".\skinled_2,2.bmp"
# End Source File
# End Group
# End Target
# End Project
