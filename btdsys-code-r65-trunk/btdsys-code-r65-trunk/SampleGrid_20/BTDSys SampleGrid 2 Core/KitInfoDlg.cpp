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

// KitInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys SampleGrid 2 Core.h"
#include "sgrid.h"
#include "KitInfoDlg.h"
#include "drumkit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKitInfoDlg dialog


CKitInfoDlg::CKitInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKitInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKitInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKitInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKitInfoDlg)
	DDX_Control(pDX, IDC_PREVIEW, m_btnPreview);
	DDX_Control(pDX, IDC_PRETRACKCOMBO, m_cmbPreviewTrack);
	DDX_Control(pDX, IDC_KITTREE, m_treeKit);
	DDX_Control(pDX, IDC_SAMPLEINFO, m_edSampleInfo);
	DDX_Control(pDX, IDC_KITINFO, m_edKitInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKitInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CKitInfoDlg)
	ON_BN_CLICKED(IDC_PREVIEW, OnPreview)
	ON_NOTIFY(TVN_SELCHANGED, IDC_KITTREE, OnSelchangedKittree)
	ON_NOTIFY(NM_DBLCLK, IDC_KITTREE, OnDblclkKittree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKitInfoDlg message handlers

BOOL CKitInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	static char txt[32768];
	
	const char *type;
	switch (pKit->nKitType)
	{
	case ktPSI:		type = "PSI";		break;
	case ktBattery:	type = "Battery";	break;
	default:		type = "???";
	}

	sprintf(txt,"Name: %s\r\n"
				"File: %s\r\n"
				"Type: %s\r\n"
				"Author: %s\r\n"
				"Comment: %s",
				pKit->Name, pKit->FileName, type, pKit->Author, pKit->Comment);

	m_edKitInfo.SetWindowText(txt);

	m_imgsKitTree.Create(IDB_KITINFOIMGS,16,0,0xFF00FF);
	m_treeKit.SetImageList(&m_imgsKitTree, TVSIL_NORMAL);

	for (int i=1; i<=200; i++)
	{
		if (pKit->sample[i].HasData())
		{
			sprintf(txt,"%03i: %s",i,pKit->sample[i].Name);
			int img = (pKit->sample[i].layers.size() <= 1) ? 0 : 1;
			HTREEITEM hSample = m_treeKit.InsertItem(txt,img,img);
			m_treeKit.SetItemData(hSample,(DWORD)&pKit->sample[i]);

			for (CDrumkitLayer **p = pKit->sample[i].layers.begin(); p < pKit->sample[i].layers.end(); p++)
			{
				sprintf(txt,"[%03i-%03i] %s",(*p)->nVelMin, (*p)->nVelMax, (*p)->dataFileName);
				img = (*p)->bDataLoaded ? 3 : 2;
				HTREEITEM hLayer = m_treeKit.InsertItem(txt,img,img,hSample);
				m_treeKit.SetItemData(hLayer,(DWORD)(*p));
			}
		}
	}

	for (i=0; i<pmi->nTriggers; i++)
	{
		char txt[16];
		sprintf(txt,"%i",i);
		m_cmbPreviewTrack.AddString(txt);
	}
	m_cmbPreviewTrack.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKitInfoDlg::OnPreview() 
{
	int trk = m_cmbPreviewTrack.GetCurSel();
	if (trk != -1)
	{
		HTREEITEM i = m_treeKit.GetSelectedItem();
		if (m_treeKit.GetParentItem(i))
		{
			CDrumkitLayer *p = (CDrumkitLayer *)m_treeKit.GetItemData(i);
			pmi->Tracks[trk].TriggerPreview(p);
			int img = p->bDataLoaded ? 3 : 2;
			m_treeKit.SetItemImage(i,img,img);
		}
		else if (i)
		{
			pmi->Tracks[trk].TriggerPreview((CDrumkitSample *)m_treeKit.GetItemData(i));

			HTREEITEM child = m_treeKit.GetChildItem(i);
			while (child)
			{
				CDrumkitLayer *p = (CDrumkitLayer *)m_treeKit.GetItemData(child);
				int img = p->bDataLoaded ? 3 : 2;
				m_treeKit.SetItemImage(child,img,img);

				child = m_treeKit.GetNextSiblingItem(child);
			}
		}
	}
}

void CKitInfoDlg::OnSelchangedKittree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	char txt[1024];

	HTREEITEM i = m_treeKit.GetSelectedItem();
	if (m_treeKit.GetParentItem(i))
	{
		((CDrumkitLayer *)m_treeKit.GetItemData(i))->GetInfo(txt);
	}
	else if (i)
	{
		((CDrumkitSample *)m_treeKit.GetItemData(i))->GetInfo(txt);
	}
	else
		txt[0] = 0;

	m_edSampleInfo.SetWindowText(txt);

	
	*pResult = 0;
}

void CKitInfoDlg::OnDblclkKittree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnPreview();
	
	*pResult = 0;
}
