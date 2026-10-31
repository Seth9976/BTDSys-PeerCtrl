/*
MidiDialog.cpp: MIDI settings dialog
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

#include "stdafx.h"
#include "BTDSys PeerCtrl.h"
#include "MidiDialog.h"
#include "ctrl.h"
#include <mmsystem.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMidiDialog dialog


CMidiDialog::CMidiDialog(int cnum, CWnd* pParent /*=NULL*/)
	: CDialog(CMidiDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMidiDialog)
	//}}AFX_DATA_INIT

	ctrlnum = cnum;
}


void CMidiDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMidiDialog)
	DDX_Control(pDX, IDC_OUTDEVCOMBO, m_OutDevCombo);
	DDX_Control(pDX, IDC_LASTCONTROLLER, m_LastControllerDisplay);
	DDX_Control(pDX, IDC_INCDECWRAPCHECK, m_IncDecWrapCheck);
	DDX_Control(pDX, IDC_INCDECCHECK, m_IncDecCheck);
	DDX_Control(pDX, IDC_FEEDBACKCHECK, m_FeedbackCheck);
	DDX_Control(pDX, IDC_CHANNELCOMBO, m_ChannelCombo);
	DDX_Control(pDX, IDC_CCCOMBO, m_CCCombo);
	DDX_Control(pDX, IDC_ASSIGNINFO, m_AssignInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMidiDialog, CDialog)
	//{{AFX_MSG_MAP(CMidiDialog)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CCCOMBO, OnSelchangeCccombo)
	ON_CBN_SELCHANGE(IDC_CHANNELCOMBO, OnSelchangeChannelcombo)
	ON_BN_CLICKED(IDC_FEEDBACKCHECK, OnFeedbackcheck)
	ON_BN_CLICKED(IDC_INCDECCHECK, OnIncdeccheck)
	ON_BN_CLICKED(IDC_INCDECWRAPCHECK, OnIncdecwrapcheck)
	ON_CBN_SELCHANGE(IDC_OUTDEVCOMBO, OnSelchangeOutdevcombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMidiDialog message handlers

CMidiDialog *g_mdlg;

VOID CALLBACK TimerProc( HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime )
{
/*	JOYINFOEX ji;
	ji.dwSize = sizeof(ji);
	ji.dwFlags = JOY_RETURNALL;

	if (joyGetPosEx(JOYSTICKID1, &ji) == JOYERR_NOERROR)
	{
		switch (g_mdlg->ptrack->JoyItemType)
		{
		case 'm': //mom button
			g_mdlg->m_JoyAxisDisplay.SetPos(
				(ji.dwButtons >> g_mdlg->ptrack->JoyItem) & 1);
			break;
		case 't': //toogle button
			g_mdlg->m_JoyAxisDisplay.SetPos(
				(ji.dwButtons >> g_mdlg->ptrack->JoyItem) & 1);
			break;
		case 'A': //axis
			switch (g_mdlg->ptrack->JoyItem)
			{
			case 'X': g_mdlg->m_JoyAxisDisplay.SetPos(ji.dwXpos); break;
			case 'Y': g_mdlg->m_JoyAxisDisplay.SetPos(ji.dwYpos); break;
			case 'Z': g_mdlg->m_JoyAxisDisplay.SetPos(ji.dwZpos); break;
			case 'R': g_mdlg->m_JoyAxisDisplay.SetPos(ji.dwRpos); break;
			case 'U': g_mdlg->m_JoyAxisDisplay.SetPos(ji.dwUpos); break;
			case 'V': g_mdlg->m_JoyAxisDisplay.SetPos(ji.dwVpos); break;
			}
		}
	}
*/

//	g_mdlg->m_JoyAxisDisplay.SetPos(g_mdlg->ptrack->JoyCurrentPos * 1000);

//	SetTimer(hwnd, 1,100,TimerProc);
}
/*
void CMidiDialog::PopulateJoystickInfo(UINT id)
{
	char jn[8];
	sprintf(jn, "Joy%i", id+1);

	JOYCAPS cap;
	if (joyGetDevCaps(id, &cap, sizeof(cap)) == JOYERR_NOERROR)
	{
		char txt[128];

		sprintf(txt, "%s X-Axis", jn); m_JoyCombo.AddString(txt);
		sprintf(txt, "%s Y-Axis", jn); m_JoyCombo.AddString(txt);

		if (cap.wCaps & JOYCAPS_HASZ)
			{sprintf(txt, "%s Z-Axis", jn); m_JoyCombo.AddString(txt);}
		if (cap.wCaps & JOYCAPS_HASR)
			{sprintf(txt, "%s R-Axis", jn); m_JoyCombo.AddString(txt);}
		if (cap.wCaps & JOYCAPS_HASU)
			{sprintf(txt, "%s U-Axis", jn); m_JoyCombo.AddString(txt);}
		if (cap.wCaps & JOYCAPS_HASV)
			{sprintf(txt, "%s V-Axis", jn); m_JoyCombo.AddString(txt);}

		for (int i=0; i<cap.wNumButtons; i++)
		{
			sprintf(txt, "%s Button %i Momentary", jn, i+1);
			m_JoyCombo.AddString(txt);

			sprintf(txt, "%s Button %i Toggle",    jn, i+1);
			m_JoyCombo.AddString(txt);
		}		
	}
}
*/
BOOL CMidiDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	g_mdlg = this;

	pmi->pmididlg = this;

	char txt[256];
	sprintf(txt, "%s, track %i (current assign: %s)",
		pmi->pCB->GetMachineName(pmi->ThisMac),
		ptrack->tracknum,
		ptrack->peerctrl[ctrlnum]->GetAssignmentString());

	m_AssignInfo.SetWindowText(txt);
	
//	m_AntiJitter.SetRange(0,1000);
//	m_AntiJitter.SetTicFreq(50);
//	m_AntiJitter.SetPos(ptrack->JoyAntiJitterThreshold * 5000);
//	sprintf(txt, "%.1f%%", ptrack->JoyAntiJitterThreshold * 100.0f);
//	m_AntiJitterDisplay.SetWindowText(txt);

	//populate controller box

	m_CCCombo.ResetContent();
	m_CCCombo.AddString("None");
	for (int i=0; i<128; i++)
	{
		m_CCCombo.AddString(_itoa(i,txt,10));
	}
	m_CCCombo.AddString("Program Change");
	m_CCCombo.AddString("Pitch Bend");

	int c;
	if (ptrack->MIDIController < 128) c = ptrack->MIDIController + 1;
	else c = ptrack->MIDIController - 125;
	m_CCCombo.SetCurSel(c);

	//populate channel box

	m_ChannelCombo.ResetContent();
	m_ChannelCombo.AddString("None");
	for (c=1; c<=16; c++)
		m_ChannelCombo.AddString(_itoa(c,txt,10));
	m_ChannelCombo.AddString("All");

	m_ChannelCombo.SetCurSel(ptrack->MIDIChannel);

	m_FeedbackCheck.SetCheck(ptrack->MIDIFeedback ?1:0);
	m_IncDecCheck.SetCheck  (ptrack->MIDIIncDec   ?1:0);
	m_IncDecWrapCheck.SetCheck  (ptrack->MIDIIncDecWrap   ?1:0);

	//populate midi out box

	m_OutDevCombo.ResetContent();
	
	MIDIOUTCAPS moc;

	for (unsigned int d=0; d < midiOutGetNumDevs(); d++)
	{
		midiOutGetDevCaps(d, &moc, sizeof(moc));
		m_OutDevCombo.AddString(moc.szPname);
	}

	m_OutDevCombo.SetCurSel(ptrack->MIDIFBOutDev);

	//populate joystick stuff

/*	m_JoyCombo.AddString("None");

	PopulateJoystickInfo(JOYSTICKID1);
	PopulateJoystickInfo(JOYSTICKID2);

	if (ptrack->JoyItemType == 0)
		strcpy(txt,"None");
	else
	{
		sprintf(txt, "Joy%i", ptrack->JoyID + 1);
		if (ptrack->JoyItemType == 'A')
			sprintf(txt, "%s %c-Axis", txt, ptrack->JoyItem);
		else
			sprintf(txt, "%s Button %i %c", txt, ptrack->JoyItem+1, ptrack->JoyItemType);
	}

	OutputDebugString(txt);

	if (m_JoyCombo.SelectString(-1,txt) == CB_ERR)
		m_JoyCombo.SetCurSel(0);

	OnSelchangeJoycombo();

	SetTimer(1,100,TimerProc);
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMidiDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
	KillTimer(1);

	pmi->pmididlg = NULL;
}


void CMidiDialog::OnSelchangeCccombo() 
{
	// TODO: Add your control notification handler code here
	
	int c = m_CCCombo.GetCurSel();
	switch (c)
	{
	case CB_ERR:
	case 0: //none
		ptrack->MIDIController = -1;
		break;
	case 129: //prog change
		ptrack->MIDIController = 254;
		break;
	case 130: //pitchbend
		ptrack->MIDIController = 255;
		break;
	default: //any other CC
		ptrack->MIDIController = c - 1;
	}
}


void CMidiDialog::OnSelchangeChannelcombo() 
{
	// TODO: Add your control notification handler code here
	
	ptrack->MIDIChannel = m_ChannelCombo.GetCurSel();
}

void CMidiDialog::OnFeedbackcheck() 
{
	// TODO: Add your control notification handler code here
	
	ptrack->MIDIFeedback = (m_FeedbackCheck.GetCheck() != 0);
	ptrack->LastFBCCValue = -1;
}

void CMidiDialog::OnIncdeccheck() 
{
	// TODO: Add your control notification handler code here
	
	ptrack->MIDIIncDec = (m_IncDecCheck.GetCheck() != 0);
}

void CMidiDialog::OnSelchangeOutdevcombo() 
{
	// TODO: Add your control notification handler code here
	
	ptrack->MIDIFBOutDev = m_OutDevCombo.GetCurSel();
	ptrack->LastFBCCValue = -1;
}

void CMidiDialog::OnIncdecwrapcheck() 
{
	// TODO: Add your control notification handler code here
	
	ptrack->MIDIIncDecWrap = (m_IncDecWrapCheck.GetCheck() != 0);
}

/*void CMidiDialog::OnSelchangeJoycombo() 
{
	// TODO: Add your control notification handler code here
	m_JoyAxisDisplay.SetRange(0, 1000);

	char txt[255];
	m_JoyCombo.GetWindowText(txt, 255);

	if (strcmp(txt,"None") == 0)
		ptrack->JoyID = ptrack->JoyItem = ptrack->JoyItemType = 0;
	else
	{
		ptrack->JoyID = txt[3] - '1';

		JOYCAPS cap;
		if (joyGetDevCaps(ptrack->JoyID, &cap, sizeof(cap)) == JOYERR_NOERROR)
		{
			switch (txt[5])
			{
			case 'X':
				ptrack->JoyItemType = 'A';
				ptrack->JoyItem = 'X';
				ptrack->JoyMin = cap.wXmin;
				ptrack->JoyMax = cap.wXmax;
				break;
			case 'Y':
				ptrack->JoyItemType = 'A';
				ptrack->JoyItem = 'Y';
				ptrack->JoyMin = cap.wYmin;
				ptrack->JoyMax = cap.wYmax;
				break;
			case 'Z':
				ptrack->JoyItemType = 'A';
				ptrack->JoyItem = 'Z';
				ptrack->JoyMin = cap.wZmin;
				ptrack->JoyMax = cap.wZmax;
				break;
			case 'R':
				ptrack->JoyItemType = 'A';
				ptrack->JoyItem = 'R';
				ptrack->JoyMin = cap.wRmin;
				ptrack->JoyMax = cap.wRmax;
				break;
			case 'U':
				ptrack->JoyItemType = 'A';
				ptrack->JoyItem = 'U';
				ptrack->JoyMin = cap.wUmin;
				ptrack->JoyMax = cap.wUmax;
				break;
			case 'V':
				ptrack->JoyItemType = 'A';
				ptrack->JoyItem = 'V';
				ptrack->JoyMin = cap.wVmin;
				ptrack->JoyMax = cap.wVmax;
				break;
			case 'B':
				ptrack->JoyItem = atoi(txt + 12) - 1;
				ptrack->JoyItemType = txt[(ptrack->JoyItem >= 10) ? 15 : 14];
				break;
			default:
				ptrack->JoyItem = ptrack->JoyItemType = 0;
			}
		}
	}
}
*/
/*void CMidiDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (pScrollBar->GetDlgCtrlID() == IDC_ANTIJITTER)
	{
		ptrack->JoyAntiJitterThreshold = m_AntiJitter.GetPos() / 5000.0f;
		char txt[16];
		sprintf(txt, "%.1f%%", ptrack->JoyAntiJitterThreshold * 100.0f);
		m_AntiJitterDisplay.SetWindowText(txt);
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

*/