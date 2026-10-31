#include "stdafx.h"
#include "midijump.h"

CMachineParameter const paraTickSnap =
{
	pt_word,			//Type
	"Tick Snap",		//Short name
	"Tick Snap",		//Long name
	1,					//Min value
	1024,				//Max value
	0,					//No value
	MPF_STATE,			//Flags
	16					//Default value
};

CMachineParameter const paraJumpNote =
{
	pt_note,			//Type
	"Note Trigger",		//Short name
	"Note Trigger",		//Long name
	NOTE_MIN,			//Min value
	NOTE_MAX,			//Max value
	NOTE_NO,			//No value
	0,					//Flags
	NOTE_NO				//Default value
};

CMachineParameter const paraJumpTick =
{
	pt_word,			//Type
	"Tick",				//Short name
	"Tick Number Trigger",		//Long name
	0,					//Min value
	0xFFFE,				//Max value
	0xFFFF,				//No value
	0,					//Flags
	0xFFFF				//Default value
};

CMachineParameter const paraJumpTickMode =
{
	pt_byte,			//Type
	"Tick Jump Mode",	//Short name
	"Mode for Tick Number Trigger (0=absolute 1=relative back 2=relative fwd)",		//Long name
	0,					//Min value
	2,					//Max value
	0xFF,				//No value
	MPF_STATE,			//Flags
	0					//Default value
};

CMachineParameter const paraOnOff =
{
	pt_switch,			//Type
	"On/Off",			//Short name
	"On/Off",			//Long name
	-1,					//Min value
	-1,					//Max value
	SWITCH_NO,			//No value
	MPF_STATE,			//Flags
	SWITCH_ON			//Default value
};

CMachineAttribute const attrMidiChannel =
{
	"MIDI channel (16=all)",	//Name
	0,		//Min
	16,		//Max
	0		//Def
};

CMachineAttribute const attrWantNoteOff =
{
	"Keep jumping until note off",	//Name
	0,		//Min
	1,		//Max
	0		//Def
};

////////////////////////////////////////////////////////////////////////

CMachineParameter const *pParameters[] = {
	&paraTickSnap,
	&paraJumpNote,
	&paraJumpTick,
	&paraJumpTickMode,
	&paraOnOff,
	NULL
};

CMachineAttribute const *pAttributes[] = {
	&attrMidiChannel,
	&attrWantNoteOff,
	NULL
};

////////////////////////////////////////////////////////////////////////

CMachineInfo const MacInfo =
{
	MT_GENERATOR,					//Machine type
	MI_VERSION,						//Interface version
	MIF_NO_OUTPUT,					//Flags
	0,								//Min tracks
	0,								//Max tracks
	NUMG,							//Number of global parameters
	0,								//Number of track parameters
	pParameters,					//Pointer to parameters
	NUMA,							//Number of attributes
	pAttributes,					//Pointer to attributes
	"BTDSys LiveJumpHACK",			//Full name
	"LiveJump",						//Short name
	"Ed Powley (BTDSys)",			//Author
	"Settings...\n"
	"Key Capture Window...\n"
	"About...\n"
	"Panic!\n"
	"Which song position functions?"		//Commands
};
