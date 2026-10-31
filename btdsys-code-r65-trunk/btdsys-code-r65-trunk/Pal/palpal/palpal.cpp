/*
palpal.cpp: the one and only source file for this PAL add-on DLL
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

#include "../palpal_data.h"

#define DE(x) extern "C" __declspec(dllexport) x __cdecl

char *supported_peers[] = {
	"BTDSys Peer LFO",
	"BTDSys PeerCtrl",
	"BTDSys PeerChord",
	"BTDSys PeerChord Lite",
	"LdC and BTDSys PeerTrigger",
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

bool GetAssigns_PeerCtrl(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa);
bool GetAssigns_PeerLFO(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa);
bool GetAssigns_PeerChord(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa);
bool GetAssigns_PeerTrigger(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa);

DE(PalPalAssign *) GetAssigns(char *macname, CMachineInterface *pmi)
{
	static PalPalAssign ppa[10000];

	try
	{
		PALDataOut d;
		pmi->Save(&d);
		d.Reset();

		if (strcmp(macname,"BTDSys PeerCtrl") == 0)
		{
			if (!GetAssigns_PeerCtrl(&d,pmi,ppa)) return NULL;
		}
		else if (strcmp(macname,"BTDSys Peer LFO") == 0)
		{
			if (!GetAssigns_PeerLFO(&d,pmi,ppa)) return NULL;
		}
		else if (strcmp(macname,"BTDSys PeerChord") == 0 || strcmp(macname,"BTDSys PeerChord Lite") == 0)
		{
			if (!GetAssigns_PeerChord(&d,pmi,ppa)) return NULL;
		}
		else if (strcmp(macname,"LdC and BTDSys PeerTrigger") == 0)
		{
			if (!GetAssigns_PeerTrigger(&d,pmi,ppa)) return NULL;
		}

		else
			return NULL;

		return ppa;
	}
	catch (...) {MessageBox(NULL,"palpal error","",MB_OK);}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////

bool GetAssigns_PeerTrigger(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa)
{
	pd->GetChar(); //fun fact: MDK machines have an extra version byte
	char version = pd->GetChar();
	if (version == 11)
	{
		for (int t=0; t<254; t++)
		{
			if (!pd->GetPeerLibData(&ppa[t],pmi->pCB)) return false;
			sprintf(ppa[t].comment,"Track %i",t);
		}

		ppa[254].targetmac = (CMachine *)-1;

		return true;
	}

	return false;
}


bool GetAssigns_PeerCtrl(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa)
{
	pd->GetChar(); //fun fact: MDK machines have an extra version byte
	char version = pd->GetChar();
	if (version == 4) //version
	{
		int i = 0;
		for (int t=0; t<255; t++)
		{
			int nassigns = pd->GetInt();

			for (int a=0; a<nassigns; a++)
			{
				if (!pd->GetPeerLibData(&ppa[i],pmi->pCB)) return false;
				if (ppa[i].targetmac)
				{
					sprintf(ppa[i].comment,"Track %i",t);
					ppa[i].targettrack = pd->GetInt();
					i++;
				}
				else
					pd->GetInt();

				//skip mapping data
				int n = pd->GetInt();
				pd->Skip(n*2*sizeof(long));
			}

			//skip midi data
			pd->Skip(15);
		}
		ppa[i].targetmac = (CMachine *) -1;


		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////

bool GetAssigns_PeerLFO(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa)
{
	pd->GetChar(); //fun fact: MDK machines have an extra version byte

	char version = pd->GetChar();
	if (version == 2) //version
	{
		unsigned char numTracks = pd->GetChar();
		for (int t=0; t<numTracks; t++)
		{
			if (pd->GetChar())
			{
				char buf[256];
				pd->GetASCIIZ(buf);
				ppa[t].targetmac = pmi->pCB->GetMachine(buf);
				ppa[t].targetparam = pd->GetInt();
				ppa[t].targetgroup = pd->GetInt();
				sprintf(ppa[t].comment,"Track %i",t);
			}
			else
				ppa[t].targetmac = NULL;
		}
		ppa[numTracks].targetmac = (CMachine *) -1;

		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////////

bool GetAssigns_PeerChord(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa)
{
	pd->GetChar(); //fun fact: MDK machines have an extra version byte

	char version = pd->GetChar();
	if (version == 1) //version
	{
		unsigned char numTracks = pd->GetChar();
		for (int t=0; t<numTracks; t++)
		{
			if (pd->GetChar())
			{
				char buf[256];
				pd->GetASCIIZ(buf);
				ppa[t*2].targetmac = ppa[t*2+1].targetmac = pmi->pCB->GetMachine(buf);
				if (pd->GetChar())
				{
					ppa[t*2].targetgroup = 2;
					ppa[t*2].targetparam = pd->GetInt();
					sprintf(ppa[t*2].comment,"Track %i note",t);
				}
				else
					ppa[t*2  ].targetmac = NULL;

				if (pd->GetChar())
				{
					ppa[t*2+1].targetgroup = 2;
					ppa[t*2+1].targetparam = pd->GetInt();
					sprintf(ppa[t*2+1].comment,"Track %i vel",t);
				}
				else
					ppa[t*2+1].targetmac = NULL;
			}
			else
			{
				ppa[t*2  ].targetmac = NULL;
				ppa[t*2+1].targetmac = NULL;
			}
		}
		ppa[numTracks*2].targetmac = (CMachine *) -1;

		return true;
	}

	return false;
}

