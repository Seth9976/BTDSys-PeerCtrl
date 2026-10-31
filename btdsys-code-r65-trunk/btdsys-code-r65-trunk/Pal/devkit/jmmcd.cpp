#include <stdlib.h>
#include <windows.h>
#include "../palpal.h"

#include "../palpal_data.h"

#define DE(x) extern "C" __declspec(dllexport) x __cdecl

// Fill in the name(s) of your machine(s) here
// They must be in exactly the same form as the "Name" field of CMachineInfo
char *supported_peers[] = {
	"jmmcd Peer Note-Pool",
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

bool GetAssigns_NotePool(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa);
bool GetAssigns_MetaI(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa);

DE(PalPalAssign *) GetAssigns(char *macname, CMachineInterface *pmi)
{
	static PalPalAssign ppa[500]; // Change this to a sensible size

	try // Make sure exceptions don't bring everything down
	{
		PALDataOut d;
		pmi->Save(&d); // Save your machine's data into memory
		d.Reset(); // Get the data ready for reading

		if (strcmp(macname,"jmmcd Peer Note-Pool") == 0)
		{
			if (!GetAssigns_NotePool(&d,pmi,ppa)) return NULL;
		}
		// If you are supporting more than one machine, add them as "else if"s here

		else
			return NULL;

		return ppa;
	}
	catch (...) {}

	return NULL;
}

/////////////////////////////////////////////////////////////////////////

bool GetAssigns_NotePool(PALDataOut *pd, CMachineInterface *pmi, PalPalAssign *ppa)
{
	pd->GetChar(); //fun fact: MDK machines have an extra version byte

	for (int t=0; t<128; t++) //NotePool saves data for 128 tracks
	{
		// GetPeerLibData fills a PalPalAssign based on the data saved by the peer lib
		if (!pd->GetPeerLibData(&ppa[t*2], pmi->pCB)) return false;

		// The "comment" can be any text you think will be useful to the user
		sprintf(ppa[t*2].comment,"Track %i Note",t);

		// More of the same
		if (!pd->GetPeerLibData(&ppa[t*2+1], pmi->pCB)) return false;
		sprintf(ppa[t*2+1].comment,"Track %i Vel",t);
	}

	// Don't forget to terminate the end of the array!
	ppa[256].targetmac = (CMachine *)-1;

	return true;
}

