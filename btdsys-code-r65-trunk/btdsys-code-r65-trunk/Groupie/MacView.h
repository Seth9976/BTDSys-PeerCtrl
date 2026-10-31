/*
Copyright (c) 2003-2006, Edward Powley
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

#if !defined(AFX_MacView_H__F76CF0AF_AAEA_11D7_AE8A_E7E207A20B71__INCLUDED_)
#define AFX_MacView_H__F76CF0AF_AAEA_11D7_AE8A_E7E207A20B71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MacView.h : header file
//

#define ID_MACHINE_MENUITEM_ZERO	33000

#define AMPPANBOX_W 50
#define AMPPANBOX_H 200


enum EDragMode {
	none,
	bbsel,
	move,
	conn,
	connvol,
};

/////////////////////////////////////////////////////////////////////////////
// CMacView window

class CMacViewFrame;

class CMacView : public CWnd
{
// Construction
public:
	CMacView();

// Attributes
public:
	CMacViewFrame *m_pFrame;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMacView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	CMachine * m_pDragConnMac;
	CPoint m_ptDrag;
	EDragMode m_nDragMode;
	float m_fDragOrigAmp, m_fDragOrigPan;

	union {CMachine* m; CConnection* c;} m_pContextMenuTarget;

	virtual ~CMacView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMacView)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnConnectionDisconnect();
	afx_msg void OnMachineParameters();
	afx_msg void OnMachineAttributes();
	afx_msg void OnMachineRename();
	afx_msg void OnMachineDelete();
	afx_msg void OnMachineMdkworkmodeMono();
	afx_msg void OnMachineMdkworkmodeStereo();
	afx_msg void OnMachineDevworkmodeinfo();
	afx_msg void OnDummy();
	afx_msg void OnNew();
	afx_msg void OnOpenbmx();
	afx_msg void OnSavebmx();
	afx_msg void OnVisualprefs();
	afx_msg void OnMachineDevforceexception();
	afx_msg void OnConnectionCenterpan();
	afx_msg void OnMachineUnload();
	afx_msg void OnMachineReplace();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MacView_H__F76CF0AF_AAEA_11D7_AE8A_E7E207A20B71__INCLUDED_)
