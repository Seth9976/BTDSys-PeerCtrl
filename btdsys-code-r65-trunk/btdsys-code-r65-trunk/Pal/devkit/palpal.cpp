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
	static PalPalAssign ppa[1000]; // Change this to a sensible size

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
	catch (...) {}

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

