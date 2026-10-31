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

#include "stdafx.h"

#include "../PCtrlPlug.h"

#include "XYPlug.h"
#include "XYDialog.h"
#include "PrefsDialog.h"

CXYPlug::CXYPlug() : pdlg(NULL), nBlobStyle(0)
{
	placement.length = 0;
}

CXYPlug::~CXYPlug()
{
	OutputDebugString("Destroy");
	if (pdlg)
		pdlg->DestroyWindow();
}

SPlugInfo *CXYPlug::GetInfo()
{
	static SPlugInfo info =
	{
		PCTRL_VERSION,								//Version
		"X-Y Window",								//Name
		"Ed Powley (BTDSys)",						//Author
		"X-Y Window",								//Description
		"Open...\n"
		"Preferences...\n"
		"About...",									//Menus
		0											//Flags
	};

	return &info;
}

bool CXYPlug::Init()
{
/*	if (pCB)
	{
*/		pCB->AddAxis("LMB: X");
		pCB->AddAxis("LMB: Y");
		pCB->AddAxis("MMB: X");
		pCB->AddAxis("MMB: Y");
		pCB->AddAxis("RMB: X");
		pCB->AddAxis("RMB: Y");

		LoadRegistrySettings();

		return true;
/*	}
	else
		return false;
*/}

void CXYPlug::MenuCommand(int nCommand)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (nCommand)
	{
	case 0: //Open
		if (pdlg)
			pdlg->SetActiveWindow();
		else
		{
			pdlg = new CXYDialog(this);
			if (placement.length)
				pdlg->SetWindowPlacement(&placement);
		}
		break;
		
	case 1: //Prefs
		{
			CPrefsDialog pref;
			pref.pparent = this;
			pref.DoModal();
		}
		break;

	case 2: //About
		MessageBox(NULL,
			"X-Y Window plugin for PeerCtrl\n"
			"©2003 Ed Powley\n"
			"mailto:zen30378@zen.co.uk",
			"About",
			MB_OK|MB_ICONINFORMATION);
		break;
	}
}

void CXYPlug::ReadFileData(CPCtrlDataRead *dr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

//	OutputDebugString("XY: Reading file data");

	char version = dr->ReadChar();
	if (version == 1)
	{
		nBlobStyle = dr->ReadChar();
		dr->Read(&colL, sizeof(COLORREF));
		dr->Read(&colM, sizeof(COLORREF));
		dr->Read(&colR, sizeof(COLORREF));
		dr->Read(&colBG, sizeof(COLORREF));

		char DlgOpen;
		DlgOpen = dr->ReadChar();
		dr->Read(&placement, sizeof(WINDOWPLACEMENT));
		if (DlgOpen)
		{
/*			pdlg = new CXYDialog(this);
			if (placement.length)
				pdlg->SetWindowPlacement(&placement);
*/		}			
	}
}

void CXYPlug::WriteFileData(CPCtrlDataWrite *dw)
{
	dw->Write((char)1); //version
	dw->Write(nBlobStyle);
	dw->Write(&colL, sizeof(COLORREF));
	dw->Write(&colM, sizeof(COLORREF));
	dw->Write(&colR, sizeof(COLORREF));
	dw->Write(&colBG, sizeof(COLORREF));
	dw->Write( (pdlg != NULL) ? (char)0xff : (char)0 );
	if (pdlg)
	{
		placement.length = sizeof(WINDOWPLACEMENT);
		pdlg->GetWindowPlacement(&placement);
	}
	dw->Write(&placement, sizeof(WINDOWPLACEMENT));
}

void CXYPlug::ValueChanged(int nAxis, float fValue)
{
	if (pdlg)
	{
		switch (nAxis)
		{
		case 0:
			pdlg->m_pLBlob->SetX(fValue);	break;
		case 1:
			pdlg->m_pLBlob->SetY(fValue);	break;
		case 2:
			pdlg->m_pMBlob->SetX(fValue);	break;
		case 3:
			pdlg->m_pMBlob->SetY(fValue);	break;
		case 4:
			pdlg->m_pRBlob->SetX(fValue);	break;
		case 5:
			pdlg->m_pRBlob->SetY(fValue);	break;
		}
	}
}

DLL_EXPORT(CXYPlug);
