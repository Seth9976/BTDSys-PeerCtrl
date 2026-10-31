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

// ScaleDlg.cpp : implementation file
//

#include "peerscale.h"
#include "machine.h"
#include "ScaleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg dialog


CScaleDlg::CScaleDlg(CWnd* pParent /*=NULL*/)
:
CDialog(CScaleDlg::IDD, pParent)
{
}

CScaleDlg::~CScaleDlg()
{
}

/////////////////////////////////////////////////////////////////////////////
// CScaleDlg message handlers

BOOL CScaleDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	HWND hCtrl = (HWND) lParam;
	WORD ctrlId = LOWORD(wParam);
	WORD notifyCode = HIWORD(wParam);

	switch(ctrlId)
	{
	case IDC_SEARCH :
		if(notifyCode == EN_CHANGE)
		{
			OnChangeSearch();
		}
		break;

	case IDC_LIST1 :
		if(notifyCode == LBN_SELCHANGE)
		{
			OnSelchangeList();
		}
		break;

	case IDC_A :
	case IDC_AS :
	case IDC_B :
	case IDC_C :
	case IDC_CS :
	case IDC_D :
	case IDC_DS :
	case IDC_E :
	case IDC_F :
	case IDC_FS :
	case IDC_G :
	case IDC_GS :
		if(notifyCode == BN_CLICKED)
		{
			OnNotesChanged();
		}
		break;

	case IDC_NOTESCOMBO :
		if(notifyCode == CBN_SELCHANGE)
		{
			OnSelchangeNotescombo();
		}
		break;

	case IDC_ROOTCOMBO :
		if(notifyCode == CBN_SELCHANGE)
		{
			OnSelchangeRootcombo();
		}
		break;

	case IDOK:
		OnOK();
		break;

	case IDCANCEL :
		OnCancel();
		break;

	default :
		return FALSE;
	}

	return TRUE;
}

BOOL CScaleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// Attach controls
	m_stC.AttachDlgItem(IDC_SC, this);
	m_stCS.AttachDlgItem(IDC_SCS, this);
	m_stD.AttachDlgItem(IDC_SD, this);
	m_stDS.AttachDlgItem(IDC_SDS, this);
	m_stE.AttachDlgItem(IDC_SE, this);
	m_stF.AttachDlgItem(IDC_SF, this);
	m_stFS.AttachDlgItem(IDC_SFS, this);
	m_stG.AttachDlgItem(IDC_SG, this);
	m_stGS.AttachDlgItem(IDC_SGS, this);
	m_stA.AttachDlgItem(IDC_SA, this);
	m_stAS.AttachDlgItem(IDC_SAS, this);
	m_stB.AttachDlgItem(IDC_SB, this);
	
	m_cmbRoot.AttachDlgItem(IDC_ROOTCOMBO, this);
	m_cmbNotes.AttachDlgItem(IDC_NOTESCOMBO, this);

	m_chkC.AttachDlgItem(IDC_C, this);
	m_chkCS.AttachDlgItem(IDC_CS, this);
	m_chkD.AttachDlgItem(IDC_D, this);
	m_chkDS.AttachDlgItem(IDC_DS, this);
	m_chkE.AttachDlgItem(IDC_E, this);
	m_chkF.AttachDlgItem(IDC_F, this);
	m_chkFS.AttachDlgItem(IDC_FS, this);
	m_chkG.AttachDlgItem(IDC_G, this);
	m_chkGS.AttachDlgItem(IDC_GS, this);
	m_chkA.AttachDlgItem(IDC_A, this);
	m_chkAS.AttachDlgItem(IDC_AS, this);
	m_chkB.AttachDlgItem(IDC_B, this);
	
	m_edLongDisplay.AttachDlgItem(IDC_LONGDISPLAY, this);
	m_edSearch.AttachDlgItem(IDC_SEARCH, this);
	m_List.AttachDlgItem(IDC_LIST1, this);

	// Set up note controls pointers
	m_chkNotes[ 0] = &m_chkC ;	m_stNotes[ 0] = &m_stC ;
	m_chkNotes[ 1] = &m_chkCS;	m_stNotes[ 1] = &m_stCS;
	m_chkNotes[ 2] = &m_chkD ;	m_stNotes[ 2] = &m_stD ;
	m_chkNotes[ 3] = &m_chkDS;	m_stNotes[ 3] = &m_stDS;
	m_chkNotes[ 4] = &m_chkE ;	m_stNotes[ 4] = &m_stE ;
	m_chkNotes[ 5] = &m_chkF ;	m_stNotes[ 5] = &m_stF ;
	m_chkNotes[ 6] = &m_chkFS;	m_stNotes[ 6] = &m_stFS;
	m_chkNotes[ 7] = &m_chkG ;	m_stNotes[ 7] = &m_stG ;
	m_chkNotes[ 8] = &m_chkGS;	m_stNotes[ 8] = &m_stGS;
	m_chkNotes[ 9] = &m_chkA ;	m_stNotes[ 9] = &m_stA ;
	m_chkNotes[10] = &m_chkAS;	m_stNotes[10] = &m_stAS;
	m_chkNotes[11] = &m_chkB ;	m_stNotes[11] = &m_stB ;

	m_SelNoteMask = 0;

	RepopulateList();
	m_List.SetCurSel(pmi->scale);

	m_cmbNotes.AddString("One or more of");
	m_cmbNotes.AddString("All of");
	m_cmbNotes.AddString("A subset of");
	m_cmbNotes.AddString("Exactly");
	m_cmbNotes.SetCurSel(0);

	LPCTSTR noteNames[] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
	for(int i = 0; i < 12; i++)
	{
		m_cmbRoot.AddString(noteNames[i]);
	}
	m_cmbRoot.SetCurSel(pmi->root);

	OnSelchangeList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool chricmp(char a, char b)
{
	char tmp[3];
	tmp[0] = a;
	tmp[1] = b;
	tmp[2] = 0;
	strlwr(tmp);
	return tmp[0] == tmp[1];
}

char *stristr(char *haystack, char *needle)
{
	for (; *haystack; haystack++)
	{
		bool b = true;

		for (char *n2 = needle, *h2 = haystack; *n2 && b; n2++, h2++)
		{
			if (!chricmp(*h2,*n2)) b = false;
		}

		if (b) return haystack;
	}

	return NULL;
}

void CScaleDlg::RepopulateList()
{
	// get current selection
	char str[1024]; str[0] = 0;
	m_List.GetText(m_List.GetCurSel(),str);
	int sel = atoi(str);

	// clear the list
	m_List.ResetContent();

	// get search box text
	tString srch = m_edSearch.GetWindowText();

	// populate the list
	for (int s = 0; s < MAX_SCALES; s++)
	{
		if (pmi->scales[s].notes[0]) // a valid scale
		{
			if (stristr(pmi->scales[s].name, (char*) srch.c_str()) // check against search text
				&& ScaleFitsNoteConstraints(&pmi->scales[s]))
			{
				char txt[1024];
				sprintf(txt,"%i: %s", s, pmi->scales[s].name);
				int item = m_List.AddString(txt);
				if (s == sel)
					m_List.SetCurSel(item);
			}
		}
	}

	OnSelchangeList();
}

bool CScaleDlg::ScaleFitsNoteConstraints(SScale *s)
{
	// we use bit masks, because sanity is boring and micro-optimisation is awesome

	// construct the bit mask corresp to the scale
	int root = m_cmbRoot.GetCurSel();
	int scalenotes = 1 << root;
	for (int i=0; i<12; i++)
		scalenotes |= 1 << ((root + s->notes[i]) % 12);

	// now the easy bit: compare them
	switch (m_cmbNotes.GetCurSel())
	{
	case 0: // One or more of
		if (m_SelNoteMask == 0) return true;
		return (m_SelNoteMask & scalenotes) != 0;
		
	case 1: // All of
		return (m_SelNoteMask & scalenotes) == m_SelNoteMask;

	case 2: // A subset of
		return (m_SelNoteMask & scalenotes) == scalenotes;

	case 3: // Exactly
		return m_SelNoteMask == scalenotes;

	default: // wtf?
		return true;
	}
}

void CScaleDlg::OnChangeSearch() 
{
	RepopulateList();	
}

void CScaleDlg::OnSelchangeList() 
{
	char str[1024]; str[0] = 0;
	m_List.GetText(m_List.GetCurSel(),str);
	m_edLongDisplay.SetWindowText(str);
	
	// display notes
	int i;
	for (i=0; i<12; i++)
		m_stNotes[i]->ShowWindow(SW_HIDE);

	SScale *s = &pmi->scales[atoi(str)];

	int root = m_cmbRoot.GetCurSel();
	for (i=0; i<12; i++)
		m_stNotes[(root + s->notes[i]) % 12]->ShowWindow(SW_SHOW);

}

void CScaleDlg::OnOK() 
{
	char str[1024]; str[0] = 0;
	int sel = m_List.GetCurSel();
	if (sel != LB_ERR)
	{
		m_List.GetText(sel,str);
		sel = atoi(str);
		pmi->pCB->ControlChange(pmi->ThisMac,1,0,numScale,sel);
	}

	sel = m_cmbRoot.GetCurSel();
	if (sel != CB_ERR)
	{
		pmi->pCB->ControlChange(pmi->ThisMac,1,0,numRoot,sel);
	}
	
	CDialog::OnOK();
}

void CScaleDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void CScaleDlg::OnNotesChanged() 
{
	// construct the bit mask corresp to the selection in the dialog
	m_SelNoteMask = 0;
	for (int i=0; i<12; i++)
		if (m_chkNotes[i]->GetCheck() == BST_CHECKED)
			m_SelNoteMask |= 1 << i;

	RepopulateList();
}

void CScaleDlg::OnSelchangeNotescombo() 
{
	RepopulateList();
}

void CScaleDlg::OnSelchangeRootcombo() 
{
	RepopulateList();
}
