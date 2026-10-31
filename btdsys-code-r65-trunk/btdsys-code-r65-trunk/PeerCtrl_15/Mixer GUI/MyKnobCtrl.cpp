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

// MyKnobCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MyKnobCtrl.h"

#include "memdc.h"

#include <float.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyKnobCtrl

const float Pi = 3.1415926535897932384626433832795f;
const float Pi_5by4 = Pi * 5.0f / 4.0f;
const float Pi_3by2 = Pi * 3.0f / 2.0f;

CMyKnobCtrl::CMyKnobCtrl()
{
	m_crKnobColor = GetSysColor(COLOR_3DFACE);
	m_iTicRadius = 7;
	m_iTicCount = 10;
	m_bDragging = false;
	m_nMouseMode = mm_rotwrap;
}

CMyKnobCtrl::~CMyKnobCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyKnobCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CMyKnobCtrl)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyKnobCtrl message handlers

BOOL CMyKnobCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	Init();

	return CSliderCtrl::PreCreateWindow(cs);
}

void CMyKnobCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	Init();

	CSliderCtrl::PreSubclassWindow();
}

void CMyKnobCtrl::Init()
{
	if (IsWindow(m_hWnd))
	{
		CRect rc;
		GetClientRect(rc);

		// Resize the window to make it square
		rc.bottom = rc.right = min(rc.bottom, rc.right);

		m_crect = rc;

		// Set the window region so mouse clicks only activate the round section 
		// of the slider
		m_rgn.DeleteObject(); 
		SetWindowRgn(NULL, FALSE);
	//	m_rgn.CreateEllipticRgnIndirect(rc);
		m_rgn.CreateRectRgnIndirect(rc);
		SetWindowRgn(m_rgn, TRUE);

		VerifyPos();
	}
}

void CMyKnobCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CSliderCtrl::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	Init();
}

/*void CMyKnobCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;
	CDC *pDC = CDC::FromHandle(lpcd->hdc);
	DWORD dwStyle = this->GetStyle();
	switch(lpcd->dwDrawStage)
	{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
		break;
		case CDDS_ITEMPREPAINT:
		{
			switch(lpcd->dwItemSpec)
			{
				case TBCD_TICS:
					DrawTics(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				case TBCD_THUMB:
					DrawLine(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					break;
				case TBCD_CHANNEL:
					DrawKnob(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
					break;
			}
			break;
		}
	}
}
*/

void CMyKnobCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	dc.SetBkColor(GetSysColor(COLOR_3DFACE));
	CMemDC pDC(&dc);
	
	// Draw (clear) the background
/*	CRect rc;
	GetClientRect(rc);
	dc.SelectStockObject(NULL_BRUSH);
	dc.SelectStockObject(NULL_PEN);
	dc.FillSolidRect(rc, ::GetSysColor(COLOR_BTNFACE));
*/
	DrawKnob(pDC);
	DrawTics(pDC);
	DrawLine(pDC);

	if (GetFocus() == this)
		pDC->DrawFocusRect(m_crect);

	// Do not call CSliderCtrl::OnPaint() for painting messages
}

void CMyKnobCtrl::DrawTics(CDC *pDC)
{
	float AngleStep = -Pi_3by2 / m_iTicCount;

	CRect crect = m_crect;
	
	int r1 = crect.Width() / 2;
	int r2 = r1 - m_iTicRadius + 2;

	CPen myPen; myPen.CreateStockObject(BLACK_PEN);
	CPen *oldpen = pDC->SelectObject(&myPen);

	float Angle = Pi_5by4;

	for (int i=0; i<=m_iTicCount; i++)
	{
		float c = cosf(Angle);
		float s = -sinf(Angle);
		pDC->MoveTo(crect.CenterPoint() + CPoint((int)(c*r2), (int)(s*r2)));
		pDC->LineTo(crect.CenterPoint() + CPoint((int)(c*r1), (int)(s*r1)));
		Angle += AngleStep;
	}

	pDC->SelectObject(oldpen);
}

void CMyKnobCtrl::DrawKnob(CDC *pDC)
{
	CRect crect = m_crect;
	crect.DeflateRect(m_iTicRadius, m_iTicRadius);

	CBrush myBrush; myBrush.CreateSolidBrush(m_crKnobColor);
	CBrush *oldbrush = pDC->SelectObject(&myBrush);
	CPen NullPen; NullPen.CreateStockObject(NULL_PEN);
	CPen *oldpen = pDC->SelectObject(&NullPen);

	pDC->Ellipse(&crect);

	CPen Pen1(PS_SOLID, 1, GetSysColor(COLOR_3DHILIGHT));
	CPen Pen2(PS_SOLID, 1, GetSysColor(COLOR_3DDKSHADOW));
	CPen Pen3(PS_SOLID, 1, GetSysColor(COLOR_3DLIGHT));
	CPen Pen4(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));
	CPoint TopRight		(crect.top,		crect.right);
	CPoint BottomLeft	(crect.bottom,	crect.left);

	pDC->SelectObject(&Pen2);
	pDC->Arc(crect, TopRight, BottomLeft);
	pDC->SelectObject(&Pen1);
	pDC->Arc(crect, BottomLeft, TopRight);
	crect.DeflateRect(1,1);
	pDC->SelectObject(&Pen4);
	pDC->Arc(crect, TopRight, BottomLeft);
	pDC->SelectObject(&Pen3);
	pDC->Arc(crect, BottomLeft, TopRight);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CMyKnobCtrl::DrawLine(CDC *pDC)
{
	float value = (float)(GetPos() - GetRangeMin()) / (float)(GetRangeMax() - GetRangeMin());
	float angle = Pi_5by4 - Pi_3by2 * value;
	float c = cosf(angle);
	float s = -sinf(angle);

	CRect crect = m_crect;
	crect.DeflateRect(m_iTicRadius, m_iTicRadius);

	int r1 = crect.Width() / 2;
	int r2 = r1 / 2;

	CPen myPen; myPen.CreateStockObject(BLACK_PEN);
	CPen *oldpen = pDC->SelectObject(&myPen);
	pDC->MoveTo(crect.CenterPoint() + CPoint((int)(c*r2), (int)(s*r2)));
	pDC->LineTo(crect.CenterPoint() + CPoint((int)(c*r1), (int)(s*r1)));

	pDC->SelectObject(oldpen);
}

void CMyKnobCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(!m_bDragging)
	{
		m_bDragging = true;
		m_bDragChanged = false;
		m_nLastValue = GetPos();
		
		SetCapture();
		SetFocus();
		if(SetKnob(point))
		{
			m_bDragChanged = true;
			PostMessageToParent(TB_THUMBTRACK);
		}
		RedrawWindow();
	}
	else
	{
		CSliderCtrl::OnLButtonDown(nFlags, point);
	}
}

void CMyKnobCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_bDragging)
	{
		if(SetKnob(point))
		{
			m_bDragChanged = true;
			PostMessageToParent(TB_THUMBTRACK);
			RedrawWindow();
		}
	}
	else
	{
		CSliderCtrl::OnMouseMove(nFlags, point);
	}
}

void CMyKnobCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_bDragging)
	{
		m_bDragging = false;
		::ReleaseCapture();
		if(SetKnob(point))
		{
			PostMessageToParent(TB_THUMBTRACK);
			m_bDragChanged = true;
		}
		if(m_bDragChanged)
		{
			PostMessageToParent(TB_THUMBPOSITION);
			m_bDragChanged = false;
		}
		RedrawWindow();
	}
	else
	{
		CSliderCtrl::OnLButtonUp(nFlags, point);
	}
}

bool CMyKnobCtrl::SetKnob(const CPoint &pt)
{
	CRect crect = m_crect;
	CPoint c = pt - crect.CenterPoint();

	switch (m_nMouseMode)
	{
	case mm_rotwrap:
	case mm_rotnowrap:
		{
			float Angle;

			if (c.x == 0)
			{
				if (c.y > 0)
					return false;
				else
					Angle = Pi/2.0f;
			}
			else
			{
				Angle = atanf(-(float)c.y / (float)c.x);

				if (c.x < 0)
					Angle += Pi;
			}

			static bool minlast = false;
			static bool maxlast = false;

			float value = (Pi_5by4 - Angle) / Pi_3by2;

			if (m_nMouseMode == mm_rotnowrap && (
				(minlast && value > 0.5f) || (maxlast && value < 0.5f) ))
				return false;

			minlast = maxlast = false;

			if (value < 0.0f)
			{
				value = 0.0f;
				minlast = true;
			}
			if (value > 1.0f)
			{
				value = 1.0f;
				maxlast = true;
			}

			int NewValue = (int)(value * (GetRangeMax() - GetRangeMin())) + GetRangeMin();
//			m_nLastValue = NewValue;
			if (NewValue != GetPos())
			{
				SetPos(NewValue);
				return true;
			}
			else
				return false;
		}
		break;

	case mm_linvert:
		c.x = -c.y;
	case mm_linhoriz:
		{
			int NewValue = m_nLastValue 
				+ (c.x * (GetRangeMax() - GetRangeMin()) / m_nLinearSize);

//			m_nLastValue = NewValue;
			if (NewValue != GetPos())
			{
				SetPos(NewValue);
				return true;
			}
			else
				return false;
		}
		break;

	default:
		return false;
	}
}



void CMyKnobCtrl::PostMessageToParent(const int nTBCode) const
{
	CWnd* pWnd = GetParent();
	if(pWnd) pWnd->PostMessage(WM_HSCROLL, (WPARAM)((GetPos() << 16) | nTBCode), (LPARAM)GetSafeHwnd());
}

BOOL CMyKnobCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;	
//	return CSliderCtrl::OnEraseBkgnd(pDC);
}

void CMyKnobCtrl::SetKnobColour(COLORREF colour)
{
	m_crKnobColor = colour;

	Invalidate();			// \ 
	EnableWindow(FALSE);	//  > redraw
	EnableWindow(TRUE);		// / 
}

COLORREF CMyKnobCtrl::GetKnobColour()
{
	return m_crKnobColor;
}

void CMyKnobCtrl::SetTicRadius(int radius)
{
	m_iTicRadius = radius;

	Invalidate();			// \ 
	EnableWindow(FALSE);	//  > redraw
	EnableWindow(TRUE);		// / 
}

void CMyKnobCtrl::SetNumTics(int numtics)
{
	m_iTicCount = numtics;

	Invalidate();			// \ 
	EnableWindow(FALSE);	//  > redraw
	EnableWindow(TRUE);		// / 
}

void CMyKnobCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch(nChar)
	{
	case VK_LEFT:
	case VK_DOWN:
		{
			int nNewPos = GetPos()-GetLineSize();
//			while(nNewPos < nMin) nNewPos += (nMax - nMin);
			SetPos(nNewPos);
			RedrawWindow();
			PostMessageToParent(TB_LINEUP);
		}
		break;
	
	case VK_RIGHT:
	case VK_UP:
		{
			int nNewPos = GetPos()+GetLineSize();
//			while(nNewPos >= nMax) nNewPos -= (nMax - nMin);
			SetPos(nNewPos);
			RedrawWindow();
			PostMessageToParent(TB_LINEDOWN);
		}
		break;

	case VK_PRIOR:
		{
			int nNewPos = GetPos()+GetPageSize();
//			while(nNewPos < nMin) nNewPos += (nMax - nMin);
			SetPos(nNewPos);
			RedrawWindow();
			PostMessageToParent(TB_PAGEUP);
		}
		break;

	case VK_NEXT:
		{
			int nNewPos = GetPos()-GetPageSize();
//			while(nNewPos >= nMax) nNewPos -= (nMax - nMin);
			SetPos(nNewPos);
			RedrawWindow();
			PostMessageToParent(TB_PAGEDOWN);
		}
		break;

	case VK_HOME:
		SetPos(GetRangeMin());
		RedrawWindow();
//		PostMessageToParent(TB_HOME);
		break;

	case VK_END:
		SetPos(GetRangeMax());
		RedrawWindow();
//		PostMessageToParent(TB_END);
		break;

	default:
		CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

BOOL CMyKnobCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default

	int nNewPos = GetPos()+(zDelta * GetLineSize() / WHEEL_DELTA);
	SetPos(nNewPos);
	RedrawWindow();

/*	BOOL res = CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);

	RedrawWindow();
	return res;
*/

	PostMessageToParent(SB_THUMBPOSITION);

	return TRUE;
}

void CMyKnobCtrl::SetPos(int nPos)
{
	CSliderCtrl::SetPos(nPos);

	RedrawWindow();
}

void CMyKnobCtrl::SetMouseMode(int nMode)
{
	m_nMouseMode = nMode;
}

void CMyKnobCtrl::SetLinearSize(int nSize)
{
	m_nLinearSize = nSize;
}
