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

#if !defined(AFX_PARAMDLG_H__2C4FE1A1_B6FE_11D7_AE8A_805DCD535D71__INCLUDED_)
#define AFX_PARAMDLG_H__2C4FE1A1_B6FE_11D7_AE8A_805DCD535D71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParamDlg.h : header file
//

#define ID_PARAM_MENUITEM_ZERO	33000

#include "resource.h"

#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CParamDlg dialog

class CMachine;
class CMachineParameter;
class mi;
class CKeyboardDlg;

class CParamDlg : public CDialog
{
// Construction
public:
	mi *m_pmi;
	int m_nContextMenuParam, m_nContextMenuBar, m_nContextMenuTrack;
	const CMachineParameter *m_pContextMenuParam;
	void SendParamValue(int nBar, int nPos = -1);
	void UpdateParamValueLabel(int nParam);
	int m_nTotalHeight;
	void Scroll (int y);
	void AddControlsForParam(const CMachineParameter *par, 
		CRect *r, CRect *r2, int id, int track, unsigned short value);
	CMachine *m_pMac;
	CParamDlg(CWnd* pParent = NULL);   // standard constructor

	CFont fntNormal;
	CFont fntNonState;

	vector<CStatic *>   vpParamLabels;
	vector<CSliderCtrl*>vpSliders;
	vector<CStatic *>   vpParamValues;	

	void DestroyParamControls();
	void CreateParamControls();

	CKeyboardDlg *pKeyDlg;

// Dialog Data
	//{{AFX_DATA(CParamDlg)
	enum { IDD = IDD_PARAMDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnAddnewtrack();
	afx_msg void OnDeletelasttrack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARAMDLG_H__2C4FE1A1_B6FE_11D7_AE8A_805DCD535D71__INCLUDED_)
