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

// Implementation file for the snazzy Matrix-style about dialog
// Props to mute and Keanu Reeves

#include "stdafx.h"
#include "btdsys samplegrid 2 core.h"
#include "sgrid.h"
#include "AboutDlg.h"
#include "memdc.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char *pAboutStrings[] = {
		"BTDSys SampleGrid 2.0 BETA",
		"©2002-2005 Ed Powley",
		"http://btdsys.cjb.net",
		"Original concept by Jeph Wacheski",
		"Multi-out system based on code by Polac",
		"Envelope code and editor by Fuzzpilz",
		NULL

	};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

CAboutDlg::~CAboutDlg()
{
//	delete[] chararray;
//	delete[] colarray;
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CHAR()
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rc; GetClientRect(rc);
	nx = rc.right / 15 + 1;
	ny = rc.bottom / 15 + 1;
//	chararray = new char[nx*ny];
//	colarray = new char[nx*ny];

	for (int t=0; t<100; t++)
	{
		trails[t].len = 0;
	}

	fnt.CreatePointFont(200,"Arial");
	nAboutString = 0;
	nAbtStringWait = 0;
	nEasterEggCount = 0;

	SetTimer(1,50,0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnPaint() 
{
	CPaintDC _dc(this); // device context for painting
	CMemDC dc(&_dc);
//	CPaintDC dc;

	CRect rc; GetClientRect(rc);
	dc.FillSolidRect(rc,0);

	if (nEasterEggCount == 8)
	{

		return;
	}

	dc.SetTextColor(0x008000);
	dc.SetTextAlign(TA_LEFT | TA_TOP);
	dc.SetBkMode(TRANSPARENT);

	for (int x=0; x<nx; x++) for (int y=0; y<ny; y++)
		dc.TextOut(x*15,y*15,".");

	for (int t=0; t<100; t++)
	{
		int y = trails[t].y;
		for (int i=0; i<trails[t].len && y >= 0; i++, y--)
		{
			COLORREF c;
			if (i==0)			c = 0xFFFFFF;
			else if (i==1)		c = 0xAAFFAA;
			else if (i==2)		c = 0x55FF55;
			else if (i>=trails[t].len-10)	c = ((trails[t].len-i)*255/10)*0x000100;
			else c = 0x00FF00;

			dc.SetTextColor(c);
			char tx[2] = {trails[t].chars[(i-trails[t].y+10*trails[t].len)%trails[t].len], 0};
			dc.TextOut(trails[t].x * 15, y*15, tx);
		}
	}	

	CFont *oldfont = dc.SelectObject(&fnt);
	dc.SetTextAlign(TA_BASELINE | TA_CENTER);
	COLORREF c = nAbtStringWait - 50;
	c = 2500 - c*c; c /= 10;
	c = (c * 0x100) + (c>>1) * (0x10001);

	dc.SetTextColor(c);

/*	for (int i=0; i<20; i++)
	{

		int x = (int)( (frand() - 0.5f) 
			* ((float)nAbtStringWait - 50.0f) / 50.0f * (float)rc.right );

		int y = (int)(frand() * 10.0f - 5.0f);

		dc.TextOut(rc.right/2+x,rc.bottom/2+y,pAboutStrings[nAboutString]);
	}

	dc.SetTextColor(0);
*/

	dc.TextOut(rc.right/2,rc.bottom/2,pAboutStrings[nAboutString]);


	dc.SelectObject(oldfont);

}

void CAboutDlg::OnTimer(UINT nIDEvent) 
{
	for (int t=0; t<100; t++)
	{
		if (trails[t].len)
		{
			trails[t].y ++;
			if (trails[t].y - trails[t].len > ny)
				trails[t].len = 0;
		}
		else if (frand() > 0.98f)
		{
			trails[t].len = (int)(frand() * 30.0f + 10.0f);
			trails[t].x = (int)(frand() * (float)nx);
			trails[t].y = -1;
			for (int c=0; c<trails[t].len; c++)
			{
				float f=frand();
				if (f < 0.45f) trails[t].chars[c] = '0';
				else if (f < 0.9f) trails[t].chars[c] = '1';
				else trails[t].chars[c] = '.';
			}
			trails[t].chars[trails[t].len] = 0;
		}
	}

	nAbtStringWait ++;
	if (nAbtStringWait >= 100)
	{
		nAbtStringWait = 0;
		nAboutString ++;
		if (pAboutStrings[nAboutString] == NULL) nAboutString = 0;
	}

	RedrawWindow();
	
	CDialog::OnTimer(nIDEvent);
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 
{
//	return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CDialog::OnOK();
}

void CAboutDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

UINT CAboutDlg::OnGetDlgCode() 
{
//	return CDialog::OnGetDlgCode();
	return DLGC_WANTALLKEYS;

}

void CAboutDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == 27) { CDialog::OnCancel(); return; }

	switch (nEasterEggCount)
	{
	case 0: if (nChar == 'F') nEasterEggCount++; else nEasterEggCount=0; break;
	case 1: if (nChar == 'M') nEasterEggCount++; else nEasterEggCount=0; break;
	case 2: if (nChar == 'P') nEasterEggCount++; else nEasterEggCount=0; break;
	case 3: if (nChar == 'L') nEasterEggCount++; else nEasterEggCount=0; break;
	case 4: if (nChar == 'I') nEasterEggCount++; else nEasterEggCount=0; break;
	case 5: if (nChar == 'B') nEasterEggCount++; else nEasterEggCount=0; break;
	case 6: if (nChar == 'B') nEasterEggCount++; else nEasterEggCount=0; break;
	case 7: 
		if (nChar == 'S')
		{
			nEasterEggCount++;



			return;
		}
		else
			nEasterEggCount=0;
		break;
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
