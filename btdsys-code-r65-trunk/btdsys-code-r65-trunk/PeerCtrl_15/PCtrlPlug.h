/*
The main header file for PeerCtrl's plugin interface
2002-2006, Edward Powley
This file is public domain.
*/

#ifndef __BTDSYS_PCTRL_PLUG_H
#define __BTDSYS_PCTRL_PLUG_H

#define PCTRL_VERSION 3


struct SPlugInfo
{
	int Version;				//should always be PCTRL_VERSION
	char const *Name;
	char const *Author;
	char const *Description;
	char const *Menus;
	int Flags;
};

//flags
#define PCPF_WANT_MIDI_CC	0x0001
#define PCPF_WANT_MIDI_NOTE	0x0002
#define PCPF_CALL_TIMER		0x0004

//host ID
#define HOSTID_BUZZ_PEERCTRL_15	1

//host caps
#define CAP_BUZZ_POINTERS	0x0001
#define CAP_SONG_POS		0x0002
#define CAP_SONG_TEMPO		0x0004

class CMachine;
class CMachineInterface;
class CMachineInfo;

struct SBuzzPtrs
{
	CMachine			*pmac;		
	CMachineInterface	*pmi;		
	CMachineInfo const	*pinfo;	
};

class CPCtrlCallbacks
{
public:
	CPCtrlCallbacks() : pHostCB(0) {};
	virtual int		AddAxis(char const *name) = 0;
	virtual void	DelAxis(int nAxis) = 0;
	virtual void	RenameAxis(int nAxis, char const *name) = 0;
	virtual void	ShowAxis(int nAxis) = 0;
	virtual void	HideAxis(int nAxis) = 0;
	virtual bool	IsAxisShowing(int nAxis) = 0;
	virtual float	GetValue(int nAxis) = 0; // result < 0 => undefined
	virtual void	SetValue(int nAxis, float fValue) = 0;
	
	virtual int		GetHostID() = 0;
	virtual int		GetHostCaps() = 0;
	virtual float	GetSongPos() = 0;
	virtual int		GetSongBPM() = 0;
	virtual int		GetSongTPB() = 0;
	virtual void	GetBuzzPointers(SBuzzPtrs *p) = 0;
	virtual bool	IsSongPlaying() = 0;

	void *pHostCB;
};

class CPCtrlDataRead
{
public:
	virtual void	Read(void *data, int length);

	virtual int		ReadInt();
	virtual short	ReadShort();
	virtual char	ReadChar();
	virtual void	ReadASCIIZ(char *c);

	void *pHostDR;
};

class CPCtrlDataWrite
{
public:
	virtual void	Write(void *data, int length);

	virtual void	Write(int	i);
	virtual void	Write(short	i);
	virtual void	Write(char	i);
	virtual void	WriteASCIIZ(char *c);

	void *pHostDW;
};

class CPCtrlPlug
{
public:
	CPCtrlPlug() {};
	virtual ~CPCtrlPlug() {};

	virtual SPlugInfo	*GetInfo()	= 0;
	virtual bool		Init()		= 0;

	virtual void		ReadFileData (CPCtrlDataRead  *dr)	{};
	virtual void		InitNoFileData()					{};
	virtual void		WriteFileData(CPCtrlDataWrite *dw)	{};

	virtual void		ValueChanged(int nAxis, float fValue)	{};

	virtual void		Timer(int numSamples)	{};

	virtual void		MenuCommand(int nCommand) {};

	virtual bool		MidiCC(int const ctrl, int const channel, int const value) {return false;}
	virtual bool		MidiNote(int const channel, int const value, int const velocity) {return false;}

public:
	CPCtrlCallbacks* pCB;
};

#define DLL_EXPORT(class_name)									\
	extern "C" __declspec(dllexport) CPCtrlPlug *MakeNewPlug()	\
	{															\
		return new class_name ;									\
	}															\
																\
	extern "C" __declspec(dllexport) int GetPCtrlVersion()		\
	{															\
		return PCTRL_VERSION;									\
	}															
//end define


#endif //__BTDSYS_PCTRL_PLUG_H
