/*
Copyright (c) 2003-2008, Edward Powley
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
#include "../mdk/mdk.h"
#include "peerstate.h"

#include "resource.h"
#include "maindialog.h"
#include "otherdialog.h"
#include "MachineSelectDlg.h"
#include "EditDialog.h"
#include "hack.h"

void CMachineDataOutput::Write(void*, int) {}

////////////////////////////////////////////////////////////////////////
// Machine info

CMachineParameter const paraSnapshot =
{
	pt_byte,			//Type
	"Snapshot",			//Short name
	"Snapshot Number",	//Long name
	0,					//Min value
	NUM_SNAPSHOTS - 1,	//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

#ifdef HAS_TAKESNAPSHOT_PARAM
CMachineParameter const paraTakeSnapshot =
{
	pt_switch,			//Type
	"Take Snap",		//Short name
	"1 = take snapshot",	//Long name
	0,					//Min value
	1,					//Max value
	SWITCH_NO,			//No value
	0,					//Flags
	SWITCH_OFF			//Default value
};
#endif

CMachineParameter const paraInertia =
{
	pt_word,			//Type
	"Inertia",			//Short name
	"Inertia in tick/16",	//Long name
	0,					//Min value
	128*16,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineAttribute const attrMIDIChan =
{
	"MIDI channel (0=off, 17=all)",
	0,	//min
	17,	//max
	0	//def
};

CMachineAttribute const attrMIDISnapKey =
{
	"MIDI note for taking snapshots (128 = none)",
	0,		//min
	128,	//max
	128		//def
};

CMachineAttribute const attrInertiaFreq =
{
	"Inertia update freq (in samples*256)",
	1,		//min
	32,		//max
	4		//def
};

CMachineAttribute const attrDblClickAction =
{
	"What to open on double click (0=param window 1=simple GUI 2=A/B GUI",
	0,		//min
	2,		//max
	1		//def
};

CMachineParameter const *pParameters[] = {
	&paraSnapshot,
#ifdef HAS_TAKESNAPSHOT_PARAM
	&paraTakeSnapshot,
#endif
	&paraInertia,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	&attrMIDIChan,
	&attrMIDISnapKey,
	&attrInertiaFreq,
	&attrDblClickAction,
	NULL
};

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_NO_OUTPUT | MIF_CONTROL_MACHINE,					//Flags
	0,								//Min tracks
	0,								//Max tracks
	NUMG,							//Number of global parameters
	0,								//Number of track parameters
	pParameters,					//Pointer to parameters
	4,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"BTDSys PeerState",				//Full name
	"PeerState",					//Short name
	"Ed Powley (BTDSys)",			//Author
	"About...\n"
	"Machines/Parameters...\n"
	"Take snapshot (&X)\n"
	"Simple GUI...\n"
	"A/B GUI...\n"
	"Snapshot Manager..."			//Commands
};

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////
// [Con/de]structors
mi::mi() 
{
	GlobalVals = &gval;
	AttrVals = (int *)&aval;

	pdlg = NULL;
	pdlg2 = NULL;

	memset(snapname,0,NUM_SNAPSHOTS*32);

//	CanUpdateParamNameAndState = true;
}

mi::~mi()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pdlg)
		pdlg->DestroyWindow();

	if (pdlg2)
		pdlg2->DestroyWindow();

	for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
	{
		for (int i=0; i<NUM_SNAPSHOTS; i++)
		{
			if (p->Snapshots[i].pg) delete[] p->Snapshots[i].pg;
			if (p->Snapshots[i].pt) delete[] p->Snapshots[i].pt;
		}

		if (p->FromSnap.pg) delete[] p->FromSnap.pg;
		if (p->FromSnap.pt) delete[] p->FromSnap.pt;
	}
}

////////////////////////////////////////////////////////////////////////

bool mi::DblClickEvent(void *p)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (aval.DblClickAction)
	{
	case 0: return false;
	case 1: Command(3); return true;
	case 2: Command(4); return true;
	}

	return false;
}

inline void ReadString (char *temp, CMachineDataInput * const pi)
//Read a null-terminated string from a CMachineDataInput
{
	byte inch;
	int i=0;

	do
	{
		pi->Read(inch);
		*temp = char(inch);
		temp++;
		i++;
	}
	while (inch != 0);
}

////////////////////////////////////////////////////////////////////////

inline void WriteString (const char *temp, CMachineDataOutput * const po)
//Write a null-terminated string to a CMachineDataOutput
{
	byte outch;
	int i=0;
	do
	{
		outch = *temp;
		temp++;
		po->Write(outch);
		i++;
	}
	while (outch != 0);
}

void mi::MDKInit (CMachineDataInput * const pi)
{
	SetOutputMode(true);
	thismac = pCB->GetThisMachine();
	pCB->SetnumOutputChannels(thismac,2);

	pCB->SetEventHandler(thismac, 
						DoubleClickMachine, 
						(EVENT_HANDLER_PTR)&mi::DblClickEvent, NULL);

	nHostVersion = 1;
	if (pCB->GetNearestWaveLevel(-2,-2))
		nHostVersion = pCB->GetHostVersion();

	FindMIOffset(thismac, this);

	nCurrentSnapshot = 0;

	if (pi)
	{
		byte version;
		pi->Read(version);
		if (version == 3)
		{
			int n;
			pi->Read(n);
			for (int i=0; i<n; i++)
			{
				char name[256];
				ReadString(name, pi);
				
				CMachineRec r(name, this);

				pi->Read(r.bActive);

				for (int s=0; s<NUM_SNAPSHOTS; s++)
				{
					int len;
					pi->Read(len);
					if (len)
					{
						r.Snapshots[s].pg = new byte[len];
						pi->Read(r.Snapshots[s].pg, len);
					}
					else
						r.Snapshots[s].pg = NULL;

					pi->Read(len);
					if (len)
					{
						r.Snapshots[s].pt = new byte[len];
						pi->Read(r.Snapshots[s].pt, len);

/*						char txt[256];
						sprintf(txt,"%X", *(unsigned int *)r.Snapshots[s].pt);
						MessageBox(NULL,txt,"",MB_OK);
*/					}
					else
						r.Snapshots[s].pt = NULL;

				}

				int len;
				pi->Read(len);
				for (int i=0; i<len; i++)
				{
					bool b;
					pi->Read(b);
					r.vbParamActive.push_back(b);
				}

				peers.push_back(r);
			}

			for (int s=0; s<NUM_SNAPSHOTS; s++)
				pi->Read(snapname[s],32);


		}
	}

	bFirstTick = bDoneWork = true;
}

void mi::MDKSave(CMachineDataOutput * const po)
{
	po->Write((byte)3); //version

	po->Write((int)peers.size());
	for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
	{
		WriteString(pCB->GetMachineName(p->pMac),po);
		po->Write(p->bActive);
		for (int s=0; s<NUM_SNAPSHOTS; s++)
		{
			po->Write((int)(p->Snapshots[s].pg ? p->nGStateSize : 0));
			if (p->Snapshots[s].pg)
				po->Write(p->Snapshots[s].pg, p->nGStateSize);

			po->Write((int)(p->Snapshots[s].pt ? p->nTStateSize : 0));
			if (p->Snapshots[s].pt)
				po->Write(p->Snapshots[s].pt, p->nTStateSize);
		}

		po->Write((int)p->vbParamActive.size());
		for (vector<bool>::iterator q = p->vbParamActive.begin(); q<p->vbParamActive.end(); q++)
			po->Write(*q);
	}

	for (int s=0; s<NUM_SNAPSHOTS; s++)
		po->Write(snapname[s],32);

}

void mi::Command(const int i)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (i)
	{
	case 0: //about
/*		{


						char txt[256];
						sprintf(txt,"%X", *(unsigned int *)r.Snapshots[s].pt);
						MessageBox(NULL,txt,"",MB_OK);
*/
		MessageBox(NULL,
			"BTDSys PeerState 1.1\n"
			"©2003-4 Ed Powley, http://btdsys.cjb.net\n"
			"Based on an idea by Ronny Pries",
			"About",MB_OK);
		break;

	case 1: //edit
		{
			CMachineSelectDlg dlg;
			dlg.pmi = this;
			dlg.DoModal();
		}
		break;

	case 2: //snapshot
		{
			for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
				if (p->bActive)
					p->TakeSnapshot(nCurrentSnapshot);

			if (pdlg)
				pdlg->UpdateParamComboEntry(nCurrentSnapshot);

			if (pdlg2)
				pdlg2->UpdateParamComboEntry(nCurrentSnapshot);

			SetModifiedFlag();
		}
		break;

	case 3: //Main GUI
		if (pdlg2)
		{
			pdlg2->DestroyWindow();
			pdlg2 = NULL;
		}

		if (pdlg)
			pdlg->SetActiveWindow();
		else
		{
			pdlg = new CMainDialog;
			pdlg->pmi = this;
			if (pdlg->Create(IDD_DIALOG))
				pdlg->ShowWindow(SW_SHOW);
			else
			{
				delete pdlg;
				pdlg = NULL;
			}
		}
		break;

	case 4: //Other GUI
		if (pdlg)
		{
			pdlg->DestroyWindow();
			pdlg = NULL;
		}

		if (pdlg2)
			pdlg2->SetActiveWindow();
		else
		{
			pdlg2 = new COtherDialog;
			pdlg2->pmi = this;
			if (pdlg2->Create(IDD_OTHERDIALOG))
				pdlg2->ShowWindow(SW_SHOW);
			else
			{
				delete pdlg2;
				pdlg2 = NULL;
			}
		}
		break;

	case 5: //Edit dialog
		{
			CEditDialog dlg;
			dlg.pmi = this;
			dlg.DoModal();
		}
		break;
	}
}

/*void mi::UpdateParamNameAndState(int n)
{
	if (CanUpdateParamNameAndState)
	{
		bool gs = GotSnap(n);

		if (pdlg)
		{
			int s = pdlg->m_SnapCombo.GetCurSel();
			pdlg->m_SnapCombo.DeleteString(n);
			char txt[64];
			sprintf(txt,"%s%i: %s", gs ? "*" : "", n,snapname[n]);
			pdlg->m_SnapCombo.InsertString(n,txt);
			pdlg->m_SnapCombo.SetCurSel(s);
			if (n == s)
				pdlg->m_NameEdit.SetWindowText(snapname[n]);
		}

		if (pdlg2)
		{
			int s = pdlg2->m_SnapCombo.GetCurSel();
			pdlg2->m_SnapCombo.DeleteString(n);
			char txt[64];
			sprintf(txt,"%s%i: %s", gs ? "*" : "", n,snapname[n]);
			pdlg2->m_SnapCombo.InsertString(n,txt);
			pdlg2->m_SnapCombo.SetCurSel(s);
			if (n == s)
				pdlg2->m_NameEdit.SetWindowText(snapname[n]);
		}
	}
}
*/

bool mi::GotSnap(int n)
{
	for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
	{
		if (p->Snapshots[n].pg || p->Snapshots[n].pt)
			return true;
	}
	return false;
}

bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)
{
	bDoneWork = true;

	nSamplesSinceLastInertia += numsamples;

	if (nSamplesSinceLastInertia > aval.InertiaFreq * 256)
	{
		for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
		{
			if (p->bSliding)
				p->UpdateInertia(nSamplesSinceLastInertia, true);
		}

		nSamplesSinceLastInertia = 0;
	}

	return false;
}

void mi::Tick()
{
	if (bDoneWork)
	{
		for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
		{
			if (!p->CheckMachine() && !p->bLoading)
			{
				peers.erase(p);
				p--;
			}
		}
	}

	if (gval.Inertia != paraInertia.NoValue)
		nInertiaLen = gval.Inertia * pMasterInfo->SamplesPerTick / 16;

	if (!bFirstTick)
	{
		if (gval.Snapshot != paraSnapshot.NoValue)
		{
			nCurrentSnapshot = gval.Snapshot;
/*			if (pdlg)
			{
				pdlg->m_SnapCombo.SetCurSel(gval.Snapshot);
				pdlg->m_NameEdit.SetWindowText(snapname[gval.Snapshot]);
			}
*/

			bool shouldrestore = true;
			if (pdlg) if (pdlg->m_DontRestore.GetState() == BST_CHECKED) shouldrestore = false;
#ifdef HAS_TAKESNAPSHOT_PARAM
			if (gval.TakeSnapshot != paraTakeSnapshot.NoValue) shouldrestore = false;
#endif

			if (shouldrestore)
			{
				for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
					if (p->bActive && !p->bLoading) p->RestoreSnapshot(gval.Snapshot);
			}
		}

#ifdef HAS_TAKESNAPSHOT_PARAM
		if (gval.TakeSnapshot == SWITCH_ON)
		{
			Command(2);
		}
#endif

		for (vector<CMachineRec>::iterator p = peers.begin(); p < peers.end(); p++)
		{
			if (p->bSliding)
				p->UpdateInertia(nSamplesSinceLastInertia, false);
		}

		nSamplesSinceLastInertia = 0;
	}
	else
		if (gval.Snapshot != paraSnapshot.NoValue)
			nCurrentSnapshot = gval.Snapshot;



	bFirstTick = false;
}

char const* mi::DescribeValue(int const param, int const value)
{
	static char txt[16];
	switch (param)
	{
	case numSnapshot:
		sprintf(txt,"%i: %s",value,snapname[value]);
		return txt;

#ifdef HAS_TAKESNAPSHOT_PARAM
	case numTakeSnapshot:
		switch (value)
		{
		case SWITCH_ON:
			return "Take snapshot";
		case SWITCH_OFF:
			return "Don't restore (this tick only)";
		default:
			return "WTF?";
		}
#endif

	case numInertia:
		if (value == 0)
			return "Off";
		else
		{
			sprintf(txt,"%i", value/16);
			if (value % 16)
				sprintf(txt,"%s %i/16 ticks", txt, value % 16);
			else
				strcat(txt," ticks");
			return txt;
		}
		
	default:
		return NULL;
	}
}

void mi::MidiNote(const int channel, const int value, const int velocity)
{
	if (velocity > 0)
	{
		if (channel+1 == aval.MIDIChan || aval.MIDIChan == 17)
		{
			if (value == aval.MIDISnapKey)
				Command(2);
			else
				pCB->ControlChange(thismac,1,0,0,value);
		}
	}
}






CMachineRec* mi::FindMachineInPeers(const char *name)
{
	return FindMachineInPeers(pCB->GetMachine(name));
}

CMachineRec* mi::FindMachineInPeers(CMachine *p)
{
	for (vector<CMachineRec>::iterator q = peers.begin(); q < peers.end(); q++)
	{
		if (q->pMac == p)
			return &*q;
	}

	return NULL;
}

void mi::AddMachineToPeers(const char *name)
{
	AddMachineToPeers(pCB->GetMachine(name));
}

void mi::AddMachineToPeers(CMachine *p)
{
	CMachineRec r(p,this);
	peers.push_back(r);
}

void CMachineRec::Init()
{
	if (pmi->nHostVersion < 2)
	{
		pGState = *(byte **) ((byte *)pMac + 0x68);
		pTState = *(byte **) ((byte *)pMac + 0x6C);
	}
	else
	{
		pGState = pTState = NULL;
	}

	const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(pMac);
	int i;

	if (vbParamActive.empty())
		for (i=0; i<pinfo->numGlobalParameters + pinfo->numTrackParameters; i++)
			vbParamActive.push_back(false);

	nTStateSize = 0;
	for (i=0; i<pinfo->numTrackParameters; i++)
	{
		nTStateSize++;
		if (pinfo->Parameters[pinfo->numGlobalParameters + i]->Type == pt_word)
			nTStateSize++;
	}

	nTStateSize *= pinfo->maxTracks;

	nGStateSize = 0;
	for (i=0; i<pinfo->numGlobalParameters; i++)
	{
		nGStateSize++;
		if (pinfo->Parameters[i]->Type == pt_word)
			nGStateSize++;
	}

	if (nGStateSize)
		FromSnap.pg = new byte[nGStateSize];
	else
		FromSnap.pg = NULL;

	if (nTStateSize)
		FromSnap.pt = new byte[nTStateSize];
	else
		FromSnap.pt = NULL;
}

CMachineRec::CMachineRec(CMachine *pm, mi *pmi_)
{
	pmi = pmi_;
	pMac = pm;
	bActive = true;
	bLoading = false;
	bSliding = false;

	nCheckCountDown = 10;

	Init();

	for (int i=0; i<NUM_SNAPSHOTS; i++)
		Snapshots[i].pg = Snapshots[i].pt = NULL;

	FromSnap.pg = FromSnap.pt = NULL;
}

CMachineRec::CMachineRec(const char *name, mi *pmi_)
{
	pmi = pmi_;
	strcpy(szLoadingMac, name);
	bLoading = true;
	bSliding = false;

	nCheckCountDown = 10;
}

CMachineRec::~CMachineRec()
{
/*	if (!bDontDeleteMyFrog)
		for (int i=0; i<NUM_SNAPSHOTS; i++)
		{
			if (Snapshots[i].pg) delete[] Snapshots[i].pg;
			if (Snapshots[i].pt) delete[] Snapshots[i].pt;
		}
*/
}

void CMachineRec::GetState(const snap &s)
{
	if (pmi->nHostVersion < 2)
	{
		if (nGStateSize > 0)
			memcpy(s.pg, pGState, nGStateSize);
		if (nTStateSize > 0)
			memcpy(s.pt, pTState, nTStateSize);
	}
	else
	{
		const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(pMac);

		if (s.pg)
		{
			byte *p = s.pg;
			for (int i=0; i<pinfo->numGlobalParameters; i++)
			{
				if (pinfo->Parameters[i]->Type == pt_word)
				{
					*((word*)p) = (word)pmi->pCB->GetParameterState(pMac, 1, 0, i);
					p += 2;
				}
				else
				{
					*p = (byte)pmi->pCB->GetParameterState(pMac, 1, 0, i);
					p ++;
				}
			}
		}

		if (s.pt)
		{
			byte *p = s.pt;
			for (int t=0; t<pinfo->maxTracks; t++)
			{
				for (int i=0; i<pinfo->numTrackParameters; i++)
				{
					if (pinfo->Parameters[pinfo->numGlobalParameters+i]->Type == pt_word)
					{
						*((word*)p) = (word)pmi->pCB->GetParameterState(pMac, 2, t, i);
						p += 2;
					}
					else
					{
						*p = (byte)pmi->pCB->GetParameterState(pMac, 2, t, i);
						p ++;
					}
				}
			}
		}
	}
}

void CMachineRec::TakeSnapshot(int n)
{
	if (!Snapshots[n].pg && nGStateSize > 0)
		Snapshots[n].pg = new byte[nGStateSize];

	if (!Snapshots[n].pt && nTStateSize > 0)
		Snapshots[n].pt = new byte[nTStateSize];

	GetState(Snapshots[n]);
}

void CMachineRec::RestoreSnapshot(int n)
{
	if (pmi->nInertiaLen)
	{
		if (!FromSnap.pg && nGStateSize > 0)
			FromSnap.pg = new byte[nGStateSize];

		if (!FromSnap.pt && nTStateSize > 0)
			FromSnap.pt = new byte[nTStateSize];

		GetState(FromSnap);

		bSliding = true;
		nSlideLen = pmi->nInertiaLen;
		nSlideProgress = 0;
		nToSnap = n;
	}
	else
	{
		bSliding = false;

		const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(pMac);

		if (Snapshots[n].pg)
		{
			byte *b = Snapshots[n].pg;

			for (int i=0; i<pinfo->numGlobalParameters; i++)
			{
				if (pinfo->Parameters[i]->Type == pt_word)
				{
					if (pinfo->Parameters[i]->Flags & MPF_STATE && vbParamActive[i])
						pmi->pCB->ControlChange(pMac, 1, 0, i, *(word *)b);
					b++;
				}
				else
					if (pinfo->Parameters[i]->Flags & MPF_STATE && vbParamActive[i])
						pmi->pCB->ControlChange(pMac, 1, 0, i, *b);

				b++;
			}
		}

		if (Snapshots[n].pt)
		{
			byte *b = Snapshots[n].pt;

			for (int t=0; t<pinfo->maxTracks; t++)
			{
				for (int i=0; i<pinfo->numTrackParameters; i++)
				{
					int j = pinfo->numGlobalParameters + i;
					if (pinfo->Parameters[j]->Type == pt_word)
					{
						if (pinfo->Parameters[j]->Flags & MPF_STATE && vbParamActive[j])
							pmi->pCB->ControlChange(pMac, 2, t, i, *(word *)b);
						b++;
					}
					else
						if (pinfo->Parameters[j]->Flags & MPF_STATE && vbParamActive[j])
							pmi->pCB->ControlChange(pMac, 2, t, i, *b);

					b++;
				}
			}
		}
	}
}

void CMachineRec::UpdateInertia(int numSamples, bool Hack)
{
	nSlideProgress += numSamples;
	if (nSlideProgress >= nSlideLen)
	{
		nSlideProgress = nSlideLen;
		bSliding = false;
		Hack = false; // force "proper" update as last one
	}

	if (CheckMachine())
	{
		const CMachineInfo *pinfo = pmi->pCB->GetMachineInfo(pMac);

		if (FromSnap.pg && Snapshots[nToSnap].pg)
		{
			byte *b1 = FromSnap.pg;
			byte *b2 = Snapshots[nToSnap].pg;

			for (int i=0; i<pinfo->numGlobalParameters; i++)
			{
				if (pinfo->Parameters[i]->Type == pt_word)
				{
					if (pinfo->Parameters[i]->Flags & MPF_STATE && vbParamActive[i])
					{
						word v1 = *(word *)b1;
						word v2 = *(word *)b2;

						int NewVal = (int)( (float)v1 + (float)nSlideProgress * ((float)v2 - (float)v1) 
											/ (float)nSlideLen );

						if (Hack)
							Hack_ControlChange(pinfo, pMac, 1, 0, i, NewVal);
						else
							pmi->pCB->ControlChange(pMac, 1, 0, i, NewVal);
					}
					b1++; b2++;
				}
				else
				{
					if (pinfo->Parameters[i]->Flags & MPF_STATE && vbParamActive[i])
					{
						byte v1 = *b1;
						byte v2 = *b2;

						int NewVal = (int)( (float)v1 + (float)nSlideProgress * ((float)v2 - (float)v1) 
											/ (float)nSlideLen );

						if (Hack)
							Hack_ControlChange(pinfo, pMac, 1, 0, i, NewVal);
						else
							pmi->pCB->ControlChange(pMac, 1, 0, i, NewVal);
					}
				}

				b1++; b2++;
			}
		}

		if (FromSnap.pt && Snapshots[nToSnap].pt)
		{
			byte *b1 = FromSnap.pt;
			byte *b2 = Snapshots[nToSnap].pt;

			for (int t=0; t<pinfo->maxTracks; t++)
			{
				for (int i=0; i<pinfo->numTrackParameters; i++)
				{
					int j = pinfo->numGlobalParameters + i;
					if (pinfo->Parameters[j]->Type == pt_word)
					{
						if (pinfo->Parameters[j]->Flags & MPF_STATE && vbParamActive[j])
						{
							word v1 = *(word *)b1;
							word v2 = *(word *)b2;

							int NewVal = (int)( (float)v1 + (float)nSlideProgress * ((float)v2 - (float)v1) 
												/ (float)nSlideLen );

/*							if (t==0 && strcmp(pinfo->Parameters[j]->Name,"Max") == 0)
							{
								char txt[256];
								sprintf(txt,"%i %i %i %i",nSlideLen,v1,v2,NewVal);
								OutputDebugString(txt);
							}
*/
							if (Hack)
								Hack_ControlChange(pinfo, pMac, 2, t, i, NewVal);
							else
								pmi->pCB->ControlChange(pMac, 2, t, i, NewVal);
						}
						b1++; b2++;
					}
					else
					{
						if (pinfo->Parameters[j]->Flags & MPF_STATE && vbParamActive[j])
						{
							byte v1 = *b1;
							byte v2 = *b2;

							int NewVal = (int)( (float)v1 + (float)nSlideProgress * ((float)v2 - (float)v1) 
												/ (float)nSlideLen );

							if (Hack)
								Hack_ControlChange(pinfo, pMac, 2, t, i, NewVal);
							else
								pmi->pCB->ControlChange(pMac, 2, t, i, NewVal);
						}
					}

					b1++; b2++;
				}
			}
		}

		if (Hack)
			GetMI(pMac)->Tick();
	}
}

bool CMachineRec::CheckMachine()
{
	if (bLoading) //The machine was just loaded in a song file
	{
		pMac = pmi->pCB->GetMachine(szLoadingMac);

		if (pMac == NULL)
		{
			nCheckCountDown--;
			if (nCheckCountDown <= 0)
				MessageBox(NULL,szLoadingMac,"PeerState couldn't find this machine:",MB_OK);

			return false;
		}
		else
		{
			bLoading = false;
			Init();
			return true;
		}
	}
	else
	{
		CMachine *p2 = pmi->pCB->GetMachine(pmi->pCB->GetMachineName(pMac));
		
		if (pMac != p2) //Something is amiss! (Should be the same)
			return false;
		else
			return true;
	}


	return false;
}

