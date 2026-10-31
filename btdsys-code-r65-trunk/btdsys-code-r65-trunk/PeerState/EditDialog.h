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

#if !defined(AFX_EDITDIALOG_H__A0F5286E_207B_4C1D_BB22_12EF7CC19490__INCLUDED_)
#define AFX_EDITDIALOG_H__A0F5286E_207B_4C1D_BB22_12EF7CC19490__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditDialog dialog

class mi;

class CEditDialog : public CDialog
{
// Construction
public:
	CEditDialog(CWnd* pParent = NULL);   // standard constructor

	mi *pmi;

// Dialog Data
	//{{AFX_DATA(CEditDialog)
	enum { IDD = IDD_EDITDIALOG };
	CEdit	m_txtMidiNoteB;
	CEdit	m_txtMidiNoteA;
	CTreeCtrl	m_treeB;
	CTreeCtrl	m_treeA;
	CComboBox	m_comboB;
	CComboBox	m_comboA;
	CStatic	m_txtAmount;
	CSliderCtrl	m_slidAmount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnComboASelchange();
	afx_msg void OnComboBSelchange();
	afx_msg void OnDeleteBtn();
	afx_msg void OnSwapBtn();
	afx_msg void OnCopyBtn();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMorphBtn();
	afx_msg void OnSpliceBtn();
	afx_msg void OnRandomiseBtn();
	afx_msg void OnSetCurrentA();
	afx_msg void OnSetCurrentB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


private:
	void PopulateTree(CTreeCtrl *ptree, int s);
	void UpdateComboItem(CComboBox *pcombo, int s);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITDIALOG_H__A0F5286E_207B_4C1D_BB22_12EF7CC19490__INCLUDED_)
