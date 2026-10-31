#if !defined(AFX_KEYCAPDLG_H__C2217EE7_0C75_40EA_AE63_190AE917DB42__INCLUDED_)
#define AFX_KEYCAPDLG_H__C2217EE7_0C75_40EA_AE63_190AE917DB42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyCapDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyCapDlg dialog

#include "resource.h"

class mi;

class CKeyCapDlg : public CDialog
{
// Construction
public:
	CKeyCapDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CKeyCapDlg();

	mi *pmi;

// Dialog Data
	//{{AFX_DATA(CKeyCapDlg)
	enum { IDD = IDD_KEYCAPDLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyCapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CKeyCapDlg)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnClose();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCAPDLG_H__C2217EE7_0C75_40EA_AE63_190AE917DB42__INCLUDED_)
