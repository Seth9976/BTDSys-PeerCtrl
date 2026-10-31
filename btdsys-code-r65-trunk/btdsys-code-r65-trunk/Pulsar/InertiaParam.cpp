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
#include "inertiaparam.h"
#include "frand.h"

#include <math.h>
#include "hermite.h"

float LFO::CalcValue()
{
	float f;
	switch (nWaveType)
	{
	case 0: //sine
		f = sinf(fPhase);
		break;
	case 1: //square
		f = (fPhase < PI) ? -1.0f : 1.0f;
		break;
	case 2: //tri
		if (fPhase < PI*0.5f)
			f = fPhase / (PI*0.5f);
		else if (fPhase < PI*1.5f)
			f = (PI - fPhase) / (PI*0.5f);
		else
			f = (fPhase - PI*2.0f) / (PI*0.5f);
		break;
	case 3: //rampup
		f = ( fPhase            / PI) - 1.0f;
		break;
	case 4: //rampdn
		f = ((PI*2.0f - fPhase) / PI) - 1.0f;
		break;
	case 5: //random
		f = fRandValue;
		break;
	case 6: //wander
		f = fPhase * (fRandValue - fLastRandValue) / (2.0f*PI) + fLastRandValue;
		break;
	case 7: //cosine wander
		f = cosf(fPhase * 0.5f) * (fLastRandValue - fRandValue)*0.5f + (fLastRandValue + fRandValue)*0.5f;
		break;
	case 8: //wavetable
		{
			const CWaveLevel *pLev = pCB->GetWaveLevel(nWTWave,0);

			if (!pLev) { f = 0.0f; break; }
			if (!pLev->pSamples) { f = 0.0f; break; }

			bool stereo = (pCB->GetWave(nWTWave)->Flags & WF_STEREO) != 0;
			float pos = fPhase / (2.0f*PI) * (float)pLev->numSamples;
			if (stereo)
			{
				f  = GetSampleL(pos, pLev->pSamples, pLev->numSamples);
				f += GetSampleR(pos, pLev->pSamples, pLev->numSamples);
				f *= 0.5f;
			}
			else
				f = GetSampleM(pos, pLev->pSamples, pLev->numSamples);

			f /= 32768.0f;
		}
		break;

	default:
		f = 0.0f;
		break;
	}
	return f * fAmount;
}

void LFO::Update(float numsamples)
{
	fPhase += fStep * numsamples;

	if (fPhase < 0 || fPhase >= 2.0f*PI)
	{
		fLastRandValue = fRandValue;
		fRandValue = frand();
		fPhase = fmodf(fPhase, 2.0f*PI);
	}

	fValue = CalcValue();
}

void InertiaParam::AttachLFO(LFO *p)
{
	vpLFO.push_back(p);
}

void InertiaParam::DetachLFO(LFO *p)
{
	for (LFO **pp = vpLFO.begin(); pp < vpLFO.end(); pp++)
	{
		if (*pp == p) pp = vpLFO.erase(pp);
	}
}

void InertiaParam::SlaveTo(InertiaParam *p)
{
	pSlaveTo = p;
}

void InertiaParam::Unslave()
{
	pSlaveTo = NULL;
}

void InertiaParam::SetValue(float f, int len, InertiaType type)
{
	if (f < fMin) f = fMin;
	if (f > fMax) f = fMax;
	fTarget = f;
	nType = type;
	switch (type)
	{
	case etLinear:
		fStep = (f - fCurrent) / (float)len;
		break;
	case etRecip:
		fStep = (1.0f/f - 1.0f/fCurrent) / (float)len;
		break;
	}
	bSliding = true;
	fCountdown = (float)len;
}

float InertiaParam::GetValue()
{
	float f = 0.0f;
	for (LFO **pp = vpLFO.begin(); pp < vpLFO.end(); pp++)
		f += (*pp)->GetValue();

	f = fCurrent + f * (fMax-fMin);
	if (f < fMin) f = fMin;
	if (f > fMax) f = fMax;
	return f;
}

void InertiaParam::Update(float numsamples)
{
	if (pSlaveTo)
		fCurrent = pSlaveTo->GetValue();
	else if (bSliding)
	{
		switch (nType)
		{
		case etLinear:
			fCurrent += fStep * numsamples;
			break;
		case etRecip:
			fCurrent = 1.0f / (1.0f / fCurrent + fStep * numsamples);
			break;
		}
		fCountdown -= numsamples;
		if (fCountdown <= 0.0f)
		{
			fCurrent = fTarget;
			bSliding = false;
		}
	}
	// else: fCurrent is correct, do nothing
}
