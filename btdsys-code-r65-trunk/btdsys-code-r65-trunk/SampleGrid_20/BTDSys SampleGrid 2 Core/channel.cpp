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

#include "channel.h"
#include <windows.h>
#include <math.h>
#include "common.h"
#include "sgrid.h"
#include "24bit.h"
#include "hermite.h"


CChannel::CChannel()
{
	bActive = false;
	nRampingOff = 0;
	nPeerValue = -1;
    bSleepy = false;
}


CChannel::~CChannel()
{
}

void CChannel::TriggerOn(	int sample, STrigInfo ti, int ramp)
{
	bKitMode = false;
	CWaveLevel const *pLevel = pCB->GetWaveLevel(sample, 0);
	CWaveInfo  const *pWInfo = pCB->GetWave(sample);
	if (pLevel && pWInfo)
	{
		if (pWInfo->Flags & WF_NOT16)
			fPos = (pLevel->numSamples - 8.0f) / 2.0f * ti.offset;
		else
			fPos = pLevel->numSamples * ti.offset;
	}
	else
		fPos = 0.0f;

	nAge = -ti.delay;
	nWave = sample;
	bActive = true;
	fVel = ti.vel;
	fPan = ti.pan;
	fPosStep = ti.rate;
	nCut = ti.cut;
	if (nCut < 0) nCut = 0xFFFFFFF;
	nRampingOff = 0;

	nRampingOn = ramp;
	if (ramp)
	{
		fRampVol = 0.0f;
		fRampVolStep = 1.0f / (float)ramp;
	}

	bForceVolEnv = bForcePanEnv = bUsePosEnv = false;

	envVol.Reset();
	if (ptr->nVolEnv)
	{
		envVol.SetEnvelope(0,0,0,0,0,0,0,
			&pmi->pCustomEnvelopeShapes[ptr->nVolEnv-1],
			ptr->fVolEnvLen, pmi->pMasterInfo->SamplesPerTick);
		envVol.Trigger(ermDown);
	}

	envPan.Reset();
	if (ptr->nPanEnv)
	{
		envPan.SetEnvelope(0,0,0,0,0,0,0,
			&pmi->pCustomEnvelopeShapes[ptr->nPanEnv-1],
			ptr->fPanEnvLen, pmi->pMasterInfo->SamplesPerTick);
		envPan.Trigger(ermDown);
	}

	if (ptr->nPosEnv)
	{
		envPos.SetEnvelope(0,0,0,0,0,0,0,
			&pmi->pCustomEnvelopeShapes[ptr->nPosEnv-1],
			ptr->fTuneEnvLen, pmi->pMasterInfo->SamplesPerTick);
		envPos.Reset();
		envPos.Trigger(ermDown);
		bUsePosEnv = true;
	}


/*PEER	if (ptr->pc->GotParam())
	{
		CMachineParameter const *par = ptr->pc->GetParamInfo();
		int a=1,b=1;
		if (par->Type != pt_switch)
		{
			a = par->MinValue + 1;
			b = par->MaxValue;
		}

		nPeerValue = (int)( vel * (float)(b-a) + (float)a );
		nPeerCountDown = delay;
	}
*/}

void CChannel::TriggerOn(	CDrumkitSample *pPSISamp, STrigInfo ti, int ramp)
{
	pLyr = pPSISamp->GetLayer(ti.vel);
	if (!pLyr)
	{
		TriggerOffHard();
		return;
	}

	TriggerOn(0,ti,ramp);
	fPos = pLyr->numSamples * ti.offset;
	bKitMode = true;
}

void CChannel::TriggerOn(	CDrumkitLayer *pLayer, STrigInfo ti, int ramp)
{
	pLyr = pLayer;
	if (!pLyr->bDataLoaded)
	{
		if (!pLyr->LoadWaveData())
		{
			TriggerOffHard();
			return;
		}
	}

	TriggerOn(0,ti,ramp);
	fPos = pLyr->numSamples * ti.offset;
	bKitMode = true;
}

void CChannel::TriggerOff(int RampLen)
{
	if (envVol.Active())
	{
		envVol.Release();
		envPan.Release();
	}
	else
	{
		if (!nRampingOff)
		{
			if (RampLen)
			{
				nRampingOff = RampLen;
				fRampVol = 1.0f;
				fRampVolStep = -1.0f / (float)RampLen;
			}
			else
				TriggerOffHard();
		}
	}

/*PEER	if (ptr->pc->GotParam())
	{
		nPeerValue = 0;
		nPeerCountDown = 0;
	}
*/}

void CChannel::TriggerOffHard()
{
	bActive = false;
}

bool CChannel::Work(float *psamples, int numsamples, int samplerate)
{
/*PEER	if (nPeerValue != -1 && ptr->pc->GotParam())
	{
		nPeerCountDown -= numsamples;
		if (nPeerCountDown < 0)
		{
			ptr->pc->ControlChange_Immediate(0,nPeerValue);
			nPeerValue = -1;
		}
	}
*/
	if (!bActive) return false;

	if (nAge + numsamples < 0)
	{
		nAge += numsamples;
		return false;
	}
	else
	{
		static float buf[512];

		double PosStep = (double)fPosStep * pow(2.0,ptr->TuneCurrent) * pow(2.0,pmi->TuneCurrent);
dbgprintf("PosStep = %.20f",PosStep);
/*		static char txt[160];
		sprintf(txt,"%.24f",PosStep);
		OutputDebugString(txt);
*/
		int i;
		for (i=0; i<numsamples && nAge < 0; i++)
		{
			buf[i*2] = buf[i*2+1] = 0.0f;
			nAge++;
		}

#		define UPDATEPOS(fnsmp)		\
			if (bUsePosEnv)			\
				fPos = envPos.NextSample() * fnsmp;	\
			else					\
			{						\
				fPos += PosStep;	\
				if (fPos >= fnsmp)	\
				{					\
					switch (ptr->loopMode)	\
					{						\
					case lmNone:		break; \
					case lmStraight:	fPos = ptr->fLoopStart * fnsmp; break; \
					case lmPingPong:	break; \
					case lmRandom:		break; \
					}						\
				}					\
			}						\
		/////////////////////////////

		if (!bKitMode)
		{
			CWaveLevel const *pLevel = pCB->GetWaveLevel(nWave, 0);
			CWaveInfo  const *pWInfo = pCB->GetWave(nWave);
			if (pLevel && pWInfo)
			{
				float fnSamples = (float)pLevel->numSamples * ptr->fLoopEnd;
				float wvol = pWInfo->Volume;

				if ((pWInfo->Flags & WF_NOT16) && (pWInfo->Flags & WF_STEREO))
				{
#					define WORKLOOP(type, veladjust)													\
					{																					\
						type *pSamp = (type *)&pLevel->pSamples[4];										\
						int nSamples2 = (pLevel->numSamples - 8) / 2;									\
						float fnSamples2 = (float)nSamples2;											\
						for (; i<numsamples && fPos < fnSamples2 && fPos >= 0.0f && nAge <= nCut; i++)	\
						{																				\
							buf[i*2  ] = GetSampleL(fPos, pSamp, nSamples2) * (veladjust) * wvol;				\
							buf[i*2+1] = GetSampleR(fPos, pSamp, nSamples2) * (veladjust) * wvol;				\
							nAge++;																		\
							UPDATEPOS(fnSamples2);														\
						}																				\
					}																					\
					/////////////////////////////////////////////////////////////////////////////////////

					switch (pLevel->pSamples[0])
					{
					case WFE_32BITF:
						WORKLOOP(float, 32768.0f);
						break;
					case WFE_32BITI:
						WORKLOOP(int, 0.0000152587890625f);
						break;
					case WFE_24BIT:
						WORKLOOP(int24, 0.0000152587890625f);
						break;
					}

#					undef WORKLOOP

					//original code at eof
				}
				else if (pWInfo->Flags & WF_NOT16) //mono
				{
#					define WORKLOOP(type, veladjust)													\
					{																					\
						type *pSamp = (type *)&pLevel->pSamples[4];										\
						int nSamples2 = (pLevel->numSamples - 8) / 2;									\
						float fnSamples2 = (float)nSamples2;											\
						for (; i<numsamples && fPos < fnSamples2 && fPos >= 0.0f && nAge <= nCut; i++)	\
						{																				\
							buf[i*2] = buf[i*2+1] = GetSampleM(fPos, pSamp, nSamples2) * (veladjust) * wvol;	\
							nAge++;																		\
							UPDATEPOS(fnSamples2);														\
						}																				\
					}																					\
					/////////////////////////////////////////////////////////////////////////////////////

					switch (pLevel->pSamples[0])
					{
					case WFE_32BITF:
						WORKLOOP(float, 32768.0f);
						break;
					case WFE_32BITI:
						WORKLOOP(int, 0.0000152587890625f);
						break;
					case WFE_24BIT:
						WORKLOOP(int24, 0.0000152587890625f);
						break;
					}

#					undef WORKLOOP
				}
				else
				{
					if (pWInfo->Flags & WF_STEREO)
					{
						for (; i<numsamples && fPos < fnSamples && fPos >= 0.0f && nAge <= nCut; i++)
						{
							buf[i*2  ] = GetSampleL(fPos, pLevel->pSamples, pLevel->numSamples) * wvol;
							buf[i*2+1] = GetSampleR(fPos, pLevel->pSamples, pLevel->numSamples) * wvol;
							nAge++;
							UPDATEPOS(fnSamples);
						}
					}
					else
					{
						for (; i<numsamples && fPos < fnSamples && fPos >= 0.0f && nAge <= nCut; i++)
						{
							buf[i*2] = buf[i*2+1] = GetSampleM(fPos, pLevel->pSamples, pLevel->numSamples) * wvol;
							nAge++;
							UPDATEPOS(fnSamples);
						}
					}
				}
			}
		}
		else if (pLyr) //kit mode
		{
			float fnSamples = (float)pLyr->numSamples;

			if (PosStep == 1.0f && !bUsePosEnv)
			{
				int nPos = (int)fPos;

				for (; i<numsamples && nPos < pLyr->numSamples && nPos >= 0 && nAge <= nCut; i++)
				{
					buf[i*2  ] = pLyr->GetRawSampleL(nPos) * pLyr->fVol * pLyr->fVolCompensate;
					buf[i*2+1] = pLyr->GetRawSampleR(nPos) * pLyr->fVol * pLyr->fVolCompensate;
					nAge++;
					nPos++;
				}

				fPos = (float)nPos;
			}
			else //PosStep != 1.0f
			{
				for (; i<numsamples && fPos < fnSamples && fPos >= 0.0f && nAge <= nCut; i++)
				{
					buf[i*2  ] = pLyr->GetSampleL(fPos) * pLyr->fVol * pLyr->fVolCompensate;
					buf[i*2+1] = pLyr->GetSampleR(fPos) * pLyr->fVol * pLyr->fVolCompensate;
					nAge++;
					UPDATEPOS(fnSamples);
				}
			}
		}

#		undef UPDATEPOS

		for (; i<numsamples; i++)
		{
			bActive = false;
			buf[i*2] = buf[i*2+1] = 0;
		}

		if (nRampingOff || nRampingOn)
		{
			bool off = (nRampingOff != 0);
			bool on  = (nRampingOn  != 0);
			for (i=0; i<numsamples*2 && (nRampingOff > 0 || nRampingOn > 0); i+=2)
			{
				buf[i  ] *= fRampVol;
				buf[i+1] *= fRampVol;
				fRampVol += fRampVolStep;
				if (nRampingOff) nRampingOff--;
				if (nRampingOn ) nRampingOn--;
			}

			if (off)
			{
				for (; i<numsamples*2; i++)
				{
					buf[i] = 0.0f;
					bActive = false;
				}
			}
		}


		for (i=0; i<numsamples*2; i+=2)
		{
			float evol,epan;
			if (ptr->nVolEnv || bForceVolEnv)
				evol = envVol.NextSample();
			else
				evol = 1.0f;

			if (ptr->nPanEnv || bForcePanEnv)
				epan = envPan.NextSample() * 2.0f;
			else
				epan = 1.0f;

			psamples[i  ] += buf[i  ] * fVel * (2.0f - fPan) * epan * evol;
			psamples[i+1] += buf[i+1] * fVel * fPan * (2.0f - epan) * evol;
		}

	}

	return true;
}








































/*					switch (pLevel->pSamples[0])
					{
					case WFE_32BITF:
						{
							float *pSamp = (float *)&pLevel->pSamples[4];
							int nSamples2 = (pLevel->numSamples - 8) / 2;
							float fnSamples2 = (float)nSamples2;
							for (; i<numsamples && fPos < fnSamples2 && fPos >= 0.0f && nAge <= nCut; i++)
							{
								buf[i*2  ] = GetSampleL(fPos, pSamp, nSamples2) * 32768.0f;
								buf[i*2+1] = GetSampleR(fPos, pSamp, nSamples2) * 32768.0f;
								nAge++;
								fPos += PosStep;
							}
						}
						break;
					case WFE_32BITI:
						{
							int *pSamp = (int *)&pLevel->pSamples[4];
							int nSamples2 = (pLevel->numSamples - 8) / 2;
							float fnSamples2 = (float)nSamples2;
							for (; i<numsamples && fPos < fnSamples2 && fPos >= 0.0f && nAge <= nCut; i++)
							{
								buf[i*2  ] = GetSampleL(fPos, pSamp, nSamples2) / 65536.0f;
								buf[i*2+1] = GetSampleR(fPos, pSamp, nSamples2) / 65536.0f;
								nAge++;
								fPos += PosStep;
							}
						}
						break;
					case WFE_24BIT:
						{
							int24 *pSamp = (int24 *)&pLevel->pSamples[4];
							int nSamples2 = (pLevel->numSamples - 8) / 2;
							float fnSamples2 = (float)nSamples2;
							for (; i<numsamples && fPos < fnSamples2 && fPos >= 0.0f && nAge <= nCut; i++)
							{
								buf[i*2  ] = GetSampleL(fPos, pSamp, nSamples2) / 65536.0f;
								buf[i*2+1] = GetSampleR(fPos, pSamp, nSamples2) / 65536.0f;

								nAge++;
								fPos += PosStep;
							}
						}
						break;
					}
*/
