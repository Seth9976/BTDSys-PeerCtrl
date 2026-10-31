// reverse engineering CMachine
// 2005 Ed Powley
// http://btdsys.cjb.net

#ifndef __CMACHINE_H
#define __CMACHINE_H

//sizeof(CMachine) <= 0x258

#pragma pack(1)

class CMachine
{
public:
	int unk000;
	int unk004;
	int unk008;
	int unk00C;
	int unk010;
	char *name; //int unk014;
	int unk018;
	int unk01C;
	int unk020;
	int unk024;
	int unk028;
	int unk02C;
	int unk030;
	int unk034;
	int unk038;
	int unk03C;
	int unk040;
	int unk044;
	int unk048;
	CMachineInterface *pmi;	//int unk04C;
	int unk050;
	int unk054;
	int unk058;
	int unk05C;
	int unk060;
	int unk064;
	int unk068;
	int unk06C;
	int unk070;
	int unk074;
	int numTracks; //int unk078;
	int unk07C;
	int unk080;
	int unk084;
	int unk088;
	int unk08C;
	int unk090;
	int unk094;
	int unk098;
	int unk09C;
	int unk0A0;
	int unk0A4;
	int unk0A8;
	float posx; //int unk0AC;
	float posy; //int unk0B0;
	int unk0B4;
	int unk0B8;
	int unk0BC;
	int unk0C0;
	int unk0C4;
	int unk0C8;
	int unk0CC;
	int unk0D0;
	int unk0D4;
	int unk0D8;
	int unk0DC;
	int unk0E0;
	int unk0E4;
	char mutethru; //0 = normal, 1 = <mute>, 2 = <thru>
	char unk0E9; char unk0EA; char unk0EB; //int unk0E8;
	int unk0EC;
	int unk0F0;
	int unk0F4;
	int unk0F8;
	char unk0FC; char mute; char unk0FE; char unk0FF; //int unk0FC;
};

#pragma pack()








#endif
