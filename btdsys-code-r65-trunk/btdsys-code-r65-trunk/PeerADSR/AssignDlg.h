/*
Copyright (c) 2002-2011, Edward Powley
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

//{{AFX_INCLUDES()
//}}AFX_INCLUDES
#if !defined(AFX_ASSIGNDLG_H__69C7284D_B6AF_11D6_BDC3_D0404FC10100__INCLUDED_)
#define AFX_ASSIGNDLG_H__69C7284D_B6AF_11D6_BDC3_D0404FC10100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssignDlg.h : header file
//

class mi;
class CTrack;

/////////////////////////////////////////////////////////////////////////////
// CAssignDlg dialog

class CAssignDlg : public CDialog
{
// Construction
public:
	CAssignDlg(CWnd* pParent = NULL);   // standard constructor

	mi *g_mi;
	CTrack *g_track;
	int g_tracknum;

// Dialog Data
	//{{AFX_DATA(CAssignDlg)
	enum { IDD = IDD_ASSIGNPAR };
	CListBox	m_ParaList;
	CComboBox	m_MachineCombo;
	CString	m_ThisMacInfoText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssignDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAssignDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeMachinecombo();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnDeassign();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASSIGNDLG_H__69C7284D_B6AF_11D6_BDC3_D0404FC10100__INCLUDED_)
