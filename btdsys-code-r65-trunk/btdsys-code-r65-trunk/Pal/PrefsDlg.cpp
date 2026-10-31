/*
PrefsDlg.cpp: Preferences dialog
Copyright (c) 2005, Edward Powley
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
#include "BTDSys PAL.h"
#include "PrefsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefsDlg dialog


CPrefsDlg::CPrefsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrefsDlg)
	//}}AFX_DATA_INIT
}


void CPrefsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefsDlg)
	DDX_Control(pDX, IDC_UPDATEFREQ, m_cmbUpdateFreq);
	DDX_Control(pDX, IDC_PAINTFREQ, m_cmbPaintFreq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefsDlg, CDialog)
	//{{AFX_MSG_MAP(CPrefsDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefsDlg message handlers

BOOL CPrefsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cmbPaintFreq.AddString("On mouse movement");
	m_cmbPaintFreq.AddString("0.1 secs");
	m_cmbPaintFreq.AddString("1 sec");
	m_cmbPaintFreq.AddString("2 secs");
	m_cmbPaintFreq.AddString("5 secs");
	m_cmbPaintFreq.AddString("10 secs");

	switch (pw->nRepaintInt)
	{
	case -1:	m_cmbPaintFreq.SetCurSel(0); break;
	case 100:	m_cmbPaintFreq.SetCurSel(1); break;
	case 1000:	m_cmbPaintFreq.SetCurSel(2); break;
	case 2000:	m_cmbPaintFreq.SetCurSel(3); break;
	case 5000:	m_cmbPaintFreq.SetCurSel(4); break;
	case 10000:	m_cmbPaintFreq.SetCurSel(5); break;
	default:	m_cmbPaintFreq.SetCurSel(0); break;
	}

	m_cmbUpdateFreq.AddString("Never");
	m_cmbUpdateFreq.AddString("0.1 secs");
	m_cmbUpdateFreq.AddString("1 sec");
	m_cmbUpdateFreq.AddString("2 secs");
	m_cmbUpdateFreq.AddString("5 secs");
	m_cmbUpdateFreq.AddString("10 secs");

	switch (pw->nUpdInt)
	{
	case -1:	m_cmbUpdateFreq.SetCurSel(0); break;
	case 100:	m_cmbUpdateFreq.SetCurSel(1); break;
	case 1000:	m_cmbUpdateFreq.SetCurSel(2); break;
	case 2000:	m_cmbUpdateFreq.SetCurSel(3); break;
	case 5000:	m_cmbUpdateFreq.SetCurSel(4); break;
	case 10000:	m_cmbUpdateFreq.SetCurSel(5); break;
	default:	m_cmbUpdateFreq.SetCurSel(0); break;
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CPrefsDlg::OnOK() 
{
	switch (m_cmbPaintFreq.GetCurSel())
	{
	case 0:	pw->PaintInt(-1);		break;
	case 1:	pw->PaintInt(100);		break;
	case 2:	pw->PaintInt(1000);		break;
	case 3:	pw->PaintInt(2000);		break;
	case 4:	pw->PaintInt(5000);		break;
	case 5:	pw->PaintInt(10000);	break;
	}

	switch (m_cmbUpdateFreq.GetCurSel())
	{
	case 0:	pw->UpdInt(-1);		break;
	case 1:	pw->UpdInt(100);	break;
	case 2:	pw->UpdInt(1000);	break;
	case 3:	pw->UpdInt(2000);	break;
	case 4:	pw->UpdInt(5000);	break;
	case 5:	pw->UpdInt(10000);	break;
	}

	CDialog::OnOK();
}
