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
// PrefsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XYPlug.h"
#include "XY Window.h"
#include "PrefsDialog.h"
#include "Blob.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrefsDialog dialog


CPrefsDialog::CPrefsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPrefsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrefsDialog)
	//}}AFX_DATA_INIT
}


void CPrefsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrefsDialog)
	DDX_Control(pDX, IDC_FONTBTN, m_FontBtn);
	DDX_Control(pDX, IDC_RIGHTCOLOUR, m_RColour);
	DDX_Control(pDX, IDC_MIDDLECOLOUR, m_MColour);
	DDX_Control(pDX, IDC_LEFTCOLOUR, m_LColour);
	DDX_Control(pDX, IDC_BLOBSTYLECOMBO, m_BlobStyle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrefsDialog, CDialog)
	//{{AFX_MSG_MAP(CPrefsDialog)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_BLOBSTYLECOMBO, OnBlobstylecomboSelchange)
	ON_BN_CLICKED(IDC_BGCOLOUR, OnBgcolour)
	ON_BN_CLICKED(IDC_FONTBTN, OnFontbtn)
	ON_BN_CLICKED(IDC_SAVEDEFBTN, OnSavedefbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrefsDialog message handlers

BOOL CPrefsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_bTextFontIsValid = false;

	m_LColour.fgcolour = pparent->colL;  
	m_LColour.bgcolour = pparent->colBG; 

	m_MColour.fgcolour = pparent->colM;  
	m_MColour.bgcolour = pparent->colBG; 

	m_RColour.fgcolour = pparent->colR;  
	m_RColour.bgcolour = pparent->colBG; 

	m_BlobStyle.AddString("None");
	m_BlobStyle.AddString("Dot");
	m_BlobStyle.AddString("Crosshair");
	m_BlobStyle.AddString("Trail");
	m_BlobStyle.AddString("Particle Stream");
	m_BlobStyle.SetCurSel(pparent->nBlobStyle);
	OnBlobstylecomboSelchange();

	previewphase = 0.0f;

	SetTimer(1, 33, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

#define PI 3.1415926535897932384626433832795f

void CPrefsDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default


	CRect r;

	m_LColour.SetPos(previewphase);
	m_MColour.SetPos(previewphase + PI * 2.0f / 3.0f);
	m_RColour.SetPos(previewphase + PI * 4.0f / 3.0f);

	m_LColour.RedrawWindow();
	m_MColour.RedrawWindow();
	m_RColour.RedrawWindow();

	previewphase += 0.2f;
	while (previewphase > 2.0f * PI) previewphase -= 2.0f * PI;

	CDialog::OnTimer(nIDEvent);

}

void CPrefsDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

/*	m_LColour.RedrawWindow();
	m_MColour.RedrawWindow();
	m_RColour.RedrawWindow();
*/
	// Do not call CDialog::OnPaint() for painting messages
}

void CPrefsDialog::OnBlobstylecomboSelchange() 
{
	// TODO: Add your control notification handler code here
/*
	None");
	Dot");
	Crosshair");
	Trail");
	Particle Stream");
*/	

	if (m_LColour.m_blob) delete m_LColour.m_blob;
	if (m_MColour.m_blob) delete m_MColour.m_blob;
	if (m_RColour.m_blob) delete m_RColour.m_blob;
	m_LColour.m_blob = m_MColour.m_blob = m_RColour.m_blob = NULL;

	switch (m_BlobStyle.GetCurSel())
	{
	case 0:
		m_LColour.m_blob = new CXYNoBlob;
		m_MColour.m_blob = new CXYNoBlob;
		m_RColour.m_blob = new CXYNoBlob;
		break;

	case 1:
		m_LColour.m_blob = new CXYDotBlob;
		m_MColour.m_blob = new CXYDotBlob;
		m_RColour.m_blob = new CXYDotBlob;
		break;

	case 2:
		m_LColour.m_blob = new CXYCrosshairBlob;
		m_MColour.m_blob = new CXYCrosshairBlob;
		m_RColour.m_blob = new CXYCrosshairBlob;
		break;

	case 3:
		m_LColour.m_blob = new CXYTrailBlob;
		m_MColour.m_blob = new CXYTrailBlob;
		m_RColour.m_blob = new CXYTrailBlob;
		break;

	case 4:
		m_LColour.m_blob = new CXYParticleBlob;
		m_MColour.m_blob = new CXYParticleBlob;
		m_RColour.m_blob = new CXYParticleBlob;
		break;
	}

	m_LColour.ReinitBlob();
	m_MColour.ReinitBlob();
	m_RColour.ReinitBlob();

}

void CPrefsDialog::OnBgcolour() 
{
	// TODO: Add your control notification handler code here


	static COLORREF customs[16];

	CHOOSECOLOR cc;
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = NULL;
	cc.rgbResult = m_LColour.bgcolour;
	cc.lpCustColors = customs;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		m_LColour.bgcolour = m_MColour.bgcolour = m_RColour.bgcolour 
			= cc.rgbResult;
		if (m_LColour.m_blob) 
			m_LColour.m_blob->SetBGColour(m_LColour.bgcolour);
		if (m_MColour.m_blob) 
			m_MColour.m_blob->SetBGColour(m_MColour.bgcolour);
		if (m_RColour.m_blob) 
			m_RColour.m_blob->SetBGColour(m_RColour.bgcolour);
	}

}

void CPrefsDialog::OnFontbtn() 
{
	// TODO: Add your control notification handler code here

	CHOOSEFONT cf;

	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = NULL;
	cf.lpLogFont = &m_TextFont;
	cf.Flags = CF_FORCEFONTEXIST | 
				(m_bTextFontIsValid ? CF_INITTOLOGFONTSTRUCT : 0) |
				CF_SCREENFONTS;

	if (ChooseFont(&cf))
	{
		char txt[256];
		m_bTextFontIsValid = true;
		sprintf(txt,"&Font for text display... (%s %ipt)",
			m_TextFont.lfFaceName, 
			m_TextFont.lfHeight);
		m_FontBtn.SetWindowText(txt);
	}

}

void CPrefsDialog::OnOK() 
{
	// TODO: Add extra validation here
	

	pparent->nBlobStyle = m_BlobStyle.GetCurSel();
	pparent->colL = m_LColour.fgcolour;
	pparent->colM = m_MColour.fgcolour;
	pparent->colR = m_RColour.fgcolour;
	pparent->colBG = m_LColour.bgcolour;

	CDialog::OnOK();

}

