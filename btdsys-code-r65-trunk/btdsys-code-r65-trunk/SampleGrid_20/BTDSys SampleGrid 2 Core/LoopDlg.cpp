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

// LoopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys samplegrid 2 core.h"
#include "LoopDlg.h"
#include "sgrid.h"
#include "WaveOffset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoopDlg dialog


CLoopDlg::CLoopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoopDlg::IDD, pParent)
{
	nWave = 0;

	//{{AFX_DATA_INIT(CLoopDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLoopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoopDlg)
	DDX_Control(pDX, IDC_DISPLAY, m_WO);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoopDlg, CDialog)
	//{{AFX_MSG_MAP(CLoopDlg)
	ON_WM_CLOSE()
	ON_COMMAND(IDOK, OnOk)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoopDlg message handlers

void CLoopDlg::OnClose() 
{
	pmi->pLoopDlg = NULL;

	CDialog::OnClose();

	DestroyWindow();
	delete this;
}

void CLoopDlg::OnOk() 
{
	OnClose();
}

void CLoopDlg::OnCancel() 
{
	OnClose();
}

BOOL CLoopDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CMenu *m = GetMenu()->GetSubMenu(0);
	if (m)
	{
		m->DeleteMenu(0,MF_BYPOSITION);
	

		for (int i=0; i<200; i++)
		{
			if (pmi->pCB->GetWave(i))
			{
				char txt[256];
				sprintf(txt,"%i: %s",i,pmi->pCB->GetWaveName(i));
				if (!nWave) nWave = i;
				m->AppendMenu(MF_STRING | MF_ENABLED | ((i == nWave) ? MF_CHECKED : 0),
					33000+i, txt);
			}
		}
	}

	if (nWave)
	{
		WaveChanged();
	}
	else
	{
		MessageBox("Error: no waves in wavetable");
	}

	CRect r; GetClientRect(r);
	m_WO.SetWindowPos(NULL,0,0,r.right,r.bottom,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLoopDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	m_WO.SetWindowPos(NULL,0,0,cx,cy,SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void CLoopDlg::WaveChanged()
{
	const CWaveInfo  *pwi = pmi->pCB->GetWave(nWave);
	const CWaveLevel *pwl = pmi->pCB->GetWaveLevel(nWave,0);
	if (pwi && pwl)
	{
		m_WO.pSamples = pwl->pSamples;
		m_WO.numSamples = pwl->numSamples;
		m_WO.bStereo = ((pwi->Flags & WF_STEREO) != 0);
	}
}
