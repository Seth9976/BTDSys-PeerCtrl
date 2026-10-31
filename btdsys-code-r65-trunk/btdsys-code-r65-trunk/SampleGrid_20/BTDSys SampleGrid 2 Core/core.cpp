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

#include "stdafx.h"

#include "core.h"
#include "sgrid.h"
#include "drumkit.h"
#include "KitInfoDlg.h"
#include "KitSaveDlg.h"
#include <vector>
using namespace std;

vector<MacInfo> g_vMacs;



DE(int) GetSGCoreVersion() {return 100;}

/*

DE(void) Reg(void *pmi, PFSGridCallback cb)
{
	MacInfo i;
	i.pmi = (CMachineInterface *)pmi;
	i.cb = cb;
	g_vMacs.push_back(i);
}

DE(void) Unreg(void *pmi)
{
	for (MacInfo *p = g_vMacs.begin(); p < g_vMacs.end(); p++)
	{
		if (p->pmi == pmi)
		{
			g_vMacs.erase(p);
			return;
		}
	}
}

DE(void) KitInfoDialog(CDrumkit *pKit, void *pmi)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CKitInfoDlg dlg;

	dlg.pmi = pmi;
	dlg.CallbackFunc = NULL;
	for (MacInfo *p = g_vMacs.begin(); p < g_vMacs.end() && dlg.CallbackFunc == NULL; p++)
	{
		if (p->pmi == pmi)
			dlg.CallbackFunc = p->cb;
	}

	dlg.pKit = pKit;
	dlg.DoModal();
}

DE(int) KitSaveModeDialog(void *pmi, int mode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CKitSaveDlg dlg;

	dlg.pmi = pmi;
	dlg.CallbackFunc = NULL;
	for (MacInfo *p = g_vMacs.begin(); p < g_vMacs.end() && dlg.CallbackFunc == NULL; p++)
	{
		if (p->pmi == pmi)
			dlg.CallbackFunc = p->cb;
	}

	dlg.nMode = mode;
	dlg.DoModal();
	return dlg.nMode;
}
*/
DE(mi *) CreateMachine(int n, char type)
{
	return new mi(n,type);
}
