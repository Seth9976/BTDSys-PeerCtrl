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
#include "frand.h"

#include "iwaves.h"


//Global vars... I feel dirty


bool Track::Work(float *psamples, const float *pinput, int numsamples, int const mode)
{
	bool stereo; int srcLen;

#	ifndef PULSARFX
		short *srcSamples;
#	else
		float *srcSamples;
#	endif

	bool bResult;
	float fLenP, fLenD;
	float fWStart;
	float fWEnd;
	float fStochMask;
	float fRandomPan;
	float fVolume;
	float fPan;	
	int nBurst;		
	int nRest;		
	int nOverlap;	
	float realnumsamples = (float)numsamples / (float)pmi->nOversample;
	lfo1.Update(realnumsamples);
	lfo2.Update(realnumsamples);

/*	char txt[256];
	sprintf(txt,"trackwork %i %i %s",numsamples,mode,bOn ? "on" : "off");
	OutputDebugString(txt);
*/
	switch (nPParam)
	{
	case 0: //Freq
		ipFreqP.Update(realnumsamples);
		if (pmi->nFreqSliderMode == 0)
			fLenP = pmi->pMasterInfo->SamplesPerSec * pmi->nOversample / ipFreqP.GetValue() / pmi->fLinPFreqMult;
		else
			fLenP = pmi->pMasterInfo->SamplesPerSec * pmi->nOversample 
														/ powf(2.0f,ipFreqP.GetValue() / 2048.0f + pmi->fLogPFreqC);
		break;

	case 2: //Note
		ipFreqP.Update(realnumsamples);
		fLenP = pmi->pMasterInfo->SamplesPerSec * pmi->nOversample / ipFreqP.GetValue() / pmi->fLinDFreqMult;
		break;

	case 1: //Period
		ipPeriodP.Update(realnumsamples);
		fLenP = pmi->pMasterInfo->SamplesPerTick * pmi->nOversample * ipPeriodP.GetValue();
		break;
	}

/*	char txt[256];
	sprintf(txt,"%f",fLenP);
	OutputDebugString(txt);
*/
	ipFreqD.Update(realnumsamples);		
	if (pmi->nFreqSliderMode == 0)
		fLenD = pmi->pMasterInfo->SamplesPerSec * pmi->nOversample / ipFreqD.GetValue() / pmi->fLinDFreqMult;		
	else
		fLenD = pmi->pMasterInfo->SamplesPerSec * pmi->nOversample 
													/ powf(2.0f,ipFreqD.GetValue() / 2048.0f + pmi->fLogDFreqC);

	ipWStart.Update(realnumsamples);	fWStart		= ipWStart.GetValue();	
	ipWEnd.Update(realnumsamples);		fWEnd		= ipWEnd.GetValue();		
	ipStochMask.Update(realnumsamples);	fStochMask	= ipStochMask.GetValue();	
	ipRandomPan.Update(realnumsamples);	fRandomPan	= ipRandomPan.GetValue();	
	ipVolume.Update(realnumsamples);	fVolume		= ipVolume.GetValue();	
	ipPan.Update(realnumsamples);		fPan		= ipPan.GetValue();	
	ipBurst.Update(realnumsamples);		nBurst		= (int)(ipBurst.GetValue()	 + 0.5f);
	ipRest.Update(realnumsamples);		nRest		= (int)(ipRest.GetValue()	 + 0.5f);
	ipOverlap.Update(realnumsamples);	nOverlap	= (int)(ipOverlap.GetValue() + 0.5f);

	ipEnvParam.Update(realnumsamples);
	if (fEnvParam != ipEnvParam.GetValue())
	{
		fEnvParam = ipEnvParam.GetValue();
		RecalcEnvTable();
	}

	ipLFO1Amount.Update(realnumsamples);
	ipLFO2Amount.Update(realnumsamples);
	if (!pmi->bLFOAmtExpInvert)
	{
		lfo1.fAmount = powf(ipLFO1Amount.GetValue(), pmi->fLFOAmtExp);
		lfo2.fAmount = powf(ipLFO2Amount.GetValue(), pmi->fLFOAmtExp);
	}
	else
	{
		lfo1.fAmount = 1.0f - powf(1.0f - ipLFO1Amount.GetValue(), pmi->fLFOAmtExp);
		lfo2.fAmount = 1.0f - powf(1.0f - ipLFO2Amount.GetValue(), pmi->fLFOAmtExp);
	}

	ipLFO1Period.Update(realnumsamples);	float period = ipLFO1Period.GetValue() * pmi->pMasterInfo->SamplesPerTick;
	if (period < pmi->fMinLFOPeriod) pmi->fMinLFOPeriod = period;			lfo1.SetPeriod(period);

	ipLFO2Period.Update(realnumsamples);		  period = ipLFO2Period.GetValue() * pmi->pMasterInfo->SamplesPerTick;
	if (period < pmi->fMinLFOPeriod) pmi->fMinLFOPeriod = period;			lfo2.SetPeriod(period);

	if (bOn)
	{
		bResult = false;

#		ifndef PULSARFX
			if (nWave <= 200)
			{
				const CWaveLevel *pLev = pmi->pCB->GetWaveLevel(nWave,0);

				if (!pLev) return false;
				if (!pLev->pSamples) return false;

				stereo = (pmi->pCB->GetWave(nWave)->Flags & WF_STEREO) != 0;
				srcSamples = pLev->pSamples;
				srcLen = pLev->numSamples;
			}
			else if (nWave <= 206)
			{
				stereo = false;
				srcSamples = (short *)pmi->pCB->GetOscillatorTable(nWave - 201);
				srcLen = 2048;
			}
			else if (nWave < 207 + NUM_INTERNAL_WAVES)
			{
				stereo = g_bWaveStereo[nWave - 207];
				srcSamples = g_pWaveData[nWave - 207];
				srcLen = g_pWaveLen[nWave - 207];
			}
			else
				return false;
#		else
//			if (!pfPrBuffer[0]) return false;
			stereo = true;
			srcSamples = pfPrBuffer[nRecentPulsaret];
			srcLen = nPrBufferLength[nRecentPulsaret];
			if (bHold)
			{
				srcSamples = pfPrBuffer[nHoldPulsaret];
				srcLen = nPrBufferLength[nHoldPulsaret];
			}
#		endif

		if (pOscDlg && bUpdateOscDlg && srcSamples)
		{
			UpdateOscDlg(fLenP,fLenD,fWStart,fWEnd,srcSamples,srcLen,stereo);
		}

//		i = fLenP*(1-ph)

		for (int i=0; i<numsamples;)
		{
			int step = (int)(fLenP * (1.0f - fOverallPhase)) + 1;
			int n = i+step; if (n > numsamples) n = numsamples;

#			ifdef PULSARFX
				for (int k = i >> pmi->nOversampExp; k < n >> pmi->nOversampExp; k++)
				{
					if (mode & WM_READ)
					{
						pfInBuffer[nInBufferHead++] = *pinput++;
						pfInBuffer[nInBufferHead++] = *pinput++;
					}
					else
					{
						pfInBuffer[nInBufferHead++] = 0.0f;
						pfInBuffer[nInBufferHead++] = 0.0f;
					}
					if (nInBufferHead > pmi->aval.InBufSize*2)
					{
						nInBufferHead = 0;
						bInBufferOverflow = true;
					}
				}
#			endif

			for (int j=0; j<MAX_OVERLAP; j++)
			{
				if (bPulsaretPlaying[j])
				{
	/*#				ifdef PULSARFX
						if (!bHold)
						{
							float *srcSamples = pfPrBuffer[j];
							int srcLen = nPrBufferLength[j];
						}
	#				endif
	*/
//					WorkPulsaret(j,i,psamples,pinput,n,mode);

					for (int k=i; k<n; k++)
					{
						bResult = true;
						float pos = fPhase[j] * fLenP / fLenD;
						if (pos >= 0.0f && pos <= 1.0f)
						{
							float l,r,e = 1.0f;
							float fEPos = pos * ENVTABLE_LENGTH;
							e = GetSampleM(fEPos+2.0f,pfEnvTable,ENVTABLE_LENGTH+4);
					
							pos = (fWStart + pos * (fWEnd - fWStart)) * (float)srcLen;
					
							if (stereo)
							{
								l = GetSampleL(pos, srcSamples, srcLen) * e;
								r = GetSampleR(pos, srcSamples, srcLen) * e;
							}
							else
								l = r = GetSampleM(pos, srcSamples, srcLen) * e;
					
							psamples[k*2  ] += l * fVolume * (2.0f - fPan) * (2.0f - fPulsaretPan[j]);
							psamples[k*2+1] += r * fVolume * fPan          * fPulsaretPan[j];
						}
						else
						{
							bPulsaretPlaying[j] = false;
							break;
						}
						
						fPhase[j] += 1.0f / fLenP;
					}				
				}
			}

			fOverallPhase += (float)(n-i) / fLenP;
			i = n;

			while (fOverallPhase >= 1.0f)
			{
				fOverallPhase -= 1.0f;
				int p = -1;
				for (int k=0; k<nOverlap && p == -1; k++)
					if (!bPulsaretPlaying[k]) p = k;
				if (p == -1) p = 0;
				nRecentPulsaret = p;
				
#				ifdef PULSARFX
					if (!bHold)
					{
						float *tmp = pfInBuffer;
						pfInBuffer = pfPrBuffer[p];
						pfPrBuffer[p] = tmp;
						nPrBufferLength[p] = nInBufferHead/2;
						srcSamples = pfPrBuffer[p];
						srcLen = nPrBufferLength[p];
					}
					bInBufferOverflow = false;
					nInBufferHead = 0;
#				endif

				if (nBurstCounter < nBurst && frand() < fStochMask)
				{
					bPulsaretPlaying[p] = true;
					fPhase[p] = fOverallPhase;
					fPulsaretPan[p] = 1.0f - fRandomPan + frand()*fRandomPan*2.0f;
//					WorkPulsaret(p,i,psamples,pinput,numsamples,mode);
				}

				nBurstCounter++;
				if (nBurstCounter >= nBurst + nRest) nBurstCounter = 0;
			}
		}

/*		//already playing pulsarets
		for (int j=0; j<MAX_OVERLAP; j++)
		{
			if (bPulsaretPlaying[j])
			{
/*#				ifdef PULSARFX
					if (!bHold)
					{
						float *srcSamples = pfPrBuffer[j];
						int srcLen = nPrBufferLength[j];
					}
#				endif
*//*
				WorkPulsaret(j,0,psamples,pinput,numsamples,mode);
			}
		}

		//new pulsarets
		for (int i=0; i<numsamples; i++)
		{
			fOverallPhase += 1.0f / fLenP;
			
			while (fOverallPhase >= 1.0f)
			{
				fOverallPhase -= 1.0f;
				int p = -1;
				for (int k=0; k<nOverlap && p == -1; k++)
					if (!bPulsaretPlaying[k]) p = k;
				if (p == -1) p = 0;
				nRecentPulsaret = p;
				
#				ifdef PULSARFX
					if (!bHold)
					{
						float *tmp = pfInBuffer;
						pfInBuffer = pfPrBuffer[p];
						pfPrBuffer[p] = tmp;
						nPrBufferLength[p] = nInBufferHead/2;
						srcSamples = pfPrBuffer[p];
						srcLen = nPrBufferLength[p];
					}
					bInBufferOverflow = false;
					nInBufferHead = 0;
#				endif

				if (nBurstCounter < nBurst && frand() < fStochMask)
				{
					bPulsaretPlaying[p] = true;
					fPhase[p] = fOverallPhase;
					fPulsaretPan[p] = 1.0f - fRandomPan + frand()*fRandomPan*2.0f;
					WorkPulsaret(p,i,psamples,pinput,numsamples,mode);
				}

				nBurstCounter++;
				if (nBurstCounter >= nBurst + nRest) nBurstCounter = 0;
			}
		}
*/
		return bResult;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void mi::HalfBuffer(float *buf, int nLen, int nFilter)
{
/*	hbFilter[nFilter].process(buf,nLen);

	float *f1 = buf;
	float *f2 = buf;

	for (int i=0; i<nLen; i+=2)
	{
		*f1++ = *f2++;
		*f1++ = *f2++;
		f2+=2;
	}*/
}

bool mi::MDKWorkStereo(float *psamples, int numsamples, int const mode)
{
/*	char txt[256];
	sprintf(txt,"WorkStereo %i %i",numsamples,mode);
	OutputDebugString(txt);
*/	static float buf[MAX_OVERSAMPLE * MAX_BUFFER_LENGTH * 2];
	float *f = buf;
	int i;
	for (i=0; i<nOversample*numsamples*2; i++) *f++ = 0.0f;

	bool res = false;

	fMinLFOPeriod = 100000.0f;

	int n = numsamples*nOversample;
	f = buf;
	float *pin = psamples;
	while (n)
	{
		int m = min(n, nMaxWorkBlockSize);
		if (m)
		{
			for (int t=0; t<numTracks; t++)
				res = tracks[t].Work(f, pin, m, mode) || res;
		}
		n -= m;
		f += m*2;
		pin += (m >> nOversampExp)*2;
	}

	int minlfo = (int)fMinLFOPeriod / 8;
	if (minlfo < 1) minlfo = 1;
	if (minlfo < nMaxWorkBlockSize) nMaxWorkBlockSize = minlfo;
	else if (nMaxWorkBlockSize < 256*nOversample)
	{
		nMaxWorkBlockSize += minlfo;
		nMaxWorkBlockSize /= 2;
	}
	if (nMaxWorkBlockSize > 256*nOversample) nMaxWorkBlockSize = 256*nOversample;

/*	char txt[256];
	sprintf(txt,"%i\n",nMaxWorkBlockSize);
	OutputDebugString(txt);
*/
//	while (minlfo < nMaxWorkBlockSize && nMaxWorkBlockSize >= nOversample) nMaxWorkBlockSize /= 2;

	if (!res && !bRemoveDC) return false;

	int j = numsamples * nOversample;
	for (i=0; i<nOversampExp; i++)
	{
		hbFilter[i].process(buf,j,1 << i);
		j /= 2;
	}

	float *f1 = buf;
	float *f2 = buf;

	for (i=0; i<numsamples; i++)
	{
		*f1++ = *f2++;
		*f1++ = *f2++;
		f2+=nOversample*2-2;
	}
	
#	ifdef PULSARFX
		if (!(mode & WM_READ)) res = false;
#	endif

	f = buf;
	if (bRemoveDC)
	{
		for (i=0; i<numsamples*2; i++)
		{
			dcout = *f - dcin + (0.99f*dcout);
			dcin = *f++;
			*psamples++ = dcout;
			res = res || (dcout > 0.0000001f) || (dcout < -0.0000001f);
		}
//		if (dcin  > -1e-10f && dcin  < 1e-10f) dcin  = 0.0f;
//		if (dcout > -1e-10f && dcout < 1e-10f) dcout = 0.0f;
	}
	else
	{
#		ifdef PULSARFX
			res = res || (*f > 0.0000001f) || (*f < -0.0000001f);
#		endif
		for (i=0; i<numsamples*2; i++) *psamples++ = *f++;
	}

	return res;
}
