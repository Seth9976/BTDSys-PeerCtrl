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

// SelectMachineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mooter.h"
#include "SelectMachineDlg.h"
#include "../mdk/mdk.h"
#include "machine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectMachineDlg dialog


CSelectMachineDlg::CSelectMachineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectMachineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectMachineDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSelectMachineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectMachineDlg)
	DDX_Control(pDX, IDC_CURRENT, m_stCurrent);
	DDX_Control(pDX, IDC_LIST1, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectMachineDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectMachineDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectMachineDlg message handlers

class CMyDataOut : public CMachineDataOutput
//A data out class for receiving machine names
{
public:
	virtual void Write(void *pbuf, int const numbytes)
	{
		if (strcmp((char*)pbuf, "Master") != 0)
		{
			int i = pl->AddString((char*)pbuf);
			if (strcmp((char *)pbuf, selectme) == 0)
				pl->SetCurSel(i);
		}
	}

	CSelectMachineDlg *pd;
	CListBox *pl;
	char *selectme;
};

BOOL CSelectMachineDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMyDataOut mydo;
	mydo.pd = this;
	mydo.pl = &m_List;
	mydo.selectme = ptr->mac ? ptr->macname : "";

	pmi->pCB->GetMachineNames(&mydo);

	char txt[256];
	sprintf(txt,"Track %i (current: %s)",ntr,ptr->mac ? ptr->macname : "No assign");
	m_stCurrent.SetWindowText(txt);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectMachineDlg::OnOK() 
{
	ptr->mac = NULL;
	ptr->macname[0] = 0;

	int s = m_List.GetCurSel();
	if (s != LB_ERR)
	{
		m_List.GetText(s,ptr->macname);
		ptr->CheckMachine();
	}
	
	CDialog::OnOK();
}
