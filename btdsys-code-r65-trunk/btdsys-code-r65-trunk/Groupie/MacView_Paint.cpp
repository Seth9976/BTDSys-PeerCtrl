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

#include "stdafx.h"
#include "groupie.h"
#include "MacView.h"
#include "BuzzMI.h"
#include "MacViewFrame.h"
#include "AddMachineDlg.h"

void CMacView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	dc.SetBkColor(m_pFrame->pmi->colBack);
	CMemDC pDC(&dc);

	CRect r;
	GetClientRect(&r);

	CBrush backBrush, genBrush, effBrush, masterBrush, selBrush;
	backBrush.CreateSolidBrush(m_pFrame->pmi->colBack);
	genBrush.CreateSolidBrush(m_pFrame->pmi->colGen);
	effBrush.CreateSolidBrush(m_pFrame->pmi->colEff);
	masterBrush.CreateSolidBrush(m_pFrame->pmi->colMaster);
	selBrush.CreateSolidBrush(RGB(255,255,255));

	CBrush *pOldBrush = pDC->GetCurrentBrush();
	CPen *pOldPen = (CPen *)pDC->SelectStockObject(BLACK_PEN);

//	pDC->FillRect(r,&backBrush);

	pDC->SetTextColor(0);
	pDC->SetBkColor(m_pFrame->pmi->colBack);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextAlign(TA_CENTER + TA_TOP);

	CFont *pOldFont = pDC->GetCurrentFont();
	CFont smallfont; smallfont.CreatePointFont(80,"Arial");

	// Calculate rectangles
	for (vector<CMachine*>::iterator pp = m_pFrame->pmi->vpMacs.begin(); 
		 pp < m_pFrame->pmi->vpMacs.end(); pp++)
	{
		CMachine *p = *pp;

		float x = p->fX * 0.5f + 0.5f;
		float y = p->fY * 0.5f + 0.5f;

		p->rect.left = p->rect.right = (int)(x * (float)r.Width());
		p->rect.top = p->rect.bottom = (int)(y * (float)r.Height());
		p->rect.InflateRect(m_pFrame->pmi->nMacWidth/2,m_pFrame->pmi->nMacHeight/2);
	}

	// Draw connections
	pDC->SelectStockObject(WHITE_BRUSH);

	for (vector<CConnection>::iterator p = m_pFrame->pmi->vConns.begin();
		 p < m_pFrame->pmi->vConns.end(); p++)
	{
		CPoint p1,p2,pt[3];
		float x3,y3,x4,y4;

		p1 = p->pMac1->rect.CenterPoint();
		p2 = p->pMac2->rect.CenterPoint();

		pDC->MoveTo(p1); pDC->LineTo(p2);

		//arrow
		x3 = p2.x - p1.x;
		y3 = p2.y - p1.y;
		x4 = sqrtf(x3*x3 + y3*y3);
		x3 /= x4;
		y3 /= x4;

		x4 = (p1.x + p2.x) * .5f;
		y4 = (p1.y + p2.y) * .5f;
		pt[0].x = (int)(x4 + 10.0f*x3);
		pt[0].y = (int)(y4 + 10.0f*y3);
		x4 -= 10.0f*x3;
		y4 -= 10.0f*y3;
		pt[1].x = (int)(x4 + 10.0f*y3);
		pt[1].y = (int)(y4 - 10.0f*x3);
		pt[2].x = (int)(x4 - 10.0f*y3);
		pt[2].y = (int)(y4 + 10.0f*x3);

		pDC->Polygon(pt,3);
		p->rect.left = p->rect.right = pt[0].x;
		p->rect.top = p->rect.bottom = pt[0].y;
		if (pt[1].x < p->rect.left  ) p->rect.left   = pt[1].x;
		if (pt[1].x > p->rect.right ) p->rect.right  = pt[1].x;
		if (pt[1].y < p->rect.top   ) p->rect.top    = pt[1].y;
		if (pt[1].y > p->rect.bottom) p->rect.bottom = pt[1].y;
		if (pt[2].x < p->rect.left  ) p->rect.left   = pt[2].x;
		if (pt[2].x > p->rect.right ) p->rect.right  = pt[2].x;
		if (pt[2].y < p->rect.top   ) p->rect.top    = pt[2].y;
		if (pt[2].y > p->rect.bottom) p->rect.bottom = pt[2].y;
	}

	// Draw rectangles
	for (pp = m_pFrame->pmi->vpMacs.begin(); 
		 pp < m_pFrame->pmi->vpMacs.end(); pp++)
	{
		CMachine *p = *pp;

		if (p->bSelected)
		{
			CRect r2 = p->rect;
			r2.InflateRect(2,2);
			pDC->FillRect(r2,&selBrush);

			pDC->SetTextColor(0xFFFFFF);
		}
		else
			pDC->SetTextColor(0);

		if (p->bSpecial)
			pDC->SelectObject(&masterBrush);
		else if (p->bIsEffect)
			pDC->SelectObject(&effBrush);
		else
			pDC->SelectObject(&genBrush);

		pDC->Rectangle(p->rect);
		pDC->SelectObject(pOldFont);
		pDC->ExtTextOut(p->rect.CenterPoint().x, p->rect.CenterPoint().y - 10,
			ETO_CLIPPED, p->rect, p->name, strlen(p->name), NULL);
		pDC->SelectObject(&smallfont);
		pDC->ExtTextOut(p->rect.CenterPoint().x, p->rect.CenterPoint().y + 6,
			ETO_CLIPPED, p->rect, p->dllname, strlen(p->dllname), NULL);
	}

	// Connection in progress
	if (m_nDragMode == conn)
	{
		pDC->MoveTo(m_pDragConnMac->rect.CenterPoint());
		pDC->LineTo(m_ptDrag);
	}

	if (m_nDragMode == connvol)
	{
		CRect r2(m_ptDrag,m_ptDrag);
		r2.InflateRect(
			(int)((     m_fDragOrigPan) * 0.5f * (float)AMPPANBOX_W) + 10,
			(int)((1.0f-m_fDragOrigAmp) * 1.0f * (float)AMPPANBOX_H) + 10,
			(int)((2.0f-m_fDragOrigPan) * 0.5f * (float)AMPPANBOX_W) + 10,
			(int)((     m_fDragOrigAmp) * 1.0f * (float)AMPPANBOX_H) + 10
		);

		pDC->SelectObject(&masterBrush);
		pDC->Rectangle(r2);

		r2.DeflateRect(10,10);

		int x = (int)( (float)r2.Width() * 0.5f * m_pContextMenuTarget.c->fPan );
		int y = (int)( (float)r2.Height()       * m_pContextMenuTarget.c->fAmp );

		r2.left += x; r2.right = r2.left;
		r2.bottom -= y; r2.top = r2.bottom;
		r2.InflateRect(10,10);
		pDC->SelectStockObject(BLACK_BRUSH);
		pDC->Rectangle(r2);
	}


	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldFont);

}
