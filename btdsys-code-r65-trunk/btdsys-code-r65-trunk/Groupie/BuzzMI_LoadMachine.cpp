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

typedef CMachineInfo const * (__cdecl *tpGetInfo)();
typedef CMachineInterface * (__cdecl *tpCreateMachine)();

bool CMachine::LoadMachine(char const *dllName, mi *ph)
{
	strcpy(dllname,dllName);

	char dllPathName[256];
	strcpy(dllPathName,"Gear\\Generators\\");
	strcat(dllPathName,dllName);

	hm = LoadLibrary(dllPathName);
	bIsEffect = false;

	if (!hm)
	{
		strcpy(dllPathName,"Gear\\Effects\\");
		strcat(dllPathName,dllName);

		hm = LoadLibrary(dllPathName);
		bIsEffect = true;
	}

	if (hm)
	{
		tpGetInfo pGetInfo = (tpGetInfo)GetProcAddress(hm,"GetInfo");
		tpCreateMachine pCreateMachine = (tpCreateMachine)GetProcAddress(hm,"CreateMachine");
		if (pGetInfo && pCreateMachine)
		{
			pInfo = pGetInfo();
			pMI   = pCreateMachine();

			pCB = new CMICallbacksInternal;
			pCB->pHostMachine = this;
			pCB->pHostCB = ph->pCB;
			pCB->pHost = ph;
			pMI->pCB = pCB;
			pMI->pMasterInfo = ph->pMasterInfo;

			bNoOut = (pInfo->Flags & MIF_NO_OUTPUT) != 0;
			bStereoOut = (pInfo->Flags & MIF_MONO_TO_STEREO) != 0;

			strcpy(name, pInfo->ShortName);
			if (ph->FindMachine(name))
			{
				char tmpname[256];
				int i=2;
				sprintf(tmpname,"%s%i",name,i);
				while (ph->FindMachine(tmpname))
				{
					i++;
					sprintf(tmpname,"%s%i",name,i);
				}
				strcpy(name,tmpname);
			}

			bSpecial = false;

			return true;
		}
		else
		{
			FreeLibrary(hm);
			return false;
		}
	}
	else
		return false;

}

void CMachine::InitMachineNoFileData()
{
	if (pInfo->numGlobalParameters > 0)
	{
		pGlobalState = new word[pInfo->numGlobalParameters];
		for (int i=0; i<pInfo->numGlobalParameters; i++)
			pGlobalState[i] = pInfo->Parameters[i]->DefValue;
	}

	if (pInfo->maxTracks > 0 && pInfo->numTrackParameters > 0)
	{
		pTrackState = new word[pInfo->maxTracks*pInfo->numTrackParameters];
		for (int i=0; i<pInfo->maxTracks; i++)
		{
			for (int j=0; j<pInfo->numTrackParameters; j++)
			{
				pTrackState[i*pInfo->numTrackParameters + j]
					= pInfo->Parameters[pInfo->numGlobalParameters + j]
					->DefValue;
			}
		}
	}

	InitParamsAttrs();

	pMI->Init(NULL);
	pMI->AttributesChanged();
	nTracks = pInfo->minTracks;
	pMI->SetNumTracks(nTracks);
	if (pMDKMI)
	{
		bStereoOut = true;
		pMDKMI->OutputModeChanged(true);
	}

	try { pMI->Tick(); }
	catch (...) { LogException("Tick",__FILE__,__LINE__); }

	ClearParams();
}

void CMachine::InitMachineFromFileData(CMachineDataInput * const pi)
{
	pi->Read(name,256);
	OutputDebugString(name);
	pi->Read(fX);
	pi->Read(fY);
	pi->Read(bStereoOut);

	if (!bSpecial)
	{
		if (pInfo->numGlobalParameters > 0)
		{
			pGlobalState = new word[pInfo->numGlobalParameters];
			pi->Read(pGlobalState, pInfo->numGlobalParameters * sizeof(word));
		}

		if (pInfo->maxTracks > 0 && pInfo->numTrackParameters > 0)
		{
			pTrackState = new word[pInfo->maxTracks*pInfo->numTrackParameters];
			pi->Read(pTrackState, pInfo->maxTracks * pInfo->numTrackParameters * sizeof(word));
		}

		InitParamsAttrs();

		pi->Read(nTracks);
		pMI->Init(pi);
		pMI->AttributesChanged();
		pMI->SetNumTracks(nTracks);
		if (pMDKMI)
			pMDKMI->OutputModeChanged(bStereoOut);

		try { pMI->Tick(); }
		catch (...) { LogException("Tick",__FILE__,__LINE__); }

		ClearParams();
	}
	OutputDebugString("... done");
}

void CMachine::SaveMachineFileData(CMachineDataOutput * const po)
{
	po->Write(name,256);
	po->Write(fX);
	po->Write(fY);
	po->Write(bStereoOut);

	if (!bSpecial)
	{
		if (pGlobalState)
		{
			po->Write(pGlobalState, pInfo->numGlobalParameters * sizeof(word));
		}

		if (pTrackState)
		{
			po->Write(pTrackState, pInfo->maxTracks * pInfo->numTrackParameters * sizeof(word));
		}

		po->Write(nTracks);
		pMI->Save(po);
	}
}

void CMachine::InitParamsAttrs(bool bNoAttr)
{
	byte *b = (byte *)pMI->GlobalVals;
	for (int g=0; g<pInfo->numGlobalParameters; g++)
	{
		const CMachineParameter *par = pInfo->Parameters[g];
		if (par->Type == pt_word)
		{
			if (par->Flags & MPF_STATE)
				*(word *)b = pGlobalState[g];
			else
				*(word *)b = par->NoValue;
			b+=2;
		}
		else
		{
			if (par->Flags & MPF_STATE)
				*b = (byte)pGlobalState[g];
			else
				*b = (byte)par->NoValue;
			b++;
		}
	}

	b = (byte *)pMI->TrackVals;
	for (int t=0; t<pInfo->maxTracks; t++)
	{
		for (int u=0; u<pInfo->numTrackParameters; u++)
		{
			const CMachineParameter *par 
				= pInfo->Parameters[pInfo->numGlobalParameters+u];

			const word stateval = pTrackState[u + pInfo->numTrackParameters*t];

			if (par->Type == pt_word)
			{
				if (par->Flags & MPF_STATE)
					*(word *)b = stateval;
				else
					*(word *)b = par->NoValue;
				b+=2;
			}
			else
			{
				if (par->Flags & MPF_STATE)
					*b = (byte)stateval;
				else
					*b = (byte)par->NoValue;
				b++;
			}
		}
	}

	if (!bNoAttr)
	{
		for (int a=0; a<pInfo->numAttributes; a++)
		{
			pMI->AttrVals[a] = pInfo->Attributes[a]->DefValue;
		}
	}
}

void CMachine::ClearParams()
{
	if (!bSpecial)
	{
		byte *b = (byte *)pMI->GlobalVals;
		for (int g=0; g<pInfo->numGlobalParameters; g++)
		{
			if (pInfo->Parameters[g]->Type == pt_word)
			{
				*(word *)b = (word)pInfo->Parameters[g]->NoValue;
				b+=2;
			}
			else
			{
				*b = (byte)pInfo->Parameters[g]->NoValue;
				b++;
			}
		}

		b = (byte *)pMI->TrackVals;
		for (int t=0; t<pInfo->maxTracks; t++)
		{
			for (int u=0; u<pInfo->numTrackParameters; u++)
			{
				if (pInfo->Parameters[pInfo->numGlobalParameters+u]->Type == pt_word)
				{
					*(word *)b = (word)pInfo->Parameters[pInfo->numGlobalParameters+u]->NoValue;
					b+=2;
				}
				else
				{
					*b = (byte)pInfo->Parameters[pInfo->numGlobalParameters+u]->NoValue;
					b++;
				}
			}
		}
	}
}

