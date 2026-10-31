// KeyCapEdit.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys MidiJumpHACK.h"
#include "KeyCapEdit.h"
#include "QWERTYkeynames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyCapEdit

CKeyCapEdit::CKeyCapEdit()
{
	nKey = -1;
}

CKeyCapEdit::~CKeyCapEdit()
{
}


BEGIN_MESSAGE_MAP(CKeyCapEdit, CEdit)
	//{{AFX_MSG_MAP(CKeyCapEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyCapEdit message handlers

void CKeyCapEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	char txt[256];
	sprintf(txt,"%03i %c",nChar,nChar);
	SetWindowText(txt);
	nKey = nChar;

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CKeyCapEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	char txt[256];
	sprintf(txt,"%03i %s",nChar,KeyNames(nChar));
	SetWindowText(txt);
	nKey = nChar | 0x10000;

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
