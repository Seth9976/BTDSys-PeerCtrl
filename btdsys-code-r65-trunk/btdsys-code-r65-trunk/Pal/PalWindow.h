/*
PalWindow.h: The main GUI window
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

#if !defined(AFX_PALWINDOW_H__79EC4F62_F7B9_4FFB_B0C7_2D847D27D40E__INCLUDED_)
#define AFX_PALWINDOW_H__79EC4F62_F7B9_4FFB_B0C7_2D847D27D40E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PalWindow.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPalWindow dialog

class CMachine;
class CMachineInfo;

struct MachineRec
{
	CMachine *mac;
	const CMachineInfo *minfo;
	char name[128];
	float x;
	float y;
	bool peer;
	bool interesting;
};

struct ConnRec
{
	int mac1, mac2;
	char text[4096];
};

struct TextRec
{
	int x,y;
	char *text;
	bool bigfont;
};


class CPalWindow : public CDialog
{
// Construction
public:
	CPalWindow(CWnd* pParent = NULL);   // standard constructor

	CBTDSysPALApp *app;

	vector<MachineRec> macs;
	vector<ConnRec> conns;

	void UpdateRecs();

	CFont smallfont;
	CFont bigfont;

	CPoint ptMouse;
	bool bShift;

	void DrawConnLine(CDC *pdc, int x1, int y1, int x2, int y2);
	CSize GetMultilineTextExtent(CDC *pdc, char *txt);
	void MultilineTextOut(CDC *pdc, CRect r, char *txt);

	void Transp(char t);
	void UpdInt(int i);
	void UpdUpdInt(CCmdUI* pCmdUI, int i);
	void PaintInt(int i);
	int nUpdInt;
	int nRepaintInt;

	void ShowMouse(bool b);

	bool bKeyedBackground;

// Dialog Data
	//{{AFX_DATA(CPalWindow)
	enum { IDD = IDD_PALWINDOW };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPalWindow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPalWindow)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdatingUpdatenow();
	virtual BOOL OnInitDialog();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTransparency100off();
	afx_msg void OnTransparency10();
	afx_msg void OnTransparency20();
	afx_msg void OnTransparency30();
	afx_msg void OnTransparency40();
	afx_msg void OnTransparency50();
	afx_msg void OnTransparency60();
	afx_msg void OnTransparency70();
	afx_msg void OnTransparency80();
	afx_msg void OnTransparency90();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnAbout();
	afx_msg void OnPreferences();
	afx_msg void OnTransparencyAlltransparencyoff();
	afx_msg void OnTransparencyTogglekeyedbackground();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALWINDOW_H__79EC4F62_F7B9_4FFB_B0C7_2D847D27D40E__INCLUDED_)
