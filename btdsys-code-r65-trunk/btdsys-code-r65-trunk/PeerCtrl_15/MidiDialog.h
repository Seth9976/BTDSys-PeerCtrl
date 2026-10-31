/*
MidiDialog.h: MIDI settings dialog
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

#if !defined(AFX_MIDIDIALOG_H__BFEDA8E1_C599_11D6_BDC3_B04A4FC100CF__INCLUDED_)
#define AFX_MIDIDIALOG_H__BFEDA8E1_C599_11D6_BDC3_B04A4FC100CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MidiDialog.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMidiDialog dialog

class mi;
class CTrack;

class CMidiDialog : public CDialog
{
// Construction
public:
	CMidiDialog(int cnum, CWnd* pParent = NULL);   // standard constructor

	mi *pmi;
	CTrack *ptrack;
	int ctrlnum;

// Dialog Data
	//{{AFX_DATA(CMidiDialog)
	enum { IDD = IDD_MIDIDIALOG };
	CComboBox	m_OutDevCombo;
	CStatic	m_LastControllerDisplay;
	CButton	m_IncDecWrapCheck;
	CButton	m_IncDecCheck;
	CButton	m_FeedbackCheck;
	CComboBox	m_ChannelCombo;
	CComboBox	m_CCCombo;
	CStatic	m_AssignInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMidiDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void PopulateJoystickInfo(UINT id);

	// Generated message map functions
	//{{AFX_MSG(CMidiDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeCccombo();
	afx_msg void OnSelchangeChannelcombo();
	afx_msg void OnFeedbackcheck();
	afx_msg void OnIncdeccheck();
	afx_msg void OnIncdecwrapcheck();
	afx_msg void OnSelchangeOutdevcombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIDIDIALOG_H__BFEDA8E1_C599_11D6_BDC3_B04A4FC100CF__INCLUDED_)
