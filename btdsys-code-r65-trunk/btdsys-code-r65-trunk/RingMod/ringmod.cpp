/*
Copyright (c) 2002, Edward Powley
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

#include "../mdk/mdk.h"
#include "../dsplib/dsplib.h"
#include <float.h>
#include <stdlib.h>
#include <vector>	//<-- we use a vector to store info about connections

////////////////////////////////////////////////////////////////////////
// Parameters

CMachineParameter const paraDryOut =
{
	pt_byte,			//Type
	"Dry Out",			//Short name
	"Dry Out (0=0% 0x80=100%)",				//Long name
	0,					//Min value
	0x80,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraWetOut =
{
	pt_byte,			//Type
	"Wet Out",			//Short name
	"Wet Out (0=0% 0x80=100%)",				//Long name
	0,					//Min value
	0x80,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0x80				//Default value
};

////////////////////////////////////////////////////////////////////////
// Param-related pointer arrays and classes

CMachineParameter const *pParameters[] = {
	&paraDryOut,
	&paraWetOut,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	NULL
};

#pragma pack(1)

class gvals
{
public:
	byte DryOut;
	byte WetOut;
};

#pragma pack()

////////////////////////////////////////////////////////////////////////
// Machine info

CMachineInfo const MacInfo =
{
	MT_EFFECT,						//Machine type
	MI_VERSION,						//Interface version
	MIF_MONO_TO_STEREO | MIF_DOES_INPUT_MIXING,			//Flags
	0,								//Min tracks
	0,								//Max tracks
	2,								//Number of global parameters
	0,								//Number of track parameters
	pParameters,					//Pointer to parameters
	0,								//Number of attributes
	pAttributes,					//Pointer to attributes
	"BTDSys RingMod",				//Full name
	"RingMod",						//Short name
	"Ed Powley (BTDSys)",			//Author
	"About..."						//Commands
};

////////////////////////////////////////////////////////////////////////
// The "miex" class is needed for handling multiple inputs

class mi;

class miex : public CMDKMachineInterfaceEx
{
public:
	virtual void AddInput(const char *macname, bool stereo);
	virtual void DeleteInput(const char *macname);
	virtual void RenameInput(char const *macoldname, char const *macnewname);			
	virtual void SetInputChannels(char const *macname, bool stereo);

	virtual void Input(float *psamples, int numsamples, float amp);

	mi *pmi; // pointer to 'parent'
};

////////////////////////////////////////////////////////////////////////
//This little class is used to store info about each connected machine

class CInput
{
public:
	char MacName[256];
	bool Stereo;
};

////////////////////////////////////////////////////////////////////////
// The main machine interface class

class mi : public CMDKMachineInterface
{
public:
	mi();
	virtual ~mi();
	virtual void Tick();
	virtual void Stop() {}
	virtual void MDKInit(CMachineDataInput * const pi);
	virtual bool MDKWork(float *psamples, int numsamples, int const mode) { return false; };
	virtual bool MDKWorkStereo(float *psamples, int numsamples, int const mode);
	virtual void MDKSave(CMachineDataOutput * const po);
	virtual char const *DescribeValue(int const param, int const value);
	virtual CMDKMachineInterfaceEx *GetEx() { return &ex; }
	virtual void Command(const int i);
	virtual void OutputModeChanged(bool stereo) {}

	miex ex;

	gvals gval;

	float drybuffer[MAX_BUFFER_LENGTH*2]; //buffers used for combining inputs
	float wetbuffer[MAX_BUFFER_LENGTH*2]; //set for max length of a stereo buffer

	float DryOut, WetOut; //amp levels

	bool SilentInput; //were any inputs silent?
	bool InitBuffer;  //do we need to initialise the buffer?
	int CurrentInput; //which input are we working on?
	std::vector<CInput> Inputs; //info on the inputs

	inline int FindInput (const char *macname); //find an input by name
};

DLL_EXPORTS

////////////////////////////////////////////////////////////////////////
// [Con/de]structors
mi::mi() 
{
	GlobalVals = &gval;
	ex.pmi = this;
}

mi::~mi()
{
}

////////////////////////////////////////////////////////////////////////

void mi::MDKInit (CMachineDataInput * const pi)
{
	SetOutputMode(true);
	pCB->SetnumOutputChannels(pCB->GetThisMachine(), 2);

	//Don't need any custom data
}

void mi::MDKSave(CMachineDataOutput * const po)
{
}

////////////////////////////////////////////////////////////////////////

void mi::Tick()
{
	if (gval.DryOut != paraDryOut.NoValue)
		DryOut = gval.DryOut / 128.0f;

	if (gval.WetOut != paraWetOut.NoValue)
		WetOut = gval.WetOut / 128.0f;
}

////////////////////////////////////////////////////////////////////////

//This is called when a machine is connected to our machine's input.
//For some reason it is called once with macname pointing to the name
//of the machine, then again with macname == NULL. We'll just ignore
//the NULL one.
void miex::AddInput(const char *macname, bool stereo)
{
	if (macname != NULL)
	{
		CInput Temp;
		strcpy(Temp.MacName, macname);
		Temp.Stereo = stereo;

		pmi->Inputs.push_back(Temp); //Add to end of connection vector
	}
}

////////////////////////////////////////////////////////////////////////

//This function is used to find the index of a named input. Just makes
//life a bit easier.
//Returns the index of the input, or -1 if not found.
int mi::FindInput(const char *macname)
{
	for (int i=0; i<Inputs.size(); i++)
	{
		if (strcmp(Inputs[i].MacName, macname) == 0) return i;
	}

	return -1;
}

////////////////////////////////////////////////////////////////////////

//This is called when a machine gets disconnected from our input.
void miex::DeleteInput(const char *macname)
{
	int i = pmi->FindInput(macname);
	if (i != -1) pmi->Inputs.erase(pmi->Inputs.begin() + i);
}

////////////////////////////////////////////////////////////////////////

//This is called when a connected machine has its name changed.
void miex::RenameInput(const char *macoldname, const char *macnewname)
{
	int i = pmi->FindInput(macoldname);
	if (i != -1) strcpy(pmi->Inputs[i].MacName, macnewname);
}

////////////////////////////////////////////////////////////////////////

//This is called when the stereo state of a connected machine changes.
void miex::SetInputChannels(const char *macname, bool stereo)
{
	int i = pmi->FindInput(macname);
	if (i != -1) pmi->Inputs[i].Stereo = stereo;
}

////////////////////////////////////////////////////////////////////////

//The big one... this is where we process the input signals.
//If numsamples == 0, then this particular machine is off.
//amp is as set by the volume slider on the little triangle in machine view.
void miex::Input(float *psamples, int numsamples, float amp)
{
	if (numsamples == 0) //machine is off
	{
		//If machine is off, we would multiply the whole buffer by zero.
		//ie we zero the whole buffer.
		DSP_Zero(pmi->wetbuffer, MAX_BUFFER_LENGTH * 2);
		pmi->SilentInput = true;
	}
	else
	{
		//Four separate loops
		if (pmi->Inputs[pmi->CurrentInput].Stereo)
		{
			if (pmi->InitBuffer) //need to initialise it
			{
				for (int i=0; i < numsamples*2; i++)
					pmi->wetbuffer[i] = psamples[i] * amp;
			}
			else //mult with current wet buffer
			{
				for (int i=0; i < numsamples*2; i++)
					pmi->wetbuffer[i] *= psamples[i] * amp / 32768.0f;
			}

			//Mix with dry buffer as normal
			DSP_Add(pmi->drybuffer, psamples, numsamples*2, amp);
		}
		else //mono
		{
			if (pmi->InitBuffer)
			{
				for (int i=0; i < numsamples; i++)
					pmi->wetbuffer[i*2] = pmi->wetbuffer[i*2+1]
										= psamples[i] * amp;
			}
			else
			{
				for (int i=0; i < numsamples; i++)
				{
					pmi->wetbuffer[i*2] *= psamples[i] * amp / 32768.0f;
					pmi->wetbuffer[i*2+1] *= psamples[i] * amp / 32768.0f;
				}
			}

			//Mix with dry buffer as normal
			DSP_AddM2S(pmi->drybuffer, psamples, numsamples, amp);
		}
	}

	pmi->CurrentInput++; //increment our counter
	pmi->InitBuffer = false; //no need to init the buffer, we did it already
}

////////////////////////////////////////////////////////////////////////

//The Work() function is called after Input() has been called a bunch of
//times. This is where we output what we collected, and get ready for the
//next pass.
bool mi::MDKWorkStereo(float *psamples, int numsamples, const int mode)
{
	if (mode & WM_READ)
	{
		if (mode & WM_WRITE)
		{
			DSP_Copy(psamples, drybuffer, numsamples*2, DryOut);
			DSP_Add(psamples, wetbuffer, numsamples*2, WetOut);
		}
	}

	InitBuffer = true; //initialise it next time
	bool HadSilentInput = SilentInput; //store result for this pass
	SilentInput = false; //assumption for next pass
	CurrentInput = 0; //set counter to first input
	DSP_Zero(drybuffer, numsamples*2); //clear the buffer

	return (mode & WM_READ) && !(HadSilentInput && DryOut < 0.0078125f);
	//A bit of boolean logic to determine if we are silent
}

////////////////////////////////////////////////////////////////////////

void mi::Command(const int i)
{
	//The exciting about box
	char txt[10000];
	strcpy(txt,
		"BTDSys RingMod v1.0\n"
		"©2002 Ed Powley (BTDSys)\n\n"
		"Comments/suggestions/bug reports to e@btd2001.freeserve.co.uk\n\n");
	sprintf(txt,"%s%i inputs", txt, Inputs.size());
	for (int j=0; j<Inputs.size(); j++)
		sprintf(txt, "%s\n%i: %s, %s", txt, j, Inputs[j].MacName, Inputs[j].Stereo?"Stereo":"Mono");

	pCB->MessageBox(txt);
}

////////////////////////////////////////////////////////////////////////

char const *mi::DescribeValue(const int param, const int value)
{
	static char txt[16];

	switch (param)
	{
	case 0: //dry out
	case 1: //wet out
		sprintf(txt, "%.2f%%", value / 1.28f);
		return txt;
	default:
		return NULL;
	}
}
