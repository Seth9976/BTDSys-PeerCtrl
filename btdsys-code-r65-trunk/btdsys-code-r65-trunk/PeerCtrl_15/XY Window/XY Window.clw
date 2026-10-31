; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
ClassCount=4
Class1=CXYWindowApp
LastClass=CXYDialog
NewFileInclude2=#include "XY Window.h"
ResourceCount=2
NewFileInclude1=#include "stdafx.h"
Class2=CXYDialog
LastTemplate=CStatic
Resource1=IDD_XYDIALOG
Class3=CPrefsDialog
Class4=CBlobPreview
Resource2=IDD_PREFSDIALOG

[CLS:CXYWindowApp]
Type=0
HeaderFile=XY Window.h
ImplementationFile=XY Window.cpp
Filter=N
LastObject=CXYWindowApp

[DLG:IDD_XYDIALOG]
Type=1
Class=CXYDialog
ControlCount=0

[DLG:IDD_PREFSDIALOG]
Type=1
Class=CPrefsDialog
ControlCount=14
Control1=IDC_STATIC,static,1342308866
Control2=IDC_BLOBSTYLECOMBO,combobox,1344339971
Control3=IDC_FONTBTN,button,1208025088
Control4=IDC_STATIC,static,1342308354
Control5=IDC_STATIC,static,1342308353
Control6=IDC_LEFTCOLOUR,static,1350697216
Control7=IDC_STATIC,static,1342308353
Control8=IDC_MIDDLECOLOUR,static,1350697216
Control9=IDC_STATIC,static,1342308353
Control10=IDC_RIGHTCOLOUR,static,1350697216
Control11=IDC_SAVEDEFBTN,button,1342242816
Control12=IDOK,button,1342242817
Control13=IDCANCEL,button,1342242816
Control14=IDC_BGCOLOUR,button,1342251008

[CLS:CXYDialog]
Type=0
HeaderFile=xydialog.h
ImplementationFile=xydialog.cpp
BaseClass=CDialog
LastObject=CXYDialog
Filter=W
VirtualFilter=dWC

[CLS:CPrefsDialog]
Type=0
HeaderFile=PrefsDialog.h
ImplementationFile=PrefsDialog.cpp
BaseClass=CDialog
Filter=W
VirtualFilter=dWC
LastObject=IDC_OPACITYCOMBO

[CLS:CBlobPreview]
Type=0
HeaderFile=BlobPreview.h
ImplementationFile=BlobPreview.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CBlobPreview

