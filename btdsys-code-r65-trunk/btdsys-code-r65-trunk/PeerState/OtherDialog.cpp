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

// OtherDialog.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys PeerState.h"
#include "OtherDialog.h"
#include "peerstate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COtherDialog dialog


COtherDialog::COtherDialog(CWnd* pParent /*=NULL*/)
	: CDialog(COtherDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(COtherDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COtherDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherDialog)
	DDX_Control(pDX, IDC_PREV, m_PrevBtn);
	DDX_Control(pDX, IDC_NEXT, m_NextBtn);
	DDX_Control(pDX, IDC_SNAPCOMBO, m_SnapCombo);
	DDX_Control(pDX, IDC_NAME, m_NameEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COtherDialog, CDialog)
	//{{AFX_MSG_MAP(COtherDialog)
	ON_EN_CHANGE(IDC_NAME, OnChangeName)
	ON_CBN_SELCHANGE(IDC_SNAPCOMBO, OnSelchangeSnapcombo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PREV, OnPrev)
	ON_BN_CLICKED(IDC_NEXT, OnNext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherDialog message handlers

BOOL COtherDialog::OnInitDialog() 
{
	bInitialising = true;

	CDialog::OnInitDialog();
	
	for (int s=0; s<NUM_SNAPSHOTS; s++)
	{
		char txt[64];
		sprintf(txt,"%s%i: %s",pmi->GotSnap(s) ? "*" : "",s,pmi->snapname[s]);
		m_SnapCombo.AddString(txt);
	}

	m_SnapCombo.SetCurSel(pmi->nCurrentSnapshot);
	m_NameEdit.SetWindowText(pmi->snapname[pmi->nCurrentSnapshot]);

	HICON hPrev = LoadIcon(theApp.m_hInstance, MAKEINTRESOURCE(IDI_PREV));
	m_PrevBtn.SetIcon(hPrev);
	HICON hNext = LoadIcon(theApp.m_hInstance, MAKEINTRESOURCE(IDI_NEXT));
	m_NextBtn.SetIcon(hNext);
	
	bInitialising = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COtherDialog::OnChangeName() 
{
	int s = m_SnapCombo.GetCurSel();
	if (s != CB_ERR)
	{
		m_NameEdit.GetWindowText(pmi->snapname[s],31);
//		pmi->UpdateParamNameAndState(s);
		m_SnapCombo.DeleteString(s);
		char txt[64];
		sprintf(txt,"%s%i: %s", pmi->GotSnap(s) ? "*" : "", s,pmi->snapname[s]);
		m_SnapCombo.InsertString(s,txt);
		m_SnapCombo.SetCurSel(s);

//		if (pmi->pdlg)
//			pmi->pdlg->UpdateParamComboEntry(s);
	}
}

void COtherDialog::OnSelchangeSnapcombo() 
{
	if (!bInitialising)
	{
		int s = m_SnapCombo.GetCurSel();
		if (s != CB_ERR)
		{
			pmi->Command(2);	

			pmi->nCurrentSnapshot = s;
			pmi->pCB->ControlChange(pmi->thismac,1,0,0,s);
			m_NameEdit.SetWindowText(pmi->snapname[s]);
		}
	}
}

void COtherDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	pmi->pdlg2 = NULL;
	CDialog::OnClose();
	delete this;
}

void COtherDialog::OnPrev() 
{
	int s = m_SnapCombo.GetCurSel();
	if (s != CB_ERR)
	{
		pmi->Command(2);	

		s--;
		if (s < 0) s = 127;
		m_SnapCombo.SetCurSel(s);
		pmi->nCurrentSnapshot = s;
		pmi->pCB->ControlChange(pmi->thismac,1,0,0,s);
		m_NameEdit.SetWindowText(pmi->snapname[s]);
	}
}

void COtherDialog::OnNext() 
{
	int s = m_SnapCombo.GetCurSel();
	if (s != CB_ERR)
	{
		pmi->Command(2);	

		s++;
		if (s > 127) s = 0;
		m_SnapCombo.SetCurSel(s);
		pmi->nCurrentSnapshot = s;
		pmi->pCB->ControlChange(pmi->thismac,1,0,0,s);
		m_NameEdit.SetWindowText(pmi->snapname[s]);
	}
}

void COtherDialog::UpdateParamComboEntry(int n)
{
	int s = m_SnapCombo.GetCurSel();

	m_SnapCombo.DeleteString(n);
	char txt[64];
	sprintf(txt,"%s%i: %s", pmi->GotSnap(n) ? "*" : "", n,pmi->snapname[n]);
	m_SnapCombo.InsertString(n,txt);
	m_SnapCombo.SetCurSel(s);
	if (n == s)
		m_NameEdit.SetWindowText(pmi->snapname[n]);

}
