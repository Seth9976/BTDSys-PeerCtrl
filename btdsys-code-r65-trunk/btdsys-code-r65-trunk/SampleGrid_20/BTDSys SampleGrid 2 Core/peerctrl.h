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


#ifndef __BTDSYS__PEERCTRL__
#define __BTDSYS__PEERCTRL__

#include "windows.h"
#include "../../mdk/mdk.h"
#include <float.h>
#include <stdlib.h>
#include <math.h>

//NOTES:
//* Don't forget to link peerctrl.lib with your project! :)
//*	Param numbers are zero based for both global and track params (ie 0 refers
//	to both first global and first track
//* Parameter string format: "[G] 1: Cutoff"
//	ie "[%s] %i: %s", (Group == 1) ? "G" : "T", ParNum, ParName

#define GROUP_GLOBAL	1
#define GROUP_TRACK		2

#define ALLOW_SWITCH		1
#define ALLOW_BYTE			2
#define ALLOW_NOTE			4
#define ALLOW_WORD			8

#define ALLOW_GLOBAL		16
#define ALLOW_TRACK			32

#define ALLOW_ALL_TYPES		ALLOW_SWITCH | ALLOW_BYTE | ALLOW_NOTE | ALLOW_WORD
#define ALLOW_ALL_GROUPS	ALLOW_GLOBAL | ALLOW_TRACK

#define ALLOW_ATTRIBUTES	64



//The biggie... this class encapsulates all data/methods for controlling one
//parameter of one machine

class CPeerCtrl
{
public:
	//Call this in Init(). pmac = pCB->GetThisMachine(), pmi = this
	CPeerCtrl(CMachine *pmac, CMachineInterface *pmi);

	~CPeerCtrl();

	//Call in Init() as part of your custom data input (if any)
	void ReadFileData (CMachineDataInput  * const pi);

	//Call in Save() as part of your custom data output (if any)
	void WriteFileData(CMachineDataOutput * const po);

	//Populate dialog items with machines
	//hWnd = parent window (eg hDlg), ID = id of control (eg IDC_MACHINECOMBO)
	//ExcludeName = Name of calling machine if it cannot control itself, 
	//  or "" otherwise
	int  GetMachineNamesToCombo	(HWND hWnd, int ComboID, const char *ExcludeName);
	int  GetMachineNamesToList	(HWND hWnd, int ListID , const char *ExcludeName);

	//Populate dialog items with parameters on specified machine
	//hWnd = parent window (eg hDlg), ID = id of control (eg IDC_MACHINECOMBO)
	//TypesAllowed: see ALLOW_ constants above ('|' them together)
	int  GetParamNamesToCombo	(CMachine *pmac, HWND hWnd, int ComboID, int TypesAllowed);
	int  GetParamNamesToList	(CMachine *pmac, HWND hWnd, int ListID , int TypesAllowed);

	//Try to select current machine/param in dialog item
	//hWnd = parent window (eg hDlg), ID = id of control (eg IDC_MACHINECOMBO)
	bool SelectMachineInCombo	(HWND hWnd, int ComboID);
	bool SelectMachineInList	(HWND hWnd, int ListID);
	bool SelectParameterInCombo	(HWND hWnd, int ComboID);
	bool SelectParameterInList	(HWND hWnd, int ListID);

	//Create assignments
	//ParaName: see note on parameter strings above
	void AssignParameter(CMachine *pMachine, int Group, int Param);
	void AssignParameter(char *MacName, char *ParaName);

	//Forget the current assignment
	void UnassignParameter();

	//Get pointers to things relating to current machine/parameter
	//If there is no current assignment, all return NULL
	CMachine *GetMachine();
	char const *GetMachineName();
	CMachineInfo const *GetMInfo();
	CMachineInterface *GetMInterface();
	CMachineParameter const *GetParamInfo();
	char const *GetParamString();

	//Get a string describing the current assignment (like "Bass 3->Cutoff")
	char const *GetAssignmentString();

	//Call CheckMachine on EVERY tick!!!!!! VERY important!
	//Returns false if the machine isn't there (eg user deleted it)
	bool CheckMachine();

	//Surround each bit of code where peer control will happen with if(GotParam())
	//Also use it to check if there is a current assignment
	bool GotParam();

	//Change the current parameter to the specified value
	//Track is ignored if we have a global parameter
	//"Non-hacky" method, only changes on tick
	void ControlChange_NextTick (int track, int value);

	//"Hacky" method, changes immediately
	void ControlChange_Immediate(int track, int value);

	const char *DescribeValue(int value);

	int min, max;
	int MapValue(float v);

private:
	int GetParamNames(CMachine *pmac, HWND DlgItem, int AddMessage, int TypesAllowed);
	int  FindMachine(HWND DlgItem, int SelectStringMessage);
	bool FindParam(HWND DlgItem, int SelectStringMessage);
	
	CMICallbacks *pCB;
	CMachine *ParentMac;

	bool HavePar;
	bool JustLoaded;
	char JustLoadedMacName[128];

	CMachine *BusMachine;
	int BusParNo, BusGroup;
	const CMachineParameter *BusPar;

	CMachineParameter dummypar;

	bool bRecord;
};








#endif
