/*
Copyright (c) 2004-2006, Edward Powley
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

// OscilloscopeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys Pulsar.h"
#include "OscilloscopeDlg.h"
#include "pulsar.h"
#include "memdc.h"
#include "hermite.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COscilloscopeDlg dialog


COscilloscopeDlg::COscilloscopeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COscilloscopeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COscilloscopeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	bKillMe = false;
}


void COscilloscopeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COscilloscopeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COscilloscopeDlg, CDialog)
	//{{AFX_MSG_MAP(COscilloscopeDlg)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COscilloscopeDlg message handlers

void COscilloscopeDlg::OnClose() 
{
	bKillMe = true;
	KillMe();
}

void COscilloscopeDlg::KillMe()
{
	ptr->pOscDlg = NULL;
	KillTimer(1);
	CDialog::OnClose();
	delete this;
}

void DrawLine(int x, int y, int lasty, COLORREF col, int w, int h, COLORREF *scrn)
{
	if (y< 0) y=0;
	if (y>=h) y=h-1;
	if (lasty< 0) lasty=0;
	if (lasty>=h) lasty=h-1;
	scrn[lasty*w+x-1] = scrn[y*w+x] = col;

	if (lasty > y)
	{
		for (int cy=y; cy<lasty; cy++)
		{
			float f = (float)(cy-y)/(float)(lasty-y);
			if (f>0.5f)
				scrn[cy*w+x-1] = col;
			else
				scrn[cy*w+x  ] = col;
		}
	}
	else if (lasty < y)
	{
		for (int cy=lasty; cy<y; cy++)
		{
			float f = (float)(cy-lasty)/(float)(y-lasty);
			if (f<0.5f)
				scrn[cy*w+x-1] = col;
			else
				scrn[cy*w+x  ] = col;
		}
	}
}

void COscilloscopeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rc; GetClientRect(rc);

	CDC bdc;
	bdc.CreateCompatibleDC(&dc);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc,rc.right,rc.bottom);
	CBitmap *oldbmp = bdc.SelectObject(&bmp);
	bdc.FillSolidRect(rc,pmi->colVisBG);

	if (!vfWaveData.empty())
	{
		CPen *oldpen = bdc.GetCurrentPen();

		int x;
		float fm = -(float)rc.bottom / 65536.0f;
		float fc = (float)rc.bottom * 0.5f;


		float fStep = fLen / fLenD;
		if (fStep < 1.0f)
		{
			int rx = rc.right * fStep;
/*			COLORREF *clr = new COLORREF[rc.right-rx];
			for (x=0; x<rc.right-rx; x++)
			{
				if (rx) clr[x] = 0xCC + 0x33*(x%rx)/(rx); else clr[x] = 0xCC;
				clr[x] *= 0x10101;
			}
*/
			for (x=rx; x<rc.right; x++)
			{
				COLORREF c;
				float f = (float)x / ((float)rc.right * fStep);
				f -= (int)f;
				float omf = 1.0f-f;
				float r = (pmi->colVisBG & 0x0000FF) * f + (pmi->colVisOverlap & 0x0000FF) * omf;
				float g = (pmi->colVisBG & 0x00FF00) * f + (pmi->colVisOverlap & 0x00FF00) * omf;
				float b = (pmi->colVisBG & 0xFF0000) * f + (pmi->colVisOverlap & 0xFF0000) * omf;
				c = ((int)r & 0x0000FF) | ((int)g & 0x00FF00) | ((int)b & 0xFF0000);
//				if (rx) c = 0xCC + 0x33*((x-rx)%rx)/(rx); else c = 0xCC;
				CPen pen(PS_SOLID,1,c);
				bdc.SelectObject(pen);
				bdc.MoveTo(x,0);
				bdc.LineTo(x,rc.bottom);
				bdc.SelectObject(oldpen);
			}

/*			for (int y=0; y<rc.bottom; y++)
			{
				int yw = y * rc.right;
				for (x=rx; x<rc.right; x++)
				{
					scrn[yw+x] = clr[x-rx];
				}
			}

			delete[] clr;
/*			CRect r = rc;
			r.left = r.right * fStep;
			bdc.FillSolidRect(r,0xCCCCCC);
*/			fStep = 1.0f;
		}
		fStep *= (float)vfWaveData.size() / (float)rc.right;
		float fMult = fEnd-fStart;
		float fAdd = fStart * (float)vfWaveData.size();

		int ly1=0, ly2=0, ly3=0;
		int y1,y2,y3;

		CPen blackpen(PS_SOLID,1,pmi->colVisEnvWave);
		CPen bluepen (PS_SOLID,1,pmi->colVisWave);
		CPen redpen  (PS_SOLID,1,pmi->colVisEnv);

		for (x=0; x<rc.right; x++)
		{
			float fw = 0.0f;
			if (x*fStep < vfWaveData.size()) 
				fw = GetSampleM(x*fStep*fMult+fAdd,vfWaveData.begin(),vfWaveData.size());

			float fe = GetSampleM(x*fStep/(float)vfWaveData.size()*(float)ENVTABLE_LENGTH+2.0f, pfEnvelope, ENVTABLE_LENGTH+4);

			y1 = fm*fw+fc;
			y2 = fm*fe*fw+fc;
			y3 = fm*fe*32768.0f+fc;

			if (x)
			{
				bdc.SelectObject(bluepen);  bdc.MoveTo(x-1,ly1); bdc.LineTo(x,y1);
				bdc.SelectObject(blackpen); bdc.MoveTo(x-1,ly2); bdc.LineTo(x,y2);
				bdc.SelectObject(redpen);   bdc.MoveTo(x-1,ly3); bdc.LineTo(x,y3);

/*				DrawLine(x,y1,ly1,0x0000FF,rc.right,rc.bottom,scrn);
				DrawLine(x,y2,ly2,0x000000,rc.right,rc.bottom,scrn);
				DrawLine(x,y3,ly3,0xFF0000,rc.right,rc.bottom,scrn);
*/			}
			ly1=y1; ly2=y2; ly3=y3;
/*
			bdc.SetPixelV(x,fm*fw+fc,0xFF0000);
			bdc.SetPixelV(x,fm*fe*fw+fc,0);
			bdc.SetPixelV(x,fm*fe*32768.0f+fc,0xFF);
*/
		}

		bdc.SelectObject(oldpen);

	}

//	bmp.SetBitmapBits(rc.right*rc.bottom*4,scrn);

//	CDC bdc;
//	bdc.CreateDC("DISPLAY",NULL,NULL,NULL);//CompatibleDC(&dc);
//	if (!bdc.SelectObject(&bmp)) MessageBox("Shit!");

	dc.BitBlt(0,0,rc.right,rc.bottom,&bdc,0,0,SRCCOPY);

	bdc.SelectObject(oldbmp);

//	delete[] scrn;
}

BOOL COscilloscopeDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

BOOL COscilloscopeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	bDontRedrawMe = false;

	SetTimer(1,100,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COscilloscopeDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	RedrawWindow();
	
	// TODO: Add your message handler code here
	
}

void COscilloscopeDlg::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		RedrawWindow();
	}

	CDialog::OnTimer(nIDEvent);
}
