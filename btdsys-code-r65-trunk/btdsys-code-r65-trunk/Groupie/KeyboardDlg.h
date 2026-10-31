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

#if !defined(AFX_KEYBOARDDLG_H__E7ACC8D7_E6F5_4E55_951C_BD670BF5EBB5__INCLUDED_)
#define AFX_KEYBOARDDLG_H__E7ACC8D7_E6F5_4E55_951C_BD670BF5EBB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyboardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyboardDlg dialog

class CParamDlg;
class CMachine;

class CKeyboardDlg : public CDialog
{
// Construction
public:
	CKeyboardDlg(CWnd* pParent = NULL);   // standard constructor

	CMachine *pMac;
	int nBar;

	CKeyboardDlg **pMyRef;
	CParamDlg *pParamDlg;

	void Note(int n);

// Dialog Data
	//{{AFX_DATA(CKeyboardDlg)
	enum { IDD = IDD_KEYBOARDDLG };
	CComboBox	m_cmbOctave;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyboardDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyboardDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnNoteC1();
	afx_msg void OnNoteCs1();
	afx_msg void OnNoteD1();
	afx_msg void OnNoteDs1();
	afx_msg void OnNoteE1();
	afx_msg void OnNoteF1();
	afx_msg void OnNoteFs1();
	afx_msg void OnNoteG1();
	afx_msg void OnNoteGs1();
	afx_msg void OnNoteA1();
	afx_msg void OnNoteAs1();
	afx_msg void OnNoteB1();
	afx_msg void OnNoteC2();
	afx_msg void OnNoteCs2();
	afx_msg void OnNoteD2();
	afx_msg void OnNoteDs2();
	afx_msg void OnNoteE2();
	afx_msg void OnNoteF2();
	afx_msg void OnNoteFs2();
	afx_msg void OnNoteG2();
	afx_msg void OnNoteGs2();
	afx_msg void OnNoteA2();
	afx_msg void OnNoteAs2();
	afx_msg void OnNoteB2();
	afx_msg void OnClose();
	afx_msg void OnNoteoff();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYBOARDDLG_H__E7ACC8D7_E6F5_4E55_951C_BD670BF5EBB5__INCLUDED_)
