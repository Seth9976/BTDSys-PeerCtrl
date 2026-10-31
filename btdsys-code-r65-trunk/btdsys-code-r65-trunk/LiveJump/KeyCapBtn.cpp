// KeyCapBtn.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys MidiJumpHACK.h"
#include "KeyCapBtn.h"
#include "qwertykeynames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyCapBtn

CKeyCapBtn::CKeyCapBtn()
{
}

CKeyCapBtn::~CKeyCapBtn()
{
}


BEGIN_MESSAGE_MAP(CKeyCapBtn, CButton)
	//{{AFX_MSG_MAP(CKeyCapBtn)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyCapBtn message handlers

void CKeyCapBtn::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	char txt[256];
	if (nChar >= 32)
		sprintf(txt,"%03i (ASCII) = '%c'",nChar,nChar);
	else
		sprintf(txt,"%03i (ASCII) = %s",nChar,KeyNames(nChar));
	SetWindowText(txt);
	nKey = nChar;

	CButton::OnChar(nChar, nRepCnt, nFlags);
}

void CKeyCapBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	char txt[256];
	sprintf(txt,"%03i (VKey) = %s",nChar,KeyNames(nChar));
	SetWindowText(txt);
	nKey = nChar | 0x10000;
	
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

UINT CKeyCapBtn::OnGetDlgCode() 
{
	// TODO: Add your message handler code here and/or call default
	
	return DLGC_WANTALLKEYS;
}
/*
void CKeyCapBtn::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

/*	CRect rc; GetClientRect(rc);

	CPen blackpen(PS_SOLID,1,(COLORREF)0);
	CPen *oldpen = dc.SelectObject(&blackpen);
	dc.FillSolidRect(rc,0xFFFFFF);
	dc.Rectangle(rc);

	dc.SelectObject(oldpen);
	
	// Do not call CButton::OnPaint() for painting messages
}

*/
void CKeyCapBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	dc.SetBkColor(0xFFFFFF);
	CRect rc = lpDrawItemStruct->rcItem;

/*	CPen blackpen(PS_SOLID,1,(COLORREF)0);
	CPen *oldpen = dc.SelectObject(&blackpen);
	CBrush bkbrush;
	if (lpDrawItemStruct->itemState & ODS_FOCUS)
		bkbrush.CreateSolidBrush(RGB(195,255,13));
	else
		bkbrush.CreateSolidBrush(0xFFFFFF);

	CBrush *oldbsh = dc.SelectObject(&bkbrush);

	dc.Rectangle(rc);
*/

	if (lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		dc.FillSolidRect(rc, GetSysColor(COLOR_3DHILIGHT));
		dc.DrawEdge(rc,EDGE_SUNKEN,BF_RECT);
	}
	else
	{
		dc.FillSolidRect(rc, GetSysColor(COLOR_3DFACE));
		dc.DrawEdge(rc,EDGE_RAISED,BF_RECT);
	}

/*	LOGFONT lfont;
	lfont.lfHeight = -11;
	lfont.lfWidth = 0;
	lfont.lfEscapement = 0;
	lfont.lfOrientation = 0;
	lfont.lfWeight = FW_BOLD;
	lfont.lfItalic = 0;
	lfont.lfUnderline = 0;
	lfont.lfStrikeOut = 0;
	lfont.lfCharSet = ANSI_CHARSET;
	lfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lfont.lfQuality = ANTIALIASED_QUALITY;
	lfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	lstrcpy(lfont.lfFaceName,"Arial");

	CFont fnt; fnt.CreateFontIndirect(&lfont);
	CFont *oldfnt = dc.SelectObject(&fnt);
*/	dc.SetTextColor(0);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextAlign(TA_TOP | TA_CENTER);

	CString txt, tx2;
	GetWindowText(txt);
	bool twolines = false;
	
	for (int i=0; i<txt.GetLength() && !twolines; i++)
	{
		if (txt[i] == '\n')
		{
			twolines = true;
			tx2 = txt.Mid(i+1);
			txt = txt.Left(i);
		}
	}

	if (!twolines)
		dc.TextOut((rc.left+rc.right)/2, (rc.top+rc.bottom)/2-6, txt);
	else
	{
		dc.TextOut((rc.left+rc.right)/2, rc.top+2, txt);
		dc.TextOut((rc.left+rc.right)/2, (rc.top+rc.bottom)/2+2, tx2);
	}


//	dc.SelectObject(oldpen);
//	dc.SelectObject(oldbsh);
//	dc.SelectObject(oldfnt);

	dc.Detach();
}

BOOL CKeyCapBtn::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}
