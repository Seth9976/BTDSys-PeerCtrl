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

#if !defined(AFX_MIXERDLG_H__70EA2421_1360_11D7_BDC3_605F4FC10301__INCLUDED_)
#define AFX_MIXERDLG_H__70EA2421_1360_11D7_BDC3_605F4FC10301__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MixerDlg.h : header file
//

#include "Mixer GUI.h"

#include "MySliderCtrl.h"
#include "MyKnobCtrl.h"

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CMixerDlg dialog

#define MAX_CHANNELS 128
#define NUM_KNOBS 7

class CMixerDlg;

class CMixerChannel
{
public:
	CMixerChannel(CMixerDlg *pParent, int xpos, UINT id_base);
	~CMixerChannel();

	void Resize(int h);

	CMySliderCtrl *slider;
	CMyKnobCtrl *knob[NUM_KNOBS];
	CButton *mute;
	CButton *solo;
	CEdit *label[NUM_KNOBS + 1];

//	int TrackBinds[NUM_KNOBS + 1];

	CMixerDlg *m_pParent;
};

class CMixerDlg : public CDialog
{
// Construction
public:
	CMixerDlg(CMixerGuiPlug *p_pl, CWnd* pParent = NULL);   // standard constructor
	~CMixerDlg();

	CMixerGuiPlug *ppl;

	int numMixerChannels;

	CBrush *m_pEditBkBrush;

	CMixerChannel *channel[MAX_CHANNELS];

	int m_PopupChannel;
	int m_PopupKnob;

	int nKnobMouseMode;

	bool bSoloMode;

// Dialog Data
	//{{AFX_DATA(CMixerDlg)
	enum { IDD = IDD_MIXERDIALOG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMixerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AssignsToSubMenu(CMenu *pm, int Active);
	void UpdateKnobMouseModes();

	// Generated message map functions
	//{{AFX_MSG(CMixerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnHelp();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKnobcolour();
	afx_msg void OnSlidercolour();
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKnobmin();
	afx_msg void OnKnobmax();
	afx_msg void OnKnobcenter();
	afx_msg void OnSlidermin();
	afx_msg void OnSlidermax();
	afx_msg void OnSlidercenter();
	afx_msg void OnKnobmodeRotwrap();
	afx_msg void OnKnobmodeRotnowrap();
	afx_msg void OnKnobmodeLinhorz();
	afx_msg void OnKnobmodeLinvert();
	afx_msg void OnUpdateKnobmodeRotwrap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKnobmodeRotnowrap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKnobmodeLinhorz(CCmdUI* pCmdUI);
	afx_msg void OnUpdateKnobmodeLinvert(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIXERDLG_H__70EA2421_1360_11D7_BDC3_605F4FC10301__INCLUDED_)
