/*
Copyright (c) 2004-2006, Edward Powley
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
#include "pulsar.h"

#include <math.h>
#include "bandlimit.h"
#include "frand.h"
#include "hermite.h"
#include "resource.h"

////////////////////////////////////////////////////////////////////////
// [Con/de]structors
mi::mi() 
{
	GlobalVals = &gval;
	TrackVals = &tval;
	AttrVals = (int *)&aval;
	numTracks = 0;
	dcin = dcout = 0.0f;

	nMaxWorkBlockSize = 256;
}

mi::~mi()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	for (int t=0; t<254; t++)
		if (tracks[t].pOscDlg) delete tracks[t].pOscDlg;
}

Track::Track()
{
	for (int i=0; i<MAX_OVERLAP; i++)
	{
		bPulsaretPlaying[i] = false;
		pfPrBuffer[i] = NULL;
	}

//	nOverlap = 0;
	pOscDlg = NULL;
	pfInBuffer = NULL;
	bInBufferOverflow = false;
}

Track::~Track()
{
	if (pfInBuffer) delete[] pfInBuffer;
	for (int i=0; i<MAX_OVERLAP; i++)
	{
		if (pfPrBuffer[i]) delete[] pfPrBuffer[i];
	}
}

////////////////////////////////////////////////////////////////////////

bool mi::DblClickEvent(void *p)
{
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
	ex.pmi = this;

	SetOutputMode(true);
	thismac = pCB->GetThisMachine();
	pCB->SetnumOutputChannels(thismac,2);

	pCB->SetEventHandler(thismac, 
						DoubleClickMachine, 
						(EVENT_HANDLER_PTR)DblClickEvent, NULL);

	for (int t=0; t<254; t++)
	{
		tracks[t].nTrack = t;
		tracks[t].pmi = this;
		tracks[t].tval = &tval[t];

		tracks[t].ipFreqP		.SetMinMax(1.0f,	32768.0f);
		tracks[t].ipPeriodP		.SetMinMax(0.001f,	16.0f);
		tracks[t].ipFreqD		.SetMinMax(1.0f,	32768.0f);
		tracks[t].ipWStart		.SetMinMax(0.0f,	1.0f);
		tracks[t].ipWEnd		.SetMinMax(0.0f,	1.0f);
		tracks[t].ipStochMask	.SetMinMax(0.0f,	1.0f);
		tracks[t].ipRandomPan	.SetMinMax(0.0f,	1.0f);
		tracks[t].ipVolume		.SetMinMax(0.0f,	2.0f);
		tracks[t].ipPan			.SetMinMax(0.0f,	2.0f);
		tracks[t].ipLFO1Amount	.SetMinMax(0.0f,	1.0f);
		tracks[t].ipLFO2Amount	.SetMinMax(0.0f,	1.0f);
		tracks[t].ipLFO1Period	.SetMinMax(0.01f,	256.0f);
		tracks[t].ipLFO2Period	.SetMinMax(0.01f,	256.0f);
		tracks[t].ipEnvParam	.SetMinMax(0.0f,	1.0f);
		tracks[t].ipBurst		.SetMinMax(1.0f,	128.0f);
		tracks[t].ipRest		.SetMinMax(0.0f,	128.0f);
		tracks[t].ipOverlap		.SetMinMax(1.0f,	(float)MAX_OVERLAP - 1.0f);

		tracks[t].lfo1.fAmount = tracks[t].lfo2.fAmount = 0.5f;
		tracks[t].lfo1.pCB = tracks[t].lfo2.pCB = pCB;
	}

	if (pi)
	{
	}

	HKEY hk;

	colVisBG = 0xFFFFFF;
	colVisWave = 0xFF0000;
	colVisEnv = 0x0000FF;
	colVisEnvWave = 0x000000;
	colVisOverlap = 0xCCCCCC;
	bShowGraphicalSeparator = 1;

	if(RegCreateKey(HKEY_CURRENT_USER,"Software\\BTDSys\\Pulsar_Buzz",&hk) == ERROR_SUCCESS)
	{
		unsigned long i = 4;
		unsigned long type = REG_DWORD;

		if (RegQueryValueEx(hk,"VisBG",0,&type,(unsigned char *)&colVisBG,&i) != ERROR_SUCCESS)
			RegSetValueEx(hk,"VisBG",0,REG_DWORD,(unsigned char *)&colVisBG,4);

		if (RegQueryValueEx(hk,"VisWave",0,&type,(unsigned char *)&colVisWave,&i) != ERROR_SUCCESS)
			RegSetValueEx(hk,"VisWave",0,REG_DWORD,(unsigned char *)&colVisWave,4);

		if (RegQueryValueEx(hk,"VisEnv",0,&type,(unsigned char *)&colVisEnv,&i) != ERROR_SUCCESS)
			RegSetValueEx(hk,"VisEnv",0,REG_DWORD,(unsigned char *)&colVisEnv,4);

		if (RegQueryValueEx(hk,"VisEnvWave",0,&type,(unsigned char *)&colVisEnvWave,&i) != ERROR_SUCCESS)
			RegSetValueEx(hk,"VisEnvWave",0,REG_DWORD,(unsigned char *)&colVisEnvWave,4);

		if (RegQueryValueEx(hk,"VisOverlap",0,&type,(unsigned char *)&colVisOverlap,&i) != ERROR_SUCCESS)
			RegSetValueEx(hk,"VisOverlap",0,REG_DWORD,(unsigned char *)&colVisOverlap,4);

		if (RegQueryValueEx(hk,"ShowGraphicalSeparator",0,&type,(unsigned char *)&bShowGraphicalSeparator,&i) != ERROR_SUCCESS)
			RegSetValueEx(hk,"ShowGraphicalSeparator",0,REG_DWORD,(unsigned char *)&bShowGraphicalSeparator,4);

		RegCloseKey(hk);
	}
	else
		MessageBox(NULL,"Warning: couldn't open reg key HKEY_CURRENT_USER\\Software\\BTDSys\\Pulsar_Buzz","Pulsar",MB_OK);

	bFirstTick = true;
}

void mi::MDKSave(CMachineDataOutput * const po)
{
}

void mi::SetNumTracks(const int n)
{
	int t;

#	ifdef PULSARFX
		for (t=n; t<numTracks; t++)
		{
			if (tracks[t].pfInBuffer) delete[] tracks[t].pfInBuffer;
			tracks[t].pfInBuffer = NULL;
			for (int i=0; i<MAX_OVERLAP; i++)
			{
				if (tracks[t].pfPrBuffer[i]) delete[] tracks[t].pfPrBuffer[i];
				tracks[t].pfPrBuffer[i] = NULL;
			}
		}
#	endif

	for (t=numTracks; t<n; t++)
	{
#		ifdef PULSARFX
			if (tracks[t].pfInBuffer) delete[] tracks[t].pfInBuffer;
			tracks[t].pfInBuffer = new float[aval.InBufSize * 2];
			tracks[t].nInBufferHead = 0;
			for (int i=0; i<MAX_OVERLAP; i++)
			{
				if (tracks[t].pfPrBuffer[i]) delete[] tracks[t].pfPrBuffer[i];
				tracks[t].pfPrBuffer[i] = new float[aval.InBufSize * 2];
				tracks[t].nPrBufferLength[i] = 1;
			}
#		endif

		tvals *p = tracks[t].tval;
		tracks[t].tval = (tvals *)&def_tvals;
		tracks[t].tval->LFO1TargetTrack = t;
		tracks[t].tval->LFO2TargetTrack = t;
		tracks[t].Tick();
		tracks[t].tval = p;
		pCB->ControlChange(thismac,2,t,numLFO1TargetTrack - NUMG, t);
		pCB->ControlChange(thismac,2,t,numLFO2TargetTrack - NUMG, t);
	}
	numTracks = n;
}

void mi::AttributesChanged()
{
//	fLogPFreqC = logf(aval.LowPFreq / 1000.0f) / logf(2.0f);
//	fLogDFreqC = logf(aval.LowDFreq / 1000.0f) / logf(2.0f);

//	if (aval.LFOAmtMidPt <= 500)
	{
		fLFOAmtExp = -logf(aval.LFOAmtMidPt / 1000.0f) / logf(2.0f);
		bLFOAmtExpInvert = false;
	}
/*	else
	{
		fLFOAmtExp = -logf(1.0f - aval.LFOAmtMidPt / 1000.0f) / logf(2.0f);
		bLFOAmtExpInvert = true;
	}
*/
#	ifdef PULSARFX
		for (int t=0; t<numTracks; t++)
		{
			if (tracks[t].pfInBuffer) delete[] tracks[t].pfInBuffer;
			tracks[t].pfInBuffer = new float[aval.InBufSize * 2];
			tracks[t].nInBufferHead = 0;
			tracks[t].bInBufferOverflow = false;
			for (int i=0; i<MAX_OVERLAP; i++)
			{
				if (tracks[t].pfPrBuffer[i]) delete[] tracks[t].pfPrBuffer[i];
				tracks[t].pfPrBuffer[i] = new float[aval.InBufSize * 2];
				tracks[t].nPrBufferLength[i] = 1;
			}
		}
#	endif

	for (int i=0; i<MAX_OVERSAMPEXP; i++)
	{
		hbFilter[i].quality = aval.OversampleQuality;
		hbFilter[i].reset();
	}
}

#include <fstream.h>
void Track::DBG_WriteEnvFn()
{
	ofstream f("h:\\pulsar_envelope.csv");
	if (f.good())
	{
		for (int i=0; i<ENVTABLE_LENGTH+4; i++)
		{
			f << i << "," << pfEnvTable[i] << endl;
		}

		MessageBox(NULL,"h:\\pulsar_envelopes.csv written","",MB_OK);
	}
	else
		MessageBox(NULL,"couldnt write h:\\pulsar_envelopes.csv","",MB_OK);
}

void miex::GetSubMenu(int const i, CMachineDataOutput *pout)
{
	switch (i)
	{
	case 1:
		{
			char txt[16];
			for (int t=0; t<pmi->numTracks; t++)
			{
				sprintf(txt,"Track %i",t);
				pout->Write(txt);
			}
		}
		break;

	case 2:
		pout->Write("Background...");
		pout->Write("Waveform...");
		pout->Write("Envelope...");
		pout->Write("Waveform after envelope...");
		pout->Write("Overlap region...");
		break;
	}

}

void mi::CustomiseColour(COLORREF &c, char *regname)
{
	CHOOSECOLOR cc;
	cc.lStructSize = sizeof(cc);
	cc.hwndOwner = NULL;
	cc.rgbResult = c;
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	static COLORREF cust[16];
	cc.lpCustColors = cust;

	if (ChooseColor(&cc))
	{
		c = cc.rgbResult;

		HKEY hk;

		if(RegCreateKey(HKEY_CURRENT_USER,"Software\\BTDSys\\Pulsar_Buzz",&hk) == ERROR_SUCCESS)
		{
			RegSetValueEx(hk,regname,0,REG_DWORD,(unsigned char *)&c,4);
			RegCloseKey(hk);
		}
	}

}

void mi::Command(const int i)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (i)
	{
	case 0: //about
		{
			MessageBox(NULL,"BTDSys Pulsar v1.0\n"
				"©2004 Ed Powley  http://btdsys.cjb.net\n"
				"Ideas by KM Krebs","About",MB_OK);
		}
		break;

	case 512: CustomiseColour(colVisBG, "VisBG"); break;
	case 513: CustomiseColour(colVisWave, "VisWave"); break;
	case 514: CustomiseColour(colVisEnv, "VisEnv"); break;
	case 515: CustomiseColour(colVisEnvWave, "VisEnvWave"); break;
	case 516: CustomiseColour(colVisOverlap, "VisOverlap"); break;

	default:
		if (i >= 256 && i < 512) //oscilloscope
		{
			int t = i & 255;

			if (tracks[t].pOscDlg)
				tracks[t].pOscDlg->SetActiveWindow();
			else
			{
				tracks[t].pOscDlg = new COscilloscopeDlg;
				tracks[t].pOscDlg->pmi = this;
				tracks[t].pOscDlg->ptr = &tracks[t];
				if (tracks[t].pOscDlg->Create(COscilloscopeDlg::IDD))
					tracks[t].pOscDlg->ShowWindow(SW_SHOW);
				else
				{
					delete tracks[t].pOscDlg;
					tracks[t].pOscDlg = NULL;
				}
			}
			if (tracks[t].pOscDlg)
			{
				char txt[256];
				sprintf(txt,"%s track %i",pCB->GetMachineName(thismac),t);
				tracks[t].pOscDlg->SetWindowText(txt);
			}
		}
		break;
	}
}

void Track::UpdateOscDlg(float fLenP, float fLenD, 
						 float fWStart, float fWEnd, 
#						ifdef PULSARFX
						 float *pSamples, int numSamples,
#						else
						 short *pSamples, int numSamples,
#						endif
						 bool bStereo)
{
	pOscDlg->fLen = fLenP;
	pOscDlg->fLenD = fLenD;
	pOscDlg->fStart = fWStart;
	pOscDlg->fEnd = fWEnd;
	pOscDlg->vfWaveData.resize(numSamples);
	pOscDlg->pfEnvelope = pfEnvTable;

	if (bStereo)
	{
		for (int i=0; i<numSamples; i++)
		{
			pOscDlg->vfWaveData[i] = (pSamples[i*2] + pSamples[i*2+1]) * 0.5f;
		}
	}
	else
	{
		for (int i=0; i<numSamples; i++)
		{
			pOscDlg->vfWaveData[i] = pSamples[i];
		}
	}

/*	for (int i=0; i<nLen; i++)
	{
		if (!pOscDlg) return;
		float pos = (float)i / fLenD;
		if (pos >= 0.0f && pos <= 1.0f)
		{
			float f;
			pos = (fWStart + pos * (fWEnd - fWStart)) * (float)numSamples;
			if (bStereo)
			{
				f  = GetSampleL(pos, pSamples, numSamples);
				f += GetSampleR(pos, pSamples, numSamples);
				f *= 0.5f;
			}
			else
				f = GetSampleM(pos, pSamples, numSamples);

			pOscDlg->vfWaveData[i] = f;
		}
		else
			pOscDlg->vfWaveData[i] = 0.0f;
	}
*/
	bUpdateOscDlg = false;
}


void mi::Stop()
{
	if (aval.OffOnStop)
	{
		for (int t=0; t<numTracks; t++)
			pCB->ControlChange(thismac,2,t,numOnOff - NUMG, SWITCH_OFF);
	}
}
