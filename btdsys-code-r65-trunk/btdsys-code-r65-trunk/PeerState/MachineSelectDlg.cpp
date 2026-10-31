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

// MachineSelectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys PeerState.h"
#include "MachineSelectDlg.h"

#include "../mdk/mdk.h"
#include "peerstate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMachineSelectDlg dialog


CMachineSelectDlg::CMachineSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMachineSelectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMachineSelectDlg)
	//}}AFX_DATA_INIT
}


void CMachineSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMachineSelectDlg)
	DDX_Control(pDX, IDC_MACTREE, m_MacTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMachineSelectDlg, CDialog)
	//{{AFX_MSG_MAP(CMachineSelectDlg)
	ON_BN_CLICKED(IDC_SELALL, OnSelall)
	ON_BN_CLICKED(IDC_SELNONE, OnSelnone)
	ON_BN_CLICKED(IDC_SELINVERT, OnSelinvert)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMachineSelectDlg message handlers

class CMyDO : public CMachineDataOutput
{
public:
	CMachineSelectDlg *pdlg;
	mi *pmi;
	const char *exclname;

	void Write(void *pbuf, int const numbytes);
};

void CMyDO::Write(void *pbuf, int const numbytes)
{
	if (strcmp(exclname, (const char *)pbuf) != 0)
	{
/*			int i = pdlg->m_MacList.AddString((const char *)pbuf);
		CMachineRec *pr = pdlg->pmi->FindMachineInPeers((const char *)pbuf);

		if (pr) if (pr->bActive)
			pdlg->m_MacList.SetSel(i,TRUE);
*/		
		char txt[256];
		const CMachineInfo *pinfo 
			= pmi->pCB->GetMachineInfo(pmi->pCB->GetMachine((const char *)pbuf));

		HTREEITEM hMac = pdlg->m_MacTree.InsertItem((const char *)pbuf);

		CMachineRec *pr = pmi->FindMachineInPeers((const char *)pbuf);

		for (int i=0; i<pinfo->numGlobalParameters + pinfo->numTrackParameters; i++)
		{
			const CMachineParameter *par = pinfo->Parameters[i];
			if (par->Flags & MPF_STATE)
			{
				sprintf(txt,"[%c] %s", i < pinfo->numGlobalParameters ? 'G' : 'T',
					par->Name);
				HTREEITEM hI = pdlg->m_MacTree.InsertItem(txt, hMac);
				pdlg->m_MacTree.SetItemData(hI, i);

				if (pr)
				{
					if (pr->bActive && pr->vbParamActive[i])
						pdlg->m_MacTree.SetState(hI, MYCH_CHECKED);
					else
						pdlg->m_MacTree.SetState(hI, MYCH_UNCHECKED);
				}
				else
					pdlg->m_MacTree.SetState(hI, MYCH_UNCHECKED);

			}
		}

		pdlg->m_MacTree.SetCheck(hMac, MYSETCH_REFRESH);
	}

};


BOOL CMachineSelectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
//	m_MacTree.ResetContent();
	
	CMyDO mydo;
	mydo.pdlg = this;
	mydo.pmi = pmi;
	mydo.exclname = pmi->pCB->GetMachineName(pmi->thismac);
	pmi->pCB->GetMachineNames(&mydo);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMachineSelectDlg::OnOK() 
{
	HTREEITEM hM = m_MacTree.GetChildItem(TVI_ROOT);

	while (hM)
	{
		CMachineRec *pr = pmi->FindMachineInPeers(m_MacTree.GetItemText(hM));

		if (!pr && m_MacTree.GetState(hM) != MYCH_UNCHECKED)
		{
			pmi->AddMachineToPeers(m_MacTree.GetItemText(hM));
			pr = pmi->FindMachineInPeers(m_MacTree.GetItemText(hM));
		}

		if (pr)
		{
			if (m_MacTree.GetState(hM) == MYCH_UNCHECKED)
			{
				pr->bActive = false;
				for (vector<bool>::iterator p = pr->vbParamActive.begin(); p<pr->vbParamActive.end(); p++)
					*p = false;
			}

			else if (m_MacTree.GetState(hM) == MYCH_CHECKED)
			{
				pr->bActive = true;
				for (vector<bool>::iterator p = pr->vbParamActive.begin(); p<pr->vbParamActive.end(); p++)
					*p = true;
			}
			else
			{
				pr->bActive = true;
				HTREEITEM hI = m_MacTree.GetChildItem(hM);
				while (hI)
				{
					pr->vbParamActive[m_MacTree.GetItemData(hI)] 
						= (m_MacTree.GetState(hI) == MYCH_CHECKED);

					hI = m_MacTree.GetNextSiblingItem(hI);
				}
			}

		}

		hM = m_MacTree.GetNextSiblingItem(hM);
	}

	pmi->SetModifiedFlag();

/*	for (int i=0; i<m_MacList.GetCount(); i++)
	{
		char name[64];
		m_MacList.GetText(i,name);

		CMachineRec *pr = pmi->FindMachineInPeers(name);

		if (m_MacList.GetSel(i))
		{
			if (pr)
				pr->bActive = true;
			else
				pmi->AddMachineToPeers(name);
		}
		else
		{
			if (pr)
				pr->bActive = false;
		}
	}
*/
	CDialog::OnOK();
}

void CMachineSelectDlg::OnSelall() 
{
	HTREEITEM hM = m_MacTree.GetChildItem(TVI_ROOT);

	while (hM)
	{
		HTREEITEM hI = m_MacTree.GetChildItem(hM);
		while (hI)
		{
			m_MacTree.SetState(hI, MYCH_CHECKED);

			hI = m_MacTree.GetNextSiblingItem(hI);
		}

		m_MacTree.SetCheck(hM, MYSETCH_REFRESH);

		hM = m_MacTree.GetNextSiblingItem(hM);
	}
}

void CMachineSelectDlg::OnSelnone() 
{
	HTREEITEM hM = m_MacTree.GetChildItem(TVI_ROOT);

	while (hM)
	{
		HTREEITEM hI = m_MacTree.GetChildItem(hM);
		while (hI)
		{
			m_MacTree.SetState(hI, MYCH_UNCHECKED);

			hI = m_MacTree.GetNextSiblingItem(hI);
		}

		m_MacTree.SetCheck(hM, MYSETCH_REFRESH);

		hM = m_MacTree.GetNextSiblingItem(hM);
	}
}

void CMachineSelectDlg::OnSelinvert() 
{
	HTREEITEM hM = m_MacTree.GetChildItem(TVI_ROOT);

	while (hM)
	{
		HTREEITEM hI = m_MacTree.GetChildItem(hM);
		while (hI)
		{
			m_MacTree.SetState(hI,
				(m_MacTree.GetState(hI) == MYCH_CHECKED) ? MYCH_UNCHECKED : MYCH_CHECKED);

			hI = m_MacTree.GetNextSiblingItem(hI);
		}

		m_MacTree.SetCheck(hM, MYSETCH_REFRESH);

		hM = m_MacTree.GetNextSiblingItem(hM);
	}	
}
