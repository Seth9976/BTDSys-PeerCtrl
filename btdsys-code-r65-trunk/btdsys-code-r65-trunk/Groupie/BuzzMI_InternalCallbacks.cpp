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

#include "groupie.h"

void CMICallbacksInternal::MessageBox(const char *txt)
{
	::MessageBox(NULL,txt,pHostMachine->dllname,MB_OK);
}

int CMICallbacksInternal::GetEnvSize(int const wave, int const env)
{
//	return 0;
	return pHostCB->GetEnvSize(wave,env);
}

bool CMICallbacksInternal::GetEnvPoint(int const wave, int const env, int const i, word &x, word &y, int &flags)
{
//	return false;
	return pHostCB->GetEnvPoint(wave,env,i,x,y,flags);
}

void CMICallbacksInternal::SetMachineInterfaceEx(CMachineInterfaceEx *pex)
{
	OutputDebugString(" SetMachineInterfaceEx");
	pHostMachine->pEx = (CMDKMachineInterfaceEx *)pex;
}

void CMICallbacksInternal::ControlChange__obsolete__(int group, int track, int param, int value)
{
	ControlChange(pHostMachine,group,track,param,value);
}

CMachine *CMICallbacksInternal::GetThisMachine()
{
	return pHostMachine;
}

void CMICallbacksInternal::ControlChange(CMachine *pmac, int group, int track, int param, int value)
{
	//implement me!
}

void CMICallbacksInternal::SetnumOutputChannels(CMachine *pmac, int n)
{
	OutputDebugString(" SetnumOutputChannels");
	pmac->bStereoOut = (n == 2);
}

void CMICallbacksInternal::SetEventHandler(CMachine *pmac, BEventType et, EVENT_HANDLER_PTR p, void *param)
{
	//implement me!
}

void CMICallbacksInternal::GetMachineNames(CMachineDataOutput *pout)
{
	for (vector<CMachine *>::iterator pp = pHost->vpMacs.begin();
		 pp < pHost->vpMacs.end(); pp++)
	{
		pout->Write((*pp)->name);
	}
}

CMachine *CMICallbacksInternal::GetMachine(char const *name)
{
	return pHost->FindMachine(name);
}

CMachineInfo const *CMICallbacksInternal::GetMachineInfo(CMachine *pmac)
{
	return pmac->pInfo;
}

char const *CMICallbacksInternal::GetMachineName(CMachine *pmac)
{
	return pmac->name;
}

bool CMICallbacksInternal::GetInput(int index, float *psamples, int numsamples, bool stereo, float *extrabuffer)
{
	OutputDebugString(" GetInput");
	//implement me!
	return false;
}

CWaveLevel const *CMICallbacksInternal::GetNearestWaveLevel(int const i, int const note)
{
	char txt[256];
	sprintf(txt,"GetNearestWaveLevel(%i,%i)",i,note);
	OutputDebugString(txt);

	if (i == -1)
	{
		if (!pHostMachine->pMDKI)
			pHostMachine->pMDKI = new CMDKInterface;

		pHostMachine->pMDKI->pHostMachine = pHostMachine;

		pHostMachine->pMDKI->pMDKInterface = pHostMachine->pMDKMI 
			= (CMDKMachineInterface *)pHostMachine->pMI;

		pHostMachine->pMDKI->pMDKInterfaceEx = pHostMachine->pMDKEx 
			= pHostMachine->pMDKMI->GetEx();

		pHostMachine->pEx = (CMachineInterfaceEx*)pHostMachine->pMDKEx;

		if (pHostMachine->fBMX) fseek(pHostMachine->fBMX,1,SEEK_CUR);

		return (CWaveLevel const *)pHostMachine->pMDKI;
	}
		
	return pHostCB->GetNearestWaveLevel(i,note);
}
