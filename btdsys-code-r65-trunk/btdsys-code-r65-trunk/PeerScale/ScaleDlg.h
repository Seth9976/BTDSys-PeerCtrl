/*
Copyright (c) 2005-2011, Edward Powley
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

#pragma once

#include "..\Win32++\include\dialog.h"
#include "..\Win32++\include\controls.h"
#include "..\Win32++\include\stdcontrols.h"

// ScaleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog

class mi;

class CScaleDlg : public CDialog
{
// Construction
public:
	CScaleDlg(CWnd* pParent = NULL);
	~CScaleDlg();

	mi *pmi;

	void RepopulateList();

	bool ScaleFitsNoteConstraints(SScale *s);

	CButton *m_chkNotes[12];
	CStatic *m_stNotes[12];

	int m_SelNoteMask;

	enum { IDD = IDD_SCALEDLG };
	CStatic	m_stGS;
	CStatic	m_stG;
	CStatic	m_stFS;
	CStatic	m_stF;
	CStatic	m_stE;
	CStatic	m_stDS;
	CStatic	m_stD;
	CStatic	m_stCS;
	CStatic	m_stC;
	CStatic	m_stB;
	CStatic	m_stAS;
	CStatic	m_stA;
	CComboBox	m_cmbRoot;
	CComboBox	m_cmbNotes;
	CButton	m_chkGS;
	CButton	m_chkG;
	CButton	m_chkFS;
	CButton	m_chkF;
	CButton	m_chkE;
	CButton	m_chkDS;
	CButton	m_chkD;
	CButton	m_chkCS;
	CButton	m_chkC;
	CButton	m_chkB;
	CButton	m_chkAS;
	CButton	m_chkA;
	CEdit	m_edLongDisplay;
	CEdit	m_edSearch;
	CListBox	m_List;

// Implementation
protected:

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	void OnChangeSearch();
	void OnSelchangeList();
	virtual void OnOK();
	virtual void OnCancel();
	void OnNotesChanged();
	void OnSelchangeNotescombo();
	void OnSelchangeRootcombo();
};
