#if !defined(AFX_KEYCAPEDIT_H__9975974C_CCBD_4234_891C_E7002B90E127__INCLUDED_)
#define AFX_KEYCAPEDIT_H__9975974C_CCBD_4234_891C_E7002B90E127__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyCapEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyCapEdit window

class CKeyCapEdit : public CEdit
{
// Construction
public:
	CKeyCapEdit();

	int nKey;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyCapEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKeyCapEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKeyCapEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCAPEDIT_H__9975974C_CCBD_4234_891C_E7002B90E127__INCLUDED_)
