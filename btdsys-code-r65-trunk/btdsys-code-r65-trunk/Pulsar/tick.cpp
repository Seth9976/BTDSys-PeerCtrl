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
#include <math.h>

#include "pulsar.h"

int mi::FreqToParam(int mode, int oldmode, float f, float mult, float c, float oldmult, float oldc)
{
	if (oldmode == 0)
		f *= oldmult;
	else
		f = powf(2.0f, f/2048.0f + oldc);

	int r;

	if (mode == 0) //linear
		r = (int)(f / mult + 0.5f);
	else
		r = (int)(2048.0f*(logf(f) / logf(2.0f) - c) + 0.5f);

	if (r > paraFreqP.MaxValue) r = paraFreqP.MaxValue;
	if (r < paraFreqP.MinValue) r = paraFreqP.MinValue;
	return r;
}

void mi::Tick()
{
	if (gval.Inertia != paraInertia.NoValue)
		nInertiaLen = gval.Inertia * pMasterInfo->SamplesPerTick / 16;

	bool bShouldResetSliders = false;
	float oldPc = fLogPFreqC;
	float oldDc = fLogDFreqC;
	float oldPm = fLinPFreqMult;
	float oldDm = fLinDFreqMult;
	int oldmode = nFreqSliderMode;

	if (gval.FreqSliderMode != paraFreqSliderMode.NoValue)
	{
		bShouldResetSliders = (nFreqSliderMode != gval.FreqSliderMode);
		nFreqSliderMode = gval.FreqSliderMode;
	}

	if (gval.MinPFreq != paraMinPFreq.NoValue)
	{
		fLinPFreqMult = gval.MinPFreq / 1000.0f;
		fLogPFreqC = logf(fLinPFreqMult) / logf(2.0f);
	}

	if (gval.MinDFreq != paraMinDFreq.NoValue)
	{
		fLinDFreqMult = gval.MinDFreq / 1000.0f;
		fLogDFreqC = logf(fLinDFreqMult) / logf(2.0f);
	}

	// check for preset changes or programmed values
	for (int t=0; t<numTracks && bShouldResetSliders; t++)
		bShouldResetSliders = (tval[t].FreqP == paraFreqP.NoValue && tval[t].FreqD == paraFreqD.NoValue);

	if (bShouldResetSliders)
	{
		for (int t=0; t<numTracks; t++)
		{
			int val;

			val = FreqToParam(nFreqSliderMode, oldmode, 
								tracks[t].ipFreqP.GetTargetValue(), fLinPFreqMult, fLogPFreqC, oldPm, oldPc);
			tracks[t].ipFreqP.SetValueNoInertia(val);
			pCB->ControlChange(thismac,2,t,numFreqP - NUMG, val);

			val = FreqToParam(nFreqSliderMode, oldmode,
								tracks[t].ipFreqD.GetTargetValue(), fLinDFreqMult, fLogDFreqC, oldDm, oldDc);
			tracks[t].ipFreqD.SetValueNoInertia(val);
			pCB->ControlChange(thismac,2,t,numFreqD - NUMG, val);
		}
	}

	if (gval.Oversample != paraOversample.NoValue)
	{
		nOversampExp = gval.Oversample;
		nOversample = 1 << nOversampExp;
		nMaxWorkBlockSize = 256 * nOversample;
	}

	if (gval.DCRemoval != paraDCRemoval.NoValue)
	{
		bRemoveDC = (gval.DCRemoval == SWITCH_ON);
	}

	if (bFirstTick)
	{
		for (int t=0; t<254; t++)
			tracks[t].Tick();
	}
	else
	{
		for (int t=0; t<numTracks; t++)
			tracks[t].Tick();
	}

	bFirstTick = false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

inline float NoteToFreq(int note)
{
	int n = (note >> 4)*12 
			+ (note & 15) - 1 - (4*12+9); // ?

	return 880.0f * powf(2.0f, (float)n / 12.0f);
}

#define INERTIA_MACRO(ip,t,f) /**********************/	\
	{													\
		if (pmi->bFirstTick || pmi->nInertiaLen == 0)	\
			(ip).SetValueNoInertia(f);					\
		else											\
			(ip).SetValue(f,pmi->nInertiaLen,t);		\
	}	/////////////////////////////////////////////////
//end #define

void Track::Tick()
{
	if (tval->Synch == SWITCH_ON)
	{
		nBurstCounter = 0;
		fOverallPhase = 0.0f;
		lfo1.Reset();
		lfo2.Reset();
	}

	if (tval->Overlapping != paraOverlapping.NoValue)
		INERTIA_MACRO(ipOverlap, etLinear, tval->Overlapping);
/*	{
		for (int i=nOverlap; i<tval->Overlapping; i++)
			bPulsaretPlaying[i] = false;
		nOverlap = tval->Overlapping;
	}
*/
	if (tval->FreqD != paraFreqD.NoValue)
		INERTIA_MACRO(ipFreqD, etLinear, tval->FreqD);

	if (tval->PParam != paraPParam.NoValue)
	{
		if (tval->PParam == 3 && nTrack != 0) // slave
		{
			nPParam = pmi->tracks[nTrack-1].nPParam;
			ipFreqP  .SlaveTo(&pmi->tracks[nTrack-1].ipFreqP  );
			ipPeriodP.SlaveTo(&pmi->tracks[nTrack-1].ipPeriodP);
		}
		else
		{
			ipFreqP.Unslave();
			ipPeriodP.Unslave();
			nPParam = tval->PParam;
			if (nPParam == 3) nPParam = 0;
		}
	}

	if (tval->FreqP != paraFreqP.NoValue && nPParam != 2)
		INERTIA_MACRO(ipFreqP, etLinear, tval->FreqP);

	if (tval->PeriodP != paraPeriodP.NoValue)
		INERTIA_MACRO(ipPeriodP, etLinear, tval->PeriodP / 16.0f);

	if (tval->NoteP != paraNoteP.NoValue && nPParam == 2)
	{
		if (tval->NoteP != NOTE_OFF)
			INERTIA_MACRO(ipFreqP, etLinear, NoteToFreq(tval->NoteP));

		if (pmi->aval.OnOffOnNote)
		{
			int val = (tval->NoteP == NOTE_OFF) ? SWITCH_OFF : SWITCH_ON;
			pmi->pCB->ControlChange(pmi->thismac, 2, nTrack, numOnOff - NUMG, val);
			tval->OnOff = val;
		}
	}

#	ifndef PULSARFX
		if (tval->Wave != paraWave.NoValue)
			nWave = tval->Wave;
#	endif

	if (tval->WStart != paraWStart.NoValue)
		INERTIA_MACRO(ipWStart, etLinear, tval->WStart / 65534.0f);

	if (tval->WEnd != paraWEnd.NoValue)
		INERTIA_MACRO(ipWEnd, etLinear, tval->WEnd / 65534.0f);

	if (tval->EnvWTWave != paraEnvWTWave.NoValue)
	{
		nEnvWTWave = tval->EnvWTWave;
		RecalcEnvTable();
	}

	if (tval->EnvNorm != paraEnvNorm.NoValue)
	{
		bEnvNorm = (tval->EnvNorm == SWITCH_ON);
		RecalcEnvTable();
	}

	if (tval->Envelope != paraEnvelope.NoValue)
	{
		nEnvShape = tval->Envelope;
		RecalcEnvTable();
	}

	if (tval->EnvParam != paraEnvParam.NoValue)
		INERTIA_MACRO(ipEnvParam, etLinear, tval->EnvParam / 128.0f);

	if (tval->Burst != paraBurst.NoValue)
		INERTIA_MACRO(ipBurst, etLinear, tval->Burst);

	if (tval->Rest != paraRest.NoValue)
		INERTIA_MACRO(ipRest, etLinear, tval->Rest);

	if (tval->StochMask != paraStochMask.NoValue)
		INERTIA_MACRO(ipStochMask, etLinear, tval->StochMask / 32768.0f);

	if (tval->RandomPan != paraRandomPan.NoValue)
		INERTIA_MACRO(ipRandomPan, etLinear, tval->RandomPan / 32768.0f);

	if (tval->OnOff != paraOnOff.NoValue)
	{
#		ifdef PULSARFX
			bOn   = true;
			bHold = (tval->OnOff == SWITCH_ON);
			nHoldPulsaret = nRecentPulsaret;
#		else
			bHold = false;
			bOn   = (tval->OnOff == SWITCH_ON);
#		endif
	}
	if (tval->Volume != paraVolume.NoValue)
		INERTIA_MACRO(ipVolume, etLinear, tval->Volume / 32768.0f);

	if (tval->Pan != paraPan.NoValue)
		INERTIA_MACRO(ipPan, etLinear, tval->Pan / 128.0f);

	if (tval->LFO1Period != paraLFO1Period.NoValue)
		INERTIA_MACRO(ipLFO1Period, etLinear, tval->LFO1Period / 16.0f);

	if (tval->LFO2Period != paraLFO2Period.NoValue)
		INERTIA_MACRO(ipLFO2Period, etLinear, tval->LFO2Period / 16.0f);

	if (tval->LFO1WTWave != paraLFO1WTWave.NoValue)
		lfo1.nWTWave = tval->LFO1WTWave;

	if (tval->LFO2WTWave != paraLFO2WTWave.NoValue)
		lfo2.nWTWave = tval->LFO2WTWave;

	if (tval->LFO1Shape != paraLFO1Shape.NoValue)
		lfo1.nWaveType = tval->LFO1Shape;

	if (tval->LFO2Shape != paraLFO2Shape.NoValue)
		lfo2.nWaveType = tval->LFO2Shape;

	if (tval->LFO1Target != paraLFO1Target.NoValue)
	{
		ClearLFO1Target();
		nLFO1Target = tval->LFO1Target;
		if (tval->LFO1TargetTrack != paraLFO1TargetTrack.NoValue)
			nLFO1Track = tval->LFO1TargetTrack;
		SetLFO1Target();
	}
	else if (tval->LFO1TargetTrack != paraLFO1TargetTrack.NoValue)
	{
		ClearLFO1Target();
		nLFO1Track = tval->LFO1TargetTrack;
		SetLFO1Target();
	}

	if (tval->LFO2Target != paraLFO2Target.NoValue)
	{
		ClearLFO2Target();
		nLFO2Target = tval->LFO2Target;
		if (tval->LFO2TargetTrack != paraLFO2TargetTrack.NoValue)
			nLFO2Track = tval->LFO2TargetTrack;
		SetLFO2Target();
	}
	else if (tval->LFO2TargetTrack != paraLFO2TargetTrack.NoValue)
	{
		ClearLFO2Target();
		nLFO2Track = tval->LFO2TargetTrack;
		SetLFO2Target();
	}

	if (tval->LFO1Amount != paraLFO1Amount.NoValue)
		INERTIA_MACRO(ipLFO1Amount, etLinear, tval->LFO1Amount / 32768.0f);

	if (tval->LFO2Amount != paraLFO2Amount.NoValue)
		INERTIA_MACRO(ipLFO2Amount, etLinear, tval->LFO2Amount / 32768.0f);

	if (pOscDlg)// if (!pOscDlg->bDontRedrawMe)
	{
		bUpdateOscDlg = true;
	}
}

#undef INERTIA_MACRO
