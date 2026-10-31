/*
Copyright (c) 2003-2006, Edward Powley
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
#include "BuzzMI.h"
#include "DSPStuff.h"

#include "groupie.h"

bool CMachine::Work(int numsamples, const int mode)
{
	float monobuf[256];
	bool res;

	if (!pMDKMI)
	{
		DSP_CopyS2M(monobuf,buf,numsamples);

		if (bStereoOut)
		{
			try { res = pMI->WorkMonoToStereo(monobuf,buf,numsamples,mode); }
			catch (...) { LogException("WorkMonoToStereo",__FILE__,__LINE__); }
		}
		else
		{
			try { res = pMI->Work(monobuf,numsamples,mode); }
			catch (...) { LogException("Work",__FILE__,__LINE__); }
			DSP_CopyM2S(buf,monobuf,numsamples);
		}
	}
	else
	{
		if (bStereoOut)
		{
			try { res = pMDKMI->MDKWorkStereo(buf,numsamples,mode); }
			catch (...) { LogException("MDKWorkStereo",__FILE__,__LINE__); }
		}
		else
		{
			DSP_CopyS2M(monobuf,buf,numsamples);
			try { res = pMDKMI->MDKWork(monobuf,numsamples,mode); }
			catch (...) { LogException("MDKWork",__FILE__,__LINE__); }
			DSP_CopyM2S(buf,monobuf,numsamples);
		}
	}

	return res;
}

bool mi::MixInput(float *psamples, CMachine *pm, int numsamples, const float *inbuf, const int mode)
{
	bool bRes = false;

	DSP_Zero(psamples,numsamples*2);

	for (vector<CConnection>::iterator p=vConns.begin(); p<vConns.end(); p++)
	{
		if (p->pMac2 == pm)
		{
			if (!p->pMac1->bWorked)
			{
				p->pMac1->bWorkResult = WorkThisMachine(p->pMac1, inbuf, numsamples, mode);
				p->pMac1->bWorked = true;
			}
			if (p->pMac1->bWorkResult)
			{
				bRes = true;
//				DSP_Add(psamples,p->pMac1->buf,numsamples*2);
				for (int i=0; i<numsamples*2; i+=2)
				{
					psamples[i  ] += p->pMac1->buf[i  ] * p->fAmp * (2.0f - p->fPan);
					psamples[i+1] += p->pMac1->buf[i+1] * p->fAmp * p->fPan;
				}
			}

/*			if (p->pMac2->pEx)
				p->pMac2->pEx->Input(p->pMac1->buf,
						p->pMac1->bWorkResult ? numsamples : 0,
						p->fAmp);
*/		}
	}

	return bRes;
}

bool mi::WorkThisMachine(CMachine *pm, float const *inbuf, int numsamples, const int mode)
{
	if (pm->bSpecial)
	{
		if (pm->IsInputMachine())
		{
			if (mode & WM_READ)
			{
				DSP_Copy(pm->buf, inbuf, numsamples*2);
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (pm->IsOutputMachine())
		{
			return MixInput(pm->buf, pm, numsamples, inbuf, mode);
		}
		else if (pm->IsThruMachine())
		{
			return MixInput(pm->buf, pm, numsamples, inbuf, mode);
		}
		else
			return false;
	}
	else
	{
		if (MixInput(pm->buf, pm, numsamples, inbuf, mode))
			return pm->Work(numsamples,WM_READWRITE);
		else
			return pm->Work(numsamples,WM_WRITE);
	}
}

