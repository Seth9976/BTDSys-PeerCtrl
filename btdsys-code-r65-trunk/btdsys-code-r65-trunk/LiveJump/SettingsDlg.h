#if !defined(AFX_SETTINGSDLG_H__0632E925_841A_423F_88D9_457E9C267623__INCLUDED_)
#define AFX_SETTINGSDLG_H__0632E925_841A_423F_88D9_457E9C267623__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsDlg dialog

#include "resource.h"
#include "KeyCapEdit.h"
#include "KeyCapBtn.h"

class mi;

class CSettingsDlg : public CDialog
{
// Construction
public:
	CSettingsDlg(CWnd* pParent = NULL);   // standard constructor

	mi *pmi;
	int nLastNote;

	void SetNote(int n);

	void PopulateList();

// Dialog Data
	//{{AFX_DATA(CSettingsDlg)
	enum { IDD = IDD_SETTINGSDLG };
	CEdit	m_TickEdit;
	CStatic	m_staticNote;
	CStatic	m_staticTick;
	CKeyCapBtn	m_btnKeyCap;
	CComboBox	m_JumpMode;
	CSpinButtonCtrl	m_TickSpin;
	CListBox	m_List;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRemove();
	afx_msg void OnAdd();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSDLG_H__0632E925_841A_423F_88D9_457E9C267623__INCLUDED_)
