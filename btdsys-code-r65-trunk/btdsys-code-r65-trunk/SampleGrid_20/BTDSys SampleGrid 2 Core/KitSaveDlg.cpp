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

// KitSaveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "core.h"
#include "sgrid.h"
#include "BTDSys SampleGrid 2 Core.h"
#include "KitSaveDlg.h"

char *FormatSize(int size);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKitSaveDlg dialog


CKitSaveDlg::CKitSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKitSaveDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKitSaveDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKitSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKitSaveDlg)
	DDX_Control(pDX, IDC_SIZEESTIMATE, m_stSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKitSaveDlg, CDialog)
	//{{AFX_MSG_MAP(CKitSaveDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKitSaveDlg message handlers
/*
char *FormatSize(int size)
{
	static char txt[32];

	if (size < 1024)
		sprintf(txt,"%iB",size);
	else if (size < 1024*1024)
		sprintf(txt,"%.2fkB",size/1024.0f);
	else if (size < 1024*1024*1024)
		sprintf(txt,"%.2fMB",size/1024.0f/1024.0f);
	else
		sprintf(txt,"%.2fGB",size/1024.0f/1024.0f/1024.0f);

	return txt;		
}
*/

BOOL CKitSaveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CheckRadioButton(IDC_NOSAVE,IDC_BIGSAVE,IDC_NOSAVE + nMode);

	UpdateSizeEst(nMode);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKitSaveDlg::OnOK() 
{
	nMode = GetCheckedRadioButton(IDC_NOSAVE,IDC_BIGSAVE) - IDC_NOSAVE;

	if (pmi->Kit.GetSize(nMode) >= 20*1024*1024)
	{
		if (MessageBox("The option you have chosen will make your bmx/bmw file very large "
			"and slow to save/load. Are you sure you want to do this?",NULL,MB_YESNO) == IDNO)
			return;
	}
	
	
	CDialog::OnOK();
}

void CKitSaveDlg::OnClose() 
{
	OnOK();

//	CDialog::OnClose();
}

BOOL CKitSaveDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == BN_CLICKED)
	{
		int m = GetCheckedRadioButton(IDC_NOSAVE,IDC_BIGSAVE) - IDC_NOSAVE;
		UpdateSizeEst(m);
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

void CKitSaveDlg::UpdateSizeEst(int m)
{
	char txt[256];

	int size = pmi->Kit.GetSize(m);

	sprintf(txt,"Estimated disk usage for selected option: %s",FormatSize(size));

	m_stSize.SetWindowText(txt);
}

