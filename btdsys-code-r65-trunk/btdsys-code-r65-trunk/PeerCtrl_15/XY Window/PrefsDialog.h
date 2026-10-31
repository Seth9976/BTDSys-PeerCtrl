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
#if !defined(AFX_PREFSDIALOG_H__69AF1801_45EC_11D7_BDC3_F0714FC17C0B__INCLUDED_)
#define AFX_PREFSDIALOG_H__69AF1801_45EC_11D7_BDC3_F0714FC17C0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrefsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrefsDialog dialog

#include "BlobPreview.h"
#include "resource.h"

class CXYBlob;
class CXYPlug;

class CPrefsDialog : public CDialog
{
// Construction
public:
	bool m_bTextFontIsValid;
	LOGFONT m_TextFont;
	CPrefsDialog(CWnd* pParent = NULL);   // standard constructor

	CXYPlug *pparent;

	float previewphase;

// Dialog Data
	//{{AFX_DATA(CPrefsDialog)
	enum { IDD = IDD_PREFSDIALOG };
	CButton	m_FontBtn;
	CBlobPreview	m_RColour;
	CBlobPreview	m_MColour;
	CBlobPreview	m_LColour;
	CComboBox	m_BlobStyle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrefsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrefsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnBlobstylecomboSelchange();
	afx_msg void OnBgcolour();
	afx_msg void OnFontbtn();
	virtual void OnOK();
	afx_msg void OnSavedefbtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREFSDIALOG_H__69AF1801_45EC_11D7_BDC3_F0714FC17C0B__INCLUDED_)
