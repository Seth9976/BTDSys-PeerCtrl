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

#if !defined(AFX_GROUPDLG_H__6B1854A0_FD24_4E70_B130_35D4EE288115__INCLUDED_)
#define AFX_GROUPDLG_H__6B1854A0_FD24_4E70_B130_35D4EE288115__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg dialog

#include "KeyCapBtn.h"
#include "WaveOffset.h"

class mi;

class CGroupDlg : public CDialog
{
// Construction
public:
	CGroupDlg(CWnd* pParent = NULL);   // standard constructor

	mi *pmi;

	void OnMidiNote(int chan, int key, int vel);

	void RedisplayParam(CPeerCtrl *pc, CSliderCtrl *pSliderCtrl, CEdit *pEdit, int *val = NULL);

// Dialog Data
	//{{AFX_DATA(CGroupDlg)
	enum { IDD = IDD_GROUPDLG };
	CEdit	m_edPeerMin;
	CSliderCtrl	m_slidPeerMin;
	CComboBox	m_cmbGroupMute;
	CEdit	m_edPeerMax;
	CSliderCtrl	m_slidPeerMax;
	CComboBox	m_cmbPeerParam;
	CComboBox	m_cmbPeerMac;
	CComboBox	m_cmbPeer;
	CWaveOffset	m_WaveOffset;
	CComboBox	m_cmbTrackSample;
	CKeyCapBtn	m_btnMIDIHighNote;
	CKeyCapBtn	m_btnMIDILowNote;
	CButton	m_chkXOR;
	CEdit	m_edGroupName;
	CComboBox	m_cmbGroupOut;
	CComboBox	m_cmbTrackGroup;
	CComboBox	m_cmbGroup;
	CComboBox	m_cmbTrack;
	CKeyCapBtn	m_btnGroupMuteKey;
	CKeyCapBtn	m_btnTrackKey;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InsertPeerListItem(int n, char *s1, char *s2, char *s3);

	// Generated message map functions
	//{{AFX_MSG(CGroupDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeGroupcombo();
	afx_msg void OnSelchangeTrackcombo();
	afx_msg void OnXorcheck();
	afx_msg void OnSelchangeOutcombo();
	afx_msg void OnSelchangeTracksamplecombo();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSelchangeTrackgroupcombo();
	afx_msg void OnGroupmutebtn();
	afx_msg void OnTrackkeybtn();
	afx_msg void OnSelchangePeermaccombo();
	afx_msg void OnSelchangePeercombo();
	afx_msg void OnSelchangePeerparamcombo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeGroupmutecombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPDLG_H__6B1854A0_FD24_4E70_B130_35D4EE288115__INCLUDED_)
