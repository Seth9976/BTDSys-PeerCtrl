/********************************************************
**                B T D   S Y S T E M S                **
**                =====================                **
** MidiJumpHACK									       **
** ©2004 Edward Powley                                 **
********************************************************/

#include "stdafx.h"
#include "../mdk/mdk.h"
#include <float.h>
#include <stdlib.h>

#include "midijump.h"
#include "buzzhack.h"
#include "settingsdlg.h"

#pragma optimize ("awy", on)

////////////////////////////////////////////////////////////////////////

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////

mi::mi() 
//Constructor
{ 
	ex.pmi = this;

	GlobalVals = &gval;
	AttrVals = (int*) &aval;

	pdlg = NULL;
	pKeyCapDlg = NULL;
}

mi::~mi() 
//Destructor
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (pKeyCapDlg) delete pKeyCapDlg;//->DestroyWindow();
}

////////////////////////////////////////////////////////////////////////

void mi::MDKInit (CMachineDataInput * const pi)
//Initialise machine and load data from file
{
	SetOutputMode(false);

	if (pCB->GetNearestWaveLevel(-2, -2) != 0 && pCB->GetHostVersion() >= 2)
	{
		bHackEnabled = true;
		bUseNewCallbacks = true;
	}
	else
	{
		bHackEnabled = (buzzhack_Initbuzzhack() != FALSE);
		bUseNewCallbacks = false;
	}

	if (pi)
	{
		byte version;
		pi->Read(version);
		if (version == 4)
		{
			int n; pi->Read(n);
			for (int i=0; i<n; i++)
			{
				JumpTickRec jtr;
				int ttype; pi->Read(ttype); jtr.eTrigType = (JumpTriggerType)ttype;
				pi->Read(jtr.nTrig);
				pi->Read(jtr.nTick);
				pi->Read(jtr.nJumpMode);
				vJumpTicks.push_back(jtr);
			}
		}
	}

	nNextDest = -1;
}

////////////////////////////////////////////////////////////////////////

void mi::MDKSave (CMachineDataOutput * const po)
//Save data to file
{
	po->Write((byte)4); //version
	po->Write((int)vJumpTicks.size());
	for (JumpTickRec *p = vJumpTicks.begin(); p<vJumpTicks.end(); p++)
	{
		po->Write((int)p->eTrigType);
		po->Write(p->nTrig);
		po->Write(p->nTick);
		po->Write(p->nJumpMode);
	}
}

////////////////////////////////////////////////////////////////////////

int mi::getSongPosition()
{
	if (bUseNewCallbacks)
		return pCB->GetSongPosition();
	else
		return buzzhack_GetSongPosition();
}

void mi::setSongPosition(int pos)
{
	if (bUseNewCallbacks)
		pCB->SetSongPosition(pos);
	else
		buzzhack_SetSongPosition(pos);
}

////////////////////////////////////////////////////////////////////////

bool mi::MDKWork(float *psamples, int numsamples, const int mode)
{
	return false;
}

////////////////////////////////////////////////////////////////////////

JumpTickRec *mi::FindTickRec(JumpTriggerType ttype, int trig)
{
	for (JumpTickRec *p = vJumpTicks.begin(); p<vJumpTicks.end(); p++)
	{
		if (p->eTrigType == ttype && p->nTrig == trig)
			return p;
	}

	return NULL;
}

void mi::Tick() 
//Machine-level tick stuff
{
	if (gval.OnOff != paraOnOff.NoValue)
		bOn = (gval.OnOff == SWITCH_ON);

	if (gval.TickSnap != paraTickSnap.NoValue)
		nTickSnap = gval.TickSnap;

	if (gval.JumpNote != paraJumpNote.NoValue && bHackEnabled)
	{
		int n = (gval.JumpNote / 16) * 12 + (gval.JumpNote % 16) + 11;
		if (n >= 0 && n < 128)
		{
			JumpTickRec *p = FindTickRec(jttNote, n);
			if (p)
			{
				nNextDest = p->nTick;
				nNextDestMode = p->nJumpMode;
			}
		}
		bHadNoteOff = true;
	}

	if (gval.JumpTickMode != paraJumpTickMode.NoValue)
	{
		nTickTriggerMode = gval.JumpTickMode;
	}

	if (gval.JumpTick != paraJumpTick.NoValue && bHackEnabled)
	{
		nNextDest = gval.JumpTick;
		nNextDestMode = nTickTriggerMode;
		bHadNoteOff = true;
	}

	if (pKeyCapDlg)
		pKeyCapDlg->Invalidate();

	if (bHackEnabled && nNextDest != -1)
	{
		if (getSongPosition() % nTickSnap == 0)
		{
			if (!bHeldMoreThanOneLoop || !bHadNoteOff || !aval.WantNoteOff)
			{
				switch (nNextDestMode)
				{
				case 1: //rel back
					if (bOn) setSongPosition(getSongPosition() - nNextDest);
					break;
				case 2: //rel fwd
					if (bOn) setSongPosition(getSongPosition() + nNextDest);
					break;
				case 0: //abs
				default:
					if (bOn) setSongPosition(nNextDest);
					break;
				}
			}

			if (bHadNoteOff || !aval.WantNoteOff)
				nNextDest = -1;
			else
				bHeldMoreThanOneLoop = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Stop()
//Stop the machine (if we have to) when the Stop button is clicked
{
}

////////////////////////////////////////////////////////////////////////

void mi::MidiNote(const int channel, const int value, const int velocity)
{
	if (channel == aval.MidiChannel || aval.MidiChannel == 16)
	{
		if (velocity)
		{
			nMidiNote = value;
			bHadNoteOff = false;
			bHeldMoreThanOneLoop = false;
			if (pdlg)
			{
				pdlg->SetNote(value);
			}
			if (bHackEnabled)
			{
				JumpTickRec *p = FindTickRec(jttNote, value);
				if (p)
				{
					nNextDest = p->nTick;
					nNextDestMode = p->nJumpMode;
				}
			}
		}
		else if (value == nMidiNote)
		{
			bHadNoteOff = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i) 
//Handler for context menu items
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	switch (i)
	{
	case 0:
		pdlg = new CSettingsDlg;
		pdlg->pmi = this;
		pdlg->DoModal();
		delete pdlg;
		pdlg = NULL;
		break;

	case 1:
		if (pKeyCapDlg)
			pKeyCapDlg->SetActiveWindow();
		else
		{
			pKeyCapDlg = new CKeyCapDlg;
			pKeyCapDlg->pmi = this;
			if (pKeyCapDlg->Create(CKeyCapDlg::IDD))
				pKeyCapDlg->ShowWindow(SW_SHOW);
			else
			{
				delete pKeyCapDlg;
				pKeyCapDlg = NULL;
			}
		}
		break;

	case 2:
		MessageBox(NULL,"BTDSys LiveJumpHACK v1.0.1\n"
			"©2004-2008 Ed Powley\n"
			"Parts ©2001 Peter 'P.DooM' Kaufmann\n"
			"Idea by mute","About",MB_OK);
		break;

	case 3:
		bHadNoteOff = true;
		break;

	case 4:
		if (bUseNewCallbacks)
			MessageBox(NULL, "I am using the new callbacks", "LiveJumpHACK", MB_OK);
		else
			MessageBox(NULL, "I am using P.DooM's hacks", "LiveJumpHACK", MB_OK);
	}
}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
//Describe value!
{
	switch (param)
	{
	case numJumpTickMode:
		{
			switch (value)
			{
			case 0: return "Absolute";
			case 1: return "Relative back";
			case 2: return "Relative fwd";
			default: return "???";
			}
		}

	case numOnOff:
		if (value == SWITCH_ON) return "On"; else return "Off";
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////

#pragma optimize ("", on)

