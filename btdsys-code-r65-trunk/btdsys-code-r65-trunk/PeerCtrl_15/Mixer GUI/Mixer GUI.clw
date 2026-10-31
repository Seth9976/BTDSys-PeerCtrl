; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMixerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "mixer gui.h"
LastPage=0

ClassCount=4
Class1=CMixerGUIApp
Class2=CMixerDlg
Class3=CMyKnobCtrl
Class4=CMySliderCtrl

ResourceCount=3
Resource1=IDD_MIXERDIALOG
Resource2=IDR_MIXERMENU
Resource3=IDR_MIXERPOPUP

[CLS:CMixerGUIApp]
Type=0
BaseClass=CWinApp
HeaderFile=Mixer GUI.h
ImplementationFile=Mixer GUI.cpp
Filter=N
VirtualFilter=AC
LastObject=ID_KNOBMODE_ROTWRAP

[CLS:CMixerDlg]
Type=0
BaseClass=CDialog
HeaderFile=MixerDlg.h
ImplementationFile=MixerDlg.cpp
LastObject=ID_KNOBMODE_LINVERT
Filter=D
VirtualFilter=dWC

[CLS:CMyKnobCtrl]
Type=0
BaseClass=CSliderCtrl
HeaderFile=MyKnobCtrl.h
ImplementationFile=MyKnobCtrl.cpp

[CLS:CMySliderCtrl]
Type=0
BaseClass=CSliderCtrl
HeaderFile=MySliderCtrl.h
ImplementationFile=MySliderCtrl.cpp
Filter=W
VirtualFilter=KWC
LastObject=CMySliderCtrl

[DLG:IDD_MIXERDIALOG]
Type=1
Class=CMixerDlg
ControlCount=0

[MNU:IDR_MIXERMENU]
Type=1
Class=CMixerDlg
Command1=IDM_ADD
Command2=IDM_DELETE
Command3=ID_KNOBMODE_ROTWRAP
Command4=ID_KNOBMODE_ROTNOWRAP
Command5=ID_KNOBMODE_LINVERT
Command6=ID_KNOBMODE_LINHORZ
CommandCount=6

[MNU:IDR_MIXERPOPUP]
Type=1
Class=?
Command1=IDM_KNOBMIN
Command2=IDM_KNOBCENTER
Command3=IDM_KNOBMAX
Command4=IDM_KNOBCOLOUR
Command5=IDM_SLIDERMIN
Command6=IDM_SLIDERCENTER
Command7=IDM_SLIDERMAX
Command8=IDM_SLIDERCOLOUR
CommandCount=8

