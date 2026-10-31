#if !defined(AFX_KEYCAPBTN_H__61ECD90C_1C71_478F_A0EE_B746342D040E__INCLUDED_)
#define AFX_KEYCAPBTN_H__61ECD90C_1C71_478F_A0EE_B746342D040E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KeyCapBtn.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CKeyCapBtn window

class CKeyCapBtn : public CButton
{
// Construction
public:
	CKeyCapBtn();

	int nKey;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyCapBtn)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CKeyCapBtn();

	// Generated message map functions
protected:
	//{{AFX_MSG(CKeyCapBtn)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCAPBTN_H__61ECD90C_1C71_478F_A0EE_B746342D040E__INCLUDED_)
