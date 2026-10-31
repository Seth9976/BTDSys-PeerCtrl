/*
Copyright (c) 2005-2011, Edward Powley
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

// AssignDlg.cpp : implementation file
//

#include "PeerScale.h"
#include "machine.h"

#include "AssignDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssignDlg dialog


CAssignDlg::CAssignDlg(CWnd* pParent /*=NULL*/)
:
CDialog(CAssignDlg::IDD, pParent)
{
	m_ThisMacInfoText = _T("");
}

CAssignDlg::~CAssignDlg()
{
}

/////////////////////////////////////////////////////////////////////////////
// CAssignDlg message handlers

BOOL CAssignDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Attach controls
	m_MachineCombo.AttachDlgItem(IDC_MACHINECOMBO, this);
	m_ParaList.AttachDlgItem(IDC_PARALIST, this);

	//write info on current assignment
	char txt[128] = { 0 };
	const char *assStr = g_mi->pc.GetAssignmentString();
	sprintf(txt, "%s, (current assign: %s)",
		g_mi->pCB->GetMachineName(g_mi->ThisMac),
		g_mi->pc.GetAssignmentString());

	SetDlgItemText(IDC_THISMACINFO, txt);

	//Populate the boxes
	g_mi->pc.GetMachineNamesToCombo(m_hWnd, 
		IDC_MACHINECOMBO, 
		g_mi->pCB->GetMachineName(g_mi->ThisMac));

	if (g_mi->pc.GotParam())
	{
		//select machine and parameter
		if (g_mi->pc.SelectMachineInCombo(m_hWnd, IDC_MACHINECOMBO))
		{
			g_mi->pc.GetParamNamesToList(
				g_mi->pc.GetMachine(),
				m_hWnd, IDC_PARALIST, ALLOW_NOTE | ALLOW_ALL_GROUPS);

			g_mi->pc.SelectParameterInList(m_hWnd, IDC_PARALIST);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAssignDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	HWND hCtrl = (HWND) lParam;
	WORD ctrlId = LOWORD(wParam);
	WORD notifyCode = HIWORD(wParam);

	switch(ctrlId)
	{
	case IDC_MACHINECOMBO :
		if(notifyCode == CBN_SELCHANGE)
		{
			OnSelchangeMachinecombo();
		}
		break;

	case IDC_DEASSIGN :
		if(notifyCode == BN_CLICKED)
		{
			OnDeassign();
		}
		break;

	case IDOK:
		OnOK();
		break;

	default :
		return FALSE;
	}

	return TRUE;
}

void CAssignDlg::OnSelchangeMachinecombo() 
{
	tString MacName = m_MachineCombo.GetWindowText();
	if (!MacName.empty())
	{
		// Populate parameter list
		g_mi->pc.GetParamNamesToList(
			g_mi->pCB->GetMachine(MacName.c_str()),
			m_hWnd,
			IDC_PARALIST,
			ALLOW_NOTE | ALLOW_ALL_GROUPS
			);
	}
	else
	{
		m_ParaList.ResetContent();
	}
}

void CAssignDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CAssignDlg::OnOK() 
{
	tString MacName = m_MachineCombo.GetWindowText();
	
	if (!MacName.empty())
	{
		int s = m_ParaList.GetCurSel();
		if (s != LB_ERR)
		{
			char ParName[128] = {0};
			m_ParaList.GetText(s, ParName);
			g_mi->pc.AssignParameter((char*) MacName.c_str(), ParName);
		}
	}

	CDialog::OnOK();
}

void CAssignDlg::OnDeassign() 
{
	g_mi->pc.UnassignParameter();

	m_MachineCombo.SetCurSel(CB_ERR);
	OnSelchangeMachinecombo();
}
