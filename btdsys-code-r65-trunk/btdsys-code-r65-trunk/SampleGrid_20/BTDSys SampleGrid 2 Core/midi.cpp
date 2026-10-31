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
#include "sgrid.h"
#include "groupdlg.h"

void mi::MidiNote(const int channel, const int value, const int velocity)
{
	if (pGroupDlg) pGroupDlg->OnMidiNote(channel, value, velocity);

	for (int t=0; t<nTriggers; t++)
	{
		if (Tracks[t].nTrigKeyType == ktMidi
			&& (Tracks[t].nTrigKey & 0xFF) == value
			&& (Tracks[t].nTrigKey >> 8) == channel)
		{
		    CSequence *pseq;
		    int stateflags = pCB->GetStateFlags();
						    
		    if (stateflags & SF_PLAYING && stateflags & SF_RECORDING && aval.MIDIRecord)
			    pseq = pCB->GetPlayingSequence(ThisMac);
		    else 
			    pseq = NULL;

            if (velocity)
			{
				STrigInfo ti;
				ti.vel = (float)velocity / 128.0f;
				ti.pan = ti.rate = 1.0f;
				ti.delay = 0;
				ti.cut = -1;
				ti.offset = Tracks[t].fOffset;
				Tracks[t].TriggerOn(ti);
			}
			else
            {
				Tracks[t].TriggerOff();
                if (!aval.MIDIRecordOffs) pseq = NULL;
            }

			if (pseq != NULL) //record
			{
				tvals *tdata = (tvals *)pCB->GetPlayingRow(pseq, 2, t);
                if (nTrigType == 'b')
    				tdata->TrackTrigger = velocity;
                else
                    tdata->TrackTrigger = (velocity) ? SWITCH_ON : SWITCH_OFF;

				int dly = Tracks[t].SamplesToCmdLen(pMasterInfo->PosInTick);
				if (dly > 0)
				{
                    switch (aval.MIDIRecordDelays)
                    {
                    case 0: break;
                    case 1:
					    tdata->Command1 = 0x01;
					    tdata->Argument1 = dly;
                        break;
                    case 2:
					    tdata->Command2 = 0x01;
					    tdata->Argument2 = dly;
                        break;
                    }
				}
    	    }

		}
	}

	for (int g=0; g<NUM_GROUPS; g++)
	{
		if (Groups[g].nMuteKeyType == ktMidi
			&& (Groups[g].nMuteKey & 0xFF) == value
			&& (Groups[g].nMuteKey >> 8) == channel)
		{
			if (Groups[g].nMuteMode == gmmToggle && velocity)
				Groups[g].bMuted = !Groups[g].bMuted;
			else if (Groups[g].nMuteMode == gmmMomOff)
				Groups[g].bMuted = (velocity != 0);
			else if (Groups[g].nMuteMode == gmmMomOn)
				Groups[g].bMuted = (velocity == 0);

		}
	}
}
