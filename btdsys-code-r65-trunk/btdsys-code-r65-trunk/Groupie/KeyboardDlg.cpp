/*
Copyright (c) 2003-2006, Edward Powley
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

// KeyboardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "btdsys groupie.h"
#include "KeyboardDlg.h"
#include "paramdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyboardDlg dialog


CKeyboardDlg::CKeyboardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyboardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyboardDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKeyboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyboardDlg)
	DDX_Control(pDX, IDC_OCTAVE, m_cmbOctave);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKeyboardDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyboardDlg)
	ON_BN_CLICKED(IDC_NOTE_C1, OnNoteC1)
	ON_BN_CLICKED(IDC_NOTE_CS1, OnNoteCs1)
	ON_BN_CLICKED(IDC_NOTE_D1, OnNoteD1)
	ON_BN_CLICKED(IDC_NOTE_DS1, OnNoteDs1)
	ON_BN_CLICKED(IDC_NOTE_E1, OnNoteE1)
	ON_BN_CLICKED(IDC_NOTE_F1, OnNoteF1)
	ON_BN_CLICKED(IDC_NOTE_FS1, OnNoteFs1)
	ON_BN_CLICKED(IDC_NOTE_G1, OnNoteG1)
	ON_BN_CLICKED(IDC_NOTE_GS1, OnNoteGs1)
	ON_BN_CLICKED(IDC_NOTE_A1, OnNoteA1)
	ON_BN_CLICKED(IDC_NOTE_AS1, OnNoteAs1)
	ON_BN_CLICKED(IDC_NOTE_B1, OnNoteB1)
	ON_BN_CLICKED(IDC_NOTE_C2, OnNoteC2)
	ON_BN_CLICKED(IDC_NOTE_CS2, OnNoteCs2)
	ON_BN_CLICKED(IDC_NOTE_D2, OnNoteD2)
	ON_BN_CLICKED(IDC_NOTE_DS2, OnNoteDs2)
	ON_BN_CLICKED(IDC_NOTE_E2, OnNoteE2)
	ON_BN_CLICKED(IDC_NOTE_F2, OnNoteF2)
	ON_BN_CLICKED(IDC_NOTE_FS2, OnNoteFs2)
	ON_BN_CLICKED(IDC_NOTE_G2, OnNoteG2)
	ON_BN_CLICKED(IDC_NOTE_GS2, OnNoteGs2)
	ON_BN_CLICKED(IDC_NOTE_A2, OnNoteA2)
	ON_BN_CLICKED(IDC_NOTE_AS2, OnNoteAs2)
	ON_BN_CLICKED(IDC_NOTE_B2, OnNoteB2)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_NOTEOFF, OnNoteoff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyboardDlg message handlers

void CKeyboardDlg::OnOK() 
{
}

void CKeyboardDlg::OnCancel() 
{
}

BOOL CKeyboardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cmbOctave.AddString("0/1");
	m_cmbOctave.AddString("1/2");
	m_cmbOctave.AddString("2/3");
	m_cmbOctave.AddString("3/4");
	m_cmbOctave.AddString("4/5");
	m_cmbOctave.AddString("5/6");
	m_cmbOctave.AddString("6/7");
	m_cmbOctave.AddString("7/8");
	m_cmbOctave.AddString("8/9");
	m_cmbOctave.SetCurSel(4);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKeyboardDlg::Note(int n)
{
	n += m_cmbOctave.GetCurSel()*16;

	pParamDlg->SendParamValue(nBar,n);
}

void CKeyboardDlg::OnNoteC1()	{Note(0x01);}
void CKeyboardDlg::OnNoteCs1() 	{Note(0x02);}
void CKeyboardDlg::OnNoteD1() 	{Note(0x03);}
void CKeyboardDlg::OnNoteDs1() 	{Note(0x04);}
void CKeyboardDlg::OnNoteE1() 	{Note(0x05);}
void CKeyboardDlg::OnNoteF1() 	{Note(0x06);}
void CKeyboardDlg::OnNoteFs1() 	{Note(0x07);}
void CKeyboardDlg::OnNoteG1() 	{Note(0x08);}
void CKeyboardDlg::OnNoteGs1() 	{Note(0x09);}
void CKeyboardDlg::OnNoteA1() 	{Note(0x0A);}
void CKeyboardDlg::OnNoteAs1() 	{Note(0x0B);}
void CKeyboardDlg::OnNoteB1() 	{Note(0x0C);}
void CKeyboardDlg::OnNoteC2() 	{Note(0x11);}
void CKeyboardDlg::OnNoteCs2() 	{Note(0x12);}
void CKeyboardDlg::OnNoteD2() 	{Note(0x13);}
void CKeyboardDlg::OnNoteDs2() 	{Note(0x14);}
void CKeyboardDlg::OnNoteE2() 	{Note(0x15);}
void CKeyboardDlg::OnNoteF2() 	{Note(0x16);}
void CKeyboardDlg::OnNoteFs2() 	{Note(0x17);}
void CKeyboardDlg::OnNoteG2() 	{Note(0x18);}
void CKeyboardDlg::OnNoteGs2() 	{Note(0x19);}
void CKeyboardDlg::OnNoteA2() 	{Note(0x1A);}
void CKeyboardDlg::OnNoteAs2() 	{Note(0x1B);}
void CKeyboardDlg::OnNoteB2() 	{Note(0x1C);}

void CKeyboardDlg::OnClose() 
{
	CDialog::OnClose();

	*pMyRef = NULL;

	delete this;

}

void CKeyboardDlg::OnNoteoff() 
{
	pParamDlg->SendParamValue(nBar,255);
}

BOOL CKeyboardDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (pMsg->message == WM_KEYDOWN)
	{
		OutputDebugString("OYI");
		switch (pMsg->wParam)
		{
		case 'z':	Note(0x01);	break;
		case 's':	Note(0x02);	break;
		case 'x':	Note(0x03);	break;
		case 'd':	Note(0x04);	break;
		case 'c':	Note(0x05);	break;
		case 'v':	Note(0x06);	break;
		case 'g':	Note(0x07);	break;
		case 'b':	Note(0x08);	break;
		case 'h':	Note(0x09);	break;
		case 'n':	Note(0x0A);	break;
		case 'j':	Note(0x0B);	break;
		case 'm':	Note(0x0C);	break;
		case 'q':	Note(0x11);	break;
		case '2':	Note(0x12);	break;
		case 'w':	Note(0x13);	break;
		case '3':	Note(0x14);	break;
		case 'e':	Note(0x15);	break;
		case 'r':	Note(0x16);	break;
		case '5':	Note(0x17);	break;
		case 't':	Note(0x18);	break;
		case '6':	Note(0x19);	break;
		case 'y':	Note(0x1A);	break;
		case '7':	Note(0x1B);	break;
		case 'u':	Note(0x1C);	break;
		case '1':	OnNoteoff();break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
