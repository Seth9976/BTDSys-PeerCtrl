/*
Copyright (c) 2003-2006, Edward Powley
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

// MacView.cpp : implementation file
//

#include "stdafx.h"
#include "groupie.h"
#include "MacView.h"
#include "BuzzMI.h"
#include "MacViewFrame.h"
#include "AddMachineDlg.h"
#include "RenameDlg.h"
#include "ParamDlg.h"
#include "VisualPrefsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMacView

CMacView::CMacView()
{
	m_nDragMode = none;
}

CMacView::~CMacView()
{
}


BEGIN_MESSAGE_MAP(CMacView, CWnd)
	//{{AFX_MSG_MAP(CMacView)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_CONNECTION_DISCONNECT, OnConnectionDisconnect)
	ON_COMMAND(ID_MACHINE_PARAMETERS, OnMachineParameters)
	ON_COMMAND(ID_MACHINE_ATTRIBUTES, OnMachineAttributes)
	ON_COMMAND(ID_MACHINE_RENAME, OnMachineRename)
	ON_COMMAND(ID_MACHINE_DELETE, OnMachineDelete)
	ON_COMMAND(ID_MACHINE_MDKWORKMODE_MONO, OnMachineMdkworkmodeMono)
	ON_COMMAND(ID_MACHINE_MDKWORKMODE_STEREO, OnMachineMdkworkmodeStereo)
	ON_COMMAND(ID_MACHINE_DEVWORKMODEINFO, OnMachineDevworkmodeinfo)
	ON_COMMAND(ID_DUMMY, OnDummy)
	ON_COMMAND(ID_NEW, OnNew)
	ON_COMMAND(ID_OPENBMX, OnOpenbmx)
	ON_COMMAND(ID_SAVEBMX, OnSavebmx)
	ON_COMMAND(ID_VISUALPREFS, OnVisualprefs)
	ON_COMMAND(ID_MACHINE_DEVFORCEEXCEPTION, OnMachineDevforceexception)
	ON_COMMAND(ID_CONNECTION_CENTERPAN, OnConnectionCenterpan)
	ON_COMMAND(ID_MACHINE_UNLOAD, OnMachineUnload)
	ON_COMMAND(ID_MACHINE_REPLACE, OnMachineReplace)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMacView message handlers

BOOL CMacView::OnEraseBkgnd(CDC* pDC) 
{
//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}


BOOL CMacView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}


void CMacView::OnConnectionDisconnect() 
{
/*	if (m_pContextMenuTarget.c->pMac2->pEx)
		m_pContextMenuTarget.c->pMac2->pEx->DeleteInput(m_pContextMenuTarget.c->pMac1->name);

*/	m_pFrame->pmi->vConns.erase(m_pContextMenuTarget.c);	
	RedrawWindow();
}

BOOL CMacView::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (HIWORD(wParam) == 0 && LOWORD(wParam) >= ID_MACHINE_MENUITEM_ZERO)
	{
		if (m_pContextMenuTarget.m->bSpecial)
			m_pFrame->pmi->Command(0);
		else
			m_pContextMenuTarget.m->Command(LOWORD(wParam) - ID_MACHINE_MENUITEM_ZERO,__FILE__,__LINE__);
		
		return 0;
	}
	else
		return CWnd::OnCommand(wParam, lParam);
}

void CMacView::OnMachineParameters() 
{
	if (m_pContextMenuTarget.m->pParamDlg)
		m_pContextMenuTarget.m->pParamDlg->SetActiveWindow();
	else
	{
		m_pContextMenuTarget.m->pParamDlg = new CParamDlg;

		// members
		m_pContextMenuTarget.m->pParamDlg->m_pMac = m_pContextMenuTarget.m;
		m_pContextMenuTarget.m->pParamDlg->m_pmi  = m_pFrame->pmi;

		if (m_pContextMenuTarget.m->pParamDlg->Create(CParamDlg::IDD, m_pFrame))
		{
			m_pContextMenuTarget.m->pParamDlg->ShowWindow(SW_SHOW);
		}
	}
}

void CMacView::OnMachineAttributes() 
{
	MessageBox("not yet");
}

void CMacView::OnMachineRename() 
{
	CRenameDlg dlg;
	dlg.pmi = m_pFrame->pmi;
	dlg.m_szOldName = m_pContextMenuTarget.m->name;

	if (dlg.DoModal() == IDOK)
	{
/*		for (vector<CConnection>::iterator p = m_pFrame->pmi->vConns.begin();
			 p < m_pFrame->pmi->vConns.end();
			 p++)
		{
			if (p->pMac1 == m_pContextMenuTarget.m && p->pMac2->pEx)
					p->pMac2->pEx->RenameInput(m_pContextMenuTarget.m->name,
						dlg.m_szNewName);
		}
*/
		strcpy(m_pContextMenuTarget.m->name, dlg.m_szNewName);

		RedrawWindow();
	}
}

void CMacView::OnMachineDelete() 
{
	if (MessageBox("CAUTION: This will delete ALL selected machines! "
		"This cannot be undone!\n"
		"Are you SURE?",NULL,MB_YESNO) == IDYES)
	{
		for (vector<CMachine *>::iterator pp = m_pFrame->pmi->vpMacs.begin();
			 pp < m_pFrame->pmi->vpMacs.end();
			 pp++)
		{
			if ((*pp)->bSelected)
				if (m_pFrame->pmi->TryDeleteMachine(pp))
					pp--;
		}

		RedrawWindow();
	}
}

void CMacView::OnMachineMdkworkmodeMono() 
{
	m_pContextMenuTarget.m->bStereoOut = false;
	m_pContextMenuTarget.m->OutputModeChanged(false,__FILE__,__LINE__);
}

void CMacView::OnMachineMdkworkmodeStereo() 
{
	m_pContextMenuTarget.m->bStereoOut = true;
	m_pContextMenuTarget.m->OutputModeChanged(true,__FILE__,__LINE__);
}

void CMacView::OnMachineDevworkmodeinfo() 
{
	for (CMachine **pp = m_pFrame->pmi->vpMacs.begin(); pp < m_pFrame->pmi->vpMacs.end(); pp++)
	{
		CMachine *pMac = *pp;
		if (pMac->bSelected)
		{
			char txt[32];
			sprintf(txt,"%s\n%s",pMac->bStereoOut ? "Stereo" : "Mono", pMac->IsMDK() ? "MDK" : "Non-MDK");
			MessageBox(txt,pMac->name);
		}
	}
}

void CMacView::OnDummy() 
{
	MessageBox("You clicked the magic howdy button! Howdy!");
}


void CMacView::OnNew() 
{
	if (MessageBox("Are you SURE?",NULL,MB_YESNO) == IDYES)
	{
		m_pFrame->pmi->vConns.clear();
		while (!m_pFrame->pmi->vpMacs.empty())
		{	
			CMachine **p = m_pFrame->pmi->vpMacs.begin();
			delete *p;
			m_pFrame->pmi->vpMacs.erase(p);
		}
		m_pFrame->pmi->SetupInOutMachines();
		RedrawWindow();
	}
}

void CMacView::OnOpenbmx() 
{
	char FName[MAX_PATH];
	FName[0] = 0;

	OPENFILENAME ofn;
	ofn.lStructSize			= sizeof(ofn);
	ofn.hwndOwner			= NULL;
	ofn.lpstrFilter			=	"Buzz files\0*.bmx;*.bmw\0"
								"All files\0*.*\0";
	ofn.lpstrCustomFilter	= NULL;
	ofn.nFilterIndex		= 1;
	ofn.lpstrFile			= FName;
	ofn.lpstrFileTitle		= NULL;
	ofn.lpstrInitialDir		= NULL;
	ofn.nMaxFile			= 255;
	ofn.lpstrTitle			= "Open file...";
	ofn.Flags				= OFN_FILEMUSTEXIST;
	ofn.lpstrDefExt			= ".bmw";

	if (::GetOpenFileName(&ofn))
	{
		if (MessageBox("Are you SURE?",NULL,MB_YESNO) == IDYES)
		{
			m_pFrame->pmi->vConns.clear();
			while (!m_pFrame->pmi->vpMacs.empty())
			{	
				CMachine **p = m_pFrame->pmi->vpMacs.begin();
				delete *p;
				m_pFrame->pmi->vpMacs.erase(p);
			}
			m_pFrame->pmi->SetupInOutMachines();

			mi *pmi = m_pFrame->pmi;
			FILE *f = fopen(FName,"rb");
			if (!f) MessageBox("Couldn't open file for reading");
			else
			{
				try
				{
					if (!m_pFrame->pmi->LoadFromBMW(f)) MessageBox("Loading failed.");
				}
				catch (...)
				{
					MessageBox("An exception was thrown while loading.");
				}

				fclose(f);
			}

			RedrawWindow();
		}

	}
}

void CMacView::OnSavebmx() 
{
	char FName[MAX_PATH];
	FName[0] = 0;

	OPENFILENAME ofn;
	ofn.lStructSize			= sizeof(ofn);
	ofn.hwndOwner			= NULL;
	ofn.lpstrFilter			=	"Buzz files\0*.bmx;*.bmw\0"
								"All files\0*.*\0";
	ofn.lpstrCustomFilter	= NULL;
	ofn.nFilterIndex		= 1;
	ofn.lpstrFile			= FName;
	ofn.lpstrFileTitle		= NULL;
	ofn.lpstrInitialDir		= NULL;
	ofn.nMaxFile			= 255;
	ofn.lpstrTitle			= "Save to file...";
	ofn.Flags				= OFN_OVERWRITEPROMPT;
	ofn.lpstrDefExt			= ".bmw";

	if (::GetSaveFileName(&ofn))
	{
		mi *pmi = m_pFrame->pmi;
		FILE *f = fopen(FName,"wb");
		if (!f) MessageBox("Couldn't open file for writing");
		else
		{
			try
			{
				if (!m_pFrame->pmi->SaveToBMW(f)) MessageBox("Saving failed. The created file is corrupt.");
			}
			catch (...)
			{
				MessageBox("An unhandled exception occurred while saving. The created file is corrupt.");
			}

			fclose(f);
		}
	}

}

void CMacView::OnVisualprefs() 
{
	MessageBox("VISUAL PREFS dlg is not done yet\n\n"
		"For now, you can edit the values in the registry key\n"
		"HKEY_CURRENT_USER\\SOFTWARE\\BTDSys\\Groupie");

	CVisualPrefsDlg dlg;
	dlg.DoModal();
	
}

void CMacView::OnMachineDevforceexception() 
{
	for (CMachine **pp = m_pFrame->pmi->vpMacs.begin(); pp < m_pFrame->pmi->vpMacs.end(); pp++)
	{
		CMachine *pMac = *pp;
		if (pMac->bSelected)
		{
			pMac->LogException("fake",__FILE__,__LINE__);
		}
	}
}

void CMacView::OnConnectionCenterpan() 
{
	m_pContextMenuTarget.c->fPan = 1.0f;	
}

void CMacView::OnMachineUnload() 
{
	if (MessageBox("Sure?",NULL,MB_YESNO) == IDYES)
	{
		for (CMachine **pp = m_pFrame->pmi->vpMacs.begin(); pp < m_pFrame->pmi->vpMacs.end(); pp++)
		{
			CMachine *pMac = *pp;
			if (pMac->bSelected)
			{
				pMac->UnloadMachine();
			}
		}

		RedrawWindow();
	}
}

void CMacView::OnMachineReplace() 
{
	CAddMachineDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_szMacName[0])
		{
			for (int im=0; im < m_pFrame->pmi->vpMacs.size(); im++)
			{
				CMachine *pm = m_pFrame->pmi->vpMacs[im];

				if (pm->bSelected)
				{
					CMachine *pnew = m_pFrame->pmi->TryAddMachine(dlg.m_szMacName);
					if (pnew)
					{
						pnew->fX = pm->fX;
						pnew->fY = pm->fY;
						strcpy(pnew->name,pm->name);

						for (CConnection *pc = m_pFrame->pmi->vConns.begin(); pc < m_pFrame->pmi->vConns.end(); pc++)
						{
							if (pc->pMac1 == pm) pc->pMac1 = pnew;
							if (pc->pMac2 == pm) pc->pMac2 = pnew;
						}

						if (m_pFrame->pmi->TryDeleteMachine(&m_pFrame->pmi->vpMacs[im])) im--;

						RedrawWindow();

						return;
					}				
				}
			}
		}
	}
}

void CMacView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    MessageBox("Char");
	
	CWnd::OnChar(nChar, nRepCnt, nFlags);
}
