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

#if !defined(AFX_PLUGINAXESDIALOG_H__2F5AD361_3E7B_11D7_BDC3_D06C4FC10F01__INCLUDED_)
#define AFX_PLUGINAXESDIALOG_H__2F5AD361_3E7B_11D7_BDC3_D06C4FC10F01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginAxesDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPluginAxesDialog dialog

class mi;
class CTrack;

class CPluginAxesDialog : public CDialog
{
// Construction
public:
	CPluginAxesDialog(int cnum, CWnd* pParent = NULL);   // standard constructor

	mi *pmi;
	CTrack *ptrack;
	int ctrlnum;

// Dialog Data
	//{{AFX_DATA(CPluginAxesDialog)
	enum { IDD = IDD_PLUGINAXESDIALOG };
	CListBox	m_BoundList;
	CListBox	m_AvailList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluginAxesDialog)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPluginAxesDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnArrowbutton();
	afx_msg void OnDelbutton();
	afx_msg void OnAboutaxis();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLUGINAXESDIALOG_H__2F5AD361_3E7B_11D7_BDC3_D06C4FC10F01__INCLUDED_)
