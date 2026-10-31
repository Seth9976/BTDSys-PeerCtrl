/*
Copyright (c) 2002-2011, Edward Powley
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

//PeerADSR
//©2002 Ed Powley - created 22-Aug-02
//An envelope trigger

#include "stdafx.h"

#include <windows.h>
#include "../mdk/MachineInterface.h"
#include "../PeerLib2/peerctrl.h"
#include <float.h>
#include <stdlib.h>
#include "resource.h"
#include "AssignDlg.h"

////////////////////////////////////////////////////////////////////////

#include "params.h"
CMachineParameter const *pParameters[] = {
	&paraTrigger,
	&paraEnvType,
	&paraEnvNum,
	&paraLength,
	&paraMin,
	&paraMax,
	&paraAttack,
	&paraDecay,
	&paraSustainLength,
	&paraSustainLevel,
	&paraRelease,
	&paraTrack,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	&attrLengthMax,
	&attrSubtickCtrl,
	&attrMIDIChannel,
	&attrMIDITrack,
	&attrMIDIWantNoteOffs,
	NULL
};


#include "mi_ctrack_defs.h"

CMachineInfo const MacInfo =
{
	MT_GENERATOR,							//Machine type
	MI_VERSION,								//Interface version
	MIF_NO_OUTPUT | MIF_CONTROL_MACHINE,	//Flags
	1,										//Min tracks
	MAX_TRACKS,								//Max tracks
	NUMGLOBAL,								//Number of global parameters
	NUMTRACK,								//Number of track parameters
	pParameters,							//Pointer to parameters
	NUMATTRIBUTES,							//Number of attributes
	pAttributes,							//Pointer to attributes
	"BTDSys PeerADSR",						//Full name
	"PeerADSR",								//Short name
	"Ed Powley (BTDSys)",					//Author
	"/Assign Parameter...\n"
	"Custom Envelopes...\n"
	"Show PAL\n"
	"About..."								//Commands
};

////////////////////////////////////////////////////////////////////////

DLL_EXPORTS_AFX

////////////////////////////////////////////////////////////////////////

//Just your standard constructor stuff
mi::mi()
:
Initialising(true),
envdlg(NULL),
LastMidiNote(-1)
{
	GlobalVals = &gval;
	TrackVals = &tval;
	AttrVals = (int *)&aval;

	for (int t=0; t<MAX_TRACKS; t++)
	{
		Tracks[t].pmi = this;
		Tracks[t].index = t;
	}
}

////////////////////////////////////////////////////////////////////////

mi::~mi()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (envdlg != NULL)
		envdlg->DestroyWindow();
}

////////////////////////////////////////////////////////////////////////

tvals def_t =
{
	paraTrigger.DefValue,
	paraEnvType.DefValue,
	paraEnvNum.DefValue,
	paraLength.DefValue,
	paraMin.DefValue,
	paraMax.DefValue,
	paraAttack.DefValue,
	paraDecay.DefValue,
	paraSustainLength.DefValue,
	paraSustainLevel.DefValue,
	paraRelease.DefValue,
	paraTrack.DefValue
};

void CTrack::Init()
{
	JustStoppedEnveloping = false;

	PeerCtrl.Init(pmi->ThisMac, pmi);
	PeerCtrl.SetLabelAsTrackNumber(index);

	Tick(&def_t);
}

#define CURRENT_VERSION 3

//Called when any machine is renamed
bool mi::OnRenameMachine(void *pmac)
{
	if((CMachine*)pmac == ThisMac && envdlg != NULL)
	{
		envdlg->RefreshTitleBar();
	}

	// Tell peerctrl
	CPeerCtrl::OnRenameMachine((CMachine*)pmac);

	return true;
}

//Initialise and load data (if any)
void mi::Init (CMachineDataInput * const pi)
{
	ThisMac = pCB->GetThisMachine();

	pCB->SetMachineInterfaceEx(this);

	int t;
	for (t=0; t<MAX_TRACKS; t++)
	{
		Tracks[t].Init();
	}

	if (pi != NULL) //got data
	{
		byte dummy;
		pi->Read(dummy); // Handle MDK version byte

		byte Version;
		pi->Read(Version);
		if (Version == CURRENT_VERSION)
		{
			for (int e=0; e<NUM_CUSTOM_ENVS; e++)
			{
				CustomEnv[e].Load(pi);
			}

			int NumTracks;
			pi->Read(NumTracks);

			for (t=0; t<NumTracks; t++)
			{
				Tracks[t].PeerCtrl.ReadFileData(pi); //Restore assignments
			}
		}
	}

	pCB->SetEventHandler(ThisMac, gRenameMachine, (EVENT_HANDLER_PTR) &mi::OnRenameMachine, this);

	CPeerCtrl::SetEventHandlers(pCB, ALL_EVENT_HANDLERS & ~EVENT_HANDLER_RENAME);
}

////////////////////////////////////////////////////////////////////////

//Save data
void mi::Save(CMachineDataOutput * const po)
{
	po->Write((byte)0); // Fake MDK version byte

	po->Write((byte)CURRENT_VERSION);

	for (int e=0; e<NUM_CUSTOM_ENVS; e++)
	{
		CustomEnv[e].Save(po);
	}

	po->Write(numTracks);

	for (int t=0; t<numTracks; t++)
		Tracks[t].PeerCtrl.WriteFileData(po); //Store assignments
}

////////////////////////////////////////////////////////////////////////

#include "envstuff.h"

inline int CTrack::FloatToPVal(float fval)
{
	int min = PeerCtrl.GetParamInfo()->MinValue;
	int max = PeerCtrl.GetParamInfo()->MaxValue;
	return (int)((fval * (fmax - fmin) + fmin) * (float)(max - min) + (float) min);
}

void CTrack::Trigger(byte TrigVal)
{
		if (TrigVal == SWITCH_ON)
		{
			int spt = pmi->pMasterInfo->SamplesPerTick;
			Enveloping = true;
			switch (EnvType)
			{
			case 0: //ADSR
				lenA = (int)(envA * spt);
				lenD = (int)(envD * spt);
				lenS = (int)(envS * spt);
				lenR = (int)(envR * spt);
				EnterA();
				break;

			case 1: //Custom
				len  = (int)(envLength * spt);
				EnvPos = 0.0f;
				if (pmi->CustomEnv[EnvNum-1].suspoint == -1)
					EnvPhase = epR;
				else
					EnvPhase = epA;

				EnvPosStep = 65536.0f / len;
				break;

			case 2: //Wave
				{
					len  = (int)(envLength * spt);
					EnvPos = 0.0f;
					CWaveLevel const *lev = pmi->pCB->GetWaveLevel(EnvNum, 0);
					if (lev == NULL)
						Enveloping = false;
					else
						EnvPosStep = (float)lev->numSamples / (float)len;
				}
				break;
			}
		}
		else //off
		{
			if (Enveloping)
			{
				switch (EnvType)
				{
				case 0: //ADSR
					if (EnvPhase != epR) EnterR();
					break;

				case 1: //Custom
					if (EnvPhase != epR && pmi->CustomEnv[EnvNum-1].suspoint != -1)
					{
						EnvPos = (float)pmi->CustomEnv[EnvNum-1].ptcoords[pmi->CustomEnv[EnvNum-1].suspoint * 2];
						EnvPhase = epR;
					}
					else
						Enveloping = false;
					break;

				case 2: //Wave
					Enveloping = false;
					break;
				}
			}
		}
}

void CTrack::Tick(tvals *tv)
{
	if (tv->EnvType != paraEnvType.NoValue)
		EnvType = tv->EnvType;

	if (tv->EnvNum != paraEnvNum.NoValue)
		EnvNum = tv->EnvNum;

	if (tv->Length != paraLength.NoValue)
		envLength = pmi->LengthValToTicks(tv->Length);

	if (tv->Min != paraMin.NoValue)
		fmin = tv->Min / 65534.0f;

	if (tv->Max != paraMax.NoValue)
		fmax = tv->Max / 65534.0f;

	if (tv->Attack != paraAttack.NoValue)
		envA = pmi->LengthValToTicks(tv->Attack);

	if (tv->Decay != paraDecay.NoValue)
		envD = pmi->LengthValToTicks(tv->Decay);

	if (tv->SustainLength != paraSustainLength.NoValue)
		if (tv->SustainLength == 0xFFFF) //infinite
			envS = -1.0f;
		else
			envS = pmi->LengthValToTicks(tv->SustainLength);

	if (tv->SustainLevel != paraSustainLevel.NoValue)
		envSLev = tv->SustainLevel / 65534.0f;

	if (tv->Release != paraRelease.NoValue)
		envR = pmi->LengthValToTicks(tv->Release);

	if (tv->Track != paraTrack.NoValue)
		Track = tv->Track;

	if (tv->Trigger != paraTrigger.NoValue)
		Trigger(tv->Trigger);

	if (!pmi->Initialising) //OK to send data
	{
		if (PeerCtrl.GotParam() && (Enveloping || JustStoppedEnveloping)) //Still OK to send data
		{
			//Convert our float value to an int for the param
			if (ValueCurrent < 0.0f) ValueCurrent = 0.0f;
			if (ValueCurrent > 1.0f) ValueCurrent = 1.0f;
			int ValueP = FloatToPVal(ValueCurrent);

			if (Track != 0xFE) //Just one track
				PeerCtrl.ControlChange_Immediate(Track, (int)ValueP);
			else //all tracks
			{
				for (int t=0; t<PeerCtrl.GetMInfo()->maxTracks; t++)
					PeerCtrl.ControlChange_Immediate(t, (int)ValueP);
			}
		}
	}

	JustStoppedEnveloping = false;
}

////////////////////////////////////////////////////////////////////////

void mi::Tick()
{
	for (int t=0; t<numTracks; t++)
		Tracks[t].Tick(&tval[t]);

	JustTicked = true;
}

////////////////////////////////////////////////////////////////////////

bool mi::Work(float *psamples, int numsamples, const int mode)
{
	for (int t=0; t<numTracks; t++)
		Tracks[t].UpdateEnv(numsamples);
	
	JustTicked = false;

	return false;			
}

////////////////////////////////////////////////////////////////////////

//Restore assignments after song load, clipboard paste and template import
void mi::ImportFinished()
{
	CPeerCtrl::OnImportFinished();

	Initialising = false;
}

//Get submenu items showing current assignments
void mi::GetSubMenu(const int i, CMachineDataOutput *pout)
{
	char txt[256];

	for (int t = 0; t < numTracks; t++)
	{
		sprintf(txt, "%i: %s", t, Tracks[t].PeerCtrl.GetAssignmentString());
		pout->Write(txt);
	}
}

void mi::SetDeletedState(bool deleted)
{
}

////////////////////////////////////////////////////////////////////////

#include "assigndlg.h"
#include "assigndlg2.h"
#include "customenvdlg.h"

void mi::Command(const int i)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch(i)
	{
	case 1 : //Custom env
		{
			if (envdlg == NULL)
				envdlg = new CCustomEnvDlg(this);
			else
				envdlg->SetActiveWindow();
		}
		break;

	case 2 : // Show PAL
		CPeerCtrl::ShowPAL();
		break;

	case 3 : //About
		{
			MessageBox(NULL,
				"BTDSys PeerADSR v1.1\n"
				"©2002-2011 Ed Powley (BTDSys)\n\n"
				"http://sourceforge.net/projects/btdsys/"
				"\n\nThis machine is DONATIONWARE, so if you like it, and want to "
				"send me something nice (eg hardware, CDs, money etc etc), drop "
				"me an email.\n\n"
				"Please read the help file for usage instructions",
				"BTDSys PeerADSR", 
				MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION);
		}
		break;

	default :
		if (i >= 256 && i < 512) //Assign
		{
			DoAssignPar (i - 256);
		}
		break;
	}
}

float mi::LengthValToTicks(int val)
{
	return (float)val / 65534.0f * (float)aval.LengthMax;
}

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[256];

	switch (param)
	{
	case numEnvType:
		switch (value)
		{
		case 0: return "ADSR";		break;
		case 1: return "Custom";	break;
		case 2: return "Wavetable";	break;
		default:return "Unknown";
		}
		break;

	case numEnvNum:
		return NULL;
		break;		

	case numLength:
		sprintf(txt, "%.2f ticks", LengthValToTicks(value));
		return txt;
		break;

	case numMin:
	case numMax:
		sprintf(txt, "%.2f%%", value / 655.34f);
		return txt;
		break;

	case numAttack:
	case numDecay:
	case numSustainLength:
	case numRelease:
		if (param == numSustainLength && value == 0xFFFF)
			return "Infinite";
		else
		{
			sprintf(txt, "%.2f ticks", LengthValToTicks(value));
			return txt;
		}
		break;

	case numSustainLevel:
		sprintf(txt, "%.2f%%", value / 655.34f);
		return txt;
		break;

	case numTrack:
		if (value == 0xFE)
			return "All";
		else
		{
			sprintf(txt, "%i", value);
			return txt;
		}
		break;

	default:
		return NULL;
	}
}

