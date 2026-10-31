/*
midi.cpp: MIDI input functions
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
#include "ctrl.h"
#include "MidiDialog.h"

void mi::MidiNote(int const channel, int const value, int const velocity)
{
	bool bFlag = false;

	for (int p=0; p<plugs.size(); p++)
	{
		if (plugs[p]->pInfo->Flags & PCPF_WANT_MIDI_NOTE)
			bFlag |= plugs[p]->pPl->MidiNote(channel,value,velocity);
	}
}

void miex::MidiControlChange(int const ctrl, int const channel, int const value)
{
	bool bFlag = false;

	for (int p=0; p<p_mi->plugs.size(); p++)
	{
		if (p_mi->plugs[p]->pInfo->Flags & PCPF_WANT_MIDI_CC)
			bFlag |= p_mi->plugs[p]->pPl->MidiCC(ctrl,channel,value);
	}

//	if (bFlag)
	{
		if (p_mi->pmididlg != NULL)
		{
			char tmp[32];
			switch (ctrl)
			{
			case 252: //Channel Aftertouch
			case 253: //Poly Aftertouch
				return;
			case 254: strcpy(tmp, "Program Change"); break;
			case 255: strcpy(tmp, "Pitch Bend"); break;
			default:  _itoa(ctrl, tmp, 10);
			}

			char txt[256];
			sprintf(txt, "Last controller was %s, value %i, channel %i", tmp, value, channel+1);
			if (ctrl == 96 || ctrl == 97)
				sprintf(txt, "%s (or %s controller %i?)", 
					txt, (ctrl == 96) ? "Inc" : "Dec", value);

			p_mi->pmididlg->m_LastControllerDisplay.SetWindowText(txt);
		}

		for (int t=0; t<p_mi->numTracks; t++)
		{
			CTrack *tr = &p_mi->Tracks[t];
			if (tr->MIDIChannel == channel+1 || tr->MIDIChannel == 17)
			{
				if ((ctrl == 96 || ctrl == 97) && tr->MIDIIncDec)
				{
					if (value == tr->MIDIController)
					{
						if (ctrl == 96) //inc
						{
							tr->ValueTarget += p_mi->aval.MIDIIncDecAmt / 65534.0f;
							if (tr->ValueTarget > 1.0f)
							{
								if (tr->MIDIIncDecWrap)
									tr->ValueTarget -= 1.0f;
								else
									tr->ValueTarget = 1.0f;
							}
						}
						else //dec
						{
							tr->ValueTarget -= p_mi->aval.MIDIIncDecAmt / 65534.0f;
							if (tr->ValueTarget < 0.0f)
							{
								if (tr->MIDIIncDecWrap)
									tr->ValueTarget += 1.0f;
								else
									tr->ValueTarget = 0.0f;
							}
						}
						if (p_mi->Inertia)
						{
							tr->ValueStep = (tr->ValueTarget - tr->ValueCurrent) / (p_mi->InertiaLength * p_mi->pMasterInfo->SamplesPerTick);
							tr->ValueSliding = true;
						}
						else
						{
							tr->ValueCurrent = tr->ValueTarget;
							tr->ValueSliding = false;
						}
						p_mi->pCB->ControlChange(p_mi->ThisMac, 2, t, numValueInT, (int)(tr->ValueTarget * 65534.0f + 0.5f));
					}
				}
				else //plain old controller
				{
					if (ctrl == tr->MIDIController)
					{
						tr->ValueTarget = value / ((ctrl == 255) ? 16383.0f : 127.0f);
						if (p_mi->Inertia)
						{
							tr->ValueStep = (tr->ValueTarget - tr->ValueCurrent) / (p_mi->InertiaLength * p_mi->pMasterInfo->SamplesPerTick);
							tr->ValueSliding = true;
						}
						else
						{
							tr->ValueCurrent = tr->ValueTarget;
							tr->ValueSliding = false;
						}
						p_mi->pCB->ControlChange(p_mi->ThisMac, 2, t, numValueInT, (int)(tr->ValueTarget * 65534.0f + 0.5f));
					}
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
