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

// CmdListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys samplegrid 2 core.h"
#include "CmdListDlg.h"
#include "sgrid.h"
#include "transparency.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCmdListDlg dialog


CCmdListDlg::CCmdListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCmdListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCmdListDlg)
	//}}AFX_DATA_INIT
}


void CCmdListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCmdListDlg)
	DDX_Control(pDX, IDC_SHOWCOMBO, m_cmbShow);
	DDX_Control(pDX, IDC_RICHEDIT1, m_Text);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCmdListDlg, CDialog)
	//{{AFX_MSG_MAP(CCmdListDlg)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_COMMAND(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCmdListDlg message handlers

extern HINSTANCE dllInstance;

BOOL CCmdListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HRSRC hrsrc = FindResource((HMODULE)dllInstance, MAKEINTRESOURCE(IDR_COMMANDS), "RTF");
	if (hrsrc)
	{
		HGLOBAL hglobal = LoadResource((HMODULE)dllInstance, hrsrc);
		if (hglobal)
		{
			char *c = (char *)LockResource(hglobal);
			if (c)
			{
				m_Text.SetWindowText(c);
			}
			else
				MessageBox("CCmdListDlg::OnInitDialog failure: c == NULL");
		}
		else
			MessageBox("CCmdListDlg::OnInitDialog failure: hglobal == NULL");
	}
	else
		MessageBox("CCmdListDlg::OnInitDialog failure: hrsrc == NULL");

	m_cmbShow.AddString("Command list");
	m_cmbShow.SetCurSel(0);

	Resized();

	m_Text.SetEventMask(ENM_MOUSEEVENTS);

	SetTimer(1,100,0);

	transp = 255;

/*	GetWindowRect(rc);
	CRect r; GetDesktopWindow()->GetClientRect(r);
	SetWindowPos(NULL,r.right - rc.right, r.bottom - rc.bottom, rc.right, rc.bottom,
		SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCmdListDlg::Resized()
{
	CRect rc; GetClientRect(rc);
	CRect r2; m_cmbShow.GetClientRect(r2);
	int b = r2.Height();

	m_cmbShow.SetWindowPos(NULL,0,0,rc.right,b,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	m_Text.SetWindowPos(NULL,0,b,rc.right,rc.bottom-b,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void CCmdListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	Resized();
	//m_Text.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

}

void CCmdListDlg::OnTimer(UINT nIDEvent) 
{
	m_Text.SetSel(-1,-1);
	KillTimer(1);	
	CDialog::OnTimer(nIDEvent);
}

void CCmdListDlg::OnClose() 
{
	pmi->pCmdListDlg = NULL;

	CDialog::OnClose();

	DestroyWindow();
	delete this;
}

void CCmdListDlg::OnOk() 
{
}

void CCmdListDlg::OnCancel() 
{
}
/*
void CCmdListDlg::OnMsgfilterRichedit1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	MSGFILTER *pMsgFilter = reinterpret_cast<MSGFILTER *>(pNMHDR);
	// TODO: The control will not send this notification unless you override the
	// CDialog::OnInitDialog() function to send the EM_SETEVENTMASK message
	// to the control with either the ENM_KEYEVENTS or ENM_MOUSEEVENTS flag 
	// ORed into the lParam mask.
	
	// TODO: Add your control notification handler code here

	if (pMsgFilter->msg == WM_MOUSEWHEEL)
	{
		if (LOWORD(pMsgFilter->wParam) & MK_SHIFT)
		{
			OutputDebugString("Coudhjg");
			if ((short)HIWORD(pMsgFilter->wParam) > 0)
				transp += 16;
			else
				transp -= 16;

			if (transp > 255) transp = 255;
			if (transp < 63) transp = 63;

			if (transp == 255)
				if (MakeWindowTransparent(m_hWnd,255,false)) OutputDebugString("Beh"); else OutputDebugString("Mesh");
			else
				if (MakeWindowTransparent(m_hWnd,(unsigned char)transp,true)) OutputDebugString("Sahris");


		}
	}
	
	*pResult = 0;
}
*/
