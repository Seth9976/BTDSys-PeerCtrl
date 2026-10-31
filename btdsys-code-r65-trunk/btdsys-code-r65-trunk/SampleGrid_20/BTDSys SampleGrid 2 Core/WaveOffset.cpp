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

// WaveOffset.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys samplegrid 2 core.h"
#include "WaveOffset.h"
#include <math.h>
#include "memdc.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWaveOffset

CWaveOffset::CWaveOffset()
{
	pSamples = NULL;
	fMin = 0.25f;
	fMax = 0.75f;
	fViewMin = 0.0f;
	fViewMax = 1.0f;
	fMMBDragCentre = -1.0f;
	nEndPtDrag = 0;
}

CWaveOffset::~CWaveOffset()
{
}


BEGIN_MESSAGE_MAP(CWaveOffset, CButton)
	//{{AFX_MSG_MAP(CWaveOffset)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWaveOffset message handlers

void CWaveOffset::OnPaint() 
{
	CPaintDC _dc(this); // device context for painting
	CRect rc; GetClientRect(rc);
//	CPaintDC dc(this); // device context for painting
	CMemDC dc(&_dc,&rc);
	
	dc.FillSolidRect(rc, 0);

	if (pSamples)
	{
		int x1 = f2i(fMin, rc.right);
		if (x1 < 0) x1 = 0;
		if (x1 < rc.right)
		{
			int x2 = f2i(fMax, rc.right);
			if (x2 >= rc.right) x2 = rc.right;
			if (x2 >= 0)
			{
				dc.FillSolidRect(x1,0,x2-x1,rc.bottom,0x555555);
			}
		}

		CPen greeny(PS_SOLID,1,RGB(0,255,0));
		CPen *oldpen = dc.SelectObject(&greeny);

		dc.SelectStockObject(WHITE_PEN);
		dc.MoveTo(0,rc.bottom/2);
		dc.LineTo(rc.right,rc.bottom/2);
		dc.SelectObject(&greeny);

		float step = (fViewMax - fViewMin) / (float)rc.right;
		for (int x=0; x<rc.right; x++)
		{
			float s1 = fViewMin + step*(float)x;
			float s2 = s1+step;
			int is1 = (int)(s1 * (float)numSamples);
			int is2 = (int)(s2 * (float)numSamples);
			int ymin=65536, ymax=-65536;
			for (int i=is1; i<=is2; i++)
			{
				if (i<numSamples)
				{
					int z;

					if (bStereo) 
						z = (pSamples[i*2] + pSamples[i*2+1]) / 2;
					else 
						z = pSamples[i];

					if (z < ymin) ymin = z;
					if (z > ymax) ymax = z;
				}
			}

			ymin = 0x8000 - ymin; ymin = ymin * rc.bottom / 0x10000;
			ymax = 0x8000 - ymax; ymax = ymax * rc.bottom / 0x10000;

			dc.MoveTo(x,ymax);
			dc.LineTo(x,ymin+1);
		}

		dc.SelectObject(oldpen);

	}
	else
	{
		dc.SetTextColor(RGB(0,255,0));
		dc.SetTextAlign(TA_CENTER | TA_BASELINE);
		dc.TextOut(rc.right/2, rc.bottom/2 + 5, "No Data");
	}
}

void CWaveOffset::OnMouseMove(UINT nFlags, CPoint pt) 
{
	if (GetForegroundWindow()->m_hWnd == GetParent()->m_hWnd)
	{
//		SetFocus();
		
		CRect r; GetClientRect(r);

		if (fMMBDragCentre > -0.5f)
		{
			float width = fViewMax - fViewMin;
			fViewMin = fMMBDragCentre - ((float)pt.x / (float)r.right) * width;
			fViewMax = fViewMin + width;
			if (fViewMin < 0.0f)
			{
				fViewMax += -fViewMin;
				fViewMin = 0.0f;
			}
			if (fViewMax > 1.0f)
			{
				fViewMin -= (fViewMax - 1.0f);
				fViewMax = 1.0f;
			}
			RedrawWindow();
		}

		if (nEndPtDrag)
		{
			float f = i2f(pt.x,r.right);
			if (nEndPtDrag == 1)
			{
				fMin = f;
				if (fMax < fMin)
				{
					nEndPtDrag = 2;
					fMin = fMax;
					fMax = f;
				}
			}
			else
			{
				fMax = f;
				if (fMax < fMin)
				{
					nEndPtDrag = 1;
					fMax = fMin;
					fMin = f;
				}
			}

			RedrawWindow();
		}
	}


	CButton::OnMouseMove(nFlags, pt);
}

BOOL CWaveOffset::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	CRect r; GetClientRect(r);

	ScreenToClient(&pt);

	if (!r.PtInRect(pt)) return CButton::OnMouseWheel(nFlags, zDelta, pt);

	float factor = (zDelta > 0) ? 0.75f : 1.33333f;
	float centre = i2f(pt.x, r.right);
	char txt[256];
	sprintf(txt,"%f",centre);
	OutputDebugString(txt);

	float width = (fViewMax - fViewMin) * factor;

	fViewMin = centre - ((float)pt.x / (float)r.right) * width;
	fViewMax = fViewMin + width;

	if (fViewMin < 0.0f) fViewMin = 0.0f;
	if (fViewMax > 1.0f) fViewMax = 1.0f;

	RedrawWindow();
	
	return CButton::OnMouseWheel(nFlags, zDelta, pt);
}

void CWaveOffset::OnLButtonDown(UINT nFlags, CPoint point) 
{
	float f = i2f(point.x);
	fMin = fMax = f;
	nEndPtDrag = 2;
	SetCapture();
	RedrawWindow();
	
	CButton::OnLButtonDown(nFlags, point);
}

void CWaveOffset::OnLButtonUp(UINT nFlags, CPoint point) 
{
	nEndPtDrag = 0;
	ReleaseCapture();

	RedrawWindow();
	GetParent()->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),EN_CHANGE),(LPARAM)m_hWnd);
	
	CButton::OnLButtonUp(nFlags, point);
}

void CWaveOffset::OnMButtonDown(UINT nFlags, CPoint point) 
{
	fMMBDragCentre = i2f(point.x);
	SetCapture();
	RedrawWindow();

	CButton::OnMButtonDown(nFlags, point);
}

void CWaveOffset::OnMButtonUp(UINT nFlags, CPoint point) 
{
	fMMBDragCentre = -1.0f;
	ReleaseCapture();
	RedrawWindow();
	
	CButton::OnMButtonUp(nFlags, point);
}

void CWaveOffset::OnRButtonDown(UINT nFlags, CPoint point) 
{
	float f = i2f(point.x);
	if (fabsf(f-fMax) < fabsf(f-fMin))
		nEndPtDrag = 2;
	else
		nEndPtDrag = 1;

	SetCapture();

	RedrawWindow();
	
	CButton::OnRButtonDown(nFlags, point);
}

void CWaveOffset::OnRButtonUp(UINT nFlags, CPoint point) 
{
	nEndPtDrag = 0;
	ReleaseCapture();
	RedrawWindow();

	GetParent()->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),EN_CHANGE),(LPARAM)m_hWnd);
	
	CButton::OnRButtonUp(nFlags, point);
}

float CWaveOffset::i2f(int x, int rr)
{
	if (rr == -1)
	{
		CRect r; GetClientRect(r);
		rr = r.right;
	}

	x = clamp(x,0,rr-1);

	return fViewMin + ((float)x / (float)rr) * (fViewMax - fViewMin);
}

int CWaveOffset::f2i(float f, int rr)
{
	if (rr == -1)
	{
		CRect r; GetClientRect(r);
		rr = r.right;
	}

	return (int)((f - fViewMin) / (fViewMax - fViewMin) * (float)rr);
}

BOOL CWaveOffset::OnEraseBkgnd(CDC* pDC) 
{	
//	return CButton::OnEraseBkgnd(pDC);
	return TRUE;
}
