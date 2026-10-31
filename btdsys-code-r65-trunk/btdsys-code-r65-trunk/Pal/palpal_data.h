/*
palpal.h: CMachineDataOutput class used by PAL add-on DLLs.
2005, Edward Powley
This file is public domain.
*/

#ifndef __BTDSYS_PALPAL_DATA_H
#define __BTDSYS_PALPAL_DATA_H

#include "../../mdk/machineinterface.h"
#include <vector>
using namespace std;

class PALDataOut : public CMachineDataOutput
{
public:
	vector<char> data;

	virtual void Write(void *pbuf, int const numbytes)
	{
		char *p = (char *)pbuf;

		data.reserve(data.size() + numbytes);
		for (int i=0; i<numbytes; i++)
		{
			data.push_back(p[i]);
		}
	}

	int datahead;

	void Reset() {datahead = 0;}

	void Skip(int i) {datahead += i;}

	char GetChar()
	{
		return data[datahead++];
	}

	int GetInt()
	{
		union {int i; char c[4];} x;
		x.c[0] = GetChar();
		x.c[1] = GetChar();
		x.c[2] = GetChar();
		x.c[3] = GetChar();
		return x.i;
	}

	void GetASCIIZ(char *c)
	{
		do
		{
			*c = GetChar();
		}
		while (*c++);
	}

	bool GetPeerLibData(PalPalAssign *a, CMICallbacks *pCB)
	{
		char version = GetChar();
		if (version == 1) //version
		{
			char haspar = GetChar();
			if (haspar == 0) a->targetmac = NULL;
			else
			{
				char buf[256];
				GetASCIIZ(buf);
				a->targetmac = pCB->GetMachine(buf);
				a->targetparam = GetInt();
				a->targetgroup = GetInt();
			}
			return true;
		}

		return false;
	}
};









#endif
