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

//SGrid Out machine
//Based on code by Polac

#include "../mdk/mdk.h"
#include <windows.h>
#include <vector>

#define NUM_AUX_GROUPS 8

struct SGridInfo
{
	CMachine *pMac;
	float *buf[NUM_AUX_GROUPS];
};

class mi;

using namespace std;
vector<SGridInfo> g_GridVec;
vector<mi *> g_MIVec;

extern "C" __declspec(dllexport) void __stdcall AddMe(CMachine *mac, float **buffers);
extern "C" __declspec(dllexport) void __stdcall DelMe(CMachine *mac);

CMachineParameter const paraGroup =
{
	pt_byte,                
	"Group",			//Short name		
	"Output group (0=none)",           	//Long name
	0,					//Min    
	NUM_AUX_GROUPS,		//Max   
	0xFF,				//No     		
	MPF_STATE,              		
	0					//Def  
};

CMachineParameter const *pParameters[] = 
{	
	&paraGroup,
	NULL
};

CMachineAttribute const *pAttributes[] = 
{
	NULL
};

CMachineInfo const MacInfo = 
{
            MT_EFFECT,
			MI_VERSION,
			MIF_MONO_TO_STEREO|MIF_DOES_INPUT_MIXING,
            0,				//Min tracks
			0,				//Max tracks
            1,				//Num global
            0,				//Num track
            pParameters, 
            0,				//Num attr
            pAttributes, 
            "BTDSys SampleGrid Out",	//Long name
            "Out",			//Short name
            "Ed Powley (BTDSys)",		//Author
			"About...",		//Commands
			0
};


#pragma pack(1)                        

class avals
{
public:
};

class gvals
{
public:            
	byte Group;
};

class tvals
{
public:
};



#pragma pack()

class mi;
class miex : public CMachineInterfaceEx 
{
public:					
	virtual void	GetSubMenu(int const i, CMachineDataOutput *pout);
					
	virtual void	AddInput(char const *macname, bool stereo);
	virtual void	DeleteInput(char const *macename);
	virtual void	RenameInput(char const *macoldname, char const *macnewname); 
    virtual void	SetInputChannels(char const *macname, bool stereo){};
    virtual void	Input(float *psamples, int numsamples, float amp){};
	virtual bool	HandleInput(int index, int amp, int pan) {return false;};
	char const*		DescribeParam(int const param){return NULL;};
	mi*				pmi;
			
};

class mi : public CMachineInterface
{
public:
            mi();
            virtual				~mi();
            virtual void		Tick();			                        
            virtual bool		Work(float *psamples, int numsamples, int const mode);
			virtual bool		WorkMonoToStereo(float *pin, float *pout, int numsamples, int const mode);
			virtual void		Init(CMachineDataInput * const pi);
			virtual void		Save(CMachineDataOutput * const po);
			virtual void		Command(int const i);            
			virtual void		SetNumTracks(int const n);
			virtual char const	*DescribeValue(int const param, int const value);            
            virtual void		OutputModeChanged(bool stereo) {}
			virtual void		AttributesChanged();			
			virtual void		MidiNote(int const channel, int const value, int const velocity);			
			virtual	void		Stop();			
									
public:
            		
			CMachine*			thismachine;
			miex				ex;
			int					numTracks;
            avals				aval;
			gvals				gval;
//			tvals				tval[MAX_TRACKS];	
			
			int					out_index;
			CMachine			*sourcemachine;

			float				*bufs[NUM_AUX_GROUPS];

			int last_random_ident;
};

	
HINSTANCE hAppInstance;
HANDLE mutex;

//#pragma optimize ("awy", on) 

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	hAppInstance=hinstDLL;
	return 1;
}


__declspec(dllexport) void __stdcall AddMe(CMachine *mac, float **buffers)
{
	SGridInfo t;
	t.pMac = mac;
	for (int i=0; i<NUM_AUX_GROUPS; i++)
		t.buf[i] = buffers[i];

	g_GridVec.push_back(t);
}

__declspec(dllexport) void __stdcall DelMe(CMachine *mac)
{
	for (int i=0; i<g_GridVec.size(); i++)
	{
		if (mac == g_GridVec[i].pMac)
			g_GridVec.erase(&g_GridVec[i]);
	}

	for (int j=0; j<g_MIVec.size(); j++)
	{
		if (mac == g_MIVec[j]->sourcemachine)
			g_MIVec[j]->sourcemachine = NULL;
	}
}

void miex::AddInput(char const *macname, bool stereo)
{	
	if (macname)
	{
		if (!pmi->sourcemachine)
		{		
			CMachine* cm = pmi->pCB->GetMachine(macname);
			if (cm)
			{
				for (int i=0; i<g_GridVec.size(); i++)
				{
					if (g_GridVec[i].pMac == cm)
					{
						pmi->sourcemachine = cm;
						for (int b=0; b<NUM_AUX_GROUPS; b++)
							pmi->bufs[b] = g_GridVec[i].buf[b];
					}
				}

/*				if (!pmi->sourcemachine)
					MessageBox(NULL,"The machine you just connected doesn't support multiple outs using this machine.", "BTDSys SampleGrid Out", MB_OK);
*/			}
		}
/*		else
			MessageBox(NULL,"Please only connect one machine at once to this machine", "BTDSys SampleGrid Out", MB_OK);
*/	}
}

void miex::DeleteInput(char const *macname)
{	
	if (macname)
	{
		CMachine* cm = pmi->pCB->GetMachine(macname);
		if (cm == pmi->sourcemachine)
			pmi->sourcemachine = NULL;
	}
}

void miex::RenameInput(char const *macoldname, char const *macnewname) 
{	
}

void miex::GetSubMenu(int const i, CMachineDataOutput *pout)
{	
}





mi::mi() 
{
	ex.pmi=this;	
//	TrackVals = &tval;
	GlobalVals = &gval;
	AttrVals= (int*)&aval;	
}

mi::~mi() 
{
	for (int i=0; i<g_MIVec.size(); i++)
	{
		if (g_MIVec[i] == this)
			g_MIVec.erase(&g_MIVec[i]);
	}
}

void mi::Init(CMachineDataInput * const pi)
{
	thismachine = pCB->GetThisMachine();	
	pCB->SetnumOutputChannels(thismachine,2);
	pCB->SetMachineInterfaceEx((CMachineInterfaceEx*)&ex);	

	sourcemachine = NULL;

	g_MIVec.push_back(this);
}

void mi::Stop(void)
{	

}

void mi::SetNumTracks(int const n) 
{
	numTracks=n;		
}

void mi::MidiNote(int const channel, int const value, int const velocity)
{
}

void mi::AttributesChanged(void)
{	
}

void mi::Tick() 
{	
	if (gval.Group != paraGroup.NoValue)
	{
		out_index = gval.Group;
/*		if (out_index > 0)
		{
			char txt[256];
			sprintf(txt, "%X", bufs[out_index-1]);
			MessageBox(NULL,txt,"",MB_OK);
		}
*/	}
}

void mi::Save(CMachineDataOutput * const po)
{	
}

bool mi::Work(float *psamples, int numsamples, int const mode)
{	
	return false;
}

bool mi::WorkMonoToStereo(float *pin, float *pout, int numsamples, int const mode) 
{ 
	if (sourcemachine && out_index > 0)
	{
		if (*(int*)&bufs[out_index-1][MAX_BUFFER_LENGTH*2] != last_random_ident)
		{
			last_random_ident = *(int*)&bufs[out_index-1][MAX_BUFFER_LENGTH*2];
			if (mode==WM_NOIO)
			{	
				return false;
			}
			else if (mode==WM_READ)
			{				
				return true;
			}
			else //WM_WRITE or WM_READWRITE
			{
				float *tmp = bufs[out_index-1];

				bool makingnoise = false;

				for (int i=0;i<numsamples*2;i++)
				{
					*pout = *tmp++;
					if (*pout++ != 0.0f)
						makingnoise = true;
				}
				return makingnoise;
			}
		}
		else
			return false;
	}

	return false; 
}




void mi::Command(int const i)
{
	MessageBox(NULL,
		"BTDSys SampleGrid v1.21 - Out Machine\n"
		"©2002-2003 Ed Powley (BTDSys)\n\n"
		"Comments/suggestions/bugs to zen30378@zen.co.uk\n\n"
		"This machine is DONATIONWARE, so if you like it, and want to "
		"send me something nice (eg hardware, CDs, money etc etc), drop "
		"me an email.\n\n"
		"Based on code by Frank Potulski (Polac)",
		"BTDSys SampleGrid Out",
		MB_OK|MB_SYSTEMMODAL|MB_ICONINFORMATION);
}

char const *mi::DescribeValue(int const param, int const value)
{	
	static char txt[128];

	switch (param)
	{
	case 0:
		if (sourcemachine)
		{
			if (value == 0)
				return "None";
			else
			{
				sprintf(txt, "%i(%s)", value, pCB->GetMachineName(sourcemachine));
				return txt;
			}
		}
		else
			return "No machine";
		break;
	default:
		return NULL;
	}
}

#pragma optimize ("", on) 


DLL_EXPORTS
	
