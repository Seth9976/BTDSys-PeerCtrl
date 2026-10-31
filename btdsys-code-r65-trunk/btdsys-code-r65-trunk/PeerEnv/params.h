/*
Copyright (c) 2002-2011, Edward Powley
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

CMachineParameter const paraMode =
{
	pt_byte,			//Type
	"Mode",				//Short name
	"Mode (0=Immediate 1=Absolute 2=Envelope)",		//Long name
	0,					//Min value
	2,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	2					//Default value
};

CMachineParameter const paraGetFrom =
{
	pt_byte,			//Type
	"Get From",				//Short name
	"Get Control Signal From (0=Left 1=Center 2=Right)",		//Long name
	0,					//Min value
	2,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	1					//Default value
};


CMachineParameter const paraInertia =
{
	pt_word,			//Type
	"Inertia",			//Short name
	"Inertia (0=off, 1=0.1 ticks, 500=128.0 ticks)",		//Long name
	0,					//Min value
	0x500,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraMin =
{
	pt_word,			//Type
	"Min",				//Short name
	"Min param value (0=0%, 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraMax =
{
	pt_word,			//Type
	"Max",				//Short name
	"Max param value (0=0%, 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0xFFFE				//Default value
};

CMachineParameter const paraCenter =
{
	pt_word,			//Type
	"Center",			//Short name
	"Center value (0=0%, 0xFFFE=100%)",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	MPF_STATE,			//Flags
	0x7FFF				//Default value
};

CMachineParameter const paraAmpl =
{
	pt_word,			//Type
	"Amplitude",		//Short name
	"Amplitude (1=-100%, 0x8000=0%, FFFF=+100%)",		//Long name
	1,					//Min value
	0xFFFF,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	0xC000				//Default value
};

CMachineParameter const paraExtentMode =
{
	pt_byte,			//Type
	"Extent Mode",		//Short name
	"Extent Setting Mode (0=Min/Max, 1=Center/Ampl)",		//Long name
	0,					//Min value
	1,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraTrack =
{
	pt_byte,			//Type
	"Track",			//Short name
	"Track to send parameter to (0xFE=all)",		//Long name
	0,					//Min value
	0xFE,				//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineAttribute const attrTickSubdiv =
{
	"Tick Subdivs",			//Name
	1,						//Min
	256,					//Max
	16						//Default
};

CMachineAttribute const attrEnvAtt =
{
	"Rise Smoothing",	//Name
	1,						//Min
	65535,					//Max
	1						//Default
};

CMachineAttribute const attrEnvRel =
{
	"Fall Smoothing",	//Name
	1,						//Min
	65535,					//Max
	1500					//Default
};

#define PARAMODE		0
#define PARAGETFROM		1
#define PARAINERTIA		2
#define PARAMIN			3
#define	PARAMAX			4
#define PARACENTER		5
#define PARAAMPL		6
#define PARAEXTENTMODE	7
#define PARATRACK		8


////////////////////////////////////////////////////////////////////////

CMachineParameter const *pParameters[] = {
	&paraMode,
	&paraGetFrom,
	&paraInertia,

	&paraMin,
	&paraMax,
	&paraCenter,
	&paraAmpl,
	&paraExtentMode,
	&paraTrack
};

CMachineAttribute const *pAttributes[] = {
	&attrTickSubdiv,
	&attrEnvAtt,
	&attrEnvRel
};

////////////////////////////////////////////////////////////////////////

#pragma pack(1)

class tvals
{
public:
	word Min;
	word Max;
	word Center;
	word Ampl;
	byte ExtentMode;
	byte Track;
};

class gvals
{
public:
	byte Mode;
	byte GetFrom;
	word Inertia;
};

class avals
{
public:
	int TickSubdiv;
	int EnvAtt;
	int EnvRel;
};

#pragma pack()

#define NUMG	3
#define NUMT	6
#define NUMA	3
