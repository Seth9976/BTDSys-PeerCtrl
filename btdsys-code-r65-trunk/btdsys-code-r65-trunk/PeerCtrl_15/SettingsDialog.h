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

//{{AFX_INCLUDES()
#include "envelope.h"
//}}AFX_INCLUDES
#if !defined(AFX_SETTINGSDIALOG_H__54CF20C2_BAD1_11D6_BDC3_40734FC10301__INCLUDED_)
#define AFX_SETTINGSDIALOG_H__54CF20C2_BAD1_11D6_BDC3_40734FC10301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDialog.h : header file
//

#include "ctrl.h"
#include "envelope.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog dialog

class CSettingsDialog : public CDialog
{
// Construction
public:
	CSettingsDialog(CWnd* pParent = NULL);   // standard constructor
	~CSettingsDialog();

	mi *pmi;
	int tracknum;
	CPeerCtrl *mypc;
	CEnvData curenvdata;

	int CurrentAssign;

	void AfterAssignListSelChange();

// Dialog Data
	//{{AFX_DATA(CSettingsDialog)
	enum { IDD = IDD_SETTINGSDIALOG };
	CTreeCtrl	m_AssignTree;
	CComboBox	m_TrackCombo;
	CListBox	m_ParamList;
	CComboBox	m_MachineCombo;
	CEnvelope	m_MappingEnvelope;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AssignTreeSelChange();

	// Generated message map functions
	//{{AFX_MSG(CSettingsDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnMachinecomboSelchange();
	afx_msg void OnMappingEnvelopeChanged(long numPoints, long SustainPoint, long FAR* Points);
	afx_msg void OnClearbutton();
	afx_msg void OnResetbutton();
	afx_msg void OnParamlistSelchange();
	afx_msg void OnTrackcomboSelchange();
	afx_msg void OnMacinfobutton();
	afx_msg void OnParinfobutton();
	afx_msg void OnAddbutton2();
	afx_msg void OnDelbutton();
	afx_msg void OnArrowbutton();
	afx_msg void OnAssigntreeSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPluginbtn();
	afx_msg void OnMidiccbtn();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMappingwindow();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDIALOG_H__54CF20C2_BAD1_11D6_BDC3_40734FC10301__INCLUDED_)
