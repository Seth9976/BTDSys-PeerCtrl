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

// KeyCapBtn.cpp : implementation file
//

#include "stdafx.h"
//#include "BTDSys MidiJumpHACK.h"
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

const char *NoteNames[] = {
	"C-", "C#",
	"D-", "D#",
	"E-",
	"F-", "F#",
	"G-", "G#",
	"A-", "A#",
	"B-"
};

inline char *GetMIDINoteName (const int n)
{
	int oct = (int)(n/12) - 1;
	int note = (n % 12);
	static char txt[16];
	sprintf(txt,"%s%i",NoteNames[note],oct);
	return txt;
}


void CKeyCapBtn::SetKey(int k, EKeyType t, bool notify)
{
	char txt[256];
	switch (t)
	{
	case ktNone:
		strcpy(txt,"Please click here and press\na key or MIDI note");
		break;

	case ktChar:
		if (k >= 32)
			sprintf(txt,"%03i (ASCII) = '%c'",k,k);
		else
			sprintf(txt,"%03i (ASCII) = %s",k,KeyNames(k));
		break;

	case ktVkey:
		sprintf(txt,"%03i (VKey) = %s",k,KeyNames(k));
		break;

	case ktMidi:
		sprintf(txt,"%03i (MIDI), chan %03i\n%s",k & 0xFF, k >> 8, GetMIDINoteName(k & 0xFF));
		break;

	default:
		strcpy(txt,"WTF?");
	}

	SetWindowText(txt);

	nKey = k;
	nKeyType = t;

	if (notify)
		GetParent()->SendMessage(WM_COMMAND,MAKELONG(GetDlgCtrlID(),BN_CLICKED),(LPARAM)m_hWnd);
}

void CKeyCapBtn::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
/*	char txt[256];
	if (nChar >= 32)
		sprintf(txt,"%03i (ASCII) = '%c'",nChar,nChar);
	else
		sprintf(txt,"%03i (ASCII) = %s",nChar,KeyNames(nChar));
	SetWindowText(txt);
	nKey = nChar;
*/
//	SetKey(nChar,ktChar);

	CButton::OnChar(nChar, nRepCnt, nFlags);
}

void CKeyCapBtn::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
/*	char txt[256];
	sprintf(txt,"%03i (VKey) = %s",nChar,KeyNames(nChar));
	SetWindowText(txt);
	nKey = nChar | 0x10000;
*/
	SetKey(nChar,ktVkey);
	CButton::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKeyCapBtn::OnMidiKey(int chan, int key)
{
/*	char txt[256];
	sprintf(txt,"chan %i key %i",chan,key);
	SetWindowText(txt);
	RedrawWindow();
*/

	SetKey(key | (chan << 8), ktMidi);
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
