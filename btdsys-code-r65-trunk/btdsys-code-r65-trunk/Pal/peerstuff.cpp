/*
peerstuff.cpp: Routines for interacting with peer machines
Copyright (c) 2005, Edward Powley
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
#include "BTDSys PAL.h"
#include "../mdk/machineinterface.h"
#include "CMachine.h"
#include "peerstuff_macs.h"
#include "palpal.h"
#include "palpalfuncs.h"

bool CBTDSysPALApp::IsMachineAPeer(CMachine *mac)
{
	try
	{
		const char *c = mac->pmi->DescribeValue(0xB7D0,0);
		if (c) if (strcmp(c,"hi pal") == 0) return true;
	}
	catch (...) {}

	const CMachineInfo *minfo = pCB->GetMachineInfo(mac);

	for (SupportedMachineRecord *p = vSupportedMacs.begin(); p < vSupportedMacs.end(); p++)
	{
		if (strcmp(p->name,minfo->Name) == 0)
			return true;
	}

	return false;

}


void CBTDSysPALApp::GetPeerAssigns(CMachine *mac, vector<PalPalAssign> *ass)
{
	ass->clear();

	const CMachineInfo *minfo = pCB->GetMachineInfo(mac);

	try
	{
		const char *c = mac->pmi->DescribeValue(0xB7D0,0);
		if (c)
		{
			if (strcmp(c,"hi pal") == 0)
			{
				PalPalAssign *p = (PalPalAssign *)mac->pmi->DescribeValue(0xB7D1,0);
				if (p)
				{
					for (PalPalAssign *q = p; q->targetmac != (CMachine *) -1; q++)
					{
						ass->push_back(*q);
					}
				}
			}

			return;
		}
	}
	catch (...)
	{
		MessageBox(NULL,pCB->GetMachineName(mac),"Failure: peerstuff.cpp line 61",MB_OK);
	}

	for (SupportedMachineRecord *p = vSupportedMacs.begin(); p < vSupportedMacs.end(); p++)
	{
		if (strcmp(p->name,minfo->Name) == 0)
		{
			HMODULE hm = LoadLibrary(p->dllname);
			if (!hm) {OutputDebugString("no hm"); continue;}
			PFGetAssigns pf = (PFGetAssigns)GetProcAddress(hm,"GetAssigns");
			if (!pf) {OutputDebugString("no pf"); continue;}
			try
			{
				PalPalAssign *p = pf(minfo->Name,mac->pmi);
				if (p)
				{
					for (PalPalAssign *q = p; q->targetmac != (CMachine *) -1; q++)
					{
						ass->push_back(*q);
					}
				}
			}
			catch (...)
			{
				MessageBox(NULL,pCB->GetMachineName(mac),"Failure: peerstuff.cpp line 85",MB_OK);
			}

			FreeLibrary(hm);
		}
	}
}
