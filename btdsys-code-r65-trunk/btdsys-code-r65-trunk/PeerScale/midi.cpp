/*
Copyright (c) 2005-2011, Edward Powley
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

#include "machine.h"

int __cdecl dbgprintf(const char *format, ...);

bool mi::IsNoteInScale(int note, int scaleroot, int numscalenotes, int *scalenotes)
{
	if (note == scaleroot) return true;

	for (int i=0; i<12 && scalenotes[i]; i++)
	{
		if ( ((scalenotes[i] + scaleroot) % 12) == note )
			return true;
	}

	return false;
}

int mi::FindNearestNoteInScale(int note, int scaleroot, int *scalenotes, int roundingmode)
{
	if (note == scaleroot) return note;

	int octavefudge = 0;
	if (note < scaleroot)
	{
		note += 12;
		octavefudge = 12;
	}

	for (int i=0; i<12 && scalenotes[i]; i++)
	{
		if (scalenotes[i] + scaleroot == note)
			return note - octavefudge;

		if (scalenotes[i] + scaleroot > note)
		{
			switch (roundingmode)
			{
			case 1: //always up
				return scalenotes[i] + scaleroot - octavefudge;
			case 2: //always down
				return scalenotes[i-1] + scaleroot - octavefudge;
			}
		}
	}

	// Shouldn't get here...
	dbgprintf("mi::FindNearestNoteInScale failed!");
	return note;
}

int SignedMod(int a, int b)
{
	if (a>=0) return a%b;
	else return (b - ((-a)%b)) % b;
}

void mi::MidiNote(int const channel, int const value, int const velocity)
{
	if (pc.GotParam())
	{
		if ((channel + 1 == midiChannel || midiChannel == 17))
		{
			int note = value % 12;
			int octave = value / 12;
			bool shouldplay = true;

			if(velocity) // note-on
			{
				switch (midiMode)
				{
				case 0: // silence non-scale
					shouldplay = IsNoteInScale(note, root, 0, scales[scale].notes);
					break;

				case 1: // round up
				case 2: // round down
					note = FindNearestNoteInScale(note, root, scales[scale].notes, midiMode);
					break;
				}

				if (shouldplay)
				{
					octave += note / 12;
					if (note < 0) octave--;

					int nval = (octave << 4) + SignedMod(note, 12) + 1;
					if (targettrack == 0xFE)
					{
						for (int t=0; t<pc.GetMInfo()->maxTracks; t++)
						{
							pc.ControlChange_Immediate(t, nval);
						}
					}
					else if (targettrack < pc.GetMInfo()->maxTracks)
					{
						pc.ControlChange_Immediate(targettrack, nval);
					}
				}
			}
			/*
			else // note-off
			{

			}
			*/
			
		}
	}
}
