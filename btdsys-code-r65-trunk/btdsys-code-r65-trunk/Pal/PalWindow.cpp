/*
PalWindow.cpp: The main GUI window
Copyright (c) 2005, Edward Powley
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
#include "BTDSys PAL.h"
#include "PalWindow.h"
#include "../mdk/machineinterface.h"
#include "CMachine.h"
#include "memdc.h"

#include "prefsdlg.h"

#include <float.h>
#include <math.h>
#include <vector>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);

static PSLWA pSetLayeredWindowAttributes = NULL;
static BOOL initialized = FALSE;

#define WS_EX_LAYERED           0x00080000    

#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002

BOOL MakeWindowTransparent(HWND hWnd, unsigned char factor, bool transp)
{
   /* First, see if we can get the API call we need. If we've tried
    * once, we don't need to try again. */
   if (!initialized)
   {
      HMODULE hDLL = LoadLibrary ("user32");

      pSetLayeredWindowAttributes = 
         (PSLWA) GetProcAddress(hDLL, "SetLayeredWindowAttributes");

      initialized = TRUE;
   }

   if (pSetLayeredWindowAttributes == NULL) 
      return FALSE;

   /* Windows need to be layered to be made transparent. This is done
    * by modifying the extended style bits to contain WS_EX_LAYARED. */
   SetLastError(0);

   if (transp)
	   SetWindowLong(hWnd, 
					 GWL_EXSTYLE , 
					 GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
   else
	   SetWindowLong(hWnd, 
					 GWL_EXSTYLE , 
					 GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

   if (GetLastError())
      return FALSE;

   /* Now, we need to set the 'layered window attributes'. This
    * is where the alpha values get set. */
   if (transp)
	   return pSetLayeredWindowAttributes (hWnd, 
										   RGB(255,0,255), 
										   factor,
										   LWA_COLORKEY|LWA_ALPHA);
   else
	   return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CPalWindow dialog


CPalWindow::CPalWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CPalWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPalWindow)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPalWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPalWindow)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPalWindow, CDialog)
	//{{AFX_MSG_MAP(CPalWindow)
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_COMMAND(ID_UPDATING_UPDATENOW, OnUpdatingUpdatenow)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_TRANSPARENCY_100OFF, OnTransparency100off)
	ON_COMMAND(ID_TRANSPARENCY_10, OnTransparency10)
	ON_COMMAND(ID_TRANSPARENCY_20, OnTransparency20)
	ON_COMMAND(ID_TRANSPARENCY_30, OnTransparency30)
	ON_COMMAND(ID_TRANSPARENCY_40, OnTransparency40)
	ON_COMMAND(ID_TRANSPARENCY_50, OnTransparency50)
	ON_COMMAND(ID_TRANSPARENCY_60, OnTransparency60)
	ON_COMMAND(ID_TRANSPARENCY_70, OnTransparency70)
	ON_COMMAND(ID_TRANSPARENCY_80, OnTransparency80)
	ON_COMMAND(ID_TRANSPARENCY_90, OnTransparency90)
	ON_WM_TIMER()
	ON_COMMAND(ID_ABOUT, OnAbout)
	ON_COMMAND(ID_PREFERENCES, OnPreferences)
	ON_COMMAND(ID_TRANSPARENCY_ALLTRANSPARENCYOFF, OnTransparencyAlltransparencyoff)
	ON_COMMAND(ID_TRANSPARENCY_TOGGLEKEYEDBACKGROUND, OnTransparencyTogglekeyedbackground)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalWindow message handlers

void CPalWindow::OnOK() 
{
}

void CPalWindow::OnCancel() 
{
}

void CPalWindow::OnClose() 
{
	KillTimer(1);

	app->ppw = NULL;

	CDialog::OnClose();

	delete this;
}

class MyDO : public CMachineDataOutput
{
public:
	vector<CMachine *> macs;
	CMICallbacks *pCB;

	virtual void Write(void *pbuf, int const numbytes)
	{
		macs.push_back(pCB->GetMachine((char *)pbuf));
	}
};

void CPalWindow::DrawConnLine(CDC *pdc, int x1, int y1, int x2, int y2)
{
	pdc->MoveTo(x1,y1);
	pdc->LineTo(x2,y2);

	float dx = (float)(x2-x1);
	float dy = (float)(y2-y1);
	float r = sqrtf(dx*dx + dy*dy);
	
	int xstep = (int)(dx / r * 5.0f);
	int ystep = (int)(dy / r * 5.0f);

	int xa = (x1+x2)/2 + xstep;
	int ya = (y1+y2)/2 + ystep;
	pdc->MoveTo(xa,ya);
	int xb = xa - xstep*2 + ystep;
	int yb = ya - ystep*2 - xstep;
	pdc->LineTo(xb,yb);
	int xc = xa - xstep*2 - ystep;
	int yc = ya - ystep*2 + xstep;
	pdc->LineTo(xc,yc);
	pdc->LineTo(xa,ya);
}

void CPalWindow::OnPaint() 
{
//	mb("Paint begin");
	CPaintDC pdc(this); // device context for painting
	CMemDC dc(&pdc);
	CRect rc; GetClientRect(rc);

	if (bKeyedBackground)
		dc.FillSolidRect(rc,0xff00ff);
	else
		dc.FillSolidRect(rc,0xeef6ff);

	CFont *oldfont = dc.SelectObject(&smallfont);
	dc.SetTextAlign(TA_BASELINE | TA_CENTER);
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(0);

//	vector<TextRec> texts;
	TextRec text;
	text.text = NULL;

	for (ConnRec *p = conns.begin(); p < conns.end(); p++)
	{
		if (p->mac1 == p->mac2)
		{
			int x = (int)((macs[p->mac1].x * 0.5f + 0.5f) * (float)rc.Width());
			int y = (int)((macs[p->mac1].y * 0.5f + 0.5f) * (float)rc.Height());
			dc.Ellipse(x-20,y-30,x+20,y);
		}
		else
		{
			int x1 = (int)((macs[p->mac1].x * 0.5f + 0.5f) * (float)rc.Width());
			int y1 = (int)((macs[p->mac1].y * 0.5f + 0.5f) * (float)rc.Height());
			int x2 = (int)((macs[p->mac2].x * 0.5f + 0.5f) * (float)rc.Width());
			int y2 = (int)((macs[p->mac2].y * 0.5f + 0.5f) * (float)rc.Height());

			DrawConnLine(dc,x1,y1,x2,y2);

			int x=(x1+x2)/2; int y=(y1+y2)/2;
			CRect r(x,y,x,y);
			r.InflateRect(5,5);
			if (r.PtInRect(ptMouse))
			{
				TextRec t;
				t.x = x; t.y = y;
				t.text = p->text;
				t.bigfont = false;
//				texts.push_back(t);
				text = t;
			}
		}
	}

	vector<ConnRec *> redconns;
	vector<ConnRec *> ylwconns;

	for (MachineRec *m = macs.begin(); m < macs.end(); m++)
	{
		int x = (int)((m->x * 0.5f + 0.5f) * (float)rc.Width());
		int y = (int)((m->y * 0.5f + 0.5f) * (float)rc.Height());

		CRect r(x,y,x,y);
		r.InflateRect(25,12);

		COLORREF clr;
		if (m->minfo->Type == MT_MASTER)
			clr = 0xAABEC6;
		else if (m->peer)
			clr = 0x44D6F1;
		else
			clr = 0xC9D6DA;

		if (!m->interesting && !r.PtInRect(ptMouse)) r.DeflateRect(10,5);

		dc.FillSolidRect(r,clr);

		if (r.PtInRect(ptMouse))
		{
			TextRec tr;
			tr.x = ptMouse.x;
			tr.y = ptMouse.y;
			static char mactext[1024];
			sprintf(mactext,"%s\n%s",m->name,m->minfo->Name);
			tr.text = mactext;
			tr.bigfont = true;
//			texts.push_back(tr);
			if (!text.text) text = tr;

			for (ConnRec *p = conns.begin(); p < conns.end(); p++)
			{
				if (&macs[p->mac1] == m) redconns.push_back(p);
				if (&macs[p->mac2] == m) ylwconns.push_back(p);
			}
		}

		dc.ExtTextOut(x,y+4,ETO_CLIPPED,r,m->name,NULL);
	}

	CPen redpen(PS_SOLID,2,0x0000FF);
	CPen ylwpen(PS_SOLID,2,0xFF0000);

	CPen *oldpen = dc.SelectObject(&redpen);
	ConnRec **pcr;
	for (pcr = redconns.begin(); pcr < redconns.end(); pcr++)
	{
		ConnRec *p = *pcr;
		int x1 = (int)((macs[p->mac1].x * 0.5f + 0.5f) * (float)rc.Width());
		int y1 = (int)((macs[p->mac1].y * 0.5f + 0.5f) * (float)rc.Height());
		int x2 = (int)((macs[p->mac2].x * 0.5f + 0.5f) * (float)rc.Width());
		int y2 = (int)((macs[p->mac2].y * 0.5f + 0.5f) * (float)rc.Height());

		DrawConnLine(dc,x1,y1,x2,y2);
	}
	dc.SelectObject(&ylwpen);
	for (pcr = ylwconns.begin(); pcr < ylwconns.end(); pcr++)
	{
		ConnRec *p = *pcr;
		int x1 = (int)((macs[p->mac1].x * 0.5f + 0.5f) * (float)rc.Width());
		int y1 = (int)((macs[p->mac1].y * 0.5f + 0.5f) * (float)rc.Height());
		int x2 = (int)((macs[p->mac2].x * 0.5f + 0.5f) * (float)rc.Width());
		int y2 = (int)((macs[p->mac2].y * 0.5f + 0.5f) * (float)rc.Height());

		DrawConnLine(dc,x1,y1,x2,y2);
	}

	dc.SelectObject(oldpen);

	dc.SetTextAlign(TA_TOP | TA_LEFT);
//	for (TextRec *t = texts.begin(); t < texts.end(); t++)
	if (!bShift && text.text)
	{
		TextRec *t = &text;
		if (t->bigfont) dc.SelectObject(bigfont); else dc.SelectObject(smallfont);
		CRect r(t->x,t->y,t->x,t->y);
		CSize s = GetMultilineTextExtent(dc, t->text);

//		CSize s = dc.GetTextExtent(t->text);
		r.InflateRect(0,0,s.cx,s.cy);

		if (r.right  > rc.right ) r.OffsetRect(rc.right  - r.right , 0);
		if (r.bottom > rc.bottom) r.OffsetRect(0, rc.bottom - r.bottom);
		if (r.left < 0) r.OffsetRect(-r.left,0);
		if (r.top  < 0) r.OffsetRect(0, -r.top);

		r.OffsetRect(2,2);
		dc.FillSolidRect(r,0x556677);
		r.OffsetRect(-2,-2);
		dc.FillSolidRect(r,0xFFFFFF);
		MultilineTextOut(dc,r,t->text);
	}

	if (!bShift && !bKeyedBackground && nRepaintInt < 1000)
		ShowMouse(text.text == NULL); 
	else 
		ShowMouse(true);

	dc.SelectObject(oldfont);
//	mb("Paint end");
}

CSize CPalWindow::GetMultilineTextExtent(CDC *pdc, char *txt)
{
	char *a = txt;
	char *b = a;

	CSize r(0,0);

	while (*a)
	{
		while (*b && *b != '\n') b++;
		char c = *b;
		*b = 0;
		CSize s = pdc->GetTextExtent(a);
		r.cy += s.cy;
		if (s.cx > r.cx) r.cx = s.cx;
		*b = c;
		if (c) b++;
		a = b;
	}

	return r;
}

void CPalWindow::MultilineTextOut(CDC *pdc, CRect r, char *txt)
{
	int y = r.top;
	char *a = txt;
	char *b = a;

	while (*a)
	{
		while (*b && *b != '\n') b++;
		char c = *b;
		*b = 0;
		CSize s = pdc->GetTextExtent(a);
		pdc->TextOut(r.left,y,a);
		y += s.cy;
		*b = c;
		if (c) b++;
		a = b;
	}
}



void CPalWindow::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RedrawWindow();
	
}


void CPalWindow::UpdateRecs()
{
	mb("UpdateRecs begin");
	macs.clear();
	conns.clear();

	MyDO mydo;
	mydo.pCB = app->pCB;
	app->pCB->GetMachineNames(&mydo);

	for (CMachine **pp = mydo.macs.begin(); pp < mydo.macs.end(); pp++)
	{
		MachineRec r;
		r.mac = *pp;
		r.minfo = app->pCB->GetMachineInfo(r.mac);
		strcpy(r.name, app->pCB->GetMachineName(r.mac));

		r.x = r.mac->posx;
		r.y = r.mac->posy;

		r.peer = app->IsMachineAPeer(r.mac);
		r.interesting = r.peer;

		macs.push_back(r);
	}

	int i=0;
	for (MachineRec *pr = macs.begin(); pr < macs.end(); pr++, i++)
	{
		if (pr->peer)
		{
			vector<PalPalAssign> ass;
			app->GetPeerAssigns(pr->mac,&ass);

			for (PalPalAssign *p = ass.begin(); p < ass.end(); p++)
			{
				if (p->targetmac)
				{
					ConnRec r;
					r.mac1 = i;
					r.mac2 = -1;

					const CMachineInfo *minfo = app->pCB->GetMachineInfo(p->targetmac);

					int npar = p->targetparam;
					if (p->targetgroup == 2) npar += minfo->numGlobalParameters;

					sprintf(r.text,"%s: [%c %i] %s",p->comment,
						(p->targetgroup == 1) ? 'G' : 'T',
						p->targetparam,
						minfo->Parameters[npar]->Name);

					for (int j=0; j<macs.size() && r.mac2 == -1; j++)
					{
						if (macs[j].mac == p->targetmac) r.mac2 = j;
					}

					if (r.mac2 != -1)
					{
						conns.push_back(r);
						macs[r.mac1].interesting = macs[r.mac2].interesting = true;
					}
				}
			}
		}
	}

	for (int j=0; j<conns.size(); j++)
	{
		for (int k=j+1; k<conns.size(); k++)
		{
			if (conns[j].mac1 == conns[k].mac1 && conns[j].mac2 == conns[k].mac2)
			{
				strcat(conns[j].text,"\n");
				strcat(conns[j].text,conns[k].text);
				conns.erase(&conns[k]);
				k--;
			}
		}

		static char tx2[4096];
		sprintf(tx2,"FROM: %s\nTO: %s\n%s",
			macs[conns[j].mac1].name,
			macs[conns[j].mac2].name,
			conns[j].text);
		strcpy(conns[j].text,tx2);

	}

	mb("UpdateRecs nearly end");

	RedrawWindow();

	mb("UpdateRecs end");
}

void CPalWindow::ShowMouse(bool b)
{
	static bool showd = true;
	if (b != showd)
	{
		ShowCursor(b ? TRUE : FALSE);
		showd = b;
	}
}

void CPalWindow::OnUpdatingUpdatenow() 
{
	UpdateRecs();
}


BOOL CPalWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	smallfont.CreatePointFont(80,"Tahoma");
	bigfont.CreatePointFont(110,"Tahoma");

	nUpdInt = -1;
	nRepaintInt = -1;
	bKeyedBackground = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPalWindow::OnMouseMove(UINT nFlags, CPoint point) 
{
	ptMouse = point;
	bShift = ((nFlags & MK_SHIFT) != 0);

	if (nRepaintInt == -1) RedrawWindow();

	CDialog::OnMouseMove(nFlags, point);
}

BOOL CPalWindow::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
//	return CDialog::OnEraseBkgnd(pDC);

	return TRUE;
}

void CPalWindow::Transp(char t)
{
	int a = 256*t/10;
	MakeWindowTransparent(m_hWnd,a,true);	
}


void CPalWindow::OnTransparency100off() 
{
	MakeWindowTransparent(m_hWnd,255,true);	
}

void CPalWindow::OnTransparency10() {Transp(1);}
void CPalWindow::OnTransparency20() {Transp(2);}
void CPalWindow::OnTransparency30() {Transp(3);}
void CPalWindow::OnTransparency40() {Transp(4);}
void CPalWindow::OnTransparency50() {Transp(5);}
void CPalWindow::OnTransparency60() {Transp(6);}
void CPalWindow::OnTransparency70() {Transp(7);}
void CPalWindow::OnTransparency80() {Transp(8);}
void CPalWindow::OnTransparency90() {Transp(9);}

void CPalWindow::UpdInt(int i)
{
	KillTimer(1);
	nUpdInt = i;
	if (i != -1) SetTimer(1,i,0);
}

void CPalWindow::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1) UpdateRecs();
	if (nIDEvent == 2) RedrawWindow();
	
	CDialog::OnTimer(nIDEvent);
}

void CPalWindow::PaintInt(int i)
{
	KillTimer(2);
	nRepaintInt = i;
	if (i != -1) SetTimer(2,i,0);
}

void CPalWindow::OnAbout() 
{
	static char txt[10000];
	strcpy(txt,"BTDSys PAL\n©2005 Ed Powley\nhttp://btdsys.cjb.net\n\n");

	for (SupportedMachineRecord *r = app->vSupportedMacs.begin(); r < app->vSupportedMacs.end(); r++)
	{
		sprintf(txt,"%s%s: %s\n",txt,r->dllname,r->name);
	}
	
	MessageBox(txt,"BTDSys PAL");
}

void CPalWindow::OnPreferences()
{
	CPrefsDlg dlg;
	dlg.pw = this;
	dlg.DoModal();
}

void CPalWindow::OnTransparencyAlltransparencyoff() 
{
	bKeyedBackground = false;
	MakeWindowTransparent(m_hWnd,255,false);		
}

void CPalWindow::OnTransparencyTogglekeyedbackground() 
{
	OnTransparency100off();
	bKeyedBackground = !bKeyedBackground;
	RedrawWindow();
}

void CPalWindow::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	if (nChar == ' ')
	{
		UpdateRecs();
	}
	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
