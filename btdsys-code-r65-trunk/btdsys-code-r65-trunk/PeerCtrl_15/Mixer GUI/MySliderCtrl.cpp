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

// MySliderCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "MySliderCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMySliderCtrl

CMySliderCtrl::CMySliderCtrl()
{
	m_crThumbColor = GetSysColor(COLOR_3DFACE);
}

CMySliderCtrl::~CMySliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CMySliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CMySliderCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySliderCtrl message handlers

void CMySliderCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult) 
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
					*pResult = CDRF_DODEFAULT;
					break;
				case TBCD_THUMB:
					DrawThumb(pDC, lpcd);
					*pResult = CDRF_SKIPDEFAULT;
				break;
				case TBCD_CHANNEL:
					*pResult = CDRF_DODEFAULT;
					break;
			}
			break;
		}
	}
}

void CMySliderCtrl::SetThumbColour(COLORREF colour)
{
	m_crThumbColor = colour;

	Invalidate();			// \ 
	EnableWindow(FALSE);	//  > redraw
	EnableWindow(TRUE);		// / 
}

COLORREF CMySliderCtrl::GetThumbColour()
{
	return m_crThumbColor;
}

void CMySliderCtrl::DrawThumb(CDC *pDC, LPNMCUSTOMDRAW lpcd)
{
	CRect crect;
	GetThumbRect(&crect);
	COLORREF col = m_crThumbColor;

	CBrush myBrush; myBrush.CreateSolidBrush(col);
//	HBRUSH hbrush = CreateSolidBrush(col);
//  HBRUSH hbOld = (HBRUSH)SelectObject(pDC->m_hDC, hbrush);
	pDC->FillRect(&crect, &myBrush);
/*	COLORREF hicol = GetSysColor(COLOR_3DHILIGHT);
	COLORREF locol = GetSysColor(COLOR_3DDKSHADOW);
	if (lpcd->uItemState & CDIS_SELECTED)
	{
		COLORREF t = locol; //swap
		locol = hicol;
		hicol = t;
	}
*/
	pDC->DrawEdge(&crect,
		(lpcd->uItemState & CDIS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED, 
		BF_RECT | BF_ADJUST);

/*	pDC->Draw3dRect(&crect, hicol, locol);
	crect.InflateRect(-1,-1,-1,-1);
	pDC->Draw3dRect(&crect, hicol, locol);
*/
//    Ellipse(pDC->m_hDC, crect.left, crect.top, crect.right, crect.bottom);
//    SelectObject(pDC->m_hDC, hbOld);
//    DeleteObject(hbrush);
}

void CMySliderCtrl::SetPos(int nPos)
{
	CSliderCtrl::SetPos(GetRangeMax() - nPos);
}

int CMySliderCtrl::GetPos()
{
	return GetRangeMax() - CSliderCtrl::GetPos();
}

BOOL CMySliderCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CSliderCtrl::OnMouseWheel(nFlags, zDelta, pt);

	int nNewPos = GetPos()+(zDelta * GetLineSize() / WHEEL_DELTA);
	SetPos(nNewPos);
	RedrawWindow();

	CWnd* pWnd = GetParent();
	if(pWnd) pWnd->PostMessage(WM_VSCROLL, (WPARAM)((GetPos() << 16) | SB_THUMBPOSITION), (LPARAM)GetSafeHwnd());

	return TRUE;
}
