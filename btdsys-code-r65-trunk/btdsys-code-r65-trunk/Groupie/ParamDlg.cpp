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

// ParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys groupie.h"
#include "ParamDlg.h"
#include "groupie.h"
#include "KeyboardDlg.h"

#include "BuzzMI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CParamDlg dialog


CParamDlg::CParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParamDlg::IDD, pParent)
{
	pKeyDlg = NULL;

	//{{AFX_DATA_INIT(CParamDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParamDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CParamDlg, CDialog)
	//{{AFX_MSG_MAP(CParamDlg)
	ON_WM_CLOSE()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ADDNEWTRACK, OnAddnewtrack)
	ON_COMMAND(ID_DELETELASTTRACK, OnDeletelasttrack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParamDlg message handlers

#define S_COMMON	WS_CHILD | WS_VISIBLE | WS_TABSTOP


void CParamDlg::AddControlsForParam(const CMachineParameter *par, 
									CRect *r, CRect *r2, int id,
									int track, word value)
{
	r2->left = 100;
	r2->right = r->right - 100;
	CSliderCtrl *pSB = NULL;
	CButton *pBt;
	if (par->Type == pt_note)
	{
		pBt = new CButton;
		pBt->Create("Open keyboard window...",S_COMMON,*r2,this,id);
		pBt->SetFont(&fntNormal);

		vpSliders.push_back((CSliderCtrl *)pBt);
	}
	else
	{
		pSB = new CSliderCtrl();
		pSB->Create(S_COMMON | TBS_HORZ, *r2, this, id);

		if (par->Type == pt_switch)
			pSB->SetRange(0,1);
		else
			pSB->SetRange(par->MinValue, par->MaxValue);

		pSB->SetPos(value);

		vpSliders.push_back(pSB);
	}

	char txt[256];
	if (track >= 0)
		sprintf(txt,"%i-%s",track,par->Name);
	else
		strcpy(txt,par->Name);

	r2->left = 0;
	r2->right = 98;
	CStatic *pS1 = new CStatic();
	pS1->Create(txt, S_COMMON, *r2, this, 0xffff);
	if (par->Flags & MPF_STATE)
		pS1->SetFont(&fntNormal);
	else
		pS1->SetFont(&fntNonState);

	vpParamLabels.push_back(pS1);

	r2->left = r->right - 98;
	r2->right = r->right;
	CStatic *pS2 = new CStatic();
	pS2->Create("", S_COMMON, *r2, this, 0xffff);
	pS2->SetFont(&fntNormal);
	vpParamValues.push_back(pS2);

	if (par->Type != pt_note)
		UpdateParamValueLabel(vpSliders.size()-1);

}

BOOL CParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char txt[256];
	sprintf(txt, "Parameters - \"%s\" - %s", m_pMac->name, m_pMac->pInfo->Name);
	SetWindowText(txt);
	
	CreateParamControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CParamDlg::OnClose() 
{
	DestroyParamControls();

	m_pMac->pParamDlg = NULL;

	//DestroyWindow();
	
	CDialog::OnClose();

	delete this;
}

void CParamDlg::DestroyParamControls()
{
	for (vector<CSliderCtrl *>::iterator pp = vpSliders.begin();
		 pp < vpSliders.end(); pp++)
	{
		if (*pp)
			delete (*pp);
	}

	for (vector<CStatic *>::iterator qq = vpParamLabels.begin();
		 qq < vpParamLabels.end(); qq++)
	{
		if (*qq)
			delete (*qq);
	}

	for (qq = vpParamValues.begin(); qq < vpParamValues.end(); qq++)
	{
		if (*qq)
			delete (*qq);
	}

	vpSliders.clear();
	vpParamLabels.clear();
	vpParamValues.clear();
}

void CParamDlg::CreateParamControls()
{
	LOGFONT lf;
	lf.lfCharSet = ANSI_CHARSET;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfEscapement = 0;
	strcpy(lf.lfFaceName, "Arial");
	lf.lfHeight = -12;
	lf.lfItalic = FALSE;
	lf.lfOrientation = 0;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfStrikeOut = FALSE;
	lf.lfUnderline = FALSE;
	lf.lfWeight = FW_NORMAL;
	lf.lfWidth = 0;

	fntNormal.CreateFontIndirect(&lf);
	
	lf.lfStrikeOut = TRUE;
	fntNonState.CreateFontIndirect(&lf);

	int id = 3000;
	CRect r;
	GetClientRect(r);

	CRect r2(0, 0, 0, 16);

	for (int g=0; g<m_pMac->pInfo->numGlobalParameters; g++)
	{
		AddControlsForParam(m_pMac->pInfo->Parameters[g], &r, &r2, id, -1,
			m_pMac->pGlobalState[g]);
		id++;
		r2.OffsetRect(0,20);
	}

	for (int t=0; t<m_pMac->nTracks; t++)
	{
		for (int u=0; u<m_pMac->pInfo->numTrackParameters; u++)
		{
			AddControlsForParam(
				m_pMac->pInfo->Parameters[m_pMac->pInfo->numGlobalParameters+u],
				&r, &r2, id, t, 
				m_pMac->pTrackState[t*m_pMac->pInfo->numTrackParameters + u]);
			id++;
			r2.OffsetRect(0,20);
		}
	}

	m_nTotalHeight = r2.top;

	SetScrollRange(SB_VERT,0,m_nTotalHeight - r.bottom);
}

void CParamDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	switch (nSBCode)
	{
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		SetScrollPos(SB_VERT,nPos);
		Scroll(nPos);
		break;
	case SB_LINEUP:
		SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)-5);
		Scroll(GetScrollPos(SB_VERT));
		break;
	case SB_LINEDOWN:
		SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)+5);
		Scroll(GetScrollPos(SB_VERT));
		break;
	case SB_PAGEUP:
		SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)-20);
		Scroll(GetScrollPos(SB_VERT));
		break;
	case SB_PAGEDOWN:
		SetScrollPos(SB_VERT,GetScrollPos(SB_VERT)+20);
		Scroll(GetScrollPos(SB_VERT));
		break;

	}
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CParamDlg::Scroll(int y)
{
	int cy = -y;

	CRect r;

	for (vector<CSliderCtrl *>::iterator pp = vpSliders.begin();
		 pp < vpSliders.end(); pp++)
	{
		(*pp)->GetWindowRect(r);
		ScreenToClient(r);
		(*pp)->SetWindowPos(NULL,r.left,cy,r.Width(),16,SWP_NOZORDER);
		(*pp)->RedrawWindow();
		cy += 20;
	}

	cy = -y;

	for (vector<CStatic *>::iterator qq = vpParamLabels.begin();
		 qq < vpParamLabels.end(); qq++)
	{
		(*qq)->GetWindowRect(r);
		ScreenToClient(r);
		(*qq)->SetWindowPos(NULL,r.left,cy,r.Width(),16,SWP_NOZORDER);
		(*qq)->RedrawWindow();
		cy += 20;
	}

	cy = -y;

	for (qq = vpParamValues.begin(); qq < vpParamValues.end(); qq++)
	{
		(*qq)->GetWindowRect(r);
		ScreenToClient(r);
		(*qq)->SetWindowPos(NULL,r.left,cy,r.Width(),16,SWP_NOZORDER);
		(*qq)->RedrawWindow();
		cy += 20;
	}

	
}

void CParamDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect r, r2;
	GetClientRect(r2);

	for (vector<CSliderCtrl *>::iterator pp = vpSliders.begin();
		 pp < vpSliders.end(); pp++)
	{
		(*pp)->GetWindowRect(r);
		ScreenToClient(r);
		(*pp)->SetWindowPos(NULL,100,r.top,r2.right-200,r.Height(),SWP_NOZORDER);
		(*pp)->RedrawWindow();
	}

	for (vector<CStatic *>::iterator qq = vpParamValues.begin();
		 qq < vpParamValues.end(); qq++)
	{
		(*qq)->GetWindowRect(r);
		ScreenToClient(r);
		(*qq)->SetWindowPos(NULL,r2.right-98,r.top,98,r.Height(),SWP_NOZORDER);
		(*qq)->RedrawWindow();
	}

	int s = GetScrollPos(SB_VERT);
	if (s > m_nTotalHeight - r2.bottom)
		SetScrollPos(SB_VERT, m_nTotalHeight - r2.bottom, FALSE);

	SetScrollRange(SB_VERT,0,m_nTotalHeight - r2.bottom);

}

void CParamDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	int nBar = -1;
	for (int i=0; i<vpSliders.size() && nBar == -1; i++)
	{
		if (vpSliders[i] == (CSliderCtrl *)pScrollBar)
			nBar = i;
	}

	if (nBar != -1)
	{
		UpdateParamValueLabel(nBar);

		SendParamValue(nBar);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CParamDlg::UpdateParamValueLabel(int nBar)
{
	int n = nBar;
	int t;
	if (nBar < m_pMac->pInfo->numGlobalParameters)
		t = -1;
	else
	{
		n -= m_pMac->pInfo->numGlobalParameters;
		t = n / m_pMac->pInfo->numTrackParameters;
		n %= m_pMac->pInfo->numTrackParameters;
		n += m_pMac->pInfo->numGlobalParameters;
	}

	int nPos = vpSliders[nBar]->GetPos();

	const char *txt = m_pMac->DescribeValue(n,nPos,__FILE__,__LINE__);
	static char txt2[16];
	if (txt == NULL)
	{
		txt = txt2;
		sprintf(txt2,"%i / 0x%X",nPos,nPos);
	}

	vpParamValues[nBar]->SetWindowText(txt);

}

void CParamDlg::SendParamValue(int nBar, int nPos)
{
	int n = nBar;
	int t;
	if (nBar < m_pMac->pInfo->numGlobalParameters)
		t = -1;
	else
	{
		n -= m_pMac->pInfo->numGlobalParameters;
		t = n / m_pMac->pInfo->numTrackParameters;
		n %= m_pMac->pInfo->numTrackParameters;
	}

	if (nPos == -1) nPos = vpSliders[nBar]->GetPos();

	int nFlags;
	if (t == -1)
		nFlags = m_pMac->pInfo->Parameters[n]->Flags;
	else
		nFlags = m_pMac->pInfo->Parameters[m_pMac->pInfo->numGlobalParameters+n]->Flags;

	if (nFlags & MPF_STATE)
	{
		if (t == -1 && m_pMac->pGlobalState)
		{
			m_pMac->pGlobalState[n] = (word)nPos;
		}
		else if (t != -1 && m_pMac->pTrackState)
		{
			m_pMac->pTrackState[n + t*m_pMac->pInfo->numTrackParameters] = (word)nPos;
		}
	}

	m_pMac->ChangeParam(n,t,nPos);
}

void CParamDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	m_nContextMenuBar   = -1;
	m_nContextMenuParam = -1;
	m_nContextMenuTrack = -1;

	for (int i=0; i<vpSliders.size() && m_nContextMenuBar == -1; i++)
	{
		if (vpSliders[i] == (CSliderCtrl *)pWnd)
		{
			m_nContextMenuBar = i;

			m_nContextMenuParam = i;
			if (i < m_pMac->pInfo->numGlobalParameters)
			{
				m_nContextMenuTrack = -1;
				m_pContextMenuParam = m_pMac->pInfo->Parameters[m_nContextMenuParam];
			}
			else
			{
				m_nContextMenuParam -= m_pMac->pInfo->numGlobalParameters;
				m_nContextMenuTrack = m_nContextMenuParam / m_pMac->pInfo->numTrackParameters;
				m_nContextMenuParam %= m_pMac->pInfo->numTrackParameters;
				m_pContextMenuParam = m_pMac->pInfo->Parameters[m_nContextMenuParam + m_pMac->pInfo->numGlobalParameters];
			}
		}
	}

	if (m_nContextMenuBar != -1)
	{
		CMenu mPopup;
		mPopup.LoadMenu(IDR_POPUPS);

		HMENU hMenu = GetSubMenu(mPopup.m_hMenu, 2);
		HMENU hSub  = GetSubMenu(hMenu, 3);

		int id = ID_PARAM_MENUITEM_ZERO;

		for (int t=0; t<m_pmi->numTracks; t++)
		{
			AppendMenu(hSub, MF_STRING, id, 
				m_pmi->assigns[m_pContextMenuParam->Type][t].GetDesc());
			id++;
		}

		TrackPopupMenu(GetSubMenu(mPopup.m_hMenu, 2),
			TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			point.x, point.y, 0,
			m_hWnd,
			NULL);
	}
}

BOOL CParamDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (HIWORD(wParam) == 0 && LOWORD(wParam) >= ID_PARAM_MENUITEM_ZERO)
	{	// context menu assignment command
		CParamAssignInfo *a = &m_pmi->assigns[m_pContextMenuParam->Type]
			[LOWORD(wParam) - ID_PARAM_MENUITEM_ZERO];

		a->pTargetMac = m_pMac;
		a->nParam = m_nContextMenuParam;
		a->nTrack = m_nContextMenuTrack;
		
		return 0;
	}
	else if (HIWORD(wParam) == BN_CLICKED)
	{
		for (int i=0; i<vpSliders.size(); i++)
		{
			CButton *p = (CButton *)vpSliders[i];
			if (p->GetDlgCtrlID() == LOWORD(wParam))
			{
				if (!pKeyDlg)
				{
					pKeyDlg = new CKeyboardDlg;
					pKeyDlg->pMyRef = &pKeyDlg;
					pKeyDlg->pParamDlg = this;

					if (pKeyDlg->Create(IDD_KEYBOARDDLG/*,this*/))
					{
						pKeyDlg->ShowWindow(SW_SHOW);
					}
					else
					{
						delete pKeyDlg;
						pKeyDlg = NULL;
					}
				}
				if (pKeyDlg)
				{
					pKeyDlg->pMac = m_pMac;
					pKeyDlg->nBar = i;
				}
			}
		}
		return 0;
	}
	else
		return CDialog::OnCommand(wParam, lParam);
}

void CParamDlg::OnAddnewtrack() 
{
	if (m_pMac->nTracks < m_pMac->pInfo->maxTracks-1)
	{
		m_pMac->nTracks ++;

		m_pMac->SetNumTracks(m_pMac->nTracks,__FILE__,__LINE__);

		DestroyParamControls();
		CreateParamControls();
	}
}

void CParamDlg::OnDeletelasttrack() 
{
	if (MessageBox("Sure?",NULL,MB_YESNO) == IDYES)
	{
		if (m_pMac->nTracks > m_pMac->pInfo->minTracks)
		{
			m_pMac->nTracks --;

			m_pMac->SetNumTracks(m_pMac->nTracks,__FILE__,__LINE__);

			DestroyParamControls();
			CreateParamControls();
		}
	}
}
