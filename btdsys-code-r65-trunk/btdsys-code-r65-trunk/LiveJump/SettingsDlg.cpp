// SettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys MidiJumpHACK.h"
#include "SettingsDlg.h"
#include "midijump.h"
#include "qwertykeynames.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

const char *NoteNames[] = {
	"C-", "C#",
	"D-", "D#",
	"E-",
	"F-", "F#",
	"G-", "G#",
	"A-", "A#",
	"B-"
};

const char *JumpModesShort[] = {"Abs", "Back", "Fwd"};

inline char *GetMIDINoteName (const int n)
{
	int oct = (int)(n/12) - 1;
	int note = (n % 12);
	static char txt[16];
	sprintf(txt,"%s%i",NoteNames[note],oct);
	return txt;
}

CSettingsDlg::CSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsDlg)
	//}}AFX_DATA_INIT
}


void CSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDlg)
	DDX_Control(pDX, IDC_TICKEDIT, m_TickEdit);
	DDX_Control(pDX, IDC_NOTESTATIC, m_staticNote);
	DDX_Control(pDX, IDC_TICKSTATIC, m_staticTick);
	DDX_Control(pDX, IDC_KEYCAPBTN, m_btnKeyCap);
	DDX_Control(pDX, IDC_JUMPMODECOMBO, m_JumpMode);
	DDX_Control(pDX, IDC_TICKSPIN, m_TickSpin);
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CSettingsDlg)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_WM_KEYDOWN()
	ON_WM_PAINT()
	ON_WM_MEASUREITEM()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg message handlers

void CSettingsDlg::PopulateList()
{
	m_List.ResetContent();

	char txt[256];

	for (JumpTickRec *p = pmi->vJumpTicks.begin(); p<pmi->vJumpTicks.end(); p++)
	{
		switch (p->eTrigType)
		{
		case jttNote:
			{
				sprintf(txt,"Note %03i (%s) -> %i %s",
					p->nTrig,GetMIDINoteName(p->nTrig),p->nTick,JumpModesShort[p->nJumpMode]);
				int idx = m_List.AddString(txt);
				m_List.SetItemData(idx,p->nTrig);
			}
			break;
		case jttKey:
			{
				sprintf(txt,"Key %03i (%c) -> %i %s",
					p->nTrig,p->nTrig,p->nTick,JumpModesShort[p->nJumpMode]);
				int idx = m_List.AddString(txt);
				m_List.SetItemData(idx,p->nTrig);
			}
			break;
		case jttVKey:
			{
				sprintf(txt,"VKey %03i (%s) -> %i %s",
					p->nTrig,KeyNames(p->nTrig),p->nTick,JumpModesShort[p->nJumpMode]);
				int idx = m_List.AddString(txt);
				m_List.SetItemData(idx,p->nTrig);
			}
			break;
		}
	}
}

BOOL CSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	PopulateList();

	m_btnKeyCap.SetWindowText("Play a MIDI note, or\nclick here and press a key");
	nLastNote = -1;

	m_TickSpin.SetRange32(0,0x7FFFFFFF);

	m_JumpMode.AddString("Absolute");
	m_JumpMode.AddString("Relative back");
	m_JumpMode.AddString("Relative fwd");
	m_JumpMode.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSettingsDlg::OnRemove() 
{
	// TODO: Add your control notification handler code here
	
	int s = m_List.GetCurSel();
	if (s != LB_ERR)
	{
		char txt[256];
		m_List.GetText(s,txt);
		txt[4] = 0;

		int ndata = m_List.GetItemData(s);
		JumpTriggerType jtt;
		if (strcmp(txt,"Note") == 0) jtt = jttNote;
		if (strcmp(txt,"Key ") == 0) jtt = jttKey;
		if (strcmp(txt,"VKey") == 0) jtt = jttVKey;

		JumpTickRec *p = pmi->FindTickRec(jtt,ndata);
		if (p) pmi->vJumpTicks.erase(p);

		m_List.DeleteString(s);
	}
}

void CSettingsDlg::OnAdd() 
{
	// TODO: Add your control notification handler code here
	
//	int t = m_TickSpin.GetPos();
	char buf[256]; m_TickEdit.GetWindowText(buf,255);
	int t = atoi(buf);
//char txt[256]; sprintf(txt,"%i",t); MessageBox(txt);
	int m = m_JumpMode.GetCurSel();
	int trig = m_btnKeyCap.nKey;
	JumpTriggerType ttype;
	if (trig != -1)
	{
		if (trig & 0x10000)
		{
			trig -= 0x10000;
			ttype = jttVKey;
		}
		else
			ttype = jttKey;
	}
	else
	{
		trig = nLastNote;
		ttype = jttNote;
	}

	if (trig != -1)
	{
		JumpTickRec *p = pmi->FindTickRec(ttype,trig);
		if (p)
		{
			p->nTick = t;
			p->nJumpMode = m;
		}
		else
		{
			JumpTickRec jtr;
			jtr.eTrigType = ttype;
			jtr.nTrig = trig;
			jtr.nTick = t;
			jtr.nJumpMode = m;
			pmi->vJumpTicks.push_back(jtr);
		}

		PopulateList();
	}
}

void CSettingsDlg::SetNote(int n)
{
	nLastNote = n;
	char txt[256];
	sprintf(txt,"%03i (%s)",n,GetMIDINoteName(n));
	m_btnKeyCap.SetWindowText(txt);
	m_btnKeyCap.nKey = -1;
}

void CSettingsDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
/*	char txt[256];
	sprintf(txt,"%X %c %s",nChar,nChar,KeyNames[nChar]);
	m_LastNote.SetWindowText(txt);
*/	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CSettingsDlg::PreTranslateMessage(MSG* pMsg) 
{
//	if (pMsg->message == WM_CHAR && GetFocus() == &m_LastNote)
//		SendMessage(WM_CHAR,pMsg->wParam,pMsg->lParam);

	return CDialog::PreTranslateMessage(pMsg);
}

void DrawBitmap(CDC *dc, UINT rsrc, int x, int y, int w=0, int h=0, int xSrc=0, int ySrc=0)
{
	CDC bdc;
	bdc.CreateCompatibleDC(dc);
	CBitmap bmp;
	bmp.LoadBitmap(rsrc);
	if (w==0 || h==0)
	{
		BITMAP bm; bmp.GetBitmap(&bm);
		w = bm.bmWidth; h = bm.bmHeight;
	}
	CBitmap *oldbmp = bdc.SelectObject(&bmp);
	dc->BitBlt(x,y,w,h,&bdc,xSrc,ySrc,SRCCOPY);
}

void CSettingsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc; GetClientRect(rc);


	// Do not call CDialog::OnPaint() for painting messages
}

void CSettingsDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your message handler code here and/or call default

	if (nIDCtl == IDC_JUMPMODECOMBO)
	{
		lpMeasureItemStruct->itemHeight = 16;
	}
	else
		CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
}

void CSettingsDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDCtl == IDC_JUMPMODECOMBO)
	{



	}
	else
		CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
