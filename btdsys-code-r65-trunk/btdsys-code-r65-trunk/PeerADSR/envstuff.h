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

#pragma once

int mi::GetEnvPointNumber(CEnvData *env, float pos)
{
	int p = 0;
	while (env->ptcoords[p * 2] < pos) p++;
	return p-1;
}

float mi::GetEnvPoint(CEnvData *env, float pos)
{
	int p = GetEnvPointNumber(env, pos);

	int x1 = env->ptcoords[p * 2];
	int y1 = env->ptcoords[p * 2 + 1];
	int x2 = env->ptcoords[p * 2 + 2];
	int y2 = env->ptcoords[p * 2 + 3];

	float d = (float)y1 + (float)(y2 - y1) * (pos-x1) / (float)(x2 - x1);

	return 1.0f - (d / 65536.0f);
}

#define UPDATE_VALUE_STEP(target,len)	\
	ValueStep = ((target) - ValueCurrent) / (float)(len)

void CTrack::EnterA()
{
	EnvPhase = epA;
	ValueCurrent = 0.0f;
	EnvCountdown = lenA;
	UPDATE_VALUE_STEP(1.0f, lenA);
}

void CTrack::EnterD()
{
	EnvPhase = epD;
	ValueCurrent = 1.0f;
	EnvCountdown = lenD;
	UPDATE_VALUE_STEP(envSLev, lenD);
}

void CTrack::EnterS()
{
	EnvPhase = epS;
	ValueCurrent = envSLev;
	if (lenS < 0.0f)
		EnvCountdown = -1;
	else
		EnvCountdown = lenS;
	ValueStep = 0.0f;
}

void CTrack::EnterR()
{
	EnvPhase = epR;
	//don't alter ValueCurrent
	EnvCountdown = lenR;
	UPDATE_VALUE_STEP(0.0f, lenR);
}

//Following three functions return an interpolated sample 
//from a mono buffer and left/right of stereo, respectively.

inline float GetSampleM(const float Pos, short *pSamples)
{
	float frac = Pos - int(Pos); //how far 'into' the sample we are
	return (pSamples[int(Pos)] * (1-frac)) 
		 + (pSamples[int(Pos)+1] * frac);
}

inline float GetSampleL(const float Pos, short *pSamples)
{
	float frac = Pos - int(Pos);
	return (pSamples[int(Pos)*2] * (1-frac)) 
		 + (pSamples[int(Pos)*2+2] * frac);
}

inline float GetSampleR(const float Pos, short *pSamples)
{
	float frac = Pos - int(Pos);
	return (pSamples[int(Pos)*2+1] * (1-frac)) 
		 + (pSamples[int(Pos)*2+3] * frac);
}

//Update the envelope and do some sending
void CTrack::UpdateEnv(int numsamples)
{
	if (Enveloping)
	{
		switch (EnvType)
		{
		case 0: //ADSR
			ValueCurrent += ValueStep * numsamples;
			if (EnvCountdown != -1)
			{
				EnvCountdown -= numsamples;

				if (EnvCountdown <= 0)
				{
					switch (EnvPhase)
					{
					case epA: EnterD();	break;
					case epD: EnterS();	break;
					case epS: EnterR();	break;
					case epR:
						Enveloping = false;
						JustStoppedEnveloping = true;
						ValueCurrent = 0.0f;
						break;
					}
				}
			}
			break;

		case 1: //Custom
			{
				if (EnvPhase != epS)
				{
					ValueCurrent = pmi->GetEnvPoint(&pmi->CustomEnv[EnvNum-1], EnvPos);
					EnvPos += EnvPosStep * numsamples;
				}
				else
				{
					ValueCurrent = 1.0f - (pmi->CustomEnv[EnvNum-1].ptcoords[pmi->CustomEnv[EnvNum-1].suspoint * 2 + 1] / 65536.0f);
				}

				if (EnvPhase == epA &&
					pmi->GetEnvPointNumber(&pmi->CustomEnv[EnvNum-1], EnvPos) == pmi->CustomEnv[EnvNum-1].suspoint)
				{
					EnvPhase = epS;
				}

				if (EnvPos >= 65536.0f)
				{
					Enveloping = false;
					JustStoppedEnveloping = true;
					ValueCurrent = 1.0f - (pmi->CustomEnv[EnvNum-1].ptcoords[pmi->CustomEnv[EnvNum-1].numpoints*2-1] / 65536.0f);
				}
			}
			break;

		case 2: //Wavetable
			{
				CWaveLevel const *lev = pmi->pCB->GetWaveLevel(EnvNum, 0);
				if (lev == NULL)
					Enveloping = false;
				else
				{
					if (EnvPos < lev->numSamples)
					{
						if (pmi->pCB->GetWave(EnvNum)->Flags & WF_STEREO)
							ValueCurrent = GetSampleL(EnvPos, lev->pSamples);
						else
							ValueCurrent = GetSampleM(EnvPos, lev->pSamples);

						ValueCurrent = (ValueCurrent + 32768.0f) / 65536.0f;

						EnvPos += EnvPosStep * numsamples;
						if (EnvPos >= lev->numSamples)
						{
							Enveloping = false;
							JustStoppedEnveloping = true;

							if (pmi->pCB->GetWave(EnvNum)->Flags & WF_STEREO)
								ValueCurrent = GetSampleL((float)lev->numSamples-1, lev->pSamples);
							else
								ValueCurrent = GetSampleM((float)lev->numSamples-1, lev->pSamples);

							ValueCurrent = (ValueCurrent + 32768.0f) / 65536.0f;
						}
					}
					else
					{
						Enveloping = false;
						JustStoppedEnveloping = true;

						if (pmi->pCB->GetWave(EnvNum)->Flags & WF_STEREO)
							ValueCurrent = GetSampleL((float)lev->numSamples-1, lev->pSamples);
						else
							ValueCurrent = GetSampleM((float)lev->numSamples-1, lev->pSamples);

						ValueCurrent = (ValueCurrent + 32768.0f) / 65536.0f;
					}
				}
			}		
			break;
		}

		if (PeerCtrl.GotParam() && !pmi->JustTicked)
		{
			if (ValueCurrent < 0.0f) ValueCurrent = 0.0f;
			if (ValueCurrent > 1.0f) ValueCurrent = 1.0f;
			int ValueP = FloatToPVal(ValueCurrent);

			if (pmi->aval.SubtickCtrl == 1)
			{
				if (Track != 0xFE) // Single track
				{
					PeerCtrl.ControlChange_Immediate(Track, (int)ValueP);
				}
				else // All tracks
				{
					for (int t=0; t<PeerCtrl.GetMInfo()->maxTracks; t++)
					{
						PeerCtrl.ControlChange_Immediate(t, (int)ValueP);
					}
				}
			}
		}
	}
}
