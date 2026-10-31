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

#include "stdafx.h"
#include "BTDSys PeerCtrl.h"
#include "SettingsDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "ctrl.h"
#include "mididialog.h"
#include "pluginaxesdialog.h"
//#include "nibblesdlg.h"
#include "mappingdialog.h"


/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog dialog


CSettingsDialog::CSettingsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsDialog)
	//}}AFX_DATA_INIT

	mypc = NULL;
}

CSettingsDialog::~CSettingsDialog()
{
	if (mypc) delete mypc;
}

void CSettingsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsDialog)
	DDX_Control(pDX, IDC_ASSIGNTREE, m_AssignTree);
	DDX_Control(pDX, IDC_TRACKCOMBO, m_TrackCombo);
	DDX_Control(pDX, IDC_PARAMLIST, m_ParamList);
	DDX_Control(pDX, IDC_MACHINECOMBO, m_MachineCombo);
	DDX_Control(pDX, IDC_MAPPINGENVELOPE, m_MappingEnvelope);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsDialog, CDialog)
	//{{AFX_MSG_MAP(CSettingsDialog)
	ON_CBN_SELCHANGE(IDC_MACHINECOMBO, OnMachinecomboSelchange)
	ON_BN_CLICKED(IDC_CLEARBUTTON, OnClearbutton)
	ON_BN_CLICKED(IDC_RESETBUTTON, OnResetbutton)
	ON_LBN_SELCHANGE(IDC_PARAMLIST, OnParamlistSelchange)
	ON_CBN_SELCHANGE(IDC_TRACKCOMBO, OnTrackcomboSelchange)
	ON_BN_CLICKED(IDC_MACINFOBUTTON, OnMacinfobutton)
	ON_BN_CLICKED(IDC_PARINFOBUTTON, OnParinfobutton)
	ON_BN_CLICKED(IDC_ADDBUTTON2, OnAddbutton2)
	ON_BN_CLICKED(IDC_DELBUTTON, OnDelbutton)
	ON_BN_CLICKED(IDC_ARROWBUTTON, OnArrowbutton)
	ON_NOTIFY(TVN_SELCHANGED, IDC_ASSIGNTREE, OnAssigntreeSelchanged)
	ON_BN_CLICKED(IDC_PLUGINBTN, OnPluginbtn)
	ON_BN_CLICKED(IDC_MIDICCBTN, OnMidiccbtn)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_MAPPINGWINDOW, &CSettingsDialog::OnBnClickedMappingwindow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsDialog message handlers


BOOL CSettingsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	char txt[256];

	HTREEITEM i2;

	for (int t=0; t<pmi->numTracks; t++)
	{
		sprintf(txt, "Track %i", t);
		HTREEITEM item = m_AssignTree.InsertItem(txt);
		m_AssignTree.SetItemData(item, t);

		for (int a=0; a<pmi->Tracks[t].peerctrl.size(); a++)
		{
			i2 = m_AssignTree.InsertItem(pmi->Tracks[t].peerctrl[a]->GetAssignmentString(), item);
			m_AssignTree.SetItemData(i2, a);
		}

		if (t==0)
		{
			m_AssignTree.SelectItem(item);
		}
	}

	mypc = new CPeerCtrl(pmi->ThisMac, pmi);
	mypc->GetMachineNamesToCombo(m_hWnd, IDC_MACHINECOMBO, pmi->pCB->GetMachineName(pmi->ThisMac));

	AssignTreeSelChange();

//	if (pmi->aval.MIDIIncDecAmt == 25802)
//		m_EggButton.ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::OnMachinecomboSelchange() 
{
	// TODO: Add your control notification handler code here
	
	char mname[256];
	m_MachineCombo.GetWindowText(mname, 256);

	CMachine *pmac = pmi->pCB->GetMachine(mname);
	if (pmac != NULL)
	{
		mypc->GetParamNamesToList(pmac, m_hWnd, IDC_PARAMLIST, 
			ALLOW_ALL_TYPES | ALLOW_ALL_GROUPS | ALLOW_ATTRIBUTES);
	}
	else
		m_ParamList.ResetContent();

	OnParamlistSelchange();
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::AfterAssignListSelChange()
{
	int a = CurrentAssign;

	if (pmi->Tracks[tracknum].peerctrl[a]->GotParam())
	{
		pmi->Tracks[tracknum].peerctrl[a]->SelectMachineInCombo(m_hWnd, IDC_MACHINECOMBO);
		OnMachinecomboSelchange();
		pmi->Tracks[tracknum].peerctrl[a]->SelectParameterInList(m_hWnd, IDC_PARAMLIST);
		OnParamlistSelchange();
	}
	else
	{
		m_MachineCombo.SetCurSel(-1);
		OnMachinecomboSelchange();
	}

	curenvdata = *pmi->Tracks[tracknum].mapping[a];
	m_MappingEnvelope.SetEnvelope(curenvdata.numpoints(), -1,
		&curenvdata.points[0]);

//	m_DisplayNameEdit.SetWindowText(pmi->Tracks[tracknum].DisplayName);
}


/*void CSettingsDialog::OnAssignlistSelchange() 
{
	// TODO: Add your control notification handler code here
	
	int a = m_AssignList.GetCurSel();

	OnArrowbutton();
	m_AssignList.SetCurSel(a);

	CurrentAssign = a;

	AfterAssignListSelChange();
}
*/
/*
void CSettingsDialog::OnAssigntreeSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}
*/
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////

BEGIN_EVENTSINK_MAP(CSettingsDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CSettingsDialog)
	ON_EVENT(CSettingsDialog, IDC_MAPPINGENVELOPE, 1 /* EnvelopeChanged */, OnMappingEnvelopeChanged, VTS_I4 VTS_I4 VTS_PI4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSettingsDialog::OnMappingEnvelopeChanged(long numPoints, long SustainPoint, long FAR* Points) 
{
	// TODO: Add your control notification handler code here

	curenvdata.Set(numPoints, Points);
	pmi->Tracks[tracknum].mapping[CurrentAssign]->Set(numPoints, Points);
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::OnClearbutton() 
{
	// TODO: Add your control notification handler code here
	
//	int a = CurrentAssign;

	pmi->Tracks[tracknum].peerctrl[CurrentAssign]->UnassignParameter();

	m_AssignTree.SetItemText(m_AssignTree.GetSelectedItem(),
		pmi->Tracks[tracknum].peerctrl[CurrentAssign]->GetAssignmentString());

//	m_AssignList.DeleteString(a);

//	m_AssignList.InsertString(a, pmi->Tracks[tracknum].peerctrl[a]->GetAssignmentString());
//	m_AssignList.SetCurSel(a);
//	CurrentAssign = a;
//	AfterAssignListSelChange();
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::OnResetbutton() 
{
	// TODO: Add your control notification handler code here
	
	long p[4] = {0,65535,65535,0};
	m_MappingEnvelope.SetEnvelope(2, -1, p);
	OnMappingEnvelopeChanged(2, -1, p);
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::OnParamlistSelchange() 
{
	// TODO: Add your control notification handler code here
	
	int a = CurrentAssign;
	int s = m_ParamList.GetCurSel();

	if (s == LB_ERR)
	{
		m_TrackCombo.EnableWindow(FALSE);
	}
	else
	{
		char txt[256];
		m_ParamList.GetText(s, txt);
		if (txt[1] == 'T')
		{
			m_TrackCombo.EnableWindow(TRUE);

			char macname [256];
			m_MachineCombo.GetWindowText(macname, 256);
			CMachine* machine = pmi->pCB->GetMachine(macname);
			CMachineInfo const *minfo = pmi->pCB->GetMachineInfo(machine);

			m_TrackCombo.ResetContent();
			m_TrackCombo.AddString("All");
			int numtracks = pmi->pCB->GetNumTracks(machine);
			for (int i=0; i<numtracks; i++)
			{
				sprintf(txt,"%i",i);
				m_TrackCombo.AddString(txt);
			}

			m_TrackCombo.SetCurSel(pmi->Tracks[tracknum].Track[a] + 1);

			if (m_TrackCombo.GetCurSel() == CB_ERR)
			{
				m_TrackCombo.SetCurSel(1);
			}
		}
		else
		{
			m_TrackCombo.EnableWindow(FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::OnTrackcomboSelchange() 
{
	// TODO: Add your control notification handler code here

	int a = CurrentAssign;

	char buf[16];
	m_TrackCombo.GetWindowText(buf, 16);
	if (strcmp(buf,"All") == 0)
		pmi->Tracks[tracknum].Track[a] = -1;
	else
		pmi->Tracks[tracknum].Track[a] = atoi(buf);
}

/////////////////////////////////////////////////////////////////////////////
/*
void CSettingsDialog::OnMidibtn() 
{
	// TODO: Add your control notification handler code here

	CMidiDialog mididlg(CurrentAssign);
	mididlg.pmi = pmi;
	mididlg.ptrack = &pmi->Tracks[tracknum];
	mididlg.DoModal();
}
*/


void CSettingsDialog::OnArrowbutton() 
{
	// TODO: Add your control notification handler code here
	
//	int a = CurrentAssign;
	char Mac[256]; m_MachineCombo.GetWindowText(Mac, 256);
	
	char Par[256]; 
	int cs = m_ParamList.GetCurSel();
	if (cs != LB_ERR)
	{
		m_ParamList.GetText(cs, Par);

		pmi->Tracks[tracknum].peerctrl[CurrentAssign]->AssignParameter(Mac, Par);
		*pmi->Tracks[tracknum].mapping[CurrentAssign] = curenvdata;
		pmi->Tracks[tracknum].Track[CurrentAssign] = m_TrackCombo.GetCurSel() - 1;

		m_AssignTree.SetItemText(m_AssignTree.GetSelectedItem(),
			pmi->Tracks[tracknum].peerctrl[CurrentAssign]->GetAssignmentString());

//		m_AssignList.DeleteString(a);

//		m_AssignList.InsertString(a, pmi->Tracks[tracknum].peerctrl[a]->GetAssignmentString());
//		m_AssignList.SetCurSel(a);
//		CurrentAssign = a;
	}
}

void CSettingsDialog::OnAddbutton2() 
{
	// TODO: Add your control notification handler code here
	
	CPeerCtrl *tpc = new CPeerCtrl(pmi->ThisMac, pmi);
	
	char Mac[256]; m_MachineCombo.GetWindowText(Mac, 256);
	
	char Par[256]; 
	int cs = m_ParamList.GetCurSel();
	if (cs != LB_ERR)
	{
		m_ParamList.GetText(cs, Par);

		tpc->AssignParameter(Mac, Par);
		tpc->SetLabelAsTrackNumber(tracknum);
		pmi->Tracks[tracknum].peerctrl.push_back(tpc);
		CEnvData *nenv = new CEnvData;
		*nenv = curenvdata;
		pmi->Tracks[tracknum].mapping.push_back(nenv);
		pmi->Tracks[tracknum].Track.push_back(m_TrackCombo.GetCurSel() - 1);
		int a = pmi->Tracks[tracknum].peerctrl.size() - 1;

		HTREEITEM hItem = m_AssignTree.InsertItem(pmi->Tracks[tracknum].peerctrl[a]->GetAssignmentString(),
			m_AssignTree.GetParentItem(m_AssignTree.GetSelectedItem()));
		m_AssignTree.SelectItem(hItem);

		m_AssignTree.SetItemData(hItem, a);

		AssignTreeSelChange();
//		m_AssignList.InsertString(a, pmi->Tracks[tracknum].peerctrl[a]->GetAssignmentString());
//		m_AssignList.SetCurSel(a);
//		CurrentAssign = a;
	}
	else
		delete tpc;
}

void CSettingsDialog::OnDelbutton() 
{
	// TODO: Add your control notification handler code here
	
	if (pmi->Tracks[tracknum].peerctrl.size() == 1)
		OnClearbutton();
	else
	{
		int a = CurrentAssign;
		delete pmi->Tracks[tracknum].peerctrl[a];
		delete pmi->Tracks[tracknum].mapping[a];
		pmi->Tracks[tracknum].peerctrl.erase(pmi->Tracks[tracknum].peerctrl.begin() + a);
		pmi->Tracks[tracknum].mapping.erase (pmi->Tracks[tracknum].mapping.begin() + a);
		pmi->Tracks[tracknum].Track.erase   (pmi->Tracks[tracknum].Track.begin() + a);

		HTREEITEM hNextItem = m_AssignTree.GetNextSiblingItem(m_AssignTree.GetSelectedItem());
		while (hNextItem)
		{
			m_AssignTree.SetItemData(hNextItem, m_AssignTree.GetItemData(hNextItem) - 1);
			hNextItem = m_AssignTree.GetNextSiblingItem(hNextItem);
		}

		hNextItem = m_AssignTree.GetNextSiblingItem(m_AssignTree.GetSelectedItem());
		if (!hNextItem)
			hNextItem = m_AssignTree.GetPrevSiblingItem(m_AssignTree.GetSelectedItem());

		m_AssignTree.DeleteItem(m_AssignTree.GetSelectedItem());
		m_AssignTree.SelectItem(hNextItem);
		AssignTreeSelChange();

//		m_AssignList.DeleteString(a);

/*		if (m_AssignList.SetCurSel(a) == LB_ERR)
		{
			m_AssignList.SetCurSel(a-1);
			CurrentAssign = a-1;
		}
*///		AfterAssignListSelChange();
	}
}

void CSettingsDialog::OnAssigntreeSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	AssignTreeSelChange();

	*pResult = 0;
}

void CSettingsDialog::AssignTreeSelChange()
{
	HTREEITEM hItem = m_AssignTree.GetSelectedItem();

	if (hItem)
	{
		if (m_AssignTree.ItemHasChildren(hItem))
		{
			m_AssignTree.Expand(hItem, TVE_EXPAND);
			m_AssignTree.EnsureVisible(hItem);
			m_AssignTree.SelectItem(m_AssignTree.GetChildItem(hItem));
			AssignTreeSelChange();
		}
		else
		{
			HTREEITEM hItem = m_AssignTree.GetSelectedItem();
			CurrentAssign = m_AssignTree.GetItemData(hItem);
			tracknum = m_AssignTree.GetItemData(m_AssignTree.GetParentItem(hItem));

			AfterAssignListSelChange();
		}
	}
}


void CSettingsDialog::OnPluginbtn() 
{
	// TODO: Add your control notification handler code here

	CPluginAxesDialog plugdlg(CurrentAssign);
	plugdlg.pmi = pmi;
	plugdlg.ptrack = &pmi->Tracks[tracknum];
	plugdlg.DoModal();
}

void CSettingsDialog::OnMidiccbtn() 
{
	// TODO: Add your control notification handler code here
	CMidiDialog mididlg(CurrentAssign);
	mididlg.pmi = pmi;
	mididlg.ptrack = &pmi->Tracks[tracknum];
	mididlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::OnMacinfobutton() 
{
	// TODO: Add your control notification handler code here
	
	char macname [256];
	m_MachineCombo.GetWindowText(macname, 256);
	if (pmi->pCB->GetMachine(macname) != NULL)
	{
		CMachineInfo const *macinfo = pmi->pCB->GetMachineInfo(
					pmi->pCB->GetMachine(macname));

		char txt[4096];
		char mactype[256];
		char macflags[256];

		if (macinfo->Type == MT_MASTER)
			strcpy(mactype, "Master");
		else if (macinfo->Type == MT_GENERATOR)
			strcpy(mactype, "Generator");
		else if (macinfo->Type == MT_EFFECT)
			strcpy(mactype, "Effect");
		else
			sprintf(mactype, "Unknown (%i/0x%X)", macinfo->Type, macinfo->Type);

		macflags[0] = 0;
		int f = macinfo->Flags;

#define FLAGS_MACRO(FLAG)					\
		if (f & FLAG)						\
		{									\
			strcat(macflags, #FLAG "  ");	\
			f -= FLAG;						\
		}									\
//end define

		FLAGS_MACRO (MIF_MONO_TO_STEREO		)
		FLAGS_MACRO (MIF_PLAYS_WAVES		)
		FLAGS_MACRO (MIF_USES_LIB_INTERFACE	)
		FLAGS_MACRO (MIF_USES_INSTRUMENTS	)
		FLAGS_MACRO (MIF_DOES_INPUT_MIXING	)
		FLAGS_MACRO (MIF_NO_OUTPUT			)
		FLAGS_MACRO (MIF_CONTROL_MACHINE	)
		FLAGS_MACRO (MIF_INTERNAL_AUX		)
		FLAGS_MACRO (MIF_EXTENDED_MENUS		)
		FLAGS_MACRO (MIF_PATTERN_EDITOR		)
		FLAGS_MACRO (MIF_PE_NO_CLIENT_EDGE	)
		FLAGS_MACRO (MIF_GROOVE_CONTROL		)
		FLAGS_MACRO (MIF_DRAW_PATTERN_BOX	)

#undef FLAGS_MACRO

		if (f != 0)
			sprintf (macflags, "%s+ unknowns (actual value %i)", txt, macinfo->Flags);

		sprintf(txt,
			"Name: %s\n"
			"Machine: %s - \"%s\"\n"
			"Author: %s\n"
			"CMachine pointer: %p\n"
			"Type: %s\n"
			"Flags: %s\n"
			"Min tracks: %i\n"
			"Max tracks: %i\n"
			"Global Parameters: %i\n"
			"Track Parameters: %i\n"
			"Attributes: %i\n",
			macname,
			macinfo->ShortName, macinfo->Name,
			macinfo->Author,
			pmi->pCB->GetMachine(macname),
			mactype,
			macflags,
			macinfo->minTracks,
			macinfo->maxTracks,
			macinfo->numGlobalParameters,
			macinfo->numTrackParameters,
			macinfo->numAttributes);

		MessageBox(txt, "Machine Info");
	}
	else
		MessageBox("No machine", "Machine Info");
}

/////////////////////////////////////////////////////////////////////////////

void CSettingsDialog::OnParinfobutton() 
{
	// TODO: Add your control notification handler code here
	char macname [256];
	m_MachineCombo.GetWindowText(macname, 256);
	if (pmi->pCB->GetMachine(macname) != NULL)
	{
		CMachineInfo const *macinfo = pmi->pCB->GetMachineInfo(
					pmi->pCB->GetMachine(macname));

		if (m_ParamList.GetCurSel() != LB_ERR)
		{
			char parname[256];
			m_ParamList.GetText(m_ParamList.GetCurSel(), parname);

			char group[16];
			CMachineParameter const *parinfo;

			if (parname[1] == 'G')
			{
				strcpy(group, "Global");
				parinfo = macinfo->Parameters[atoi(parname+4)];
			}
			else
			{
				strcpy(group, "Track");
				parinfo = macinfo->Parameters[atoi(parname+4) + macinfo->numGlobalParameters];
			}

			char partype[16];
			switch (parinfo->Type)
			{
			case pt_switch:	strcpy(partype, "Switch");	break;
			case pt_byte:	strcpy(partype, "Byte");	break;
			case pt_note:	strcpy(partype, "Note");	break;
			case pt_word:	strcpy(partype, "Word");	break;
			default: sprintf (partype, "Unknown (%i/0x%X)", parinfo->Type, parinfo->Type);
			}

			char parflags[256];
			int f = parinfo->Flags;
			parflags[0] = 0;

#define FLAGS_MACRO(FLAG)					\
		if (f & FLAG)						\
		{									\
			strcat(parflags, #FLAG "  ");	\
			f -= FLAG;						\
		}									\
//end define

		FLAGS_MACRO (MPF_WAVE			)
		FLAGS_MACRO (MPF_STATE			)
		FLAGS_MACRO (MPF_TICK_ON_EDIT	)

#undef FLAGS_MACRO



			if (f != 0)
				sprintf (parflags, "%s+ unknowns (actual value %i)", parflags, parinfo->Flags);

			char txt[256];
			sprintf(txt,
				"Name: %s\n"
				"Description: %s\n"
				"Group: %s\n"
				"Type: %s\n"
				"Min Value: %i (0x%X)\n"
				"Max Value: %i (0x%X)\n"
				"No Value: %i (0x%X)\n"
				"Default Value: %i (0x%X)\n"
				"Flags: %s\n",
				parinfo->Name,
				parinfo->Description,
				group,
				partype,
				parinfo->MinValue,	parinfo->MinValue,
				parinfo->MaxValue,	parinfo->MaxValue,
				parinfo->NoValue,	parinfo->NoValue,
				parinfo->DefValue,	parinfo->DefValue,
				parflags);

			MessageBox(txt, "Param Info");
		}
	}
}



/*void CSettingsDialog::OnChangeDisplayname() 
{
	// TODO: Add your control notification handler code here

	m_DisplayNameEdit.GetWindowText(pmi->Tracks[tracknum].DisplayName, 31);
}
*/
/*
void CSettingsDialog::OnEgg() 
{
	// TODO: Add your control notification handler code here
	
//	CNibblesDlg dlg;
//	dlg.DoModal();
}

*/
void CSettingsDialog::OnBnClickedMappingwindow()
{
	CMappingDialog dlg;

	dlg.m_envdata.CopyFrom(curenvdata);
	dlg.m_mapping = pmi->Tracks[tracknum].mapping[CurrentAssign];

	dlg.DoModal();

	m_MappingEnvelope.SetEnvelope(dlg.m_envdata.numpoints(), -1, &dlg.m_envdata.points[0]);
	OnMappingEnvelopeChanged(dlg.m_envdata.numpoints(), -1, &dlg.m_envdata.points[0]);
}
