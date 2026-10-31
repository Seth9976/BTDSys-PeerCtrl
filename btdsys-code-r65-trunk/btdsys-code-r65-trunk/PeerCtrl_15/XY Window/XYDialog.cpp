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
// XYDialog.cpp : implementation file
//

#include "stdafx.h"
#include "XYDialog.h"
#include "memdc.h"
#include "blob.h"
#include "XYPlug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXYDialog dialog


CXYDialog::CXYDialog(CXYPlug *pPlug, CWnd* pParent /*=NULL*/)
	: CDialog(CXYDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXYDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pPlug = pPlug;
	m_pLBlob = m_pMBlob = m_pRBlob = NULL;

	if (Create(CXYDialog::IDD, pParent))
	{
//		OutputDebugString("Create succeeded!");
		ShowWindow(SW_SHOW);
		SetTimer(1, 33, NULL);

		switch (m_pPlug->nBlobStyle)
		{
		case 0:
			m_pLBlob = new CXYNoBlob;
			m_pMBlob = new CXYNoBlob;
			m_pRBlob = new CXYNoBlob;
			break;

		case 1:
			m_pLBlob = new CXYDotBlob;
			m_pMBlob = new CXYDotBlob;
			m_pRBlob = new CXYDotBlob;
			break;

		case 2:
			m_pLBlob = new CXYCrosshairBlob;
			m_pMBlob = new CXYCrosshairBlob;
			m_pRBlob = new CXYCrosshairBlob;
			break;

		case 3:
			m_pLBlob = new CXYTrailBlob;
			m_pMBlob = new CXYTrailBlob;
			m_pRBlob = new CXYTrailBlob;
			break;

		case 4:
			m_pLBlob = new CXYParticleBlob;
			m_pMBlob = new CXYParticleBlob;
			m_pRBlob = new CXYParticleBlob;
			break;
		}

		CRect cr; GetClientRect(cr);
		float f;

		if (m_pLBlob)
		{
			m_pLBlob->SetParentRect(cr);
			m_pLBlob->SetFGColour(pPlug->colL);
			m_pLBlob->SetBGColour(pPlug->colBG);
			f = pPlug->pCB->GetValue(0); if (f >= 0.0f) m_pLBlob->SetX(f);
			f = pPlug->pCB->GetValue(1); if (f >= 0.0f) m_pLBlob->SetY(f);
		}
		if (m_pMBlob)
		{
			m_pMBlob->SetParentRect(cr);
			m_pMBlob->SetFGColour(pPlug->colM);
			m_pMBlob->SetBGColour(pPlug->colBG);
			f = pPlug->pCB->GetValue(2); if (f >= 0.0f) m_pMBlob->SetX(f);
			f = pPlug->pCB->GetValue(3); if (f >= 0.0f) m_pMBlob->SetY(f);
		}
		if (m_pRBlob)
		{
			m_pRBlob->SetParentRect(cr);
			m_pRBlob->SetFGColour(pPlug->colR);
			m_pRBlob->SetBGColour(pPlug->colBG);
			f = pPlug->pCB->GetValue(4); if (f >= 0.0f) m_pRBlob->SetX(f);
			f = pPlug->pCB->GetValue(5); if (f >= 0.0f) m_pRBlob->SetY(f);
		}
	}
//	else
//		OutputDebugString("Create failed!");

//	OutputDebugString("Opened");
}


void CXYDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXYDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXYDialog, CDialog)
	//{{AFX_MSG_MAP(CXYDialog)
	ON_WM_CLOSE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_MOVE()
	ON_WM_SHOWWINDOW()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXYDialog message handlers

void CXYDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OutputDebugString("OnClose");

	m_pPlug->placement.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&m_pPlug->placement);

	CDialog::OnClose();

	m_pPlug->pdlg = NULL;
	if (m_pLBlob) delete m_pLBlob;
	if (m_pMBlob) delete m_pMBlob;
	if (m_pRBlob) delete m_pRBlob;

	DestroyWindow();

//	OutputDebugString("Closed");
}

BOOL CXYDialog::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	return TRUE;
}

inline float frand()
// returns random value between 0 and 1
// from Waveout.cpp
{
	static long stat = /*0x16BA2118*/ GetTickCount (); //seed
	stat = (stat * 1103515245 + 12345) & 0x7fffffff;
	return (float)stat * (1.0f / 0x7fffffff);
}

void CXYDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	dc.SetBkColor(m_pPlug->colBG);
	CMemDC pDC(&dc);
	
	// TODO: Add your message handler code here

//	CRect r;
//	GetClientRect(r);

	char txt[64];

	if (m_pLBlob)
	{
		m_pLBlob->Update(pDC);
		pDC->SetTextColor(m_pPlug->colL);
		sprintf(txt, "LMB (%.1f%%, %.1f%%)", m_pLBlob->GetX()*100.0f, m_pLBlob->GetY()*100.0f);
		pDC->TextOut(2,2,txt);
	}

	if (m_pMBlob)
	{
		m_pMBlob->Update(pDC);
		pDC->SetTextColor(m_pPlug->colM);
		sprintf(txt, "MMB (%.1f%%, %.1f%%)", m_pMBlob->GetX()*100.0f, m_pMBlob->GetY()*100.0f);
		pDC->TextOut(2,20,txt);
	}

	if (m_pRBlob)
	{
		m_pRBlob->Update(pDC);
		pDC->SetTextColor(m_pPlug->colR);
		sprintf(txt, "RMB (%.1f%%, %.1f%%)", m_pRBlob->GetX()*100.0f, m_pRBlob->GetY()*100.0f);
		pDC->TextOut(2,38,txt);
	}

	// Do not call CDialog::OnPaint() for painting messages
}

void CXYDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CRect r;
	GetClientRect(r);

	if (nFlags & MK_LBUTTON && m_pLBlob)
	{
		m_pLBlob->SetScreenPos(point);
		m_pPlug->pCB->SetValue(0, m_pLBlob->GetX());
		m_pPlug->pCB->SetValue(1, m_pLBlob->GetY());
	}

	if (nFlags & MK_MBUTTON && m_pMBlob)
	{
		m_pMBlob->SetScreenPos(point);
		m_pPlug->pCB->SetValue(2, m_pMBlob->GetX());
		m_pPlug->pCB->SetValue(3, m_pMBlob->GetY());
	}

	if (nFlags & MK_RBUTTON && m_pRBlob)
	{
		m_pRBlob->SetScreenPos(point);
		m_pPlug->pCB->SetValue(4, m_pRBlob->GetX());
		m_pPlug->pCB->SetValue(5, m_pRBlob->GetY());
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CXYDialog::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	RedrawWindow();

	CDialog::OnTimer(nIDEvent);
}

void CXYDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CRect cr; GetClientRect(cr);

	if (m_pLBlob)
		m_pLBlob->SetParentRect(cr);
	if (m_pMBlob)
		m_pMBlob->SetParentRect(cr);
	if (m_pRBlob)
		m_pRBlob->SetParentRect(cr);
}

void CXYDialog::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	// TODO: Add your message handler code here
}

void CXYDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

/*	char txt[256];
	sprintf(txt,"ShowWindow bShow=%i and nStatus=%i",bShow,nStatus);
	OutputDebugString(txt);
*/
}

BOOL MakeWindowTransparent(HWND hWnd, unsigned char factor);

BOOL CXYDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Add menu items to system menu.

	if (MakeWindowTransparent(m_hWnd, 0xFF))
	{
		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			for (int i=0; i<10; i++)
			{
				CString str;
				str.Format("%i%% opacity", (10-i) * 10);

				pSysMenu->AppendMenu(MF_STRING, (i+1) * 0x10, str);
			}

			pSysMenu->CheckMenuItem(0x0010, MF_BYCOMMAND | MF_CHECKED);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXYDialog::OnSysCommand(UINT nID, LPARAM lParam) 
{
	UINT i = (nID & 0xFFF0) >> 4;

	char txt[256];
	sprintf(txt,"%i",i);
	OutputDebugString(txt);

	if (i == 0 || i > 10)
		CDialog::OnSysCommand(nID, lParam);
	else
	{
		MakeWindowTransparent(m_hWnd, (11-i)*0xFF/10);

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			for (int j=1; j<=10; j++)
				pSysMenu->CheckMenuItem(j * 0x10, MF_BYCOMMAND | MF_UNCHECKED);

			pSysMenu->CheckMenuItem(i * 0x10, MF_BYCOMMAND | MF_CHECKED);
		}
	}
}




typedef DWORD (WINAPI *PSLWA)(HWND, DWORD, BYTE, DWORD);

static PSLWA pSetLayeredWindowAttributes = NULL;
static BOOL initialized = FALSE;

#define WS_EX_LAYERED           0x00080000    

#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002

BOOL MakeWindowTransparent(HWND hWnd, unsigned char factor)
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

	if (factor == 0xFF)
	{
		SetWindowLong(hWnd, 
					GWL_EXSTYLE , 
					GetWindowLong(hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

		if (GetLastError())
			return FALSE;

		return TRUE;
	}
	else
	{
		SetWindowLong(hWnd, 
					GWL_EXSTYLE , 
					GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		if (GetLastError())
			return FALSE;

		/* Now, we need to set the 'layered window attributes'. This
		 * is where the alpha values get set. */
		return pSetLayeredWindowAttributes (hWnd, 
											RGB(255,255,255), 
											factor,
											LWA_COLORKEY|LWA_ALPHA);
	}
}

