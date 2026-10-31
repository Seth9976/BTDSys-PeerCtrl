/*
ctrl.cpp: the main cpp file for this machine
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

//Created 25-Aug-02

#include "StdAfx.h"
#include "ctrl.h"
#include "../mdk/mdk.h"
#include <float.h>
#include <stdlib.h>
#include "../peerlib2/peerctrl.h"
#include "settingsdialog.h"
#include "params.h"
#include <vector>
#include <mmsystem.h>

char MacInfo_MenuItems[1024];

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_NO_OUTPUT|MIF_CONTROL_MACHINE,					//Flags
	1,								//Min tracks
	MAX_TRACKS,						//Max tracks
	NUMG,							//Number of global parameters
	NUMT,							//Number of track parameters
	pParameters,					//Pointer to parameters
	NUMA,							//Number of attributes
	pAttributes,					//Pointer to attributes
	"BTDSys PeerCtrl",				//Full name
	"PeerCtrl",						//Short name
	"Ed Powley (BTDSys)",			//Author
	MacInfo_MenuItems				//Commands
};

/////////////////////////////////////////////////////////////////////////////

//DLL_EXPORTS

extern "C" 
{ 
	__declspec(dllexport) CMachineInfo const * __cdecl GetInfo() 
	{ 
		strcpy(MacInfo_MenuItems, BASE_MENUITEMS);
		return &MacInfo; 
	} 
	__declspec(dllexport) CMachineInterface * __cdecl CreateMachine() 
	{ 
		return new mi; 
	} 
} 

/////////////////////////////////////////////////////////////////////////////

void CTrack::Init()
{
	ValueCurrent = ValueStep = ValueTarget = 0.0f;
	ValueSliding = false;
	ValueOnLastUpdate = -1.0f;
	ValueOnLastTick = -1.0f;

	CPeerCtrl* peer = new CPeerCtrl(p_mi->ThisMac, p_mi);
	peer->SetLabelAsTrackNumber(tracknum);
	peerctrl.push_back(peer);

	mapping.push_back(new CEnvData);

	Track.push_back(0);

	MIDIChannel = 0;
	MIDIController = -1;
	MIDIFeedback = false;
	MIDIIncDec = false;
	MIDIIncDecWrap = false;
	MIDIFBOutDev = 0;
	LastFBCCValue = -1;

	Slaved = false;

/*	JoyID = JoyItem = JoyItemType = 0;
	JoyLastPos = 0.0f;
	JoyToggleState = false;
	JoyAntiJitterThreshold = 0.05f;
*/}

/////////////////////////////////////////////////////////////////////////////

mi::mi()
{
	GlobalVals	= &gval;
	TrackVals	= &tval;
	AttrVals	= (int *)&aval;

	ex.p_mi = this;
	pmididlg = NULL;
	Initialising = true;

	bMuted = false;
	bWorkedSinceLastTick = true;

#ifdef DBGCON 
	AllocConsole(); 
	SetConsoleTitle("my debug output"); 
	freopen("CON", "w", stdout); 
	freopen("CON", "w", stderr); 
#endif 
	LOG("Test\n");
}

/////////////////////////////////////////////////////////////////////////////

mi::~mi()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	for (int t=0; t<MAX_TRACKS; t++)
	{
		for (int i=0; i < Tracks[t].peerctrl.size(); i++)
		{
			delete Tracks[t].peerctrl[i];
			delete Tracks[t].mapping[i];
		}
	}

	for (int i=0; i<plugs.size(); i++)
	{
		delete plugs[i];
	}
}

/////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////

//Version == 1 => v1.0 and v1.1
//Version == 2 => v1.2 (added Polac MIDI) -- unsupported
//Version == 3 => v1.5 prerelease upto 20-May-03
//Version == 4 => v1.5

void mi::MDKInit(CMachineDataInput *const pi)
{
	if (!pCB->GetNearestWaveLevel(-2, -2) || pCB->GetHostVersion() < 2)
		MessageBox(NULL, "This version of PeerCtrl is only compatible with Buzz builds from "
			"2008 or later, or clones which implement the same interfaces.", NULL,
			MB_OK | MB_ICONERROR);

	ThisMac = pCB->GetThisMachine();

	pCB->SetEventHandler(ThisMac, DoubleClickMachine, (EVENT_HANDLER_PTR)&mi::DblClickEvent, NULL);

	bHackedParamWndLastTick = false;

	byte vers;
	if (pi != NULL)
	{
		pi->Read(vers);
		switch(vers)
		{
		case 1:
		case 3:
		case 4:
			break;
		default:
			MessageBox(NULL, "Data not compatible", "BTDSys PeerCtrl", MB_OK);
		}
	}
	else
		vers = 255; //ie vers != CURRENT_VERSION

	LoadPlugins();

	for (int t=0; t<MAX_TRACKS; t++)
	{
		Tracks[t].tracknum = t;
		Tracks[t].p_mi = this;
		Tracks[t].tval = &tval[t];
//		sprintf(Tracks[t].DisplayName, "%i-Value", t);
		Tracks[t].Init();

		if (vers == 3 || vers == 4)
		{
			for (std::vector<CPeerCtrl*>::iterator i = Tracks[t].peerctrl.begin();
				i != Tracks[t].peerctrl.end(); ++i)
			{
				delete *i;
			}
			for (std::vector<CEnvData*>::iterator i = Tracks[t].mapping.begin();
				i != Tracks[t].mapping.end(); ++i)
			{
				delete *i;
			}

			Tracks[t].peerctrl.clear();
			Tracks[t].mapping.clear();
			Tracks[t].Track.clear();

			int numctrls;
			pi->Read(numctrls);

			for (int i=0; i<numctrls; i++)
			{
				CPeerCtrl *newc = new CPeerCtrl(ThisMac, this);
				newc->SetLabelAsTrackNumber(t);
				Tracks[t].peerctrl.push_back(newc);
				newc->ReadFileData(pi);
				int tr;
				pi->Read(tr);
				Tracks[t].Track.push_back(tr);
				CEnvData *newm = new CEnvData;
				newm->ReadData(pi);
				Tracks[t].mapping.push_back(newm);
			}

			if (vers == 4)
			{
				pi->Read(Tracks[t].MIDIChannel);
				pi->Read(Tracks[t].MIDIController);
				pi->Read(Tracks[t].MIDIFeedback);
				pi->Read(Tracks[t].MIDIIncDec);
				pi->Read(Tracks[t].MIDIIncDecWrap);
				pi->Read(Tracks[t].MIDIFBOutDev);

//				pi->Read(Tracks[t].DisplayName, 32);
			}
		}
		else if (vers == 1)
		{
			Tracks[t].peerctrl[0]->ReadFileData(pi);
			pi->Read(Tracks[t].Track[0]);
			Tracks[t].mapping[0]->ReadData(pi);

			pi->Read(Tracks[t].MIDIChannel);
			pi->Read(Tracks[t].MIDIController);
			pi->Read(Tracks[t].MIDIFeedback);
			pi->Read(Tracks[t].MIDIIncDec);
			pi->Read(Tracks[t].MIDIFBOutDev);
		}
	}

	if (vers == 3 || vers == 4)
	{
		int numPlugins;
		pi->Read(numPlugins);
		char dllname[256];
		for (int i=0; i<numPlugins; i++)
		{
			ReadString(dllname, pi);
			CPlug *pPl = NULL;

			for (int p=0; p<plugs.size(); p++)
			{
				if (strcmp(dllname, plugs[p]->dllname) == 0)
				{
					pPl = plugs[p];
					pPl->bHadFileData = true;
				}
			}

/*			char txt[256];
			sprintf(txt, "%X", pPl);
			MessageBox(NULL,txt,"",MB_OK);
*/

			//read custom data
			int datalen;
			pi->Read(datalen);

			char *dat = new char[datalen];
			pi->Read(dat, datalen);
			CInternalDataRead dr;
			dr.d = dat;
			if (pPl)
				pPl->pPl->ReadFileData((CPCtrlDataRead *)&dr);
			delete[] dat;			

			// read bindings
			SAxisInfo ai;
			int numAxes;
			pi->Read(numAxes);
			for (int a=0; a<numAxes; a++)
			{
				int numBindings;
				byte numTr;
				pi->Read(numBindings);
				for (int b=0; b<numBindings; b++)
				{
					pi->Read(numTr);
					if (pPl)
					{
						if (a < pPl->axes.size())
						{
							pPl->axes[a].binding.push_back(numTr);
							ai.pPl = pPl;
							ai.nAxis = a;
							Tracks[numTr].axes.push_back(ai);
						}
					}
				}
			}
		}
	}
	else //no "global" data to read
	{
	}

	for (int p=0; p<plugs.size(); p++)
	{
		if (!plugs[p]->bHadFileData)
			plugs[p]->pPl->InitNoFileData();
	}

	MDKInitRun = true;

	UpdateCounter = SamplesSinceLastUpdate = 0;

	CPeerCtrl::SetEventHandlers(pCB);
}

/////////////////////////////////////////////////////////////////////////////

void mi::MDKSave(CMachineDataOutput *const po)
{
	po->Write((byte)4); //version

/*	if (pxydlg != NULL)
	{
		xyplacement.length = sizeof(WINDOWPLACEMENT);
		pxydlg->GetWindowPlacement(&xyplacement);
	}

	po->Write(&xyplacement, sizeof(WINDOWPLACEMENT));
*/
	const bool backwardsCompatible = true;
	for (int t=0; t<MAX_TRACKS; t++)
	{
		po->Write((int)Tracks[t].peerctrl.size());

		for (int i=0; i<Tracks[t].peerctrl.size(); i++)
		{
			Tracks[t].peerctrl[i]->WriteFileData(po, backwardsCompatible);
			po->Write(Tracks[t].Track[i]);
			Tracks[t].mapping[i]->WriteData(po);
		}

		po->Write(Tracks[t].MIDIChannel);
		po->Write(Tracks[t].MIDIController);
		po->Write(Tracks[t].MIDIFeedback);
		po->Write(Tracks[t].MIDIIncDec);
		po->Write(Tracks[t].MIDIIncDecWrap);
		po->Write(Tracks[t].MIDIFBOutDev);

//		po->Write(Tracks[t].DisplayName, 32);

	}

	po->Write((int)plugs.size());
	for (int i=0; i<plugs.size(); i++)
	{
		WriteString(plugs[i]->dllname, po);
		CPlug *pPl = plugs[i];

		//write custom data
		CInternalDataWrite dw;
		pPl->pPl->WriteFileData((CPCtrlDataWrite *)&dw);
		po->Write(dw.DataLen);

		CDataChunk *ch = &dw.d;
		while (ch)
		{
			po->Write(ch->data, ch->ActualLength);
			ch = ch->next;
		}

		// write bindings
		po->Write((int)pPl->axes.size());
		for (int a=0; a<pPl->axes.size(); a++)
		{
			po->Write((int)pPl->axes[a].binding.size());
			for (int b=0; b<pPl->axes[a].binding.size(); b++)
				po->Write((byte)pPl->axes[a].binding[b]);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool mi::DblClickEvent(void *p)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	return false;

/*	switch (aval.DblClickAction)
	{
	case 0: return false;
	case 1: Command(0); return true;
	default: return false;
	}
*/}

/////////////////////////////////////////////////////////////////////////////

void CTrack::StopInertia()
{
	ValueSliding = false;
	ValueCurrent = ValueTarget;
}

/////////////////////////////////////////////////////////////////////////////

/*int CTrack::GetParValue(int CtrlNum, float fValue)
{
	if (peerctrl[CtrlNum]->GotParam())
	{
		int min = peerctrl[CtrlNum]->GetParamInfo()->MinValue;
		int max = peerctrl[CtrlNum]->GetParamInfo()->MaxValue;

		return min + (int)( (max - min) * fValue );
	}
	else
		return 0;
}
*/
/////////////////////////////////////////////////////////////////////////////

void mi::SendMIDICC(const int device, const int channel, const int ctrl, const int value)
{
	dword data	= (0xAF + channel)
				+ (ctrl << 8)
				+ (value << 16);
	pCB->MidiOut(device, data);
}

////////////////////////////////////////////////////////////////////////

void CTrack::ValueChange(float val, CPlug *pPl, int nAxis)
{
	if (!p_mi->bMuted || !p_mi->aval.StopOnMute)
	{
		ValueTarget = val;
		if (p_mi->Inertia)
		{
			ValueStep = (ValueTarget - ValueCurrent) / (p_mi->InertiaLength * p_mi->pMasterInfo->SamplesPerTick);
			ValueSliding = true;
		}
		else
		{
			ValueCurrent = ValueTarget;
			ValueSliding = false;
		}

		static CPlug *pLastPl = NULL;

		for (int a=0; a<axes.size(); a++)
		{
	//		static char txt[256];
	//		sprintf(txt,"a %X | p %X | l %X",axes[a].pPl, pPl, pLastPl);
	//		OutputDebugString(txt);

			bool bFromThisPlug = (axes[a].pPl == pPl) ||
									(pPl == NULL && axes[a].pPl == pLastPl);
			if (pPl) bFromThisPlug = bFromThisPlug && (axes[a].nAxis == nAxis);

	//		OutputDebugString(bFromThisPlug ? ".'. true" : ".'. false");

	//		sprintf(txt,"a[a].n %i | n %i",axes[a].nAxis,nAxis);
	//		OutputDebugString(txt);

			if (!bFromThisPlug)
				axes[a].pPl->pPl->ValueChanged(axes[a].nAxis, val);
		}

		pLastPl = pPl;

		if (pPl) //from a plugin
		{
			p_mi->pCB->ControlChange(p_mi->ThisMac, 2, tracknum, numValueInT, (int)(ValueTarget * 65534.0f));
		}	

		if (tracknum < p_mi->numTracks)
			if (p_mi->Tracks[tracknum+1].Slaved)
				p_mi->Tracks[tracknum+1].ValueChange(val,pPl,nAxis);
	}
}

////////////////////////////////////////////////////////////////////////

void CTrack::Tick()
{
#	ifndef _NOSLAVE
		if (tval->Slave != paraSlave.NoValue)
			if (tracknum != 0)
			{
				Slaved = (tval->Slave == SWITCH_ON);
			}
			else
				Slaved = false;
#	endif

//	if (Slaved)
//		tval->Value = p_mi->Tracks[tracknum-1].tval->Value;

/*	char txt[256];
	sprintf(txt, "Track %i value = %X", tracknum, tval->Value);
	OutputDebugString(txt);
*/

	if (tval->Value != paraValue.NoValue && !Slaved)
	{
		ValueChange(tval->Value / 65534.0f, NULL, 0);

		//MIDI fb
		if (MIDIFeedback)
		{
			if (MIDIController >= 0 && MIDIController <= 127)
			{
				int NewFBCCValue = (int)(ValueTarget * 127.0f + 0.5f);
				if (NewFBCCValue != LastFBCCValue)
				{
					if (MIDIChannel != 0 && MIDIChannel != 17)
						p_mi->SendMIDICC(MIDIFBOutDev, MIDIChannel, MIDIController, NewFBCCValue);
					else if (MIDIChannel == 17) //all
						for (int c=1; c<=16; c++)
							p_mi->SendMIDICC(MIDIFBOutDev, c, MIDIController, NewFBCCValue);
				}
				LastFBCCValue = NewFBCCValue;
			}
		}
	}


//	UpdateInertia(p_mi->SamplesSinceLastUpdate, false);

}

/////////////////////////////////////////////////////////////////////////////

void mi::Tick()
{
	if (!MDKInitRun)
		return;

	if (pMasterInfo->PosInTick == 0)
	{
		bMuted = !bWorkedSinceLastTick;
		bWorkedSinceLastTick = false;
	}

	if (gval.Inertia != paraInertia.NoValue)
	{
		if (gval.Inertia == 0)
		{
			Inertia = false;
			for (int t=0; t<numTracks; t++) Tracks[t].StopInertia();
		}
		else
		{
			Inertia = true;
			InertiaLength = gval.Inertia / 10.0f;
		}
	}

	for (int t=0; t<numTracks; t++) Tracks[t].Tick();

	for (int i=0; i<plugs.size(); i++)
	{
		if (plugs[i]->pInfo->Flags & PCPF_CALL_TIMER)
		{
			plugs[i]->pPl->Timer(SamplesSinceLastUpdate);
		}
	}

/*	// check for parameter window
	char txt[256];
	sprintf(txt,"%s - BTDSys PeerCtrl",pCB->GetMachineName(ThisMac));
	HWND hWnd = FindWindow(NULL, txt);
	if (hWnd)
	{
		if (!bHackedParamWndLastTick)
		{
			for (int t=0; t<numTracks; t++)
			{
				sprintf(txt,"%i-Value",t);
				HWND hLabel = FindWindowEx(hWnd,NULL,NULL,txt);
				if (hLabel)
					SetWindowText(hLabel, Tracks[t].DisplayName);
			}
			bHackedParamWndLastTick = true;
		}
	}
	else
		bHackedParamWndLastTick = false;
*/


	SamplesSinceLastUpdate = 0;

	JustTicked = true;
}

/////////////////////////////////////////////////////////////////////////////

inline float GetEnvPoint(CEnvData *env, float pos)
{
	int p=0;
	while (env->points[p*2] < pos) p++;
	if (p > 0) p--;

	int x1 = env->points[p*2];
	int y1 = env->points[p*2+1];
	int x2 = env->points[p*2+2];
	int y2 = env->points[p*2+3];

	return (float)y1 + (float)(y2-y1) * (pos-x1)/(float)(x2-x1);
}

/////////////////////////////////////////////////////////////////////////////

inline int CTrack::FloatToPVal(int n, float fval)
{
	const CMachineParameter *pp = peerctrl[n]->GetParamInfo();
	int min = pp->MinValue;
	int max = pp->MaxValue + 1;
	if (pp->Type == pt_switch)
	{
		min = 0;
		max = 2;
	}
	else if (pp->Type == pt_note)
	{
		min = 0;
		max = 12*10;
	}

	float f = GetEnvPoint(mapping[n], fval * 65535.0f);

	f = 1.0f - (f / 65535.0f);
	int k = (int)(f * (float)(max-min) + (float)min);
	if (k >= max) k = max-1;

	if (pp->Type == pt_note)
	{
		return (k % 12) + 1 + ((k/12) << 4);
	}
	else
		return k;
}

/////////////////////////////////////////////////////////////////////////////

void CTrack::UpdateInertia(int numsamples)
{
	ShouldSend = false;
/*	if (Slaved)
	{
		ValueCurrent = p_mi->Tracks[tracknum-1].ValueCurrent;
//		ValueTarget  = p_mi->Tracks[tracknum-1].ValueTarget ;
		ShouldSend = p_mi->Tracks[tracknum-1].ShouldSend;
	}
	else
*/	{
		if (ValueSliding)
		{
			ValueCurrent += ValueStep * numsamples;
			if (	(ValueCurrent > ValueTarget && ValueStep > 0)
				||	(ValueCurrent < ValueTarget && ValueStep < 0))
				StopInertia();
			ShouldSend = true;
		}
		else
		{
			if (ValueOnLastUpdate != ValueCurrent)
				ShouldSend = true;
		}
	}

	if (ShouldSend && !p_mi->Initialising)
	{
		for (int i=0; i<peerctrl.size(); i++)
		{
			if (peerctrl[i]->GotParam() /*&& !p_mi->JustTicked*/)
			{
				if (ValueCurrent < 0.0f) ValueCurrent = 0.0f;
				if (ValueCurrent > 1.0f) ValueCurrent = 1.0f;
				int ValueP = FloatToPVal(i, ValueCurrent);

				if (Track[i] != -1)
					peerctrl[i]->ControlChange_Immediate(Track[i], (int)ValueP);
				else
				{
					for (int t=0; t<peerctrl[i]->GetMInfo()->maxTracks; t++)
						peerctrl[i]->ControlChange_Immediate(t, (int)ValueP);
				}
			}
		}
	}

	ValueOnLastUpdate = ValueCurrent;
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

/*void CTrack::UpdateJoy(JOYINFOEX ji)
{
	if (JoyItemType != 0)
	{
		bool ShouldControl = false;
		float NewValueTarget;
		
		switch (JoyItemType)
		{
		case 'm': //momentary button
			NewValueTarget = (float)((ji.dwButtons >> JoyItem) & 1);
			ShouldControl = (NewValueTarget != JoyCurrentPos);
			break;

		case 't': //toggle button
			{
				float NewPos = (float)((ji.dwButtons >> JoyItem) & 1);
				if (JoyLastPos == 0.0f && NewPos == 1.0f)
				{
					JoyToggleState = !JoyToggleState;
					NewValueTarget = (JoyToggleState ? 1.0f : 0.0f);
					ShouldControl = true;
				}
				JoyLastPos = NewPos;
			}
			break;

		case 'A': //axis
			{
				int NewPos;
				switch (JoyItem)
				{
				case 'X': NewPos = ji.dwXpos; break;
				case 'Y': NewPos = ji.dwYpos; break;
				case 'Z': NewPos = ji.dwZpos; break;
				case 'R': NewPos = ji.dwRpos; break;
				case 'U': NewPos = ji.dwUpos; break;
				case 'V': NewPos = ji.dwVpos; break;
				default:  NewPos = 0;
				}

				NewValueTarget = (float)(NewPos - JoyMin) / (float)(JoyMax - JoyMin);
				
				if (fabsf(NewValueTarget - JoyCurrentPos) > JoyAntiJitterThreshold)
					ShouldControl = true;
			}
		}

		if (ShouldControl)
		{
			JoyCurrentPos = ValueTarget = NewValueTarget;

			if (p_mi->Inertia)
			{
				ValueStep = (ValueTarget - ValueCurrent) / (p_mi->InertiaLength * p_mi->pMasterInfo->SamplesPerTick);
				ValueSliding = true;
			}
			else
			{
				ValueCurrent = ValueTarget;
				ValueSliding = false;
			}
			p_mi->pCB->ControlChange(p_mi->ThisMac, 2, tracknum, 1, (int)(ValueTarget * 65534.0f));
		}
	}
}
*/
/////////////////////////////////////////////////////////////////////////////

bool mi::MDKWork(float *psamples, int numsamples, const int mode)
{
	bMuted = false;
	bWorkedSinceLastTick = true;

	UpdateCounter++;
	const CSubTickInfo* sti = pCB->GetSubTickInfo();
	if ((!sti || sti->PosInSubTick == 0) && UpdateCounter >= aval.SendFreq && aval.SendFreq != 0)
	{
		CPeerCtrl::BeginImmediateBatch();

		for (int t=0; t<numTracks; t++)
			Tracks[t].UpdateInertia(SamplesSinceLastUpdate);

		for (int i=0; i<plugs.size(); i++)
		{
			if (plugs[i]->pInfo->Flags & PCPF_CALL_TIMER)
			{
				plugs[i]->pPl->Timer(SamplesSinceLastUpdate);
			}
		}

		UpdateCounter = SamplesSinceLastUpdate = 0;

		CPeerCtrl::EndImmediateBatch();
	}

	SamplesSinceLastUpdate += numsamples;

/*	UpdateJoyCounter++;
	if (UpdateJoyCounter >= UpdateJoyCounterThreshold)
	{
		UpdateJoyCounter = 0;
		JOYINFOEX ji[2];
		ji[0].dwSize = sizeof(ji[0]);
		ji[0].dwFlags = JOY_RETURNALL;
		ji[1].dwSize = sizeof(ji[1]);
		ji[1].dwFlags = JOY_RETURNALL;

		int res[2];
		res[0] = joyGetPosEx(JOYSTICKID1, &ji[0]);
		res[1] = joyGetPosEx(JOYSTICKID2, &ji[1]);

		for (t=0; t<numTracks; t++)
			if (res[Tracks[t].JoyID] == JOYERR_NOERROR)
				Tracks[t].UpdateJoy(ji[Tracks[t].JoyID]);
	}
*/
	Initialising = false;
	JustTicked = false;

	return false;
}

/////////////////////////////////////////////////////////////////////////////

void miex::GetSubMenu(const int i, CMachineDataOutput *pout)
{
	char const *Menus = p_mi->plugs[i-3]->pInfo->Menus;

	char mstr[256];
	mstr[0] = 0;
	int k = 0;

	while (*Menus != 0)
	{
		mstr[k] = *Menus;
		k++;
		mstr[k] = 0;
		Menus++;
		if (*Menus == '\n')
		{
			pout->Write(mstr);
			k = 0;
			mstr[0] = 0;
			Menus++;
		}
	}

	if (mstr[0] != 0)
		pout->Write(mstr);
}

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (i)
	{
	case 0: //settings
		{
			CSettingsDialog SetDlg;
			SetDlg.pmi = this;
			SetDlg.DoModal();
		}
		break;

	case 1: // show PAL
		CPeerCtrl::ShowPAL();
		break;

	case 2: //about
		MessageBox (NULL,
#			ifdef _NOSLAVE
				"BTDSys PeerCtrl 'Basic' 1.6\n"
#			else
				"BTDSys PeerCtrl 1.6\n"
#			endif
			"©2002-2008 Ed Powley (BTDSys)\n"
			"http://btdsys.lazytrap.com",
			"BTDSys PeerCtrl",
			MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION);
		break;

	default: //plugin
		{
			int nPlug = (i >> 8) - 1;
			int nCommand = i & 255;

			plugs[nPlug]->pPl->MenuCommand(nCommand);
		}
		break;

	}


/*	if (i == 2)
	{
		if (pmixerdlg == NULL)
			pmixerdlg = new CMixerDlg(this);
		else
			pmixerdlg->SetActiveWindow();
	}

	if (i == 0)
	{
		if (pxydlg == NULL)
			pxydlg = new CXYDialog(this);
		else
			pxydlg->SetActiveWindow();
	}

	if ((i >= 256) && (i < 512)) //config dlg
	{
		int c=0;
		for (int j=0; j<plugs.size(); j++)
		{
			if (plugs[j]->pInfo->Flags & PCPF_HAS_CONFIG_DLG)
			{
				if (c == i-256)
					plugs[j]->pPl->OpenConfigDlg();
				c++;
			}
		}
	}

	if ((i >= 512) && (i < 768)) //input dlg
	{
		int c=0;
		for (int j=0; j<plugs.size(); j++)
		{
			if (plugs[j]->pInfo->Flags & PCPF_HAS_INPUT_DLG)
			{
				if (c == i-512)
					plugs[j]->pPl->OpenInputDlg();
				c++;
			}
		}
	}
*/



}

/////////////////////////////////////////////////////////////////////////////

const char *mi::DescribeValue(const int param, const int value)
{
	static char txt[256];

	switch (param)
	{
	case numInertia:
		if (value == 0) return "Off";
		else
		{
			sprintf(txt, "%.1f ticks", value / 10.0f);
			return txt;
		}
		break;

#	ifndef _NOSLAVE
		case numSlave:
			if (value == SWITCH_ON)
				return "Yes";
			else
				return "No";
			break;
#	endif

	case numValue:
		sprintf(txt, "%.2f%%", value / 655.34f);
		return txt;
		break;

	default:
		return NULL;
	}
}
