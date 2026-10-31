// KeyCapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys MidiJumpHACK.h"
#include "KeyCapDlg.h"
#include "midijump.h"
#include "memdc.h"
#include "buzzhack.h"

#include "AntiAliasedTextOut.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyCapDlg dialog


CKeyCapDlg::CKeyCapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyCapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyCapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKeyCapDlg::~CKeyCapDlg()
{
}

void CKeyCapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyCapDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyCapDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyCapDlg)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyCapDlg message handlers

void CKeyCapDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	JumpTickRec *p = pmi->FindTickRec(jttKey, nChar);
	if (p)
	{
		pmi->nNextDest = p->nTick;
		pmi->nNextDestMode = p->nJumpMode;
		pmi->bHadNoteOff = true;
	}
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CKeyCapDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	JumpTickRec *p = pmi->FindTickRec(jttVKey, nChar);
	if (p)
	{
		pmi->nNextDest = p->nTick;
		pmi->nNextDestMode = p->nJumpMode;
		pmi->bHadNoteOff = true;
	}
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CKeyCapDlg::OnClose() 
{
	pmi->pKeyCapDlg = NULL;
	CDialog::OnClose();
	delete this;
}

BOOL CKeyCapDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CKeyCapDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	dc.SetBkColor(0);

	CMemDC mdc(&dc);

	CRect cr; GetClientRect(cr);

	CFont fnt;
	fnt.CreatePointFont(-cr.bottom * 3, "Courier New");
	CFont *oldfont = mdc.SelectObject(&fnt);

	mdc.SetTextColor(RGB(0,255,0));
	mdc.SetBkMode(TRANSPARENT);
	mdc.SetTextAlign(TA_TOP | TA_LEFT);

	int xy = cr.bottom / 10;

	char txt1[32], txt2[32];
	if (!pmi->bOn)
	{
		strcpy(txt1,"machine");
		strcpy(txt2,"is off");
	}
	else if (pmi->nNextDest == -1)
	{
		if (pmi->pCB->GetStateFlags() & SF_PLAYING)
		{
			strcpy(txt1,"ready");
			strcpy(txt2,"to rok");
		}
		else
		{
			strcpy(txt1,"press");
			strcpy(txt2,"play");
		}
	}
	else
	{
		int current = pmi->getSongPosition();
		int jumptick = ((current + pmi->nTickSnap - 1) / pmi->nTickSnap) * pmi->nTickSnap;
		int dest;
		switch (pmi->nNextDestMode)
		{
		case 1: //rel back
			dest = jumptick - pmi->nNextDest;
			break;
		case 2: //rel fwd
			dest = jumptick + pmi->nNextDest;
			break;
		case 0: //abs
			dest = pmi->nNextDest;
			break;
		}
		sprintf(txt1,"goto %i",dest);
		sprintf(txt2,"in %i",jumptick - current);
	}
	mdc.TextOut(xy,xy,txt1);
//	AntiAliasedTextOut(&dc,-cr.bottom*3,xy,xy,txt1);
//	AntiAliasedTextOut(&dc,-cr.bottom*3,xy,cr.bottom/2,txt2);
	mdc.TextOut(xy,cr.bottom / 2,txt2);

	mdc.SelectObject(oldfont);

	// Do not call CDialog::OnPaint() for painting messages
}

void CKeyCapDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Invalidate();	
}
