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

// GroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BTDSys SampleGrid 2 Core.h"
#include "GroupDlg.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg dialog


CGroupDlg::CGroupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupDlg)
	//}}AFX_DATA_INIT
}


void CGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupDlg)
	DDX_Control(pDX, IDC_PEERMINEDIT, m_edPeerMin);
	DDX_Control(pDX, IDC_PEERMINSLIDER, m_slidPeerMin);
	DDX_Control(pDX, IDC_GROUPMUTECOMBO, m_cmbGroupMute);
	DDX_Control(pDX, IDC_PEERMAXEDIT, m_edPeerMax);
	DDX_Control(pDX, IDC_PEERMAXSLIDER, m_slidPeerMax);
	DDX_Control(pDX, IDC_PEERPARAMCOMBO, m_cmbPeerParam);
	DDX_Control(pDX, IDC_PEERMACCOMBO, m_cmbPeerMac);
	DDX_Control(pDX, IDC_PEERCOMBO, m_cmbPeer);
	DDX_Control(pDX, IDC_WAVEOFFSET, m_WaveOffset);
	DDX_Control(pDX, IDC_TRACKSAMPLECOMBO, m_cmbTrackSample);
	DDX_Control(pDX, IDC_MIDIHIGHNOTE, m_btnMIDIHighNote);
	DDX_Control(pDX, IDC_MIDILOWNOTE, m_btnMIDILowNote);
	DDX_Control(pDX, IDC_XORCHECK, m_chkXOR);
	DDX_Control(pDX, IDC_GROUPNAMEEDIT, m_edGroupName);
	DDX_Control(pDX, IDC_OUTCOMBO, m_cmbGroupOut);
	DDX_Control(pDX, IDC_TRACKGROUPCOMBO, m_cmbTrackGroup);
	DDX_Control(pDX, IDC_GROUPCOMBO, m_cmbGroup);
	DDX_Control(pDX, IDC_TRACKCOMBO, m_cmbTrack);
	DDX_Control(pDX, IDC_GROUPMUTEBTN, m_btnGroupMuteKey);
	DDX_Control(pDX, IDC_TRACKKEYBTN, m_btnTrackKey);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupDlg)
	ON_CBN_SELCHANGE(IDC_GROUPCOMBO, OnSelchangeGroupcombo)
	ON_CBN_SELCHANGE(IDC_TRACKCOMBO, OnSelchangeTrackcombo)
	ON_BN_CLICKED(IDC_XORCHECK, OnXorcheck)
	ON_CBN_SELCHANGE(IDC_OUTCOMBO, OnSelchangeOutcombo)
	ON_CBN_SELCHANGE(IDC_TRACKSAMPLECOMBO, OnSelchangeTracksamplecombo)
	ON_WM_MOUSEWHEEL()
	ON_CBN_SELCHANGE(IDC_TRACKGROUPCOMBO, OnSelchangeTrackgroupcombo)
	ON_BN_CLICKED(IDC_GROUPMUTEBTN, OnGroupmutebtn)
	ON_BN_CLICKED(IDC_TRACKKEYBTN, OnTrackkeybtn)
	ON_CBN_SELCHANGE(IDC_PEERMACCOMBO, OnSelchangePeermaccombo)
	ON_CBN_SELCHANGE(IDC_PEERCOMBO, OnSelchangePeercombo)
	ON_CBN_SELCHANGE(IDC_PEERPARAMCOMBO, OnSelchangePeerparamcombo)
	ON_WM_HSCROLL()
	ON_CBN_SELCHANGE(IDC_GROUPMUTECOMBO, OnSelchangeGroupmutecombo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupDlg message handlers

BOOL CGroupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char txt[1024];

	int i;

	for (i=0; i<pmi->nTriggers; i++)
	{
		sprintf(txt,"%02i: %s",i,pmi->Tracks[i].GetSampleName());
		m_cmbTrack.AddString(txt);		
	}

	for (i=0; i<NUM_GROUPS; i++)
	{
		sprintf(txt,"%02i: %s",i,pmi->Groups[i].name);
		m_cmbGroup.AddString(txt);
		m_cmbTrackGroup.AddString(txt);
	}

	m_cmbGroupOut.AddString("0: <Master>");
	for (i=1; i<NUM_OUTPUTS+1; i++)
	{
		sprintf(txt,"%i: %s",i,"TODO name");
		m_cmbGroupOut.AddString(txt);		
	}

	m_cmbTrackSample.AddString(" 000/00 <Auto>");

	for (i=1; i<=200; i++)
	{
		if (pmi->bKitMode)
			sprintf(txt, "%c%03i/%02X %s", pmi->Kit.sample[i].HasData() ? '*' : ' ', i,i,
				pmi->Kit.sample[i].Name);
		else
			sprintf(txt, "%c%03i/%02X %s", pmi->pCB->GetWave(i) ? '*' : ' ', i,i,
				pmi->pCB->GetWaveName(i));

		m_cmbTrackSample.AddString(txt);
	}

/*	CRect rrr; m_lstPeer.GetClientRect(rrr);

	m_lstPeer.InsertColumn(0,"What",LVCFMT_LEFT,rrr.right/3);
	m_lstPeer.InsertColumn(1,"Assign",LVCFMT_LEFT,rrr.right/3);
	m_lstPeer.InsertColumn(2,"Range",LVCFMT_LEFT,rrr.right/3);
	m_lstPeer.InsertItem(0,"Trigger");
	m_lstPeer.InsertItem(1,"Velocity");
	m_lstPeer.InsertItem(2,"Tune");
	m_lstPeer.InsertItem(3,"Offset");
*/

	for (i=0; i<NUM_PEERS; i++)
		m_cmbPeer.AddString(pszPeerNames[i]);

	for (i=0; i<NUM_GROUP_MUTE_MODES; i++)
		m_cmbGroupMute.AddString(pszGroupMuteModes[i]);

	m_cmbPeer.SetCurSel(0);

	pmi->Tracks[0].pCtrl[0]->GetMachineNamesToCombo(
		m_hWnd,IDC_PEERMACCOMBO,pmi->pCB->GetMachineName(pmi->ThisMac));

	m_cmbTrack.SetCurSel(0); OnSelchangeTrackcombo();
	m_cmbGroup.SetCurSel(0); OnSelchangeGroupcombo();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGroupDlg::OnSelchangeGroupcombo() 
{
	int g = m_cmbGroup.GetCurSel();
	m_cmbGroupOut.SetCurSel(pmi->Groups[g].nOutput);
	m_edGroupName.SetWindowText(pmi->Groups[g].name);
	m_chkXOR.SetCheck(pmi->Groups[g].bXOR ? BST_CHECKED : BST_UNCHECKED);
	m_btnGroupMuteKey.SetKey(pmi->Groups[g].nMuteKey,pmi->Groups[g].nMuteKeyType,false);
	m_cmbGroupMute.SetCurSel(pmi->Groups[g].nMuteMode);
}

void CGroupDlg::OnSelchangeTrackcombo() 
{
	int t = m_cmbTrack.GetCurSel();

	m_cmbTrackGroup.SetCurSel(pmi->Tracks[t].Group);


	int i = pmi->FirstWaveNo + t;
	char txt[1024];

	if (pmi->bKitMode)
		sprintf(txt, "%cAuto: %03i/%02X %s", pmi->Kit.sample[i].HasData() ? '*' : ' ', i,i,
			pmi->Kit.sample[i].Name);
	else
		sprintf(txt, "%cAuto: %03i/%02X %s", pmi->pCB->GetWave(i) ? '*' : ' ', i,i,
			pmi->pCB->GetWaveName(i));

	m_cmbTrackSample.InsertString(0,txt);
	m_cmbTrackSample.DeleteString(1);

	m_cmbTrackSample.SetCurSel(pmi->Tracks[t].WaveNum);
	OnSelchangeTracksamplecombo();

	m_WaveOffset.fMin = pmi->Tracks[t].fOffset;
	m_WaveOffset.fMax = pmi->Tracks[t].fLoopEnd;
	m_WaveOffset.RedrawWindow();

	m_btnTrackKey.SetKey(pmi->Tracks[t].nTrigKey, pmi->Tracks[t].nTrigKeyType,false);

	OnSelchangePeercombo();
}

void CGroupDlg::OnMidiNote(int chan, int key, int vel)
{
	if (vel)
	{
		if (m_btnTrackKey.GetState() & 8) 
			m_btnTrackKey.OnMidiKey(chan, key);

		if (m_btnGroupMuteKey.GetState() & 8)
			m_btnGroupMuteKey.OnMidiKey(chan, key);

		if (m_btnMIDILowNote.GetState() & 8)
			m_btnMIDILowNote.OnMidiKey(chan, key);

		if (m_btnMIDIHighNote.GetState() & 8)
			m_btnMIDIHighNote.OnMidiKey(chan, key);
	}
}

void CGroupDlg::OnXorcheck() 
{
	pmi->Groups[m_cmbGroup.GetCurSel()].bXOR = (m_chkXOR.GetCheck() == BST_CHECKED);
}

void CGroupDlg::OnSelchangeOutcombo() 
{
	pmi->Groups[m_cmbGroup.GetCurSel()].nOutput = m_cmbGroupOut.GetCurSel();	
}

void CGroupDlg::OnSelchangeTracksamplecombo() 
{
	int w = m_cmbTrackSample.GetCurSel();
	int t = m_cmbTrack.GetCurSel();
	if (pmi->Tracks[t].WaveNum != w)
		pmi->pCB->ControlChange(pmi->ThisMac,2,t,numWaveNo - NUMGLOBAL, w);

	int w2 = w; if (w2 == 0) w2 = pmi->FirstWaveNo + t;

	m_WaveOffset.pSamples = NULL;

	if (pmi->bKitMode)
	{
	}
	else
	{
		const CWaveInfo  *pwi = pmi->pCB->GetWave(w2);
		const CWaveLevel *pwl = pmi->pCB->GetWaveLevel(w2,0);
		if (pwi && pwl)
		{
			m_WaveOffset.pSamples = pwl->pSamples;
			m_WaveOffset.numSamples = pwl->numSamples;
			m_WaveOffset.bStereo = ((pwi->Flags & WF_STEREO) != 0);
		}
	}

	m_WaveOffset.RedrawWindow();

}

BOOL CGroupDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
//	m_WaveOffset.MouseWheel(nFlags,zDelta,pt);
	
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CGroupDlg::OnSelchangeTrackgroupcombo() 
{
	int g = m_cmbTrackGroup.GetCurSel();
	int t = m_cmbTrack.GetCurSel();
	if (pmi->Tracks[t].Group != g)
		pmi->pCB->ControlChange(pmi->ThisMac,2,t,numGroup - NUMGLOBAL, g);
}


BOOL CGroupDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int t = m_cmbTrack.GetCurSel();
	if (HIWORD(wParam) == EN_CHANGE && LOWORD(wParam) == IDC_WAVEOFFSET)
	{
		int start = (int)(m_WaveOffset.fMin * (float)0x10000);
		clamp(start,0,0xFFFE);

		int loopend = (int)(m_WaveOffset.fMax * (float)0x10000);
		clamp(loopend,0,0xFFFE);

		pmi->pCB->ControlChange(pmi->ThisMac,2,t,numOffset - NUMGLOBAL, start);
		pmi->pCB->ControlChange(pmi->ThisMac,2,t,numLoopEnd - NUMGLOBAL, loopend);
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

void CGroupDlg::OnGroupmutebtn() 
{
	int g = m_cmbGroup.GetCurSel();
	pmi->Groups[g].nMuteKey = m_btnGroupMuteKey.nKey;
	pmi->Groups[g].nMuteKeyType = m_btnGroupMuteKey.nKeyType;
}


void CGroupDlg::OnTrackkeybtn() 
{
	int t = m_cmbTrack.GetCurSel();
	pmi->Tracks[t].nTrigKey = m_btnTrackKey.nKey;
	pmi->Tracks[t].nTrigKeyType = m_btnTrackKey.nKeyType;
}

void CGroupDlg::OnSelchangePeermaccombo() 
{
	char mac[256];
	m_cmbPeerMac.GetWindowText(mac,250);
	CMachine *pmac = pmi->pCB->GetMachine(mac);
	m_cmbPeerParam.ResetContent();
	if (pmac)
		pmi->Tracks[0].pCtrl[0]->GetParamNamesToCombo(
			pmac,m_hWnd,IDC_PEERPARAMCOMBO,nPeerAllowedTypes[m_cmbPeer.GetCurSel()]);

	m_slidPeerMin.EnableWindow(FALSE);
	m_slidPeerMax.EnableWindow(FALSE);

}

void CGroupDlg::OnSelchangePeercombo() 
{
	int t = m_cmbTrack.GetCurSel();
	int p = m_cmbPeer.GetCurSel();

	CPeerCtrl *pc = pmi->Tracks[t].pCtrl[p];
	m_cmbPeerMac.SetCurSel(-1);
	bool b = pc->SelectMachineInCombo(m_hWnd,IDC_PEERMACCOMBO);
	OnSelchangePeermaccombo();
	if (b)
	{
		pc->SelectParameterInCombo(m_hWnd,IDC_PEERPARAMCOMBO);
		OnSelchangePeerparamcombo();
		dbgprintf("%i %i",pmi->Tracks[t].pCtrl[p]->min,pmi->Tracks[t].pCtrl[p]->max);
		m_slidPeerMin.SetPos(pc->min);
		m_slidPeerMax.SetPos(pc->max);
		RedisplayParam(pc, &m_slidPeerMin, &m_edPeerMin);
		RedisplayParam(pc, &m_slidPeerMax, &m_edPeerMax);
	}

}

void CGroupDlg::OnSelchangePeerparamcombo() 
{
	char macname[256], partext[256];
	m_cmbPeerMac.GetWindowText(macname,250);
	m_cmbPeerParam.GetWindowText(partext,250);

	int t = m_cmbTrack.GetCurSel();
	int p = m_cmbPeer.GetCurSel();

	CPeerCtrl *pc = pmi->Tracks[t].pCtrl[p];
	pc->UnassignParameter();
	pc->AssignParameter(macname,partext);

	CMachineParameter const *pPar = pc->GetParamInfo();

	if (pPar)
	{
		int min = pPar->MinValue; if (min == -1) min = 0;
		int max = pPar->MaxValue; if (max == -1) max = 1;

		m_slidPeerMin.EnableWindow();
		m_slidPeerMin.SetRange(min,max,TRUE);
		RedisplayParam(pc, &m_slidPeerMin, &m_edPeerMin);
	//	OnHScroll(0,0,(CScrollBar *)&m_slidPeerMin); // <-- this is resetting the minixmxixaqaaaa
		m_slidPeerMax.EnableWindow();
		m_slidPeerMax.SetRange(min,max,TRUE);
		RedisplayParam(pc, &m_slidPeerMax, &m_edPeerMax);
	//	OnHScroll(0,0,(CScrollBar *)&m_slidPeerMax);
	
	}
}

void CGroupDlg::RedisplayParam(CPeerCtrl *pc, CSliderCtrl *pSliderCtrl, CEdit *pEdit, int *val)
{
	int nPos = pSliderCtrl->GetPos();

	char txt[256], tx2[16];
	const char *dv = pc->DescribeValue(nPos);
	if (!dv)
	{
		CMachineParameter const *pPar = pc->GetParamInfo();

		if (pPar->Type == pt_note)
		{
			static char *nn[] = {"??","C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-","??","??","??","??"};

			sprintf(tx2,"%s%i",nn[nPos&15],nPos>>4);
		}
		else
			sprintf(tx2,"0x%X",nPos);

		dv = tx2;
		

	}
	sprintf(txt,"%s [%i]",dv,nPos);

	pEdit->SetWindowText(txt);

	if (val) *val = nPos;
}

void CGroupDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	int t = m_cmbTrack.GetCurSel();
	int p = m_cmbPeer.GetCurSel();

	if (pScrollBar == (CScrollBar*)&m_slidPeerMax)
		RedisplayParam(pmi->Tracks[t].pCtrl[p], &m_slidPeerMax, &m_edPeerMax, &pmi->Tracks[t].pCtrl[p]->max);
	else if (pScrollBar == (CScrollBar*)&m_slidPeerMin)
		RedisplayParam(pmi->Tracks[t].pCtrl[p], &m_slidPeerMin, &m_edPeerMin, &pmi->Tracks[t].pCtrl[p]->min);

/*	nPos = ((CSliderCtrl *)pScrollBar)->GetPos();

//	nPos = m_slidPeerMax.GetPos();
	char txt[256], tx2[16];
	const char *dv = pmi->Tracks[t].pCtrl[p]->DescribeValue(nPos);
	if (!dv)
	{
		CMachineParameter const *pPar = pmi->Tracks[t].pCtrl[p]->GetParamInfo();

		if (pPar->Type == pt_note)
		{
			static char *nn[] = {"??","C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-","??","??","??","??"};

			sprintf(tx2,"%s%i",nn[nPos&15],nPos>>4);
		}
		else
			sprintf(tx2,"0x%X",nPos);

		dv = tx2;
		

	}
	sprintf(txt,"%s [%i]",dv,nPos);

	if (pScrollBar == (CScrollBar*)&m_slidPeerMax)
	{
		m_edPeerMax.SetWindowText(txt);	
		pmi->Tracks[t].pCtrl[p]->max = nPos;
	}
	else if (pScrollBar == (CScrollBar*)&m_slidPeerMin)
	{
		m_edPeerMin.SetWindowText(txt);		
		pmi->Tracks[t].pCtrl[p]->min = nPos;
	}

		dbgprintf("xx %i %i",pmi->Tracks[t].pCtrl[p]->min,pmi->Tracks[t].pCtrl[p]->max);
*/	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CGroupDlg::OnSelchangeGroupmutecombo() 
{
	int g = m_cmbGroup.GetCurSel();

	pmi->Groups[g].nMuteMode = m_cmbGroupMute.GetCurSel();
	
}
