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

#include "StdAfx.h"
#include "plug.h"
#include "ExampleDialog.h"

DLL_EXPORT(CExamplePlug)

// Constructor
CExamplePlug::CExamplePlug()
{
	pdlg = NULL;
}

// Destructor
CExamplePlug::~CExamplePlug()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pdlg)
	{
		pdlg->DestroyWindow();
		delete pdlg;
	}
}

// Called by the host to get misc info about the plugin
SPlugInfo *CExamplePlug::GetInfo()
{
	static SPlugInfo info =
	{
		PCTRL_VERSION,					//Version (always PCTRL_VERSION)
		"Example",						//Name
		"Ed Powley / btdsys",			//Author
		"Just an example",				//Description
		"Open...\nAbout...",			//Menus
		0								//Flags
	};

	return &info;
}

// Called when the plugin is loaded
bool CExamplePlug::Init()
{
	pCB->AddAxis("Slider 1");
	pCB->AddAxis("Slider 2");

	return true; //return true if initialised successfully, false otherwise
}

// Respond to a right click menu item
void CExamplePlug::MenuCommand(int nCommand)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (nCommand)
	{
	case 0: //open
		if (pdlg) //dialog is already open
			pdlg->SetActiveWindow();
		else //dialog not open
		{
			pdlg = new CExampleDialog;
			pdlg->pPl = this;
			if (pdlg->Create(CExampleDialog::IDD, NULL))
				pdlg->ShowWindow(SW_SHOW);
			else
			{
				delete pdlg;
				pdlg = NULL;
				MessageBox(NULL,"Creation failed","D'oh",MB_OK);
			}
		}
		break;

	case 1: //about
		MessageBox(NULL,"About","About",MB_OK | MB_ICONINFORMATION);
		break;
	}
}

// Called after Init() if there is data to be read from a song file
void CExamplePlug::ReadFileData (CPCtrlDataRead  *dr)
{
}

// Called after Init() if there is no data to be read from a song file
void CExamplePlug::InitNoFileData()
{
}

// Called to write data to a song file
void CExamplePlug::WriteFileData(CPCtrlDataWrite *dw)
{
}

// Notifies the plugin that the specified axis has changed its value
// This is not called if it is this plugin which caused the change
void CExamplePlug::ValueChanged(int nAxis, float fValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pdlg) //dialog is open
	{
		switch (nAxis)
		{
		case 0:
			pdlg->m_Scrollbar1.SetPos((int)(fValue * 1000.0f));
			break;
		case 1:
			pdlg->m_Scrollbar2.SetPos((int)(fValue * 1000.0f));
			break;
		}
	}
}

// If the PCPF_CALL_TIMER flag is set, Timer() is called at regular
// intervals. This could be a good time to poll hardware for values etc.
void CExamplePlug::Timer(int numSamples)
{
}
