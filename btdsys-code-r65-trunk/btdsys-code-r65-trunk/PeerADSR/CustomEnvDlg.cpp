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

// CustomEnvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys PeerADSR.h"
#include "CustomEnvDlg.h"
#include "mi_ctrack_defs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomEnvDlg dialog


CCustomEnvDlg::CCustomEnvDlg(mi *pmi, CWnd* pParent /*=NULL*/)
	: CDialog(CCustomEnvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomEnvDlg)
	m_EnvNameValue = _T("");
	//}}AFX_DATA_INIT

	m_pmi = pmi;

	if (Create(CCustomEnvDlg::IDD, pParent))
	{
		ShowWindow(SW_SHOW);
	}
}


void CCustomEnvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomEnvDlg)
	DDX_Control(pDX, IDC_ENVNAME, m_EnvName);
	DDX_Control(pDX, IDC_ENVCOMBO, m_EnvCombo);
	DDX_Control(pDX, IDC_ENVELOPECTRL1, m_EnvelopeCtrl);
	DDX_Text(pDX, IDC_ENVNAME, m_EnvNameValue);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomEnvDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomEnvDlg)
	ON_CBN_SELCHANGE(IDC_ENVCOMBO, OnEnvcomboSelchange)
	ON_EN_KILLFOCUS(IDC_ENVNAME, OnEnvnameKillfocus)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomEnvDlg message handlers

void CCustomEnvDlg::OnOK() 
{
	CDialog::OnOK();
	m_pmi->envdlg = NULL;
	DestroyWindow();
}

void CCustomEnvDlg::OnEnvcomboSelchange() 
{
	CurrentEnvelope = m_EnvCombo.GetCurSel();
	m_EnvNameValue = m_pmi->CustomEnv[CurrentEnvelope].name;
	UpdateData(FALSE);

	m_EnvelopeCtrl.SetEnvelope(
		m_pmi->CustomEnv[CurrentEnvelope].numpoints,
		m_pmi->CustomEnv[CurrentEnvelope].suspoint,
		m_pmi->CustomEnv[CurrentEnvelope].ptcoords);		
}

BOOL CCustomEnvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	RefreshTitleBar();
	
	char txt[256];

	for (int e=0; e<NUM_CUSTOM_ENVS; e++)
	{
		if (m_pmi->CustomEnv[e].name[0] != 0)
			sprintf(txt, "%i: %s", e+1, m_pmi->CustomEnv[e].name);
		else
			sprintf(txt, "%i: (no name)", e+1);

		m_EnvCombo.AddString(txt);
	}

	m_EnvCombo.SetCurSel(0);
	OnEnvcomboSelchange();

	UpdateData();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomEnvDlg::OnEnvnameKillfocus() 
{
	m_EnvName.GetWindowText(m_pmi->CustomEnv[CurrentEnvelope].name, 256);

	m_EnvCombo.DeleteString(CurrentEnvelope);
	char txt[256];
	if (m_pmi->CustomEnv[CurrentEnvelope].name[0] != 0)
		sprintf(txt, "%i: %s", CurrentEnvelope+1, m_pmi->CustomEnv[CurrentEnvelope].name);
	else
		sprintf(txt, "%i: (no name)", CurrentEnvelope+1);

	m_EnvCombo.InsertString(CurrentEnvelope, txt);
	m_EnvCombo.SetCurSel(CurrentEnvelope);
}

BEGIN_EVENTSINK_MAP(CCustomEnvDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CCustomEnvDlg)
	ON_EVENT(CCustomEnvDlg, IDC_ENVELOPECTRL1, 1 /* EnvelopeChanged */, OnEnvelopeChanged, VTS_I4 VTS_I4 VTS_PI4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CCustomEnvDlg::OnEnvelopeChanged(long numPoints, long SustainPoint, long FAR* Points) 
{
	if (m_pmi->CustomEnv[CurrentEnvelope].ptcoords == NULL || m_pmi->CustomEnv[CurrentEnvelope].numpoints != numPoints)
	{
		if (m_pmi->CustomEnv[CurrentEnvelope].ptcoords != NULL)
		{
			delete[] m_pmi->CustomEnv[CurrentEnvelope].ptcoords;
		}

		m_pmi->CustomEnv[CurrentEnvelope].ptcoords = new long [numPoints * 2];
	}

	m_pmi->CustomEnv[CurrentEnvelope].numpoints = numPoints;
	m_pmi->CustomEnv[CurrentEnvelope].suspoint = SustainPoint;

	for (int i=0; i<numPoints*2; i++)
		m_pmi->CustomEnv[CurrentEnvelope].ptcoords[i] = Points[i];
}

void CCustomEnvDlg::OnClose() 
{
	CDialog::OnClose();
	m_pmi->envdlg = NULL;
	DestroyWindow();
}

void CCustomEnvDlg::RefreshTitleBar()
{
	CString s = m_pmi->pCB->GetMachineName(m_pmi->ThisMac);
	s += " - Custom Envelopes";
	SetWindowText(s);
}
