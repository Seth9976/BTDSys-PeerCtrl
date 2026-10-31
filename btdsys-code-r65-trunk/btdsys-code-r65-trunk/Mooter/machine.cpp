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

/********************************************************
**                B T D   S Y S T E M S                **
**                =====================                **
** Mooter										       **
** ©2005 Edward Powley                                 **
********************************************************/

#include "stdafx.h"
#include "../mdk/mdk.h"
#include "../mdk/CMachine.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

#include "machine.h"
#include "selectmachinedlg.h"

#pragma optimize ("awy", on)

////////////////////////////////////////////////////////////////////////

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////


HWND FindBUZZhwnd()
{
	static HWND hBuzz = NULL;

	if (hBuzz) return hBuzz;

	char t[1024];
	HWND wnd = GetActiveWindow();
//	while (GetParent(wnd)) wnd = GetParent(wnd);
	wnd = GetTopWindow(NULL);

	if(wnd == NULL)
		return NULL;

	GetWindowText(wnd, t, 1000);
	while(strstr(t, "Buzz") == NULL && wnd != NULL)
	{
		wnd = GetWindow(wnd, GW_HWNDNEXT);
		if(wnd != NULL)
			GetWindowText(wnd, t, 1000);
	}

	if(strstr(t, "Buzz") != NULL)
	{
		hBuzz = wnd;
		return wnd;
	}

	GetWindowText(wnd, t, 1000);
	while(strstr(t, "Buzz") == NULL && wnd != NULL)
	{
		wnd = GetWindow(wnd, GW_HWNDPREV);
		if(wnd != NULL)
			GetWindowText(wnd, t, 1000);
	}

	if(strstr(t, "Buzz") != NULL)
	{
		hBuzz = wnd;
		return wnd;
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////

mi::mi() 
//Constructor
{ 
	ex.pmi = this;

	GlobalVals = &gval;
	TrackVals = &tval;
	AttrVals = (int*) &aval;
}

mi::~mi() 
//Destructor
{
}

Track::Track()
{
	mac = NULL;
	macname[0] = 0;
}

////////////////////////////////////////////////////////////////////////

bool mi::DblClickEvent(void *p)
{
	return false;
}

void ReadASCIIZ(CMachineDataInput * const pi, char *string)
{
	char c;
	do
	{
		pi->Read(c);
		*string++ = c;
	}
	while (c);
}

void WriteASCIIZ(CMachineDataOutput * const po, char *string)
{
	while (*string) po->Write(*string++);
	po->Write((byte)0);
}


void mi::MDKInit (CMachineDataInput * const pi)
//Initialise machine and load data from file
{
	RedrawMachineView();

	SetOutputMode(true);

	pCB->SetEventHandler(pCB->GetThisMachine(),
						DoubleClickMachine, 
						(EVENT_HANDLER_PTR)DblClickEvent, NULL);

	thismac = pCB->GetThisMachine();

	for (int t=0; t<200; t++)
	{
		tracks[t].pmi = this;
		tracks[t].pCB = pCB;
	}

	if (pi)
	{
		byte ver;
		pi->Read(ver);
		if (ver == 10)
		{
			for (int t=0; t<200; t++)
			{
				ReadASCIIZ(pi,tracks[t].macname);
			}
		}
	}

}

////////////////////////////////////////////////////////////////////////

void mi::MDKSave (CMachineDataOutput * const po)
//Save data to file
{
	po->Write((byte)10);

	for (int t=0; t<200; t++)
	{
		tracks[t].CheckMachine();
		WriteASCIIZ(po,tracks[t].macname);
	}
}

////////////////////////////////////////////////////////////////////////

bool mi::MDKWorkStereo(float *psamples, int numsamples, const int mode)
{
	return false;
}

////////////////////////////////////////////////////////////////////////

void Track::CheckMachine()
{
	if (mac) //Got an assignment
	{
		macname[0] = 0;
		const char *newname = pCB->GetMachineName(mac);
		if (!newname) //Machine disappeared?
		{
			mac = NULL;
		}
		else
		{
			strcpy(macname,newname); //Handles renaming
			if (pCB->GetMachine(macname) != mac) //Machine disappeared?
				mac = NULL;
		}
	}
	else //No assignment
	{
		if (macname[0]) //But we want an assignment
			mac = pCB->GetMachine(macname); //This works, whether or not the machine is found
	}
}

void mi::Tick() 
//Machine-level tick stuff
{
	for (int t=0; t<numTracks; t++)
	{
		tracks[t].CheckMachine();

		if (tracks[t].mac)
		{
			if (tval[t].Mute != paraMute.NoValue)
			{
				tracks[t].lastmute = tracks[t].mac->mute = tval[t].Mute;
				RedrawMachineView();
			}
			else if (tracks[t].mac->mute != tracks[t].lastmute)
				pCB->ControlChange(thismac,2,t,numMute - NUMG, tracks[t].mac->mute);

			if (tval[t].PatternState != paraPatternState.NoValue)
			{
				tracks[t].lastthru = tracks[t].mac->mutethru = tval[t].PatternState;
				RedrawMachineView();
			}
			else if (tracks[t].mac->mutethru != tracks[t].lastthru)
				pCB->ControlChange(thismac,2,t,numPatternState - NUMG, tracks[t].mac->mutethru);

		}
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Stop()
//Stop the machine (if we have to) when the Stop button is clicked
{
}

////////////////////////////////////////////////////////////////////////

void miex::GetSubMenu(const int i, CMachineDataOutput *pout)
{
	char txt[128];
	for (int t=0; t<pmi->numTracks; t++)
	{
		sprintf(txt,"%i: %s",t,pmi->tracks[t].mac ? pmi->tracks[t].macname : "No assign");
		pout->Write(txt);
	}
		
}

void mi::Command(const int i) 
//Handler for context menu items
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (i == 1)
		MessageBox(NULL,"About",NULL,MB_OK);
	else
	{
		int t = i & 255;

		CSelectMachineDlg dlg;
		dlg.pmi = this;
		dlg.ptr = &tracks[t];
		dlg.ntr = t;

		dlg.DoModal();

		tracks[t].lastmute = tracks[t].lastthru = 69;
	}
}

void mi::RedrawMachineView()
{
	OutputDebugString("Redrawing");

	static HWND h = NULL;
	
	if (!h)
	{
		HWND hBuzz = FindBUZZhwnd();
		if (hBuzz) h = FindWindowEx(hBuzz,0,"MDIClient",""); else OutputDebugString("Shite 1");
		if (h) h = FindWindowEx(h,0,0,"Machines"); else OutputDebugString("Shite 2");
		if (h) h = FindWindowEx(h,0,"AfxFrameOrView42s",""); else OutputDebugString("Shite 3");
	}

	if (h)
		RedrawWindow(h,0,0,RDW_INVALIDATE | RDW_ERASE);
	else
		OutputDebugString("No h");
}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
//Describe value!
{
	switch (param)
	{
	case numMute:
		switch (value)
		{
		case 0: return "Unmute";
		case 1: return "Mute";
		default: return "WTF?";
		}

	case numPatternState:
		switch (value)
		{
		case 0: return "---";
		case 1: return "<mute>";
		case 2: return "<thru>";
		default: return "WTF?";
		}
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////

#pragma optimize ("", on)

