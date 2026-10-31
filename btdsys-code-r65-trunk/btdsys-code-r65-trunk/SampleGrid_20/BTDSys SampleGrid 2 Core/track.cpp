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

#include "sgrid.h"
#include <math.h>
#include "common.h"

#include <algorithm>
using namespace std;

CTrack::CTrack()
{
	ipVolume.SetMinMax(0.0f, 2.0f);
	ipPan.SetMinMax(0.0f, 2.0f);
	ipTune.SetMinMax(-3.0f, 3.0f);
	for (int c=0; c<NUM_PEERS; c++) pCtrl[c] = NULL;
	nTrigKey = 0;
	nTrigKeyType = ktNone;
	nPosEnv = 0;
    customNumberOfChannels = false;
}

CTrack::~CTrack()
{
	for (int c=0; c<NUM_PEERS; c++) if (pCtrl[c]) delete pCtrl[c];
}

void CTrack::ResetVirtualChannels(int n)
{
    if (n == -1)
        n = pmi->aval.Channels;

	channels.clear();

	CChannel ch;
	ch.pmi = pmi;
	ch.pCB = pmi->pCB;
	ch.ptr = this;

	for (int c=0; c<n; c++)
	{
		ch.tag = c;
		channels.push_back(ch);
	}
}

void CTrack::TriggerOn(	STrigInfo ti)
{
	if (pmi->Groups[Group].bXOR)
	{
		for (int t=0; t<pmi->nTriggers; t++)
		{
			if (pmi->Tracks[t].Group == Group)
				pmi->Tracks[t].EverythingOffNow();
		}
	}


	int OldestAge = 0;
	CChannel *pOldestChan = channels.begin();
	for (CChannel *p = channels.begin(); p < channels.end(); p++)
	{
        if (!p->bSleepy)
        {
		    if (!p->IsActive())
		    {
			    pOldestChan = p;
			    p = channels.end();
		    }
		    else if (p->GetAge() > OldestAge)
		    {
			    OldestAge = p->GetAge(); 
			    pOldestChan = p; 
		    }
        }
	}

	if (pOldestChan->IsActive())
	{
		rampoffchannel = *pOldestChan;
		rampoffchannel.TriggerOff(pmi->aval.RampLen);
	}

	bool lockrate = false;

	bool minus = (ti.rate < 0.0f);
//	switch (nLoopFitMode)
	{
//	case 1: //tune
		if (nLoopFit)
		{
			ti.rate = (float) GetSampleLen(ti.vel) / ((float)(nLoopFit * pmi->pMasterInfo->SamplesPerTick) / 16.0f);
dbgprintf("ti.rate = %.20f",ti.rate);
			if (minus) ti.rate = -ti.rate;
		}
	}


	ti.vel  *= fVelocity * pmi->fVelocity;
	switch (pmi->aval.HumanMode)
	{
	case 0:
		ti.vel  += (pmi->m_frand() - 0.5f) * 2.0f * (fHumanVel + pmi->fHumanVel);
		ti.pan  += (pmi->m_frand() - 0.5f) * 2.0f * (fHumanPan + pmi->fHumanPan);
		ti.rate *= powf(2.0f, (pmi->m_frand() - 0.5f) * (fHumanTune + pmi->fHumanTune) * 2.0f / 12.0f);
		break;
	case 1:
		ti.vel = pmi->m_fgrand(ti.vel, fHumanVel + pmi->fHumanVel);
		ti.pan = pmi->m_fgrand(ti.pan, fHumanPan + pmi->fHumanPan);
		{
			bool minus = (ti.rate < 0.0f);
			if (minus) ti.rate = -ti.rate;
			float t = logf(ti.rate) / logf(2.0f);
			t = pmi->m_fgrand(t, (fHumanTune + pmi->fHumanTune) / 12.0f);
			ti.rate = powf(2.0f, t);
			if (minus) ti.rate = -ti.rate;
		}
		break;
	}

	if (lockrate) ti.rate *= GetSampleRateCorrection(ti.vel);

	if (pmi->bKitMode)
		pOldestChan->TriggerOn(GetPSISample(), ti, pmi->aval.RampLen);
	else
	{
		OutputDebugString("Trigging it");
		pOldestChan->TriggerOn(GetSampleNum(), ti, pmi->aval.RampLen);
	}

	if (pCtrl[peerTrig]->GotParam())
	{
		CMachineParameter const *par = pCtrl[peerTrig]->GetParamInfo();
		int min = par->MinValue; if (min == -1) min = 0; min++;
		int max = par->MaxValue; if (max == -1) max = 1;
		int val = min + (int)( (float)(max-min) * ti.vel ); val = clamp(val,min,max);
		pCtrl[peerTrig]->ControlChange_Immediate(0,val);
	}
}

////////////////////////////////////////////////////////////////////////

void CTrack::TriggerOff()
{
	for (CChannel *p = channels.begin(); p < channels.end(); p++)
	{
		p->TriggerOff(pmi->aval.RampLen);
	}
}

void CTrack::EverythingOffNow()
{
	for (CChannel *p = channels.begin(); p < channels.end(); p++)
	{
		p->TriggerOffHard();
	}
}

////////////////////////////////////////////////////////////////////////

int CTrack::CmdLenToSamples(int n)
{
    int unit = CmdLenUnit ? CmdLenUnit : pmi->CmdLenUnit;

    if (unit <= 0x10)
        return n * pmi->pMasterInfo->SamplesPerTick / unit;
    else
    {
        int div;
        if (unit == 0x11) 
            div = 1000;
        else if (unit == 0x12)
            div = 100;
        else if (unit == 0x13) 
            div = 10;
        else 
            div = 1;

        return n * pmi->pMasterInfo->SamplesPerSec / div;
    }
}

int CTrack::SamplesToCmdLen(int s)
{
    int unit = CmdLenUnit ? CmdLenUnit : pmi->CmdLenUnit;

    if (unit <= 0x10)
        return s * unit / pmi->pMasterInfo->SamplesPerTick;
    else
    {
        int div;
        if (unit == 0x11) 
            div = 1000;
        else if (unit == 0x12)
            div = 100;
        else if (unit == 0x13) 
            div = 10;
        else 
            div = 1;

        return s * div / pmi->pMasterInfo->SamplesPerSec;
    }
}

void CTrack::ProcessGlobalCommand(byte CommandVal, byte ArgumentVal)
{
	switch (CommandVal)
	{
	case 0x21: //Trigger volume envelope
		if (ArgumentVal > 0 && ArgumentVal <= 0x40)
		{
			for (CChannel *pc = channels.begin(); pc < channels.end(); pc++)
			{
				pc->envVol.SetEnvelope(0,0,0,0,0,0,0,
					&pmi->pCustomEnvelopeShapes[ArgumentVal-1],
					fVolEnvLen, pmi->pMasterInfo->SamplesPerTick);
				pc->envVol.Reset();
				pc->envVol.Trigger(ermDown);
				pc->bForceVolEnv = true;
			}
		}
		break;

	case 0x24: //Set position envelope
		if (ArgumentVal <= 0x40) nPosEnv = ArgumentVal;
		break;

    case 0x50: //Violently set number of virtual channels
        if (ArgumentVal == 0)
        {
            customNumberOfChannels = false;
            ResetVirtualChannels();
        }
        else
        {
            customNumberOfChannels = true;
            ResetVirtualChannels(ArgumentVal);
        }
        break;

    case 0x51: //Gently set number of virtual channels
        if (ArgumentVal == 0)
        {
            customNumberOfChannels = false;
	        for (CChannel *p = channels.begin(); p < channels.end(); p++)
	            p->bSleepy = false;
        }
        else
        {
            customNumberOfChannels = true;
            int i=0;
	        for (CChannel *p = channels.begin(); p < channels.end(); p++, i++)
	            p->bSleepy = (i >= ArgumentVal);
        }
        break;

	case 0x55: //Random reseed
		if (ArgumentVal == 0)
			pmi->m_frand(GetTickCount());
		else
			pmi->m_frand(ArgumentVal * 1234);
		break;		
	}		

}

bool CTrack::ProcessCommand(byte CommandVal, byte ArgumentVal,
							STrigInfo *pti, int &RetrigInt, int &RetrigCount)
{
	int spt = pmi->pMasterInfo->SamplesPerTick;

	switch (CommandVal)
	{
	case 1: //delay
	case 0xED:
		pti->delay += CmdLenToSamples(ArgumentVal);
		break;

	case 0x09: //offset
		dbgprintf("offset %i %i",CommandVal,ArgumentVal);
		pti->offset = ArgumentVal / 256.0f;
		break;

	case 4: //reverse+offset
	case 0xE8:
		pti->offset = ArgumentVal / 256.0f;
		pti->rate *= -1.0f;
		break;

	case 5: //probability
	case 0x10:
		if (pmi->m_frand() * 0xFF > ArgumentVal) return false;
		break;

	case 6: //cut
	case 0xEC:
		pti->cut = CmdLenToSamples(ArgumentVal);
		break;

	case 7: //delay+cut
		pti->delay += CmdLenToSamples(ArgumentVal >> 4);
		pti->cut = CmdLenToSamples(ArgumentVal & 0xF);
		break;

	case 2: //retrig
		if (ArgumentVal >> 4  == 0) ArgumentVal |= 0x10;
		if (ArgumentVal & 0xF == 0) ArgumentVal |= 0x01;
		RetrigInt = CmdLenToSamples(ArgumentVal >> 4);
		RetrigCount = pmi->pMasterInfo->SamplesPerTick * (ArgumentVal & 0xF) / RetrigInt;
		break;

	case 0x12: //typewriter retrig
		if (ArgumentVal >> 4  == 0) ArgumentVal |= 0x10;
		if (ArgumentVal & 0xF == 0) ArgumentVal |= 0x01;
		RetrigInt = CmdLenToSamples(ArgumentVal >> 4);
		RetrigCount = pmi->pMasterInfo->SamplesPerTick * (ArgumentVal & 0xF) / RetrigInt;
		pti->cut = RetrigInt;
		break;

    case 3: //xjn retrig
		if (ArgumentVal >> 4  == 0) ArgumentVal |= 0x10;
		if (ArgumentVal & 0xF == 0) ArgumentVal |= 0x01;
		RetrigInt = CmdLenToSamples(ArgumentVal >> 4);
		RetrigCount = (ArgumentVal & 0xF) - 1;
        break;

    case 0x13: //xjn typewriter retrig
		if (ArgumentVal >> 4  == 0) ArgumentVal |= 0x10;
		if (ArgumentVal & 0xF == 0) ArgumentVal |= 0x01;
		RetrigInt = CmdLenToSamples(ArgumentVal >> 4);
		RetrigCount = (ArgumentVal & 0xF) - 1;
		pti->cut = RetrigInt;
        break;

	case 0xE9: //Matilde retrig
		RetrigInt = CmdLenToSamples(ArgumentVal);
		RetrigCount = pmi->pMasterInfo->SamplesPerTick / RetrigInt;
		break;

	case 0xF9: //Matilde typewriter retrig
		RetrigInt = CmdLenToSamples(ArgumentVal);
		RetrigCount = pmi->pMasterInfo->SamplesPerTick / RetrigInt;
		pti->cut = RetrigInt;
		break;
	}

	return true;
}

bool CTrack::ProcessTrigger(int type, int val,
							byte &ArgumentVal1, byte &ArgumentVal2,
							STrigInfo *pti, int &RetrigInt, int &RetrigCount)
{
	switch (type)
	{
	case trigVel:
		pti->vel = val / 128.0f;
		break;

	case trigPan:
		pti->pan = val / 128.0f;
		break;

	case trigTune:
		pti->rate = powf(2.0f, (val - 0x80) / 48.0f);
		break;

	case trigProb:
		if (pmi->m_frand() * 0xFE > val) return false;
		break;

	case trigArg1:
		ArgumentVal1 = val;
		break;

	case trigArg2:
		ArgumentVal2 = val;
		break;

	case trigOffset:
		pti->offset = val / 256.0f;
		break;
	}

	return true;
}


void CTrack::TriggerIt(byte TrigVal, byte CommandVal1, byte ArgumentVal1, byte CommandVal2, byte ArgumentVal2,
					   int Delay /* = 0 */ )
{
dbgprintf("TriggerIt! cmds %i %i %i %i",CommandVal1,ArgumentVal1,CommandVal2,ArgumentVal2);
	if (TrigVal == 0)
		TriggerOff();
	else
	{
		STrigInfo ti;
		ti.vel = ti.pan = ti.rate = 1.0f;
		ti.offset = fOffset;
		ti.delay = Delay;

		int RetrigInt = 0, RetrigCount = 0;

		if (nCut == 0)
			ti.cut = -1;
		else
			ti.cut = CmdLenToSamples(nCut);

//		dbgprintf("%c -> %i",pmi->nTrigType, pmi->TriggerType);
		if (pmi->nTrigType == 'b')
		{
			int valA, valB;
			if (pmi->nTriggerModeB == trigInherit)
			{
				valA = TrigVal;
				valB = -1;
			}
			else
			{
				valA = TrigVal & 0xF0;
				valB = (TrigVal & 0x0F) << 4;
			}

			if (!ProcessTrigger(pmi->nTriggerModeA,valA,ArgumentVal1,ArgumentVal2,
								&ti,RetrigInt,RetrigCount))
				return;

			if (!ProcessTrigger(pmi->nTriggerModeB,valB,ArgumentVal1,ArgumentVal2,
								&ti,RetrigInt,RetrigCount))
				return;

		}

		if (!ProcessCommand(CommandVal1, ArgumentVal1, &ti, RetrigInt, RetrigCount))
			return; //probability sez: don't trigger
		if (!ProcessCommand(CommandVal2, ArgumentVal2, &ti, RetrigInt, RetrigCount))
			return; //probability sez: don't trigger

		retrigs.push_back(ti);

		for (; RetrigCount > 0; RetrigCount--)
		{
			ti.delay += RetrigInt;
			retrigs.push_back(ti);
		}

		sort(retrigs.begin(), retrigs.end());
	}
}

int CTrack::GetSampleLen(float vel)
{
	if (pmi->bKitMode)
	{
		CDrumkitLayer *plyr = GetPSISample()->GetLayer(vel);
		if (plyr) return plyr->numSamples;
		else return 1;
	}
	else
	{
		CWaveLevel const *pLevel = pmi->pCB->GetWaveLevel(GetSampleNum(),0);
		CWaveInfo const *pWInfo = pmi->pCB->GetWave(GetSampleNum());
		if (!pLevel || !pWInfo) return 0;

		if (pWInfo->Flags & WF_NOT16)
			return (pLevel->numSamples - 8) / 2;
		else
			return pLevel->numSamples;
	}
}

float CTrack::GetSampleRateCorrection(float vel)
{
	if (pmi->bKitMode)
	{
		CDrumkitLayer *plyr = GetPSISample()->GetLayer(vel);
		if (plyr) return plyr->fRate * (float)plyr->nSampleRate / (float)pmi->pMasterInfo->SamplesPerSec;
		else return 1.0f;
	}
	else
	{
		CWaveLevel const *pLevel = pmi->pCB->GetWaveLevel(GetSampleNum(),0);
		CWaveInfo const *pWInfo = pmi->pCB->GetWave(GetSampleNum());
		if (!pLevel || !pWInfo) return 1.0f;

		return (float)pLevel->SamplesPerSec / (float)pmi->pMasterInfo->SamplesPerSec;

	}

}

void CTrack::TriggerPreview(CDrumkitLayer *pLayer)
{
	int OldestAge = 0;
	CChannel *pOldestChan = channels.begin();
	for (CChannel *p = channels.begin(); p < channels.end(); p++)
	{
		if (!p->IsActive())
		{
			pOldestChan = p;
			p = channels.end();
		}
		else if (p->GetAge() > OldestAge)
		{
			OldestAge = p->GetAge(); 
			pOldestChan = p; 
		}
	}

	STrigInfo ti;
	ti.vel = ti.pan = ti.rate = 1.0f;
	ti.delay = 0;
	ti.cut = -1;
	ti.offset = 0.0f;

	pOldestChan->TriggerOn(pLayer, ti, 0);
}

void CTrack::TriggerPreview(CDrumkitSample *pSample)
{
	int OldestAge = 0;
	CChannel *pOldestChan = channels.begin();
	for (CChannel *p = channels.begin(); p < channels.end(); p++)
	{
		if (!p->IsActive())
		{
			pOldestChan = p;
			p = channels.end();
		}
		else if (p->GetAge() > OldestAge)
		{
			OldestAge = p->GetAge(); 
			pOldestChan = p; 
		}
	}

	STrigInfo ti;
	ti.vel = ti.pan = ti.rate = 1.0f;
	ti.delay = 0;
	ti.cut = -1;
	ti.offset = 0.0f;

	pOldestChan->TriggerOn(pSample, ti, 0);
}

////////////////////////////////////////////////////////////////////////

bool CTrack::Generate(float *psamples, int numsamples, int const mode)
{
	bool bRet = false;
	int samplerate = pmi->pMasterInfo->SamplesPerSec;

	while (!retrigs.empty() && retrigs[0].delay < numsamples) // sure hope && is short-circuiting!
	{
		TriggerOn(retrigs[0]);
		retrigs.erase(&retrigs[0]);
	}

	if (!retrigs.empty()) for (STrigInfo *p = retrigs.begin(); p < retrigs.end(); p++)
		p->delay -= numsamples;

/*PEER	pc->CheckMachine();
*/
	ipVolume.Update(numsamples);	VolumeCurrent = ipVolume.GetValue();
	ipPan.Update(numsamples);		LGainCurrent  = 2.0f - ipPan.GetValue();
	ipTune.Update(numsamples);		TuneCurrent   = ipTune.GetValue();

	static float buf[512];
	for (int i=0; i<numsamples; i++)
	{
		buf[i*2] = buf[i*2+1] = 0.0f;
	}

//	memset(buf,0,512*sizeof(float));

	for (CChannel *p = channels.begin(); p < channels.end(); p++)
	{
		if (p->IsActive())
			bRet = p->Work(buf, numsamples, samplerate) || bRet;
	}
	if (rampoffchannel.IsActive())
		bRet = rampoffchannel.Work(buf, numsamples, samplerate) || bRet;

	float ftot = 0.0f;

	for (i=0; i<numsamples*2; i+=2)
	{
		psamples[i]   += buf[i  ] * VolumeCurrent * LGainCurrent;
		psamples[i+1] += buf[i+1] * VolumeCurrent * (2.0f - LGainCurrent);

		fVu = 0.5f*fVu + 0.25f * (fabsf(psamples[i]) + fabsf(psamples[i+1]));
	}

	return bRet;
}

////////////////////////////////////////////////////////////////////////

inline void CTrack::UpdateVolRamping()
{
	if (RampVolCountDown)
	{
		VolumeCurrent += RampVolStep;
		RampVolCountDown--;
		if (!RampVolCountDown)
			VolumeCurrent = RampVolTarget;
	}
}

////////////////////////////////////////////////////////////////////////

#define INERTIA_MACRO(ip,t,f) /**********************/	\
	{													\
		if (pmi->Initialising || pmi->nInertiaLen == 0)	\
			(ip).SetValueNoInertia(f);					\
		else											\
			(ip).SetValue(f,pmi->nInertiaLen,t);		\
	}	/////////////////////////////////////////////////
//end #define

void CTrack::Tick()
{
	if (!pmi->Initialising) pCtrl[peerTrig]->CheckMachine();

	for (CChannel *p = channels.begin(); p < channels.end(); p++)
	{
		if (p->tag == -1 && !p->IsActive())
			p = channels.erase(p);
	}

	if (tval->WaveNo != paraWaveNo.NoValue)
		WaveNum = tval->WaveNo;

	if (tval->Offset != paraOffset.NoValue)
		fOffset = (float)tval->Offset / 65536.0f;

	if (tval->LoopStart != paraLoopStart.NoValue)
		fLoopStart = (float)tval->LoopStart / 65536.0f;

	if (tval->LoopEnd != paraLoopEnd.NoValue)
		fLoopEnd = (tval->LoopEnd == 0xFFFF) ? 1.0f : (float)tval->LoopEnd / 65536.0f;

	if (tval->LoopMode != paraLoopMode.NoValue)
		loopMode = tval->LoopMode;

	if (tval->Cut != paraCut.NoValue)
		nCut = tval->Cut;

	if (tval->LoopFit != paraLoopFit.NoValue)
		nLoopFit = tval->LoopFit;

//	if (tval->LoopFitMode != paraLoopFitMode.NoValue)
//		nLoopFitMode = tval->LoopFitMode;

	if (tval->Mute != paraMute.NoValue)
	{
		Muted = (tval->Mute == SWITCH_ON);
	}

	if (tval->Velocity != paraVelocity.NoValue)
		fVelocity = (float)tval->Velocity / 128.0f;

	if (tval->Volume != paraVolume.NoValue)
		INERTIA_MACRO(ipVolume, etLinear, tval->Volume / 128.0f);

	if (tval->Pan != paraPan.NoValue)
		INERTIA_MACRO(ipPan, etLinear, (tval->Pan-1) / 127.0f);

	if (tval->Tune != paraTune.NoValue)
		INERTIA_MACRO(ipTune, etLinear, ((float)tval->Tune - 128.0f) / 48.0f);

	if (tval->HumanVel != paraHumanVel.NoValue)
		fHumanVel = (float)tval->HumanVel / 128.0f;

	if (tval->HumanPan != paraHumanPan.NoValue)
		fHumanPan = (float)tval->HumanPan / 128.0f;

	if (tval->HumanTune != paraHumanTune.NoValue)
		fHumanTune = (float)tval->HumanTune * 0.1f;

	if (tval->Group != paraGroup.NoValue)
	{
		Group = tval->Group;
	}

	if (tval->LenUnit != paraLenUnit.NoValue)
		CmdLenUnit = tval->LenUnit;

	if (tval->VolEnv  != paraVolEnv.NoValue ) nVolEnv  = tval->VolEnv;
	if (tval->PanEnv  != paraPanEnv.NoValue ) nPanEnv  = tval->PanEnv;
	if (tval->TuneEnv != paraTuneEnv.NoValue) nTuneEnv = tval->TuneEnv;

	if (tval->VolEnvLen  != paraVolEnvLen.NoValue ) fVolEnvLen  = (float)tval->VolEnvLen  / 16.0f;
	if (tval->PanEnvLen  != paraPanEnvLen.NoValue ) fPanEnvLen  = (float)tval->PanEnvLen  / 16.0f;
	if (tval->TuneEnvLen != paraTuneEnvLen.NoValue) fTuneEnvLen = (float)tval->TuneEnvLen / 16.0f;



	ProcessGlobalCommand(tval->Command1,tval->Argument1);
	ProcessGlobalCommand(tval->Command2,tval->Argument2);

	for (int fakey = 0; fakey < 2; fakey++)
	{
		if (Trigger != 0xFF)
		{
			if (Trigger == 0) //off
				TriggerOff();
			else //on
				TriggerIt(Trigger, tval->Command1, tval->Argument1, tval->Command2, tval->Argument2,
						pmi->ShuffleThisTick ?
							pmi->ShuffleSize * (1.0f + 2.0f*(pmi->m_frand()-0.5f)*pmi->ShuffleRnd)
								* pmi->pMasterInfo->SamplesPerTick : 0);
		}
		Trigger = tval->TrackTrigger;
	}
}

#undef INERTIA_MACRO



