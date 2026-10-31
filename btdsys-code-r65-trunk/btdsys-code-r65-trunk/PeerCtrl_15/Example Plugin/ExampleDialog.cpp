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

// ExampleDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Example Plugin.h"
#include "ExampleDialog.h"
#include "plug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExampleDialog dialog


CExampleDialog::CExampleDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExampleDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExampleDialog)
	//}}AFX_DATA_INIT
}


void CExampleDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExampleDialog)
	DDX_Control(pDX, IDC_SLIDER2, m_Scrollbar2);
	DDX_Control(pDX, IDC_SLIDER1, m_Scrollbar1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExampleDialog, CDialog)
	//{{AFX_MSG_MAP(CExampleDialog)
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExampleDialog message handlers

void CExampleDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{	//a slider was moved
		if (pScrollBar == (CScrollBar *)&m_Scrollbar1)
			pPl->pCB->SetValue(0,(float)nPos / 1000.0f);

		if (pScrollBar == (CScrollBar *)&m_Scrollbar2)
			pPl->pCB->SetValue(1,(float)nPos / 1000.0f);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CExampleDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_Scrollbar1.SetRange(0,1000);
	m_Scrollbar2.SetRange(0,1000);

	//set initial slider positions
	float v = pPl->pCB->GetValue(0);
	if (v >= 0.0f) m_Scrollbar1.SetPos((int)(v * 1000.0f));

	v = pPl->pCB->GetValue(1);
	if (v >= 0.0f) m_Scrollbar2.SetPos((int)(v * 1000.0f));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExampleDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	pPl->pdlg = NULL;

	CDialog::OnClose();
}
