#include "stdafx.h"
#include "antialiasedtextout.h"

void AntiAliasedTextOut(CDC *dc, int size, int x, int y, char *text)
{
	char dbg[256];
	sprintf(dbg,"AntiAliasedTextOut %i %i",x,y);
	OutputDebugString(dbg);

	CFont fnt;
	fnt.CreatePointFont(size,"Arial");
	CFont *oldfont = dc->SelectObject(&fnt);

	CSize sz = dc->GetTextExtent(text);

	sprintf(dbg,"x %i y %i",sz.cx,sz.cy);
	OutputDebugString(dbg);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(dc,sz.cx,sz.cy);
	CBitmap *oldbmp = dc->SelectObject(&bmp);

	dc->SetTextColor(0xFF);
	dc->SetBkColor(0xFFFFFF);

	dc->TextOut(0,0,text);

	for (int x2=0; x2<sz.cx; x2++)
		for (int y2=0; y2<sz.cy; y2++)
			dc->SetPixelV(x2,y2,x2*y2);

	COLORREF *pSrc = new COLORREF[sz.cx*sz.cy];
	bmp.GetBitmapBits(sz.cx*sz.cy*4,pSrc);

	dc->SelectObject(oldbmp);

	CRect bmsz; dc->GetClipBox(bmsz);

	COLORREF *pDst = new COLORREF[bmsz.Width(),bmsz.Height()];
	oldbmp->GetBitmapBits(bmsz.Width()*bmsz.Height()*4,pDst);

	for (int ys=0; ys<sz.cy; ys++)
	{
		int yd = y + ys;
		for (int xs=0; xs<sz.cx; xs++)
		{
			int xd = x + xs;
			int i = ys*sz.cx + xs;
			int j = yd*bmsz.Width() + xd;
			pDst[j] = pSrc[i];
		}
	}

	oldbmp->SetBitmapBits(bmsz.Width()*bmsz.Height()*4,pDst);

	dc->SelectObject(oldfont);

	OutputDebugString("End AntiAliasedTextOut");
}
