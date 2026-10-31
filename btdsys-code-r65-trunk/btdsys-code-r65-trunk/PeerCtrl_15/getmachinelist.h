/*
Copyright (c) 2002-2006, Edward Powley
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

/*

#include "stdafx.h"
#include "../mdk/mdk.h"



class CTreeDataOut : public CMachineDataOutput
{
public:
	virtual void Write(void *pbuf, int const numbytes);
	CTreeCtrl *tree;
	CMICallbacks *pCB;
};

void CTreeDataOut::Write(void *pbuf, const int numbytes)
{
	HTREEITEM hmac = tree->InsertItem((char *)pbuf, 4, 5);

	CMachineInfo const *minfo = pCB->GetMachineInfo(pCB->GetMachine((char *)pbuf));

	int p;

	if (minfo->numGlobalParameters > 0)
	{
		HTREEITEM hg = tree->InsertItem("Global Params", 4, 5, hmac);
		for (p=0; p<minfo->numGlobalParameters; p++)
		{
			int i;
			switch (minfo->Parameters[p]->Type)
			{
			case pt_switch:	i=0; break;
			case pt_byte:	i=1; break;
			case pt_note:	i=2; break;
			case pt_word:	i=3; break;
			default: i=0;
			}
			tree->InsertItem(minfo->Parameters[p]->Name, i, i, hg);
		}
	}

	if (minfo->numTrackParameters > 0)
	{
		HTREEITEM ht = tree->InsertItem("Track Params", 4, 5, hmac);
		for (p=0; p<minfo->numTrackParameters; p++)
		{
			int i;
			switch (minfo->Parameters[p+minfo->numGlobalParameters]->Type)
			{
			case pt_switch:	i=0; break;
			case pt_byte:	i=1; break;
			case pt_note:	i=2; break;
			case pt_word:	i=3; break;
			default: i=0;
			}
			tree->InsertItem(minfo->Parameters[p+minfo->numGlobalParameters]->Name, i,i,ht);
		}
	}
}

void GetMachineTree (CTreeCtrl *tree, CMICallbacks *pCB)
{
	CTreeDataOut dout;

	dout.pCB = pCB;
	dout.tree = tree;

	pCB->GetMachineNames(&dout);
}
*/