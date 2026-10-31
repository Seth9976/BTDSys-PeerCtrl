/*
Copyright (c) 2002-2006, Edward Powley
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of BTDSys nor the names of its contributors may be used
      to endorse or promote products derived from this software without
      specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Mixer GUI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Mixer GUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CMixerGUIApp

BEGIN_MESSAGE_MAP(CMixerGUIApp, CWinApp)
	//{{AFX_MSG_MAP(CMixerGUIApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMixerGUIApp construction

CMixerGUIApp::CMixerGUIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMixerGUIApp object

CMixerGUIApp theApp;

/////////////////////////////////////////////////////////////////////////////



#include "../PCtrlPlug.h"

CMixerGuiPlug::CMixerGuiPlug()
{
	mixerinfo.Valid = false;
	pmixerdlg = NULL;
	placement.length = 0;
}

CMixerGuiPlug::~CMixerGuiPlug()
{
	OutputDebugString("Destructor");
	if (pmixerdlg)
	{
		pmixerdlg->DestroyWindow();
		delete pmixerdlg;
	}
}

SPlugInfo *CMixerGuiPlug::GetInfo()
{
	static SPlugInfo info =
	{
		PCTRL_VERSION,								//Version
		"Mixer GUI",					//Name
		"Ed Powley",				//Author
		"A mixer-style user interface",
		"Open...\n"
		"About...",									//Menus
		0,											//Flags
	};

	return &info;
}

bool CMixerGuiPlug::Init()
{
	return true;
}

void CMixerGuiPlug::InitNoFileData()
{
	mixerinfo.Valid = true;
	mixerinfo.NumChannels = 1;
	mixerinfo.chans[0] = new CChannelInfo;
	for (int i=0; i<NUM_KNOBS+1; i++)
	{
		mixerinfo.chans[0]->Colours[i] = -1;
//		mixerinfo.chans[0]->TrackBinds[i] = -1;
//		mixerinfo.chans[0]->Labels[i] = new char[128];
		mixerinfo.chans[0]->Labels[i][0] = 0;
	}
	char txt[256];
	for (i=0; i<NUM_KNOBS; i++)
	{
		sprintf(txt,"Channel 0 knob %i", i);
		mixerinfo.chans[0]->nAxes[i] = pCB->AddAxis(txt);
	}

	mixerinfo.chans[0]->nAxes[NUM_KNOBS] = pCB->AddAxis("Channel 0 fader");
}

void CMixerGuiPlug::ReadFileData(CPCtrlDataRead *dr)
{
	char version = dr->ReadChar();
	if (version == 1)
	{
		mixerinfo.Valid = dr->ReadChar() != 0;
		if (mixerinfo.Valid)
		{
			mixerinfo.NumChannels = dr->ReadInt();
			for (int i=0; i<mixerinfo.NumChannels; i++)
			{
				mixerinfo.chans[i] = new CChannelInfo;

				for (int j=0; j<NUM_KNOBS+1; j++)
				{
					dr->Read(&mixerinfo.chans[i]->Colours[j], sizeof(COLORREF));
					dr->ReadASCIIZ(mixerinfo.chans[i]->Labels[j]);
				}

				char txt[256];
				for (j=0; j<NUM_KNOBS; j++)
				{
					sprintf(txt,"Channel %i knob %i", i,j);
					if (mixerinfo.chans[i]->Labels[j][0])
						sprintf(txt,"%s - \"%s\"",txt,mixerinfo.chans[i]->Labels[j]);

					mixerinfo.chans[i]->nAxes[j] = pCB->AddAxis(txt);
				}

				sprintf(txt,"Channel %i fader", i);
				if (mixerinfo.chans[i]->Labels[NUM_KNOBS][0])
					sprintf(txt,"%s - \"%s\"",txt,mixerinfo.chans[i]->Labels[NUM_KNOBS]);

				mixerinfo.chans[i]->nAxes[NUM_KNOBS] = pCB->AddAxis(txt);

			}
		}
		dr->Read(&placement, sizeof(WINDOWPLACEMENT));
	}
}

void CMixerGuiPlug::WriteFileData(CPCtrlDataWrite *dw)
{
	dw->Write((char)1); //version
	dw->Write((char)(mixerinfo.Valid ? 1 : 0));
	if (mixerinfo.Valid)
	{
		dw->Write(mixerinfo.NumChannels);
		for (int i=0; i<mixerinfo.NumChannels; i++)
			for (int j=0; j<NUM_KNOBS+1; j++)
			{
				dw->Write(&mixerinfo.chans[i]->Colours[j], sizeof(COLORREF));
				dw->WriteASCIIZ(mixerinfo.chans[i]->Labels[j]);
			}
	}
	if (pmixerdlg)
	{
		placement.length = sizeof(WINDOWPLACEMENT);
		pmixerdlg->GetWindowPlacement(&placement);
	}
	dw->Write(&placement, sizeof(WINDOWPLACEMENT));

}

void CMixerGuiPlug::ValueChanged(int nAxis, float fValue)
{
	if (pmixerdlg)
	{
		for (int i=0; i<mixerinfo.NumChannels; i++)
		{
			for (int j=0; j<NUM_KNOBS; j++)
			{
				if (mixerinfo.chans[i]->nAxes[j] == nAxis)
					pmixerdlg->channel[i]->knob[j]->SetPos((int)(fValue * CONTROL_RANGE));
			}
			if (mixerinfo.chans[i]->nAxes[NUM_KNOBS] == nAxis)
			{
				if ((pmixerdlg->bSoloMode && pmixerdlg->channel[i]->solo->GetCheck() == BST_CHECKED)
				|| (!pmixerdlg->bSoloMode && pmixerdlg->channel[i]->mute->GetCheck() == BST_UNCHECKED))
					pmixerdlg->channel[i]->slider->SetPos((int)(fValue * CONTROL_RANGE));
			}
		}
	}
}

void CMixerGuiPlug::MenuCommand(int nCommand)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (nCommand)
	{
	case 0: 
		if (pmixerdlg == NULL)
		{
			pmixerdlg = new CMixerDlg(this);
			if (placement.length)
				pmixerdlg->SetWindowPlacement(&placement);
		}
		else
			pmixerdlg->SetActiveWindow();
		break;

	case 1:
		MessageBox (NULL,
			"Mixer GUI plugin for PeerCtrl\n"
			"©2003 Ed Powley\n"
			"Based on an idea by Petri Junno aka Elektrojänis\n\n"
			"mailto:zen30378@zen.co.uk",
			"About",
			MB_OK|MB_ICONINFORMATION);
		break;

	}
}

DLL_EXPORT(CMixerGuiPlug);

