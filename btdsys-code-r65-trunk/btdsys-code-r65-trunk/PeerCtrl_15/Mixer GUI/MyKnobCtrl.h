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

#if !defined(AFX_MYKNOBCTRL_H__EE8D1822_14D6_11D7_BDC3_C0554FC10100__INCLUDED_)
#define AFX_MYKNOBCTRL_H__EE8D1822_14D6_11D7_BDC3_C0554FC10100__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyKnobCtrl.h : header file
//

enum {
	mm_rotwrap,
	mm_rotnowrap,
	mm_linhoriz,
	mm_linvert
};

/////////////////////////////////////////////////////////////////////////////
// CMyKnobCtrl window

class CMyKnobCtrl : public CSliderCtrl
{
// Construction
public:
	CMyKnobCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyKnobCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyKnobCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyKnobCtrl)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void SetMouseMode(int nMode);
	void SetLinearSize(int nSize);
	void SetKnobColour(COLORREF colour);
	COLORREF GetKnobColour();
	void SetTicRadius(int radius);
	void SetNumTics(int numtics);
	virtual void SetPos(int nPos);

private:
	COLORREF m_crKnobColor;
	int m_iTicRadius;
	int m_iTicCount;
	bool m_bDragging;
	bool m_bDragChanged;
	CRgn m_rgn;
	CRect m_crect;
	int m_nMouseMode;
	int m_nLastValue, m_nLinearSize;

	void DrawKnob(CDC *pDC);
	void DrawLine(CDC *pDC);
	void DrawTics(CDC *pDC);

	bool SetKnob(const CPoint& pt);

	virtual void PostMessageToParent(const int nTBCode) const;

	void Init();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYKNOBCTRL_H__EE8D1822_14D6_11D7_BDC3_C0554FC10100__INCLUDED_)
