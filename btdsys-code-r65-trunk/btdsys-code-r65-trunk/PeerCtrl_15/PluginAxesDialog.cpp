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

#include "stdafx.h"
#include "BTDSys PeerCtrl.h"
#include "PluginAxesDialog.h"
#include "ctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPluginAxesDialog dialog


CPluginAxesDialog::CPluginAxesDialog(int cnum, CWnd* pParent /*=NULL*/)
	: CDialog(CPluginAxesDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluginAxesDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	ctrlnum = cnum;
}


void CPluginAxesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginAxesDialog)
	DDX_Control(pDX, IDC_BOUNDLIST, m_BoundList);
	DDX_Control(pDX, IDC_AVAILLIST, m_AvailList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginAxesDialog, CDialog)
	//{{AFX_MSG_MAP(CPluginAxesDialog)
	ON_BN_CLICKED(IDC_ARROWBUTTON, OnArrowbutton)
	ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
	ON_BN_CLICKED(IDC_ABOUTAXIS, OnAboutaxis)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginAxesDialog message handlers

BOOL CPluginAxesDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	char txt[256];
	sprintf(txt, "Plugin bindings for %s, track %i", 
		pmi->pCB->GetMachineName(pmi->ThisMac),
		ptrack->tracknum);

	SetWindowText(txt);

	//populate "available" list

	for (int i=0; i<pmi->plugs.size(); i++)
	{
		for (int j=0; j<pmi->plugs[i]->axes.size(); j++)
		{
			if (pmi->plugs[i]->axes[j].show)
			{
				sprintf(txt, "%s -> %s", 
					pmi->plugs[i]->pInfo->Name,
					pmi->plugs[i]->axes[j].name);

				int k = m_AvailList.AddString(txt);

				SAxisInfo *d = new SAxisInfo;
				d->pPl = pmi->plugs[i];
				d->nAxis = j;

				m_AvailList.SetItemDataPtr(k, d);
			}
		}
	}

	//populate "bound" list
	for (int j=0; j<ptrack->axes.size(); j++)
	{
		sprintf(txt, "%s -> %s", 
			ptrack->axes[j].pPl->pInfo->Name,
			ptrack->axes[j].pPl->axes[ptrack->axes[j].nAxis].name);

		if (! ptrack->axes[j].pPl->axes[ptrack->axes[j].nAxis].show)
			sprintf(txt,"[hidden: %s]",txt);

		m_BoundList.AddString(txt);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPluginAxesDialog::OnArrowbutton() 
{
	// TODO: Add your control notification handler code here
	
	int s = m_AvailList.GetCurSel();

	if (s != LB_ERR)
	{
		SAxisInfo *d = (SAxisInfo *)m_AvailList.GetItemDataPtr(s);
		CPlugAxis *pAx = &d->pPl->axes[d->nAxis];

		//check it's not already assigned
		for (int i=0; i<pAx->binding.size(); i++)
		{
			if (pAx->binding[i] == ptrack->tracknum)
				return;
		}

		pAx->binding.push_back(ptrack->tracknum);
		ptrack->axes.push_back(*d);

		char txt[256];
		sprintf(txt, "%s -> %s", 
			d->pPl->pInfo->Name,
			pAx->name);

		m_BoundList.AddString(txt);
	}
}

void CPluginAxesDialog::OnDelbutton() 
{
	// TODO: Add your control notification handler code here
	
	int s = m_BoundList.GetCurSel();

	if (s != LB_ERR)
	{
		SAxisInfo *d = &ptrack->axes[s];
		CPlugAxis *pAx = &d->pPl->axes[d->nAxis];

		for (int i=0; i<pAx->binding.size(); i++)
		{
			if (pAx->binding[i] == ptrack->tracknum)
				pAx->binding.erase(pAx->binding.begin() + i);
		}

		ptrack->axes.erase(ptrack->axes.begin() + s);

		m_BoundList.DeleteString(s);
	}
}

BOOL CPluginAxesDialog::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	for (int i=0; i<m_AvailList.GetCount(); i++)
	{
		SAxisInfo *d = (SAxisInfo *)m_AvailList.GetItemDataPtr(i);
		if (d) delete d;
	}

	return CDialog::DestroyWindow();
}


void CPluginAxesDialog::OnAboutaxis() 
{
	// TODO: Add your control notification handler code here
/*
	int s = m_AvailList.GetCurSel();

	if (s != LB_ERR)
	{
		SAxisInfo *d = (SAxisInfo *)m_AvailList.GetItemDataPtr(s);
		CPlugAxis *pAx = &d->pPl->axes[d->nAxis];

		char txt[1024];

		//check it's not already assigned
		for (int i=0; i<pAx->binding.size(); i++)
		{
			if (pAx->binding[i] == ptrack->tracknum)
				return;
		}

		pAx->binding.push_back(ptrack->tracknum);
		ptrack->axes.push_back(*d);

		char txt[256];
		sprintf(txt, "%s -> %s", 
			d->pPl->pInfo->Name,
			pAx->name);

		m_BoundList.AddString(txt);
	}
*/
}
