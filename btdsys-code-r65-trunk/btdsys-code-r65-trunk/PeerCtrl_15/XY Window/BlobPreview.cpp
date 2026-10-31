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
// BlobPreview.cpp : implementation file
//

#include "stdafx.h"
#include "XY Window.h"
#include "BlobPreview.h"
#include "memdc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlobPreview

CBlobPreview::CBlobPreview()
{
	m_blob = NULL;

	bgcolour = 0;
	fgcolour = 0xFFFFFF;
}

CBlobPreview::~CBlobPreview()
{
	if (m_blob)
		delete m_blob;
}


BEGIN_MESSAGE_MAP(CBlobPreview, CStatic)
	//{{AFX_MSG_MAP(CBlobPreview)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlobPreview message handlers

void CBlobPreview::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	dc.SetBkColor(bgcolour);
	CRect r;
	GetClientRect(r);
	CMemDC pDC(&dc, &r);

	if (m_blob)
		m_blob->Update(pDC);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void CBlobPreview::ReinitBlob()
{
	if (m_blob)
	{
		CRect r;
		GetClientRect(r);
		m_blob->SetParentRect(r);
		m_blob->SetBGColour(bgcolour);
		m_blob->SetFGColour(fgcolour);
	}
}

void CBlobPreview::SetPos(float phase)
{
	if (m_blob)
	{
		m_blob->SetX(0.5f + sinf(phase) * 0.3f);
		m_blob->SetY(0.5f + cosf(phase) * 0.3f);
	}
}

BOOL CBlobPreview::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return TRUE;
//	return CStatic::OnEraseBkgnd(pDC);
}

void CBlobPreview::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	static COLORREF customs[16];

	CHOOSECOLOR cc;
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = NULL;
	cc.rgbResult = fgcolour;
	cc.lpCustColors = customs;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;

	if (ChooseColor(&cc))
	{
		fgcolour = cc.rgbResult;
		if (m_blob) m_blob->SetFGColour(fgcolour);
	}

	CStatic::OnLButtonUp(nFlags, point);
}
