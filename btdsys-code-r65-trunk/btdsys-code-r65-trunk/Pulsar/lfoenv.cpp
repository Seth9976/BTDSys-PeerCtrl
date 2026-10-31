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
#include "hermite.h"

inline float sqrf(float x) {return x*x;}

void Track::RecalcEnvTable()
{
	pfEnvTable[0] = pfEnvTable[1] = pfEnvTable[ENVTABLE_LENGTH] = pfEnvTable[ENVTABLE_LENGTH+1] = 0.0f;

	int i;
	switch (nEnvShape)
	{
	case 1: //Gaussian
		if (fEnvParam == 0.0f) fEnvParam = 0.000001f;
		for (i=0; i<ENVTABLE_LENGTH; i++)
			pfEnvTable[i+2] = expf( -sqrf( ((float)i / (float)ENVTABLE_LENGTH - 0.5f)/fEnvParam ) );
		break;

	case 2: //lin decay
		for (i=0; i<ENVTABLE_LENGTH; i++)
			pfEnvTable[i+2] = 1.0f - (float)i / (float)ENVTABLE_LENGTH;
		break;

	case 3: //exp decay
		if (fEnvParam == 0.0f) fEnvParam = 0.000001f;
		{
			float k = -1.0f/fEnvParam;
			for (i=0; i<ENVTABLE_LENGTH; i++)
				pfEnvTable[i+2] = expf(k * (float)i / (float)ENVTABLE_LENGTH);
		}
		break;

	case 4: //lin attack
		for (i=0; i<ENVTABLE_LENGTH; i++)
			pfEnvTable[i+2] = (float)i / (float)ENVTABLE_LENGTH;
		break;

	case 5: //exp attack
		if (fEnvParam == 0.0f) fEnvParam = 0.000001f;
		{
			float k = -1.0f/fEnvParam;
			for (i=0; i<ENVTABLE_LENGTH; i++)
				pfEnvTable[i+2] = expf(k * (1.0f - (float)i / (float)ENVTABLE_LENGTH));
		}
		break;

	case 6: //wavetable
		{
			const CWaveLevel *pLev = pmi->pCB->GetWaveLevel(nEnvWTWave,0);

			if (!pLev) { goto lblRectangular; }
			if (!pLev->pSamples) { break; }

			bool stereo = (pmi->pCB->GetWave(nEnvWTWave)->Flags & WF_STEREO) != 0;

			float m = (1.0f + fEnvParam) * 0.5f;
			float c = (1.0f - fEnvParam) * 0.5f;

			for (i=0; i<ENVTABLE_LENGTH; i++)
			{
				float f;

				float pos = (float)pLev->numSamples / 128.0f * (float)i;
				if (stereo)
				{
					f  = GetSampleL(pos, pLev->pSamples, pLev->numSamples);
					f += GetSampleR(pos, pLev->pSamples, pLev->numSamples);
					f *= 0.5f;
				}
				else
					f = GetSampleM(pos, pLev->pSamples, pLev->numSamples);

				f /= 32768.0f;

				pfEnvTable[i+2] = m*f + c;
			}
		}
		break;

	case 0: //rectangular
	default:
	lblRectangular:
		for (i=0; i<ENVTABLE_LENGTH; i++)
			pfEnvTable[i+2] = 1.0f;
		break;
	}

	if (bEnvNorm)
	{
		float min; float max = min = pfEnvTable[2];
		for (i=2; i<ENVTABLE_LENGTH+2; i++)
		{
			if      (pfEnvTable[i] > max) max = pfEnvTable[i];
			else if (pfEnvTable[i] < min) min = pfEnvTable[i];
		}

		if (max != min)
		{
			float fm = 1.0f / (max - min);
			float fc = -min * fm;

			for (i=2; i<ENVTABLE_LENGTH+2; i++)
				pfEnvTable[i] = fm * pfEnvTable[i] + fc;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

#define CLEAR_MACRO1(tar) /*********************/	\
	{												\
		if (nLFO1Track == 254)						\
		{											\
			for (int t=0; t<254; t++)				\
				pmi->tracks[t].tar.DetachLFO(&lfo1);	\
		}											\
		else										\
			pmi->tracks[nLFO1Track].tar.DetachLFO(&lfo1);\
	} ///////////////////////////////////////////////
//end define

void Track::ClearLFO1Target()
{
	switch (nLFO1Target)
	{
	case lfoFreqP:		CLEAR_MACRO1(ipFreqP); CLEAR_MACRO1(ipPeriodP);	break;
	case lfoFreqD:		CLEAR_MACRO1(ipFreqD);		break;
	case lfoWStart:		CLEAR_MACRO1(ipWStart);		break;
	case lfoWEnd:		CLEAR_MACRO1(ipWEnd);		break;
	case lfoStochMask:	CLEAR_MACRO1(ipStochMask);	break;
	case lfoRandomPan:	CLEAR_MACRO1(ipRandomPan);	break;
	case lfoVolume:		CLEAR_MACRO1(ipVolume);		break;
	case lfoPan:		CLEAR_MACRO1(ipPan);		break;
	case lfoEnvParam:	CLEAR_MACRO1(ipEnvParam);	break;
	case lfoBurst:		CLEAR_MACRO1(ipBurst);		break;
	case lfoRest:		CLEAR_MACRO1(ipRest);		break;
	case lfoOverlap:	CLEAR_MACRO1(ipOverlap);	break;
	}
}

#undef CLEAR_MACRO1

#define CLEAR_MACRO2(tar) /*********************/	\
	{												\
		if (nLFO2Track == 254)						\
		{											\
			for (int t=0; t<254; t++)				\
				pmi->tracks[t].tar.DetachLFO(&lfo2);	\
		}											\
		else										\
			pmi->tracks[nLFO2Track].tar.DetachLFO(&lfo2);\
	} ///////////////////////////////////////////////
//end define

void Track::ClearLFO2Target()
{
	switch (nLFO2Target)
	{
	case lfoFreqP:		CLEAR_MACRO2(ipFreqP); CLEAR_MACRO2(ipPeriodP);	break;
	case lfoFreqD:		CLEAR_MACRO2(ipFreqD);		break;
	case lfoWStart:		CLEAR_MACRO2(ipWStart);		break;
	case lfoWEnd:		CLEAR_MACRO2(ipWEnd);		break;
	case lfoStochMask:	CLEAR_MACRO2(ipStochMask);	break;
	case lfoRandomPan:	CLEAR_MACRO2(ipRandomPan);	break;
	case lfoVolume:		CLEAR_MACRO2(ipVolume);		break;
	case lfoPan:		CLEAR_MACRO2(ipPan);		break;
	case lfoEnvParam:	CLEAR_MACRO2(ipEnvParam);	break;
	case lfoBurst:		CLEAR_MACRO2(ipBurst);		break;
	case lfoRest:		CLEAR_MACRO2(ipRest);		break;
	case lfoOverlap:	CLEAR_MACRO2(ipOverlap);	break;
	case lfoLFO1Period:	CLEAR_MACRO2(ipLFO1Period);	break;
	case lfoLFO1Amount:	CLEAR_MACRO2(ipLFO1Amount);	break;
	}
}

#undef CLEAR_MACRO2

//////////////////////////////////////////////////////////////////////////////////////////////////////

#define SET_MACRO1(tar) /***************************/	\
	{													\
		if (nLFO1Track == 254)							\
		{												\
			for (int t=0; t<254; t++)					\
			{											\
				pmi->tracks[t].tar.AttachLFO(&lfo1);		\
			}											\
		}												\
		else											\
		{												\
			pmi->tracks[nLFO1Track].tar.AttachLFO(&lfo1);	\
		}												\
	} ///////////////////////////////////////////////////
//end define

void Track::SetLFO1Target()
{
	switch (nLFO1Target)
	{
	case lfoFreqP:		SET_MACRO1(ipFreqP); SET_MACRO1(ipPeriodP);	break;
	case lfoFreqD:		SET_MACRO1(ipFreqD);		break;
	case lfoWStart:		SET_MACRO1(ipWStart);		break;
	case lfoWEnd:		SET_MACRO1(ipWEnd);			break;
	case lfoStochMask:	SET_MACRO1(ipStochMask);	break;
	case lfoRandomPan:	SET_MACRO1(ipRandomPan);	break;
	case lfoVolume:		SET_MACRO1(ipVolume);		break;
	case lfoPan:		SET_MACRO1(ipPan);			break;
	case lfoEnvParam:	SET_MACRO1(ipEnvParam);		break;
	case lfoBurst:		SET_MACRO1(ipBurst);		break;
	case lfoRest:		SET_MACRO1(ipRest);			break;
	case lfoOverlap:	SET_MACRO1(ipOverlap);		break;
	}
}

#undef SET_MACRO1

#define SET_MACRO2(tar) /***************************/	\
	{													\
		if (nLFO2Track == 254)							\
		{												\
			for (int t=0; t<254; t++)					\
			{											\
				pmi->tracks[t].tar.AttachLFO(&lfo2);		\
			}											\
		}												\
		else											\
		{												\
			pmi->tracks[nLFO2Track].tar.AttachLFO(&lfo2);	\
		}												\
	} ///////////////////////////////////////////////////
//end define

void Track::SetLFO2Target()
{
	switch (nLFO2Target)
	{
	case lfoFreqP:		SET_MACRO2(ipFreqP); SET_MACRO2(ipPeriodP);	break;
	case lfoFreqD:		SET_MACRO2(ipFreqD);		break;
	case lfoWStart:		SET_MACRO2(ipWStart);		break;
	case lfoWEnd:		SET_MACRO2(ipWEnd);		break;
	case lfoStochMask:	SET_MACRO2(ipStochMask);	break;
	case lfoRandomPan:	SET_MACRO2(ipRandomPan);	break;
	case lfoVolume:		SET_MACRO2(ipVolume);		break;
	case lfoPan:		SET_MACRO2(ipPan);		break;
	case lfoEnvParam:	SET_MACRO2(ipEnvParam);	break;
	case lfoBurst:		SET_MACRO2(ipBurst);		break;
	case lfoRest:		SET_MACRO2(ipRest);			break;
	case lfoOverlap:	SET_MACRO2(ipOverlap);		break;
	case lfoLFO1Period:	SET_MACRO2(ipLFO1Period);	break;
	case lfoLFO1Amount:	SET_MACRO2(ipLFO1Amount);	break;
	}
}

#undef SET_MACRO2

