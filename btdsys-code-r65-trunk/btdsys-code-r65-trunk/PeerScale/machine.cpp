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

#include "..\mdk\MachineInterface.h"
#include <float.h>
#include <math.h>
#include <stdlib.h>
#include <windows.h>

#include "machine.h"
#include "PeerScale.h"

#include "assigndlg.h"
#include "scaledlg.h"
#include "customscaledlg.h"
#include "mididialog.h"

#pragma optimize ("awy", on)

////////////////////////////////////////////////////////////////////////

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////

int __cdecl dbgprintf(const char *format, ...);

void ReadString(tString &str, CMachineDataInput * const pi)
{
	int s;
	pi->Read(s);
	if(s > 0)
	{
		char *buf = new char[s];
		pi->Read(buf, s);
		str = buf;
		delete [] buf;
	}
}

void WriteString(tString &str, CMachineDataOutput * const po)
{
	int s = str.size() + 1; // +1 for terminating null
	po->Write(s);
	po->Write((void*) str.c_str(), s);
}

HINSTANCE dllInstance;

////////////////////////////////////////////////////////////////////////

mi::mi() 
: initialising(false)
, specialpeerchordmode(false)
, m_note(NOTE_NO)
, m_chord(0)
, midiChannel(0)
, midiMode(1)
{ 
	GlobalVals = &gval;
	AttrVals = (int*) &aval;
}

mi::~mi() 
{
}

////////////////////////////////////////////////////////////////////////

bool mi::DblClickEvent(void *p)
{
	return false;
}

////////////////////////////////////////////////////////////////////////
void mi::ImportFinished()
{
	CPeerCtrl::OnImportFinished();

	initialising = false;
}

////////////////////////////////////////////////////////////////////////
void mi::Init (CMachineDataInput * const pi)
//Initialise machine and load data from file
{
	pCB->SetMachineInterfaceEx(this);

	ThisMac = pCB->GetThisMachine();

	pc.Init(ThisMac, this);

	// Init add/delete machine event handlers
	pCB->SetEventHandler(ThisMac, DoubleClickMachine, (EVENT_HANDLER_PTR) &mi::DblClickEvent, NULL);
	CPeerCtrl::SetEventHandlers(pCB); 

  	LoadScaleFile();

    if(pi) 
	{
		initialising = true;

		byte dummy; pi->Read(dummy); // Handle fake MDK version byte

		byte version;
		pi->Read(version);
		if (version == 11)
		{
			pc.ReadFileData(pi);
			pi->Read(specialpeerchordmode);

			ReadString(userscales, pi);
			AddScales(userscales.c_str(), numBuiltinScales);

			pi->Read(midiChannel);
			pi->Read(midiMode);
		}
	}
}


void GetLine(char *c, FILE *f)
// Read a single line from a FILE*
{
	do
	{
		fread(c,1,1,f);
		c++;
	}
	while (c[-1] != '\n' && !feof(f));

	c[-1] = 0;
}

bool IsNum(char c)
// Is this character a digit?
{
	return (c >= '0' && c <= '9');
}

void mi::LoadScaleFile()
// Load the scales.txt file
{	
	numBuiltinScales = 0;
	
	HRSRC hrsrc = FindResource((HMODULE) dllInstance, MAKEINTRESOURCE(IDR_SCALES), "TEXT");
	if (hrsrc)
	{
		HGLOBAL hglobal = LoadResource((HMODULE)dllInstance, hrsrc);
		if (hglobal)
		{
			char *c = (char *)LockResource(hglobal);
			if (c)
			{
				numBuiltinScales = AddScales(c);
				if (numBuiltinScales == 0)
					MessageBox(NULL,"LoadScaleFile failure: no scales", "BTDSys PeerScale",MB_OK | MB_ICONERROR);
			}
			else
				MessageBox(NULL,"LoadScaleFile failure: couldn't open resource (c == NULL)","BTDSys PeerScale",MB_OK | MB_ICONERROR);
		}
		else
			MessageBox(NULL,"LoadScaleFile failure: couldn't open resource (hglobal == NULL)","BTDSys PeerScale",MB_OK | MB_ICONERROR);
	}
	else
		MessageBox(NULL,"LoadScaleFile failure: couldn't open resource (hrsrc == NULL)","BTDSys PeerScale",MB_OK | MB_ICONERROR);
}

int mi::AddScales(const char *c, int s)
{
	while (*c)
	{
		char buf[1024];
		char *b = buf;
		while (*c && *c != 13 && *c != 10) *b++ = *c++;
		
		*b = 0;
		c++;

		b = buf;
		
		int i = 0; //note number
		int tot = 0; //"running total" of notes
		while (IsNum(*b) && i < 12)
		{
			tot = scales[s].notes[i] = atoi(b) + tot;
			while (*b != ' ') b++;
			while (*b == ' ') b++;

			i++;
		}

		if (i<12) scales[s].notes[i] = 0;

		if (i>0)
		{
			strcpy(scales[s].name, b);
			s++;
		}
	}

	return s;

}

////////////////////////////////////////////////////////////////////////

void mi::Save (CMachineDataOutput * const po)
//Save data to file
{
	po->Write((byte)0); // Fake MDK version byte

	po->Write((byte)11); // version

	pc.WriteFileData(po, true);
	po->Write(specialpeerchordmode);

	WriteString(userscales, po);

	po->Write(midiChannel);
	po->Write(midiMode);
}

////////////////////////////////////////////////////////////////////////

bool mi::Work(float *psamples, int numsamples, const int mode)
{
	CPeerCtrl::BeginImmediateBatch();

	if(pc.GotParam())
	{
		if(m_note != NOTE_NO)
		{
			if(specialpeerchordmode)
			{
				const CMachineInfo *minfo = pc.GetMInfo();
				CMachine *mac = pc.GetMachine();

				if (targettrack == 0xFE) // All tracks
				{
					for (int t=0; t<pc.GetMInfo()->maxTracks; t++)
					{
						pCB->ControlChangeImmediate(pc.GetMachine(), GROUP_TRACK, t, 1, m_chord);
						pc.ControlChange_Immediate(t, m_note);

					}
				}
				else if (targettrack < pc.GetMInfo()->maxTracks)
				{
					pCB->ControlChangeImmediate(pc.GetMachine(), GROUP_TRACK, targettrack, 1, m_chord);
					pc.ControlChange_Immediate(targettrack, m_note);
				}
			}
			else
			{
				if (targettrack == 0xFE) // All tracks
				{
					for (int t=0; t<pc.GetMInfo()->maxTracks; t++)
					{
						pc.ControlChange_Immediate(t, m_note);
					}
				}
				else if (targettrack < pc.GetMInfo()->maxTracks)
				{
					pc.ControlChange_Immediate(targettrack, m_note);
				}
			}
		}
	}

	m_note = NOTE_NO;

	CPeerCtrl::EndImmediateBatch();

	return false;
}

////////////////////////////////////////////////////////////////////////

void mi::Tick() 
//Machine-level tick stuff
{
	bool b = false;
	if (gval.Scale		!= paraScale	.NoValue) { b=true; scale	= gval.Scale;}
	if (gval.Root		!= paraRoot		.NoValue) { b=true; root	= gval.Root;}
	if (gval.LowOct		!= paraLowOct	.NoValue) {         lowoct	= gval.LowOct;}
	if (gval.HighOct	!= paraHighOct	.NoValue) {         highoct	= gval.HighOct;}

	if (b && specialpeerchordmode) RecalculateChords();
	
	if (gval.TargetTrack != paraTargetTrack.NoValue) targettrack = gval.TargetTrack;

	if (!initialising)
	{
		if(gval.PeerMe != paraPeerMe.NoValue)
		{
			if(highoct >= lowoct)
			{
				if(specialpeerchordmode)
				{
					int octave = lowoct;
					SChordRec *p = PeerValToChord(gval.PeerMe,octave);
					if(p)
					{
						m_note = (octave << 4) + p->root + 1;
						m_chord = p->chord;
					}
				}
				else
				{
					m_note = PeerValToNote(gval.PeerMe);
				}
			}
		}
	}
}

void mi::RecalculateChords()
{
	chords.clear();

	for (int r=0; r<12; r++)
	{
		for (int c=1; c<NUMCHORDS-1; c++)
		{
			if (IsChordInScale(r,ChordTable[c].NumNotes,ChordTable[c].Note,root,0,scales[scale].notes))
			{
				SChordRec scr; 
				scr.root = r; 
				scr.chord = c;
				chords.push_back(scr);
			}
		}
	}
}

int mi::PeerValToNote(int p)
{
	int numnotes = 0;
	while (numnotes < 12 && scales[scale].notes[numnotes]) numnotes++;
	if (numnotes == 0) return -1;

	int range = numnotes * (highoct - lowoct + 1);

	int k = p * range / 0x8000;

	int octave = lowoct + k / numnotes;
	int note = root;
	if (k % numnotes != 0)
		note += scales[scale].notes[k % numnotes - 1];
	octave += note/12;
	note = note%12;

	return (octave << 4) + note + 1;

}

SChordRec *mi::PeerValToChord(int p, int &octave)
{
	if (chords.empty()) return NULL;

	int range = chords.size() * (highoct - lowoct + 1);

	int k = p * range / 0x8000;

	octave = lowoct + k / chords.size();
	return &chords[k % chords.size()];
}



////////////////////////////////////////////////////////////////////////

void mi::Stop()
//Stop the machine (if we have to) when the Stop button is clicked
{
}

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i) 
//Handler for context menu items
{
	switch (i)
	{
	case 0:
		{
			CAssignDlg dlg;
			dlg.g_mi = this;
			dlg.SetDlgParent(dlg.FromHandle((HWND) pCB->GetMainWindow()));
			dlg.DoModal();

			specialpeerchordmode = false;
			chords.clear();

			if (pc.GotParam()) 
			{
				const char *name = pc.GetMInfo()->Name;
				if (strcmp(name,"BTDSys PeerChord") == 0)
				{
					specialpeerchordmode = true;
					RecalculateChords();
				}
			}
		}
		break;

	case 1:
		{
			CScaleDlg dlg;
			dlg.pmi = this;
			dlg.SetDlgParent(dlg.FromHandle((HWND) pCB->GetMainWindow()));
			dlg.DoModal();
		}
		break;

	case 2:
		{
			CCustomScaleDlg dlg;
            dlg.pmi = this;
			dlg.SetDlgParent(dlg.FromHandle((HWND) pCB->GetMainWindow()));
			dlg.DoModal();
		}
		break;

	case 3:
		{
			MidiDialog dlg;
            dlg.pmi = this;
			dlg.SetDlgParent(dlg.FromHandle((HWND) pCB->GetMainWindow()));
			dlg.DoModal();
		}
		break;

	case 4:
		MessageBox(
			(HWND) pCB->GetMainWindow(),
			MACHINE_FULL_NAME"\n"
			"©2005-2013 Ed Powley / Phil Considine\n"
			"http://sourceforge.net/projects/btdsys\n\n"
			"Concept by tinga\n"
			"Uses Win32++ (http://sourceforge.net/projects/win32-framework/)",
			MACHINE_NAME,
			MB_OK
			);

/*		{
			char txt[10000]; txt[0] = 0;
			for (int r=0; r<12; r++)
			{
				for (int c=1; c<NUMCHORDS-1; c++)
				{
					if (IsChordInScale(r,ChordTable[c].NumNotes,ChordTable[c].Note,root,0,scales[scale].notes))
						sprintf(txt,"%s\n%s %s (%i/0x%X)",txt,DescribeValue(numRoot,r),ChordTable[c].Name,c,c);
				}
			}

			MessageBox(NULL,txt,"Chords",MB_OK);
		}
*/
		break;
	}
}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
//Describe value!
{
	static char txt[1024];

	switch (param)
	{
	case numRoot:
		switch (value)
		{
		case  0: return "C";
		case  1: return "C#";
		case  2: return "D";
		case  3: return "D#";
		case  4: return "E";
		case  5: return "F";
		case  6: return "F#";
		case  7: return "G";
		case  8: return "G#";
		case  9: return "A";
		case 10: return "A#";
		case 11: return "B";
		default: return "WTF?";
		}

	case numScale:
		sprintf(txt,"%i:%s",value,scales[value].name);
		return txt;

	case numPeerMe:
		{
			static const char *notenames[] = {"??","C-","C#","D-","D#","E-","F-","F#","G-","G#","A-","A#","B-","??","??","??"};
			if (specialpeerchordmode)
			{
				int octave = lowoct;
				SChordRec *p = PeerValToChord(value, octave);
				if (p == NULL)
					strcpy(txt,"No chords!");
				else
					sprintf(txt,"%s%i_%s",notenames[p->root+1],octave,ChordTable[p->chord].Name);
			}
			else
			{
				int n = PeerValToNote(value);
				if (n == -1) return NULL;
				sprintf(txt,"%i: %s%i",value,notenames[n&15],n>>4);
			}
			return txt;
		}

	case numTargetTrack:
		if (value == 0xFE) return "All";
		if (pc.GotParam()) if (value >= pc.GetMInfo()->maxTracks) return "No";
		sprintf(txt,"%i",value);
		return txt;

	//PAL interface
	/*
	case 0xB7D0:
		return "hi pal";

	case 0xB7D1:
		{
			static PalPalAssign a[2];
			a[0].targetmac = pc.GetMachine();
			if (a[0].targetmac)
			{
				const char *s = pc.GetParamString();
				a[0].targetgroup = (s[1] == 'G') ? 1 : 2;
				a[0].targetparam = atoi(s+4);
				a[0].targettrack = targettrack;
			}
			a[0].comment[0] = 0;

			a[1].targetmac = (CMachine *)-1;

			return (char *) a;
		}
	*/
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////

#pragma optimize ("", on)

