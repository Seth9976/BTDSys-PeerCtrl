/*
btdsys2.cpp: the one and only source file for this PAL add-on DLL
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

#include <stdlib.h>
#include <windows.h>
#include "../palpal.h"

#include "peerstate.h"
#include "../palpal_data.h"

#define DE(x) extern "C" __declspec(dllexport) x __cdecl

char *supported_peers[] = {
	"BTDSys PeerState",
	"Quence PyBuzz",
	NULL
};

DE(char **) GetSupportedMacs()
{
	return supported_peers;
}

DE(bool) QuerySupportPeer(char *name)
{
	for (char **pp = supported_peers; *pp; pp++)
	{
		if (strcmp(name,*pp) == 0) return true;
	}

	return false;
}

DE(PalPalAssign *) GetAssigns(char *macname, CMachineInterface *pmi_)
{
	static PalPalAssign ppa[10000];

	try
	{
		if (strcmp(macname,"BTDSys PeerState") == 0)
		{
			PeerState::mi *pmi = (PeerState::mi *)pmi_;

			int j = 0;

			for (PeerState::CMachineRec *p = pmi->peers.begin(); p < pmi->peers.end(); p++)
			{
				const CMachineInfo *minfo = pmi->pCB->GetMachineInfo(p->pMac);

				if (p->bActive)
				{
					for (int i=0; i<p->vbParamActive .size(); i++)
					{
						if (p->vbParamActive[i])
						{
							ppa[j].targetmac = p->pMac;
							ppa[j].targetgroup = (i < minfo->numGlobalParameters) ? 1 : 2;
							ppa[j].targetparam = (i < minfo->numGlobalParameters) ? i : i-minfo->numGlobalParameters;
							ppa[j].targettrack = -1;
							ppa[j].comment[0] = 0;

							j++;
						}
					}
				}
			}

			ppa[j].targetmac = (CMachine *) -1;

			return ppa;
		}
		else if (strcmp(macname,"Quence PyBuzz") == 0)
		{
			PALDataOut d;
			pmi_->Save(&d);
			d.Reset();
			int version = d.GetInt();
			if (version >= 0x100)
			{
				int s = d.GetInt(); d.Skip(s);
				if (s && version >= 0x102)
				{
					s = d.GetInt(); d.Skip(s);
				}

				for (int i=0; i<256; i++)
				{
					PalPalAssign *a = &ppa[i];
					a->targetmac = NULL;

					char peerversion = d.GetChar();
					if (peerversion == 3)
					{
						char buf[256];
						d.GetASCIIZ(buf); //name
						char haspar = d.GetChar();
						if (haspar)
						{
							d.GetASCIIZ(buf);
							a->targetmac = pmi_->pCB->GetMachine(buf);
							a->targetparam = d.GetInt();
							a->targetgroup = d.GetInt();
							a->targettrack = d.GetInt();
						}
					}
					else
						OutputDebugString("Wrong peer ver");

					sprintf(ppa[i].comment,"Peer %i",i);
				}

				ppa[256].targetmac = (CMachine *) -1;

				return ppa;
			}
			else
				OutputDebugString("Wrong pybuzz ver");
		}
	}
	catch (...) { OutputDebugString("Exception in btdsys2.pll"); }

	return NULL;
}









