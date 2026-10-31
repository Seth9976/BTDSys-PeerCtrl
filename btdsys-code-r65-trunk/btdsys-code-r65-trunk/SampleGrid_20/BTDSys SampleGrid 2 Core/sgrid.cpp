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
#include "../../mdk/mdk.h"
#include "../../dsplib/dsplib.h"
#include <windows.h>
#include <commctrl.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <process.h>
#include "common.h"

#include "kitsavedlg.h"
#include "groupdlg.h"
#include "groupperfdlg.h"
#include "loopdlg.h"
#include "fuzzpilz_envelope.h"
#include "aboutdlg.h"
#include "cmdlistdlg.h"
#include "shuffledlg.h"

#pragma optimize ("awy", on)

////////////////////////////////////////////////////////////////////////

#include "sgrid.h"

const CEnvelopeInfo envDummy =
{
	"Dummy",
	0			//flags
};

const CEnvelopeInfo *pEnvelopes[] =
{
	&envDummy,
	NULL
};

const CEnvelopeInfo **mi::GetEnvelopeInfos() {return pEnvelopes;}

//#include "midi.h"

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

#define RGainCurrent (2.0f - LGainCurrent)

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////


void (__stdcall *AddMe)(CMachine *mac, float **buffers);
void (__stdcall *DelMe)(CMachine *mac);

HMENU henvemenu = NULL;
HMENU henvebgmenu = NULL;
HMENU henvelmmenu = NULL;
HMENU henvermmenu = NULL;
HMENU henveptmenu = NULL;

mi::mi(int n, char type) 
{ 
	pszMacName[0] = 0;
	nTriggers = n;
	nTrigType = type;

	dbgprintf("%c %i",type,n);

	AttrVals = (int*) &aval;
	GlobalVals = &gval.Triggers[MAXTRIGGERS - nTriggers];
	TrackVals = &tval;
	ex.pmi = this;

	hMidiDlg = 0;

	ipVolume.SetMinMax(0.0f, 2.0f);
	ipPan.SetMinMax(0.0f, 2.0f);
	ipTune.SetMinMax(-3.0f, 3.0f);

	for (int t=0; t<n; t++)
	{
		Tracks[t].pmi = this;
		Tracks[t].tval = &tval[t];
		Tracks[t].tracknum = t;
		Tracks[t].Reset();
	}

	bKitMode = false;
	nKitSaveMode = 0;

	pGroupPerfDlg = NULL;
	pLoopDlg = NULL;
	pGroupDlg = NULL;
	pEnveDlg = NULL;
	pCmdListDlg = NULL;

	if (!henvemenu)
	{
		henvemenu = ::LoadMenu(dllInstance,MAKEINTRESOURCE(IDR_FUZZPILZENVMENU));
		henvebgmenu = GetSubMenu(henvemenu,0);
		henvelmmenu = GetSubMenu(henvebgmenu,1);
		henvermmenu = GetSubMenu(henvebgmenu,2);
		henveptmenu = GetSubMenu(henvemenu,1);

		int id = 44096;
		int i;
		DeleteMenu(henvelmmenu,0,MF_BYPOSITION);
		DeleteMenu(henvermmenu,0,MF_BYPOSITION);
		for (i=0; i<NumEnvLoopModes; i++, id++)
			InsertMenu(henvelmmenu,i,MF_BYPOSITION | MF_STRING,id,EnvLoopModeNames[i]);
		for (i=0; i<NumEnvReleaseModes; i++, id++)
			InsertMenu(henvermmenu,i,MF_BYPOSITION | MF_STRING,id,EnvReleaseModeNames[i]);

	}
}

mi::~mi() 
{
	pszMacName[0] = 0;

	for (int i=0; i<g_vpmi.size(); i++)
		if (g_vpmi[i] == this) g_vpmi.erase(&g_vpmi[i]);

	if (DelMe)
		DelMe(ThisMac);

	if (hm)
		FreeLibrary(hm);

	if (henvemenu)
		DestroyMenu(henvemenu);
	henvemenu = NULL;
	henvebgmenu = NULL;
	henvelmmenu = NULL;
	henvermmenu = NULL;
	henveptmenu = NULL;

/*	if (hCoreDLL)
	{
		PFUnreg f = (PFUnreg)GetProcAddress(hCoreDLL,"Unreg");
		f(this);

		FreeLibrary(hCoreDLL);
	}
*/
}

////////////////////////////////////////////////////////////////////////

void CTrack::Reset()
{
	Playing = false;
	ipVolume.SetValueNoInertia(1.0f);
	ipPan.SetValueNoInertia(1.0f);
	ipTune.SetValueNoInertia(0.0f);
	RampingOn = RampingOff = MuteRamping = Waiting = Muted = false;
	Group = 0;
}

////////////////////////////////////////////////////////////////////////

void ReadString (char *temp, CMachineDataInput * const pi)
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

void WriteString (const char *temp, CMachineDataOutput * const po)
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

HWND FindBUZZhwnd()
{
  char t[1024];
  HWND wnd = GetActiveWindow();

  if(wnd == NULL)
    return NULL;

  GetWindowText(wnd, t, 1000);
  while(strstr(t, "Buzz") == NULL && wnd != NULL)
  {
    wnd = GetWindow(wnd, GW_HWNDNEXT);
    if(wnd != NULL)
      GetWindowText(wnd, t, 1000);
  }

  if(strstr(t, "Buzz") != NULL)
    return wnd;
  
  GetWindowText(wnd, t, 1000);
  while(strstr(t, "Buzz") == NULL && wnd != NULL)
  {
    wnd = GetWindow(wnd, GW_HWNDPREV);
    if(wnd != NULL)
      GetWindowText(wnd, t, 1000);
  }

  if(strstr(t, "Buzz") != NULL)
    return wnd;
  
  return NULL;
}

void mi::MDKInit (CMachineDataInput * const pi)
{
	SetOutputMode (true);
	Initialising = true;

	ThisMac = pCB->GetThisMachine();

	hm = 0;

	hm = LoadLibrary("Gear\\Effects\\BTDSys SampleGrid Out.dll");
	if (hm)
	{
		AddMe = (void (__stdcall *)(CMachine*, float**))GetProcAddress(hm, "AddMe");
		DelMe = (void (__stdcall *)(CMachine*))GetProcAddress(hm, "DelMe");

		if (AddMe)
		{
			float *bufs[NUM_OUTPUTS];
			for (int g=0; g<NUM_OUTPUTS; g++)
				bufs[g] = pfOutBuf[g];

			AddMe(ThisMac, bufs);
		}
	}
	else
		MessageBox(NULL, "Couldn't find BTDSys SampleGrid Out.dll", "BTDSys SampleGrid", MB_OK);

	HWND hBuzz = FindBUZZhwnd();
	if (hBuzz)
	{
		HWND h = FindWindowEx(hBuzz,NULL,"MDIClient","");
		h = FindWindowEx(h,NULL,NULL,"Pattern Editor");
		hPatEdStatusBar = FindWindowEx(h,NULL,"msctls_statusbar32","");
		h = FindWindowEx(h,NULL,(LPCTSTR)32770,"");
		hPatEdMachineCombo = GetDlgItem(h,0x3E9);
	}
	else
		hPatEdStatusBar = hPatEdMachineCombo = NULL;

	dbgprintf("hPatEdStatusBar == %X", hPatEdStatusBar);

	for (int t=0; t<nTriggers; t++) for (int c=0; c<NUM_PEERS; c++)
		Tracks[t].pCtrl[c] = new CPeerCtrl(ThisMac,this);

	strcpy(Groups[0].name,"Default");

	int g;
	for (g=1; g<=8; g++)
	{
		sprintf(Groups[g].name,"Output %i",g);
		Groups[g].nOutput = g;
	}
	for (g=9; g<16; g++)
	{
		sprintf(Groups[g].name,"XOR %c",g-9+'A');
		Groups[g].bXOR = true;
	}

	//read data
	if (pi != NULL)
	{
		byte Version;
		pi->Read(Version);
		if (Version == 7)
		{	//data is compatible
			int t;

			//MIDI data
			for (t=0; t<nTriggers; t++)
			{
				pi->Read(Tracks[t].nTrigKey);
				byte tmp;
				pi->Read(tmp);
				Tracks[t].nTrigKeyType = (EKeyType)tmp;
			}

			//drumkit data
			pi->Read(bKitMode);
			if (bKitMode)
			{
				Kit.Reset();
				Kit.dllInstance = dllInstance;

				pi->Read(nKitSaveMode);
				if (nKitSaveMode == 1 || nKitSaveMode == 2)
					Kit.Load(pi);
				else
				{
					char fname[256];
					ReadString(fname, pi);

					if (!Kit.LoadFromFile(fname))
					{
						char *fname2 = fname;
						char *c = fname;
						while (*c)
						{
							if (*c == '\\') fname2 = c+1;
							c++;
						}
						char txt[256];
						sprintf(txt,"%s%s",GetDefaultKitPath(),fname2);
						if (!Kit.LoadFromFile(txt))
						{
							sprintf(txt,"Couldn't load drumkit:\n%s",fname2);
							MessageBox(NULL,txt,"SampleGrid",MB_OK | MB_ICONERROR);
							bKitMode = false;
						}
					}
				}
			}

			//peer data
			for (t=0; t<nTriggers; t++) for (int c=0; c<NUM_PEERS; c++)
				Tracks[t].pCtrl[c]->ReadFileData(pi);

			//group data
			for (int g=0; g<NUM_GROUPS; g++)
			{
				ReadString(Groups[g].name,pi);
				pi->Read(Groups[g].nOutput);
				pi->Read(Groups[g].bXOR);
				pi->Read(Groups[g].nMuteKey);
				byte tmp;
				pi->Read(tmp);
				Groups[g].nMuteKeyType = (EKeyType)tmp;
				pi->Read(Groups[g].nMuteMode);
			}

			//envelope data
			for (int e=0; e<NumCustomEnvelopes; e++)
				pCustomEnvelopeShapes[e].Load(pi,1);

		}
	}

//	_beginthread(::ThreadFunc,0,this);

	g_vpmi.push_back(this);
}

const char *mi::GetDefaultKitPath()
{
	static char path[256];
	unsigned long i;
	unsigned long type;
	HKEY hk;
	if(RegCreateKey(HKEY_CURRENT_USER,"Software\\BTDSys\\SampleGrid",&hk) == ERROR_SUCCESS)
	{
		i = 256;
		if (RegQueryValueEx(hk,"KitPath",0,&type,(unsigned char *)path,&i) != ERROR_SUCCESS)
		{
			strcpy(path,"Gear\\Generators\\");
			RegSetValueEx(hk,"KitPath",0,REG_SZ,(unsigned char *)path,strlen(path)+1);
		}
		RegCloseKey(hk);

		if (path[strlen(path)-1] != '\\') strcat(path,"\\");

		if (path[1] != ':') //relative path
		{
			if(RegCreateKey(HKEY_CURRENT_USER,"Software\\Jeskola\\Buzz\\Settings",&hk) == ERROR_SUCCESS)
			{
				static char buzzpath[256];
				i = 256;
				if (RegQueryValueEx(hk,"BuzzPath",0,&type,(unsigned char *)buzzpath,&i) == ERROR_SUCCESS)
				{
					if (buzzpath[strlen(buzzpath)-1] != '\\') strcat(buzzpath,"\\");
					if (path[0] == '\\')
						strcat(buzzpath,path+1);
					else
						strcat(buzzpath,path);

					return buzzpath;
				}
			}
		}

		return path;
	}

	return "Gear\\Generators\\";
}

////////////////////////////////////////////////////////////////////////

void mi::MDKSave(CMachineDataOutput * const po)
{
	int t;

	//write data
	po->Write((byte)7); //version

	//MIDI data
	for (t=0; t<nTriggers; t++)
	{
		po->Write(Tracks[t].nTrigKey);
		po->Write((byte)Tracks[t].nTrigKeyType);
	}

	//drumkit data
	po->Write(bKitMode);
	if (bKitMode)
	{
		po->Write(nKitSaveMode);
		if (nKitSaveMode == 1 || nKitSaveMode == 2)
			Kit.Save(po, nKitSaveMode);
		else
			WriteString(Kit.FileName, po);
	}

	//peer data
	for (t=0; t<nTriggers; t++) for (int c=0; c<NUM_PEERS; c++)
		Tracks[t].pCtrl[c]->WriteFileData(po);

	//group data
	for (int g=0; g<NUM_GROUPS; g++)
	{
		WriteString(Groups[g].name,po);
		po->Write(Groups[g].nOutput);
		po->Write(Groups[g].bXOR);
		po->Write(Groups[g].nMuteKey);
		po->Write((byte)Groups[g].nMuteKeyType);
		po->Write(Groups[g].nMuteMode);
	}

	//envelope data
	for (int e=0; e<NumCustomEnvelopes; e++)
		pCustomEnvelopeShapes[e].Save(po);
}

////////////////////////////////////////////////////////////////////////

int CTrack::GetSampleNum()
{
	if (WaveNum == 0)
		return pmi->FirstWaveNo + tracknum;
	else
		return WaveNum;
}

CDrumkitSample *CTrack::GetPSISample()
{
/*	int ngrp = WaveNum >> 5;
	if (ngrp > 5)
	{
		Playing = false;
		return false;
	}
	int nsmp = WaveNum & 0x1F;

	return (CDrumkitSample *)pmi->Kit.GetSample(ngrp+1, nsmp);
*/
	return &pmi->Kit.sample[GetSampleNum()];
}

const char *CTrack::GetSampleName()
{
	if (pmi->bKitMode)
	{
		return GetPSISample()->Name;
	}
	else
		return pmi->pCB->GetWaveName(GetSampleNum());
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

void mi::InertiaOff()
{
/*	Inertia = false;

	if (VolumeSliding)
	{
		VolumeSliding = false;
		VolumeCurrent = VolumeTarget;
	}

	if (LGainSliding)
	{
		LGainSliding = false;
		LGainCurrent = LGainTarget;
	}

	if (TuneSliding)
	{
		TuneSliding = false;
		TuneCurrent = TuneTarget;
		for (int t=0; t<nTriggers; t++)
			Tracks[t].UpdatePosStep();
	}

	for (int t=0; t<nTriggers; t++)
	{
		if (Tracks[t].VolumeSliding)
		{
			Tracks[t].VolumeSliding = false;
			Tracks[t].VolumeCurrent = Tracks[t].VolumeTarget;
		}

		if (Tracks[t].LGainSliding)
		{
			Tracks[t].LGainSliding = false;
			Tracks[t].LGainCurrent = Tracks[t].LGainTarget;
		}

		if (Tracks[t].TuneSliding)
		{
			Tracks[t].TuneSliding = false;
			Tracks[t].TuneCurrent = Tracks[t].TuneTarget;
			Tracks[t].UpdatePosStep();
		}
	}
*/
}

////////////////////////////////////////////////////////////////////////

#define INERTIA_MACRO(ip,t,f) /**********************/	\
	{													\
		if (Initialising || nInertiaLen == 0)			\
			(ip).SetValueNoInertia(f);					\
		else											\
			(ip).SetValue(f,nInertiaLen,t);				\
	}	/////////////////////////////////////////////////
//end #define

void mi::Tick() 
{
//dbgprintf("Commands on track 1: %i(%i) %i(%i)",tval[1].Command1,tval[1].Argument1,tval[1].Command2,tval[1].Argument2);
	if (pMasterInfo->PosInTick == 0)
	{
		strcpy(pszMacName,pCB->GetMachineName(ThisMac));

		if (!bWorkedSinceLastTick)
		{
			for (int t=0; t<nTriggers; t++)
			{
				Tracks[t].EverythingOffNow();
			}

		}

		bWorkedSinceLastTick = false;
	}

	ShuffleThisTick = false;

	if (gval.Solo != paraSolo.NoValue)
	{
		if (gval.Solo == nTriggers)
			SoloTrack = -1;
		else
			SoloTrack = gval.Solo;
	}

	if (gval.Inertia != paraInertia.NoValue)
		nInertiaLen = gval.Inertia * pMasterInfo->SamplesPerTick / 16;

	if (gval.Triggers[MAXTRIGGERS - nTriggers] != paraFirstWaveNo.NoValue)
	{
		FirstWaveNo = gval.Triggers[MAXTRIGGERS - nTriggers];
	}

	if (gval.TriggerType != paraTriggerType.NoValue)
	{
		if (gval.TriggerType < NUM_TRIGGER_MODES)
		{
			nTriggerModeA = gval.TriggerType;
			nTriggerModeB = trigInherit;
		}
		else
		{
			int t = gval.TriggerType - NUM_TRIGGER_MODES;
			int a = t / (NUM_TRIGGER_MODES-1);
			int b = t % (NUM_TRIGGER_MODES-1);
			if (b >= a) b++;

			nTriggerModeA = a;
			nTriggerModeB = b;
		}
	}

	if (gval.GlobalVelocity != paraVelocity.NoValue)
		fVelocity = (float)gval.GlobalVelocity / 128.0f;

	if (gval.GlobalVolume != paraVolume.NoValue)
		INERTIA_MACRO(ipVolume, etLinear, gval.GlobalVolume / 128.0f);

	if (gval.GlobalPan != paraPan.NoValue)
		INERTIA_MACRO(ipPan, etLinear, 2.0f - (gval.GlobalPan-1) / 127.0f);

	if (gval.GlobalTune != paraTune.NoValue)
		INERTIA_MACRO(ipTune, etLinear, ((float)gval.GlobalTune - 128.0f) / 48.0f);

	if (gval.GlobalHumanVel != paraHumanVel.NoValue)
		fHumanVel = (float)gval.GlobalHumanVel / 128.0f;

	if (gval.GlobalHumanPan != paraHumanPan.NoValue)
		fHumanPan = (float)gval.GlobalHumanPan / 128.0f;

	if (gval.GlobalHumanTune != paraHumanTune.NoValue)
		fHumanTune = (float)gval.GlobalHumanTune * 0.1f;

	if (gval.GlobalLenUnit != paraGlobalLenUnit.NoValue)
		CmdLenUnit = gval.GlobalLenUnit;

    if (gval.ShuffleSize != paraShuffleSize.NoValue)
	{
		if (gval.ShuffleSize == 0)
			Shuffle = false;
		else
		{
			ShuffleSize = gval.ShuffleSize / 512.0f;
			Shuffle = true;
		}
	}

	if (gval.ShuffleStep != paraShuffleStep.NoValue)
	{
		ShuffleStep = gval.ShuffleStep;
	}

	if (gval.ShuffleRandom != paraShuffleRandom.NoValue)
	{
		ShuffleRnd = gval.ShuffleRandom / 254.0f;
	}

	if (gval.ShuffleReset == SWITCH_ON)
	{
		ShuffleCount = 0;
	}

	if (Shuffle)
	{
		ShuffleCount++;
		if (ShuffleCount >= ShuffleStep)
		{
			ShuffleThisTick = true;
			ShuffleCount = 0;
		}
	}

	for (int t=0; t<nTriggers; t++)
	{
//		if (!Initialising)
//			sprintf(textTriggers[t],"Trigger %i for %s (or 0=off)",t, Tracks[t].GetSampleName());

		Tracks[t].Trigger = gval.Triggers[MAXTRIGGERS+2 - nTriggers + t];

		Tracks[t].Tick();
	}
}

#undef INERTIA_MACRO

////////////////////////////////////////////////////////////////////////

void mi::AttributesChanged()
{
//	DSP_BW_Reset(bwstate);
//	DSP_BW_InitLowpass(bwstate, 0.5f / (float)(1 << aval.Oversamp));

	if (aval.RampLen == 0)
		RampVolStep = 1.0f;
	else
		RampVolStep = 1.0f / aval.RampLen;

	for (int t=0; t<nTriggers; t++)
    {
        if (!Tracks[t].customNumberOfChannels)
    		Tracks[t].ResetVirtualChannels();
    }
}

////////////////////////////////////////////////////////////////////////

void mi::Stop() 
{
	for (int t=0; t<nTriggers; t++)
		Tracks[t].TriggerOff();
}

////////////////////////////////////////////////////////////////////////

#define INERTIAUPDATELEVEL 100

#define CHECKUPDATEINERTIA							\
{													\
	InertiaCounter++;								\
	if (InertiaCounter >= INERTIAUPDATELEVEL)		\
	{												\
		InertiaCounter = 0;							\
		UpdateInertia();							\
	}												\
}													\
//end define

////////////////////////////////////////////////////////////////////////

bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)
{
	Initialising = false;

	bWorkedSinceLastTick = true;

	static float NullBuf[512];
	static float buf[512];

	ipVolume.Update(numsamples);	VolumeCurrent = ipVolume.GetValue();
	ipPan.Update(numsamples);		LGainCurrent  = ipPan.GetValue();
	ipTune.Update(numsamples);		TuneCurrent   = ipTune.GetValue();
	
	DSP_Zero(psamples, numsamples*2);
	DSP_Zero(buf,numsamples*2);
	bool OutWorked[NUM_OUTPUTS];
	for (int g=0; g<NUM_OUTPUTS; g++)
	{
		DSP_Zero(pfOutBuf[g], numsamples*2);
		OutWorked[g] = false;
	}

	bool DidWork = false;

	if (SoloTrack == -1)
	{
		for (int t=0; t < nTriggers; t++)
		{
			if (Tracks[t].Muted || Groups[Tracks[t].Group].bMuted)
			{
				Tracks[t].Generate(NullBuf, numsamples, mode);
			}
			else if (Groups[Tracks[t].Group].nOutput == 0)
			{
				DidWork = Tracks[t].Generate(buf, numsamples, mode) || DidWork;
			}
			else
			{
				g = Groups[Tracks[t].Group].nOutput - 1;
				OutWorked[g] = Tracks[t].Generate(pfOutBuf[g], numsamples, mode) || OutWorked[g];
			}
		}
	}
	else
	{
		for (int t=0; t < nTriggers; t++)
		{
			if (t == SoloTrack)
			{
				if (Groups[Tracks[t].Group].nOutput == 0)
				{
					DidWork = Tracks[t].Generate(buf, numsamples, mode) || DidWork;
				}
				else
				{
					g = Groups[Tracks[t].Group].nOutput - 1;
					OutWorked[g] = Tracks[t].Generate(pfOutBuf[g], numsamples, mode) || OutWorked[g];
				}
			}
			else
			{
				Tracks[t].Generate(NullBuf, numsamples, mode);
			}
		}
	}

	if (DidWork)
	{
		int i;

		float *f1 = psamples;
		float *f2 = buf;

		for (i=0; i<numsamples; i++)
		{
			*f1++ = *f2++ * VolumeCurrent * LGainCurrent;;
			*f1++ = *f2++ * VolumeCurrent * RGainCurrent;;
		}
	}

	for (g=0; g<NUM_OUTPUTS; g++)
		(*(int*)&pfOutBuf[g][512]) ++;

	return DidWork;
}

////////////////////////////////////////////////////////////////////////

char *FormatSize(int size)
{
	static char txt[32];

	if (size < 1024)
		sprintf(txt,"%iB",size);
	else if (size < 1024*1024)
		sprintf(txt,"%.2fkB",size/1024.0f);
	else if (size < 1024*1024*1024)
		sprintf(txt,"%.2fMB",size/1024.0f/1024.0f);
	else
		sprintf(txt,"%.2fGB",size/1024.0f/1024.0f/1024.0f);

	return txt;		
}


void miex::GetSubMenu(const int i, CMachineDataOutput *pout)
{
	if (i == cmdDrumkitMenu)
	{
		if (!pmi->bKitMode)
			pout->Write("Load drumkit...");
		else
		{
			char txt[128];
			sprintf(txt,"Load drumkit... (current kit: %s)",pmi->Kit.Name);
			pout->Write(txt);
			pout->Write("Drop drumkit and go back to wavetable samples");
			pout->Write("Drumkit information...");
			pout->Write("Save kit in bmx/bmw settings...");

//			if (pmi->Kit.nKitType == ktBattery)
			{
				sprintf(txt,"Flush loaded samples from memory (approx %s)",
					FormatSize(pmi->Kit.GetSize(1)));
				pout->Write(txt);
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////

void mi::EverythingOffNow()
{
	for (int t=0; t<nTriggers; t++)
		Tracks[t].EverythingOffNow();
}

void mi::Command(const int i) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (i)
	{
	case cmdGroupSetup:
		{
			CGroupDlg dlg;
			dlg.pmi = this;
			pGroupDlg = &dlg;
			dlg.DoModal();
			pGroupDlg = NULL;
		}
		break;

	case cmdGroupMatrix:
		{
			if (pGroupPerfDlg)
				pGroupPerfDlg->SetActiveWindow();
			else
			{
				pGroupPerfDlg = new CGroupPerfDlg;
				pGroupPerfDlg->pmi = this;
				if (pGroupPerfDlg->Create(CGroupPerfDlg::IDD))
					pGroupPerfDlg->ShowWindow(SW_SHOW);
				else
				{
					delete pGroupPerfDlg;
					pGroupPerfDlg = NULL;
				}
			}

		}
		break;

/*	case cmdLoopSlicer:
		{
			if (pLoopDlg)
				pLoopDlg->SetActiveWindow();
			else
			{
				pLoopDlg = new CLoopDlg;
				pLoopDlg->pmi = this;
				if (pLoopDlg->Create(CLoopDlg::IDD))
					pLoopDlg->ShowWindow(SW_SHOW);
				else
				{
					delete pLoopDlg;
					pLoopDlg = NULL;
				}
			}

		}
		break;
*/
	case cmdEnvelope:
		if (!pEnveDlg)
			pEnveDlg = new CEnvelopeEditor(pCustomEnvelopeShapes,&pEnveDlg,pCB->GetMachineName(ThisMac));
		break;

/*	case cmdShuffle:
		{
			CShuffleDlg dlg;
			dlg.DoModal();
		}
		break;
*/
	case cmdCommandList:
		{
			if (pCmdListDlg)
				pCmdListDlg->SetActiveWindow();
			else
			{
				pCmdListDlg = new CCmdListDlg;
				pCmdListDlg->pmi = this;
				if (pCmdListDlg->Create(CCmdListDlg::IDD))
					pCmdListDlg->ShowWindow(SW_SHOW);
				else
				{
					delete pCmdListDlg;
					pCmdListDlg = NULL;
				}
			}
		}
		break;

	case cmdAbout:
		{
			CAboutDlg dlg;
			dlg.DoModal();
		}

/*		MessageBox(NULL,
			"BTDSys SampleGrid v2.0 BETA 2005????\n"
			"©2002-2005 Ed Powley (BTDSys)\n"
			"http://btdsys.cjb.net\n\n"
			"Original concept by jph [zug island], with extra ideas from lots of people\n"
			"Multi-output system based on code by Frank Potulski (Polac)\n"
			"Envelope editor and code by Fuzzpilz",
			"BTDSys SampleGrid",
			MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION);

		{
			FILE *f = fopen("f:\\temp\\sgparams.txt","wt");
			if (f)
			{
				const CMachineInfo *pMacInfo = pCB->GetMachineInfo(ThisMac);
				fprintf(f,"\\section{Global Parameters}\n");
				for (int i=0; i<pMacInfo->numGlobalParameters; i++)
				{
					fprintf(f,"\\subsection{%s%s}\n%s\n\n",
						pMacInfo->Parameters[i]->Name,
						(pMacInfo->Parameters[i]->Flags & MPF_STATE) ? "" : " \\emph{(pattern editor only)}",
						pMacInfo->Parameters[i]->Description);
				}

				fprintf(f,"\\section{Track Parameters}\n");
				for (int j=0; j<pMacInfo->numTrackParameters; j++)
				{
					int i=j+pMacInfo->numGlobalParameters;
					fprintf(f,"\\subsection{%s%s}\n%s\n\n",
						pMacInfo->Parameters[i]->Name,
						(pMacInfo->Parameters[i]->Flags & MPF_STATE) ? "" : " \\emph{(pattern editor only)}",
						pMacInfo->Parameters[i]->Description);
				}

				fclose(f);
			}
		}
*/
		break;

	case 256: //drumkit > load
		{
	//OutputDebugString("Drumkit > load");
			char FName[MAX_PATH];
			FName[0] = 0;

			OPENFILENAME ofn;
			ofn.lStructSize			= sizeof(ofn);
			ofn.hwndOwner			= NULL;
			ofn.lpstrFilter			= "All supported files\0*.drumkit;*.kit;*.lpz\0"
										"PSI Drumkit Files (*.drumkit)\0*.drumkit\0"
										"NI Battery Files (*.kit, *.lpz)\0*.kit;*.lpz\0"
										"All files\0*.*\0";
			ofn.lpstrCustomFilter	= NULL;
			ofn.nFilterIndex		= 0;
			ofn.lpstrFile			= FName;
			ofn.lpstrFileTitle		= NULL;
			ofn.lpstrInitialDir		= GetDefaultKitPath();
			ofn.nMaxFile			= MAX_PATH;
			ofn.lpstrTitle			= "Select drumkit file...";
			ofn.Flags				= OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt			= NULL;

			if (GetOpenFileName(&ofn))
			{
				EverythingOffNow();
				Kit.Reset();
				Kit.dllInstance = dllInstance;

				if (Kit.LoadFromFile(FName))
				{
					bKitMode = true;
				}
				else
				{
					bKitMode = false;

					MessageBox(NULL,"Loading failed","SampleGrid",MB_OK);
				}
			}
		}
		break;

	case 257: //drumkit > drop
		if (MessageBox(NULL,"Are you sure you want to drop this drumkit?",
			"SampleGrid",MB_YESNO) == IDYES)
		{
			EverythingOffNow();
			Kit.Reset();
			bKitMode = false;
		}
		break;

	case 258: //drumkit > info
		Kit.ShowInfoDlg(this);
		break;

	case 259: //drumkit > save in file
		{
			CKitSaveDlg dlg;

			dlg.pmi = this;

			dlg.nMode = nKitSaveMode;
			dlg.DoModal();
			nKitSaveMode = dlg.nMode;
		}
		break;

	case 260: //drumkit > flush memory
		EverythingOffNow();
		pCB->Lock();
		Kit.FlushSampleMemory();
		pCB->Unlock();
		break;

	}

}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeTrigger(int const type, int const value)
{
	static char txt[256];

	switch (type)
	{
		case trigVel:
			sprintf(txt, "%.1f%%", value / 1.28f);
			return txt;

		case trigPan:
			if (value == 0x80)
				return "Center";
			else
			{
				sprintf(txt, "%.1f%% %s", abs(value - 0x80) / 1.27f,
				(value < 0x80) ? "Left" : "Right");
				return txt;
			}

		case trigTune:
			sprintf(txt, "%s%.2f st", (value > 0x80) ? "+" : "",
				(value - 0x80) * 0.25f);
			return txt;

		case trigProb:
			sprintf(txt, "%.1f%%", value / 2.54f);
			return txt;

		case trigArg1:
		case trigArg2:
			sprintf(txt, "%i or %i,%i", value, value>>4, value&15);
			return txt;

		case trigOffset:
			sprintf(txt,"%.1f%%",value / 2.56f);
			return txt;

		default:
			sprintf(txt,"%i",value);
			return txt;
	}
}

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[256];
	if (param == 0) //first wave
	{
		if (value == 0)
			return "0 Auto";
		else if (bKitMode)
		{
			sprintf(txt, "%i\xA0%s", value, Kit.sample[value].Name);
			return txt;
		}
		else
		{
			sprintf(txt, "%i\xA0%s", value, pCB->GetWaveName(value));
			return txt;
		}
	}

	else if (param >= 2 && param < 2+nTriggers) //global
	{
		if (value == 0) return "Off";
		else
		{
			if (nTrigType == 'b')
			{
				if (nTriggerModeB == trigInherit)
					return DescribeTrigger(nTriggerModeA,value);
				else
				{
					strcpy(txt,DescribeTrigger(nTriggerModeA,value & 0xF0));
					strcat(txt," / ");
					strcat(txt,DescribeTrigger(nTriggerModeB,(value & 0x0F) << 4));
					return txt;
				}

			}
			else
				return "On";
		}
	}
	else
	{
		switch (param - nTriggers - 2){

		case numTriggerType:
			if (value < NUM_TRIGGER_MODES)
				return pszTriggerModes[value];
			else
			{
				int t = value - NUM_TRIGGER_MODES;
				int a = t / (NUM_TRIGGER_MODES-1);
				int b = t % (NUM_TRIGGER_MODES-1);
				if (b >= a) b++;
				sprintf(txt,"%s/%s",pszTriggerModes[a],pszTriggerModes[b]);
				return txt;
			}

		case numSolo:
			if (value == nTriggers)
				return "Off";
			else
			{
				sprintf(txt, "%i\xA0(%s)", value, Tracks[value].GetSampleName());
				return txt;
			}

		case numShuffleSize:
			if (value == 0)
				return "Off";
			else
			{
				sprintf(txt,"%.2f tick",value / 512.0f);
				return txt;
			}

		case numShuffleStep:
			sprintf(txt, "%i", value);
			return txt;

		case numShuffleRandom:
			sprintf(txt, "%.1f%%", value / 2.54f);
			return txt;

		case numShuffleReset:
			if (value == SWITCH_ON)
				return "Reset";
			else
				return "No effect";

		case numInertia:
			if (value == 0) return "Off";
			else
			{
				sprintf(txt, "%.1f ticks", float(value)/10.0f);
				return txt;
			}

		case numMute:
			return (value == SWITCH_ON) ? "Muted" : "Not Muted";

		case numGlobalVolume:
		case numVolume:
		case numGlobalVelocity:
		case numVelocity:
			sprintf(txt, "%.1f%%", value / 1.28f);
			return txt;

		case numGlobalHumanVel:
		case numHumanVel:
		case numGlobalHumanPan:
		case numHumanPan:
			sprintf(txt, "±%.1f%%", value / 1.28f);
			return txt;

		case numGlobalHumanTune:
		case numHumanTune:
			sprintf(txt, "±%.2f st", value * 0.1f);
			return txt;

		case numGlobalPan:
		case numPan:
			if (value == 0x80)
				return "Center";
			else
			{
				sprintf(txt, "%.1f%% %s", abs(value - 0x80) / 1.27f,
				(value < 0x80) ? "Left" : "Right");
				return txt;
			}

		case numGlobalTune:
		case numTune:
			sprintf(txt, "%s%.2f st", (value > 0x80) ? "+" : "",
				(value - 0x80) * 0.25f);
			return txt;

//		case numFirstWaveNo:
		case numWaveNo:
			if (value == 0)
				return "0 Auto";
			else if (bKitMode)
			{
				sprintf(txt, "%i\xA0%s", value, Kit.sample[value].Name);
				return txt;
			}
			else
			{
				sprintf(txt, "%i\xA0%s", value, pCB->GetWaveName(value));
				return txt;
			}

		case numCommand1:
		case numCommand2:
			switch (value)
			{
			case 1: case 0xED:	return "Delay";
			case 2:				return "SG1 Retrigger";
			case 3: case 0x09:	return "Offset";
			case 4: case 0xE8:	return "Reverse+Offset";
			case 5: case 0x10:	return "Probability";
			case 6: case 0xEC:	return "Cut";
			case 7:				return "Delay+Cut";
			case 0x12:			return "SG1 Retrig+Cut";
			case 0xE9:			return "MTrk Retrigger";
			case 0xF9:			return "MTrk Retrig+Cut";
			case 0x55:			return "Random reseed";
			case 0x21:			return "Volume envelope";
			case 0x22:			return "Pan envelope";

			default: return "None";
			}

		case numArgument1:
		case numArgument2:
			sprintf(txt, "%i or %i,%i", value, value>>4, value&15);
			return txt;

		case numLenUnit:
		case numGlobalLenUnit:
            switch (value)
            {
            case 0x00: return "As global";
            case 0x01: return "Tick";
            case 0x11: return "ms";
            case 0x12: return "sec/100";
            case 0x13: return "sec/10";
            case 0x14: return "sec";
            default:                
			    sprintf(txt, "tick/%i", value);
    			return txt;
            }

		case numGroup:
			sprintf(txt,"%i: %s", value, Groups[value].name);
			return txt;

		case numOffset:
		case numLoopStart:
		case numLoopEnd:
			if (value == 0xFFFF)
				return "100.000%";
			else
			{
				sprintf(txt,"%.3f%%",value / 655.36f);
				return txt;
			}

		case numLoopMode:
			if (value < NUM_LOOP_MODES)
				return pszLoopModes[value];
			else
				return "WTF?";

		case numCut:
			if (value == 0) return "Infinite";
			sprintf(txt,"%i subdivs",value);
			return txt;

		case numLoopFit:
		case numVolEnvLen:
		case numPanEnvLen:
		case numTuneEnvLen:
			if (value == 0) return "Off";
			sprintf(txt,"%i %i/16 ticks", value / 16, value % 16);
			return txt;

/*		case numLoopFitMode:
			switch (value)
			{
			case 1: return "Tune";
			case 2: return "Not done yet";
			default: return "WTF?!?";
			}
*/
		case numVolEnv:
		case numPanEnv:
		case numTuneEnv:
			if (value == 0) return "None";
			else 
			{
				sprintf(txt,"%i",value);
				return txt;
			}
			
		case numSeparator01:
			return "--------sg2";

		case numSeparator02:
		case numSeparator03:
		case numSeparator04:
		case numSeparator05:
			return "----------0";


		default:
			return NULL;
		}
	}

	return NULL;
}

char const *miex::DescribeParam(const int param)
{
	switch (param - pmi->nTriggers - 2)
	{
	case numSeparator01:
	case numSeparator02:
	case numSeparator03:
	case numSeparator04:
	case numSeparator05:
		return "----------";
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////


float mi::m_frand(int seed)
// returns random value between 0 and 1
// from Waveout.cpp
{
	static long stat = /*0x16BA2118*/ GetTickCount (); //seed
	if (seed != -1) stat = seed;
	stat = (stat * 1103515245 + 12345) & 0x7fffffff;
	return (float)stat * (1.0f / 0x7fffffff);
}

float mi::m_fgrand(float m, float s)
// http://www.musicdsp.org/archive.php?classid=1#113
{
	return sqrtf( -2.0f * logf( m_frand() )) * cosf( 2.0f * PI * m_frand() ) * s + m;
}



#pragma optimize ("", on)

