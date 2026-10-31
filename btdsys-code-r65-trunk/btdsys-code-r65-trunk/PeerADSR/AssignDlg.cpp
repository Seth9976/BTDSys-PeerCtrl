/*
Copyright (c) 2002-2011, Edward Powley
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

#include "stdafx.h"
#include "BTDSys PeerADSR.h"

#ifndef __BTDSYS_PEERADSR_MI_CTRACK_DEFS
#include "mi_ctrack_defs.h"
#endif

#include "AssignDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssignDlg dialog


CAssignDlg::CAssignDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssignDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssignDlg)
	m_ThisMacInfoText = _T("");

	//}}AFX_DATA_INIT
}


void CAssignDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssignDlg)
	DDX_Control(pDX, IDC_PARALIST, m_ParaList);
	DDX_Control(pDX, IDC_MACHINECOMBO, m_MachineCombo);
	DDX_Text(pDX, IDC_THISMACINFO, m_ThisMacInfoText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAssignDlg, CDialog)
	//{{AFX_MSG_MAP(CAssignDlg)
	ON_CBN_SELCHANGE(IDC_MACHINECOMBO, OnSelchangeMachinecombo)
	ON_BN_CLICKED(IDC_DEASSIGN, OnDeassign)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssignDlg message handlers

BOOL CAssignDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//write info on current assignment
	char txt[128];
	sprintf(txt, "%s, track %i (current assign: %s)",
		g_mi->pCB->GetMachineName(g_mi->ThisMac),
		g_tracknum,
		g_track->PeerCtrl.GetAssignmentString());

	SetDlgItemText(IDC_THISMACINFO, txt);

	//Populate the boxes
	g_track->PeerCtrl.GetMachineNamesToCombo(m_hWnd, 
		IDC_MACHINECOMBO, 
		g_mi->pCB->GetMachineName(g_mi->ThisMac));

	if (g_track->PeerCtrl.GotParam())
	{
		//select machine and parameter
		if (g_track->PeerCtrl.SelectMachineInCombo(m_hWnd, IDC_MACHINECOMBO))
		{
			g_track->PeerCtrl.GetParamNamesToList(
				g_track->PeerCtrl.GetMachine(),
				m_hWnd, IDC_PARALIST, ALLOW_BYTE | ALLOW_WORD | ALLOW_ALL_GROUPS);

			g_track->PeerCtrl.SelectParameterInList(m_hWnd, IDC_PARALIST);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAssignDlg::OnSelchangeMachinecombo() 
{
	char MacName[128];
	
	// If a machine is selected, populate parameter list
	if (GetDlgItemText(IDC_MACHINECOMBO, MacName, 128) != 0)
	{
		g_track->PeerCtrl.GetParamNamesToList(
			g_mi->pCB->GetMachine(MacName),
			m_hWnd, IDC_PARALIST, ALLOW_BYTE | ALLOW_WORD | ALLOW_ALL_GROUPS);
	}
	else
		m_ParaList.ResetContent();
}

void CAssignDlg::OnCancel() 
{
	CDialog::OnCancel();
}

void CAssignDlg::OnOK() 
{
	g_track->DoEndAssignDlg(m_hWnd, g_track);

	CDialog::OnOK();
}

void CAssignDlg::OnDeassign() 
{
	g_track->PeerCtrl.UnassignParameter();

	m_MachineCombo.SetCurSel(CB_ERR);
	OnSelchangeMachinecombo();
}
