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

// GroupPerfDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys samplegrid 2 core.h"
#include "GroupPerfDlg.h"
#include "sgrid.h"
#include "memdc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupPerfDlg dialog


CGroupPerfDlg::CGroupPerfDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupPerfDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupPerfDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGroupPerfDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupPerfDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupPerfDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupPerfDlg)
	ON_WM_CLOSE()
	ON_COMMAND(IDOK, OnOk)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupPerfDlg message handlers

void CGroupPerfDlg::OnClose() 
{
	pmi->pGroupPerfDlg = NULL;

	CDialog::OnClose();

	DestroyWindow();
	delete this;
}

void CGroupPerfDlg::OnOk() 
{
}

void CGroupPerfDlg::OnCancel() 
{
}

BOOL CGroupPerfDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetTimer(1,100,0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGroupPerfDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CMemDC mdc(&dc);

	CRect rc; GetClientRect(rc);

	mdc.FillSolidRect(rc,0);

	for (int x=0; x<4; x++) for (int y=0; y<4; y++)
	{
		int g = y*4+x;

		float vu = 0.0f;
		for (int t=0; t<pmi->nTriggers; t++)
		{
			if (pmi->Tracks[t].Group == g) vu += pmi->Tracks[t].fVu;
		}

		vu = fabsf(vu) / 32768.0f; if (vu > 1.0f) vu = 1.0f;

		int c = (int)(vu * 255.0f);

		int w = rc.right / 4; int h = rc.bottom / 4;

		mdc.FillSolidRect(x*w,y*h,w-1,h-1,c*0x000010);

	}
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CGroupPerfDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CDialog::OnEraseBkgnd(pDC);

	return TRUE;
}

void CGroupPerfDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nFlags & (1 << 14)) == 0) //was up, now down
	{
		for (int t=0; t<pmi->nTriggers; t++)
		{
			if (pmi->Tracks[t].nTrigKeyType == ktVkey
				&& pmi->Tracks[t].nTrigKey == nChar)
			{
				STrigInfo ti;
				ti.vel = ti.pan = ti.rate = 1.0f;
				ti.delay = 0;
				ti.cut = -1;
				ti.offset = pmi->Tracks[t].fOffset;
				pmi->Tracks[t].TriggerOn(ti);
			}
		}

		for (int g=0; g<NUM_GROUPS; g++)
		{
			if (pmi->Groups[g].nMuteKeyType == ktVkey
				&& pmi->Groups[g].nMuteKey == nChar)
			{
				switch (pmi->Groups[g].nMuteMode)
				{
				case gmmToggle:
					pmi->Groups[g].bMuted = !pmi->Groups[g].bMuted;
					break;
				case gmmMomOff:
					pmi->Groups[g].bMuted = true;
					break;
				case gmmMomOn:
					pmi->Groups[g].bMuted = false;
					break;
				}
			}
		}
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CGroupPerfDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	for (int t=0; t<pmi->nTriggers; t++)
	{
		if (pmi->Tracks[t].nTrigKeyType == ktVkey
			&& pmi->Tracks[t].nTrigKey == nChar)
		{
			pmi->Tracks[t].TriggerOff();
		}
	}

	for (int g=0; g<NUM_GROUPS; g++)
	{
		if (pmi->Groups[g].nMuteKeyType == ktVkey
			&& pmi->Groups[g].nMuteKey == nChar)
		{
			switch (pmi->Groups[g].nMuteMode)
			{
			case gmmToggle:
				break;
			case gmmMomOff:
				pmi->Groups[g].bMuted = false;
				break;
			case gmmMomOn:
				pmi->Groups[g].bMuted = true;
				break;
			}
		}
	}

	
	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGroupPerfDlg::OnTimer(UINT nIDEvent) 
{
	RedrawWindow();
	
	CDialog::OnTimer(nIDEvent);
}
