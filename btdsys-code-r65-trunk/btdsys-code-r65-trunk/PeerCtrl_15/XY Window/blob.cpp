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

#include "stdafx.h"
#include "blob.h"
#include "memdc.h"
#include "particle.h"

CXYBlob::CXYBlob()
{
	px = py = 0.0f;
	screenx = screeny = 0;
}

void CXYBlob::SetX(float fx)
{
	px = fx;
	screenx = xoff + (int)(parw * px);
}

void CXYBlob::SetY(float fy)
{
	py = fy;
	screeny = yoff + (int)(parh * py);
}

void CXYBlob::SetScreenPos(CPoint pos)
{
	screenx = pos.x;
	screeny = pos.y;
	px = (float)(screenx - xoff) / (float)parw;
	py = (float)(screeny - yoff) / (float)parh;
}

float CXYBlob::GetX()
{
	return px;
}

float CXYBlob::GetY()
{
	return py;
}

void CXYBlob::SetParentRect(CRect r)
{
	xoff = r.left;
	yoff = r.top;
	parw = r.Width();
	parh = r.Height();
	screenx = xoff + (int)(parw * px);
	screeny = yoff + (int)(parh * py);
}

void CXYBlob::SetBGColour(COLORREF c)
{
	bgcolour = c;
}

void CXYBlob::SetFGColour(COLORREF c)
{
	fgcolour = c;
}

//////////////////////////////////////////////////////////////////////

void CXYDotBlob::Update(CDC *pdc)
{
	CPen *oldpen = (CPen*)pdc->SelectStockObject(NULL_PEN);
	CBrush mybrush;
	mybrush.CreateSolidBrush(fgcolour);
	CBrush *oldbrush = pdc->SelectObject(&mybrush);

	pdc->Ellipse(screenx-2, screeny-2, screenx+2, screeny+2);

	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);
}

//////////////////////////////////////////////////////////////////////

void CXYCrosshairBlob::Update(CDC *pdc)
{
	CPen mypen(PS_SOLID, 1, fgcolour);
	CPen *oldpen = pdc->SelectObject(&mypen);

	pdc->MoveTo(screenx,	yoff);
	pdc->LineTo(screenx,	yoff + parh);
	pdc->MoveTo(xoff,		screeny);
	pdc->LineTo(xoff + parw,screeny);

	pdc->SelectObject(oldpen);
}

//////////////////////////////////////////////////////////////////////

CXYTrailBlob::CXYTrailBlob()
{
	for (int p=0; p<16; p++)
		oldpts[p] = CPoint(0,0);

	oldpts_head = 0;
}

void CXYTrailBlob::SetBGColour(COLORREF c)
{
	CXYBlob::SetBGColour(c);

	RespreadCols();
}

void CXYTrailBlob::SetFGColour(COLORREF c)
{
	CXYBlob::SetFGColour(c);

	RespreadCols();
}

void CXYTrailBlob::RespreadCols()
{
	int r1 = (fgcolour & 0xFF    );
	int g1 = (fgcolour & 0xFF00  ) >> 8;
	int b1 = (fgcolour & 0xFF0000) >> 16;

	int r2 = (bgcolour & 0xFF    );
	int g2 = (bgcolour & 0xFF00  ) >> 8;
	int b2 = (bgcolour & 0xFF0000) >> 16;

	for (int c=0; c<16; c++)
	{
		cols[c] = RGB(r1 + ((r2-r1) * c/16),
					  g1 + ((g2-g1) * c/16),
					  b1 + ((b2-b1) * c/16));
	}
}

void CXYTrailBlob::Update(CDC *pdc)
{
	CPen *oldpen = pdc->GetCurrentPen();

	pdc->MoveTo(screenx, screeny);

	int i = oldpts_head;
	for (int c=0; c<16; c++)
	{
		CPen mypen(PS_SOLID, 1, cols[c]);
		pdc->SelectObject(mypen);

		pdc->LineTo(oldpts[i]);
		i = (i+1) & 15;
		pdc->SelectObject(oldpen);
	}

	oldpts_head = (oldpts_head-1) & 15;
	oldpts[oldpts_head] = CPoint(screenx, screeny);

	pdc->SelectStockObject(NULL_PEN);
	CBrush mybrush;
	mybrush.CreateSolidBrush(fgcolour);
	CBrush *oldbrush = pdc->SelectObject(&mybrush);

	pdc->Ellipse(screenx-2, screeny-2, screenx+2, screeny+2);

	pdc->SelectObject(oldpen);
	pdc->SelectObject(oldbrush);

}

//////////////////////////////////////////////////////////////////////

void CXYParticleBlob::SetBGColour(COLORREF c)
{
	CXYBlob::SetBGColour(c);

	RespreadCols();
}

void CXYParticleBlob::SetFGColour(COLORREF c)
{
	CXYBlob::SetFGColour(c);

	RespreadCols();
}

void CXYParticleBlob::RespreadCols()
{
	int r1 = (fgcolour & 0xFF    );
	int g1 = (fgcolour & 0xFF00  ) >> 8;
	int b1 = (fgcolour & 0xFF0000) >> 16;

	int r2 = (bgcolour & 0xFF    );
	int g2 = (bgcolour & 0xFF00  ) >> 8;
	int b2 = (bgcolour & 0xFF0000) >> 16;

	for (int c=0; c<16; c++)
	{
		cols[c] = RGB(r1 + ((r2-r1) * c/16),
					  g1 + ((g2-g1) * c/16),
					  b1 + ((b2-b1) * c/16));
	}
}

void CXYParticleBlob::Update(CDC *pdc)
{
	strm.LastPoint = CPoint(screenx, screeny);
	strm.Update();

	for (int i=0; i<64; i++)
	{
		pdc->SetPixelV((int)strm.Particles[i].x, (int)strm.Particles[i].y,
						cols[15 - strm.Particles[i].strength]);
	}
}

