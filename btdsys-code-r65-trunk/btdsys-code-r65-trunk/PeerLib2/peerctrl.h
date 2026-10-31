/*
Copyright (c) 2003-2008, Edward Powley
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

#pragma once

#include <vector>
#include <set>
#include <string>
#include <windows.h>

#ifndef __MACHINE_INTERFACE_H
#include "../mdk/MachineInterface.h"
#endif

/** \mainpage
\section intro Introduction
	<b>BTDSys Peer Library</b> is a library for programming peer machines for 
	<a href="http://www.buzzmachines.com">Buzz</a>.
\section changes Changes since version 1
	<ul><li>Completely rewritten, no pointer hacks whatsoever. Incompatible with all Buzz
		builds before 1042, especially the October 2000 one.</li>
	<li>There is no longer any need to call CPeerCtrl::CheckMachine() at regular intervals.
		Indeed, there's no such thing as CPeerCtrl::CheckMachine() now.</li>
	<li>However, you must use CPeerCtrl's handlers for add and delete machine events. See
		CPeerCtrl::SetEventHandlers().</li>
	<li>There is a new form of constructor which takes no arguments -- you can instead provide
		the arguments via CPeerCtrl::Init(). So you no longer need to allocate all your
		CPeerCtrl objects on the heap using the "new" operator (although that way still works,
		so no need to change old code).</li>
	<li>Better handling of target tracks. You can specify a track number in
		CPeerCtrl::AssignParameter(), and it becomes the default for control changes and is
		stored in the song file.</li>
	<li>CPeerCtrl::ControlChange_NextTick() is deprecated, although it is still available if
		you add PEERLIB_NEXTTICK to your project's C++ preprocessor definitions.</li>
	<li>Batch mode for ControlChange_Immediate, intended for controlling multiple parameters on
		the same target machine. See CPeerCtrl::BeginImmediateBatch().</li>
	<li>Automatically interfaces with a new version of BTDSys PAL (Peer Assignment List). Unlike
		the old version of PAL, no extra code or "palpal" DLLs are required.</li>
	</ul>
\section porting Upgrading
	"Porting" old machines to PeerLib2 should be relatively simple:
	<ul><li>Include the new peerctrl.h, remove peerctrl.lib from your linker options, and add
		peerctrl.cpp to your project.</li>
	<li>Remove all calls to CPeerCtrl::CheckMachine() (consider using CPeerCtrl::GotParam() if
		the call was a condition for an if statement).</li>
	<li>Add this to your Init or MDKInit function:
		\code CPeerCtrl::SetEventHandlers(pCB); \endcode
		or if you have your own event handlers for gAddMachine and gDeleteMachine, call
		CPeerCtrl::OnAddMachine() and CPeerCtrl::OnDeleteMachine() from there.</li>
	<li>Remove all calls to CPeerCtrl::ControlChange_NextTick(), and make sure you only call
		CPeerCtrl::ControlChange_Immediate() from your machine's Work() function (i.e. not from
		Tick()). Optionally, you may wish to enclose all calls to
		CPeerCtrl::ControlChange_Immediate() in a given Work() call between
		CPeerCtrl::BeginImmediateBatch() and CPeerCtrl::EndImmediateBatch().</li>
	<li>(Optional but recommended) Give each CPeerCtrl instance a label to be displayed by PAL,
		using CPeerCtrl::SetLabel() or CPeerCtrl::SetLabelAsTrackNumber().</li>
	<li>(Optional but recommended) Add a new menu command, "Show PAL...", whose handler calls
		\code CPeerCtrl::ShowPAL(); \endcode </li>
	</ul>
	And it's as simple as that :)
*/


enum
{
	GROUP_GLOBAL = 1,
	GROUP_TRACK = 2,
	GROUP_ATTRIBUTE = 'A', // strange choice, for backwards compatibility

	ALLOW_SWITCH	= 1 << 0,
	ALLOW_BYTE		= 1 << 1,
	ALLOW_NOTE		= 1 << 2,
	ALLOW_WORD		= 1 << 3,

	ALLOW_GLOBAL	= 1 << 4,
	ALLOW_TRACK		= 1 << 5,
	ALLOW_ATTRIBUTES = 1 << 6,

	ALLOW_ALL_TYPES = ALLOW_SWITCH | ALLOW_BYTE | ALLOW_NOTE | ALLOW_WORD,
	ALLOW_ALL_GROUPS = ALLOW_GLOBAL | ALLOW_TRACK,

	TRACK_DEFAULT = -1,

	EVENT_HANDLER_ADD		= 1 << 0,
	EVENT_HANDLER_DELETE	= 1 << 1,
	EVENT_HANDLER_RENAME	= 1 << 2,
	EVENT_HANDLER_UNDELETE	= 1 << 3,
	ALL_EVENT_HANDLERS = EVENT_HANDLER_ADD | EVENT_HANDLER_DELETE | EVENT_HANDLER_RENAME | EVENT_HANDLER_UNDELETE,
};

enum PeerEvent
{
	TargetAssigned,
	TargetUnassigned,
	TargetLoaded,
	TargetRemoved,
	TargetRestored,
	TargetRenamed,
	InstanceDestroyed,
};

/** Thrown by various CPeerCtrl member functions when something goes terribly wrong. */
class PeerCtrlException : public std::exception
{
public:
	PeerCtrlException(const char* msg) { m_msg = msg; }
	
	/// Retrieve a text string describing the error.
	virtual const char* what() const throw() { return m_msg; }

protected:
	const char* m_msg;
};

/** Used to store data about old assignments for Buzz undelete support */
struct SPeerUndoData
{
	SPeerUndoData(CMachine *pmac, int group, int paramn, int track)
		: m_targetmac(pmac), m_group(group), m_paramn(paramn), m_track(track) { }

	~SPeerUndoData() { }

	CMachine *m_targetmac;
	int m_group, m_paramn, m_track;
};

/** Encapsulates an assignment to a single parameter.
*/
class CPeerCtrl
{
public:
	/// \name Event handlers

	/**	You must call this as a handler for the gAddMachine event. During song loading, if your peer
		machine loads before the target machine, we use OnAddMachine() to watch for the target machine
		being loaded.
		\param pmac The added machine.
		\returns true
		\sa SetEventHandlers()
	*/
	static bool OnAddMachine(CMachine *pmac);

	/**	You must call this as a handler for the gDeleteMachine event. It unassigns CPeerCtrl instances
		when their target machines are deleted.
		\param pmac The deleted machine.
		\returns true
		\sa SetEventHandlers()
	*/
	static bool OnDeleteMachine(CMachine *pmac);

	/** Optionally call this as a handler for the gRenameMachine event. It simply calls OnAssignmentChange().
		\param pmac The renamed machine.
		\returns true
		\sa SetEventHandlers()
	*/
	static bool OnRenameMachine(CMachine *pmac);

	/** You must call this as a handler for the gUndeleteMachine event. It checks to whether any
		instance was targetting the restored machine and calls ReassignQuery() for any that it finds.
		If ReassignQuery() returns true, the assignment is restored.
		\param pmac The restored machine.
		\returns true
		\sa SetEventHandlers()
	*/
	static bool OnUndeleteMachine(CMachine *pmac);

	/** Called when a previously assigned machine is undeleted. Override this if you want to prompt the user
		whether to reassign or not.
		\param data SPeerUndoData object containing info about the old assignment.
		\returns true to reassign the restored machine, false to keep current assignment
	*/
	virtual bool ReassignQuery(SPeerUndoData &data) { return true; }

	/** Set OnAddMachine() and OnDeleteMachine() as handlers for the appropriate events, by calling
		CMICallbacks::SetEventHandler. In most cases,
		you can just call SetEventHandlers() in Init or MDKInit and forget about event handlers
		from that point on. If you need to do your own handling of those events, make sure you call
		OnAddMachine() and OnDeleteMachine() from your own handlers as appropriate.
		\param pCB Your peer machine's pCB member.
		\param which Which event handlers to set. "Or" together one or more of EVENT_HANDLER_ADD,
			EVENT_HANDLER_DELETE, and EVENT_HANDLER_RENAME.
	*/
	static void SetEventHandlers(CMICallbacks *pCB, int which = ALL_EVENT_HANDLERS);

	/// \name Initialisation

	/**	Constructor suitable for creating instances before mi::Init() (eg as members of your
		mi class). If you use this form, you must remember to call Init() before using this instance.
	*/
	CPeerCtrl();

	/**	Constructor suitable for creating instances within or after mi::Init(). This form calls
		Init() for you.
		\param pmac Your machine's CMachine pointer (obtained by pCB->GetThisMachine()).
		\param pmi You machine's CMachineInterface pointer.
	*/
	CPeerCtrl(CMachine *pmac, CMachineInterface *pmi);

	/** Destructor.
	*/
	~CPeerCtrl();

	/** Initialise this instance. Call this if <em>and only if</em> you use the CPeerCtrl()
		constructor.
		\param pmac Your machine's CMachine pointer (obtained by pCB->GetThisMachine()).
		\param pmi You machine's CMachineInterface pointer.
	*/
	void Init(CMachine *pmac, CMachineInterface *pmi);

	/**	Set a human-readable label for this CPeerCtrl instance. This label will be shown in PAL,
		for example. For a typical peer machine with one CPeerCtrl per track, the label will be
		a string of the form "Track %i" where %i is the track number; such a label can easily
		be set with SetLabelAsTrackNumber(). */
	void SetLabel(const std::string& label);

	/** As SetLabel(const std::string&), but takes a null-terminated C string. The string is
		copied, so you may safely overwrite it or free it after calling this function. */
	void SetLabel(const char* label);

	/** Set the label (see SetLabel()) to a string of the form "Track %i", where %i is replaced
		by the integer <tt>t</tt>. */
	void SetLabelAsTrackNumber(int t);

	/// \name Song file data

	/**	Read settings from the song file. Call this in your mi::Init() function.
		\param pi The data input object
		\sa WriteFileData()
	*/
	void ReadFileData(CMachineDataInput * const pi);

	/** Give CPeerCtrl some data to treat as if it was read by ReadFileData(). This is intended
		for porting old peer machines to PeerLib, and should not be used in new machines.
		\param macname The target machine name, or NULL if not assigned.
		\param paramnum The parameter number, as returned by GetParamNumber_Grouped().
		\param group The parameter group (GROUP_GLOBAL or GROUP_TRACK).
		\sa GetMachineName(), GetParamNumber_Grouped(), GetGroup()
	*/
	void PushFileData(const char* macname, int paramnum, int group);

	/**	Write settings to the song file. Call this in your mi::Save() function.
		\param po The data output object
		\param backwardsCompatible Set to <tt>true</tt> to write file data compatible with
			version 1 of the library. In particular, assigned track numbers will <em>not</em>
			be saved. This is mainly for old machines, which presumably take care of their own
			track number assignments.
		\sa ReadFileData()
	*/
	void WriteFileData(CMachineDataOutput * const po, bool backwardsCompatible = false);

	/** You should manually call this after song/template load or paste (from CMachineInterfaceEx::ImportFinished())
		It loops through all instances and restores saved assignments. If not called, pasted/imported machines
		may lose their assignment settings.
	*/
	static void OnImportFinished();

	/// \name Setting assignments

	/**	Set the current assignment.
		\param pMachine The target machine.
		\param group The group of the target parameter (one of GROUP_GLOBAL, GROUP_TRACK or
			GROUP_ATTRIBUTE).
		\param param The number of the target parameter, in the format described for
			GetParamNumber_Ungrouped().
		\param track The target track number. Can be omitted.
		\sa UnassignParameter
	*/
	void AssignParameter(CMachine *pMachine, int group, int param, int track = TRACK_DEFAULT);

	/** Clear the current assignment.
		\sa AssignParameter(CMachine*, int, int, int)
	*/
	void UnassignParameter();

	/// \name Getting assignments

	/// Get the instance label
	char const *GetLabel();
	/// Get the current target machine.
	CMachine *GetMachine();
	/// Get the name of the current target machine.
	char const *GetMachineName();
	/// Get the info struct for the current target machine.
	CMachineInfo const *GetMInfo();
	/// Get the info struct for the current target parameter.
	CMachineParameter const *GetParamInfo();
	/// Get a string describing the current target parameter.
	char const *GetParamString();

	/**	Get a string describing the current target machine and parameter. Typically this string
		is displayed in the machine's context menu and assignment dialog.
	*/
	char const *GetAssignmentString();

	/**	Get the currently assigned parameter number, with the first track parameter being number 0.
		This is the format used by CMICallbacks::ControlChange, for example, and is also the format
		expected by AssignParameter(CMachine*, int, int, int).
		\return The parameter number, or -1 if no parameter is assigned.
		\sa GetParamNumber_Ungrouped
	*/
	int GetParamNumber_Grouped();

	/** Get the currently assigned parameter number, with the first track parameter numbered one
		greater than the last global parameter. This is the format used by
		CMachineInterface::DescribeValue, for example.
		\return The parameter number, or -1 if no parameter is assigned.
		\sa GetParamNumber_Grouped
	*/
	int GetParamNumber_Ungrouped();

	/** Get the group of the currently assigned parameter.
	*/
	int GetGroup();

	/** Get the currently assigned track number.
	*/
	int GetTrack();

	/**	Check whether we have a current assignment.
		\return true if there is a current assignment, otherwise false.
	*/
	bool GotParam();

	/// \name Control change batching

	/**	Enter batch mode for "immediate" control changes. After calling this function, calls to
		ControlChange_Immediate() will not take effect until the next call to EndImmediateBatch().
		CMachineInterface::Tick() will only be called once per target machine; for example, this
		might be used to ensure that the target machine receives the note and the velocity at the
		same time.
		\code
CPeerCtrl::BeginImmediateBatch();
for (int t=0; t<numTracks; t++)
{
	track[t]->peerNote->ControlChange_Immediate(track[t]->notenum);
	track[t]->peerVelo->ControlChange_Immediate(track[t]->velocity);
}
CPeerCtrl::EndImmediateBatch();
		\endcode
		\sa EndImmediateBatch()
	*/
	static void BeginImmediateBatch();

	/**	Exit batch mode for "immediate" control changes. See BeginImmediateBatch() for details.
	*/
	static void EndImmediateBatch();

	/// \name Control changes

#	ifdef PEERLIB_NEXTTICK
	/**	Send a parameter change on the next tick. Now that Buzz can do immediate control changes
		natively, this function is not as useful as it once was. In fact it is deprecated, and
		is only available if you add <tt>PEERLIB_NEXTTICK</tt> to your project's C++
		preprocessor definition list (Project properties -> C/C++ -> Preprocessor ->
		Preprocessor Definitions in VC++2005). */
	void ControlChange_NextTick (int track, int value);
	
	///	As ControlChange_NextTick(int,int), but with TRACK_DEFAULT as the track number.
	void ControlChange_NextTick (int value) { ControlChange_NextTick(TRACK_DEFAULT, value); }
#	endif
	
	/**	Send a parameter change "immediately".
	*/
	void ControlChange_Immediate(int track, int value);

	///	As ControlChange_Immediate(int,int), but with TRACK_DEFAULT as the track number.
	void ControlChange_Immediate(int value) { ControlChange_Immediate(TRACK_DEFAULT, value); }

	/// \name Notifications

	/** Override this to do something when assignment settings change.
		\param trigger PeerEvent value indicating what triggered the call to OnChangeAssignment():
			TargetAssigned/TargetUnassigned - User changed assignment settings
			TargetLoaded					- Assignment was successfully restored (song load, template import, paste)
			TargetRemoved					- Target machine was deleted
			TargetRestored					- A previously targetted machine was undeleted
			TargetRenamed					- The target machine was renamed
			InstanceDestroyed				- The CPeerCtrl instance destructor was called
	*/
	virtual void OnChangeAssignment(PeerEvent trigger) {}

	/// \name GUI helper functions

	/** Populate a combo box with machine names.
		\param hWnd The handle of your dialog window.
		\param ComboID The dialog item ID of your combo box.
		\param ExcludeName A machine name to exclude from the list. Typically used to prevent a peer
			machine from controlling itself.
		\returns The number of combo box items added.
	*/
	int  GetMachineNamesToCombo	(HWND hWnd, int ComboID, const char *ExcludeName);

	/** Populate a list box with parameter names.
		\param pmac The machine whose parameters to list.
		\param hWnd The handle of your dialog window.
		\param ListID The dialog item ID of your list box.
		\param TypesAllowed Any combination of the ALLOW_ constants.
		\returns The number of list items added.
	*/
	int  GetParamNamesToList	(CMachine *pmac, HWND hWnd, int ListID , int TypesAllowed);

	/// Like GetParamNamesToList(), but for populating a combo box.
	int  GetParamNamesToCombo	(CMachine *pmac, HWND hWnd, int ComboID, int TypesAllowed);

	/** Select the currently assigned machine in a combo box previously populated by
		GetMachineNamesToCombo().
		\param hWnd The handle of your dialog window.
		\param ComboID The dialog item ID of your combo box.
	*/
	bool SelectMachineInCombo	(HWND hWnd, int ComboID);

	/** Select the currently assigned parameter in a combo box previously populated by
		GetParamNamesToList().
		\param hWnd The handle of your dialog window.
		\param ListID The dialog item ID of your list box.
	*/
	bool SelectParameterInList	(HWND hWnd, int ListID);

	/// Like SelectParameterInList(), but for a combo box.
	bool SelectParameterInCombo	(HWND hWnd, int ComboID);

	/** Set the current assignment, based on strings previously written into dialog items with
		GetMachineNamesToCombo() and GetParamNamesToList() or GetParamNamesToCombo().
		\param MacName The machine name string.
		\param ParaName The parameter name string.
		\param track The track number, or TRACK_DEFAULT to assign to no particular track.
	*/
	void AssignParameter(char *MacName, char *ParaName, int track = TRACK_DEFAULT);

	/// \name PAL

	/** Show the GUI for BTDSys Peer Assignment List. */
	static void ShowPAL();

	/// @}

protected:
	bool m_initialised;
	CMICallbacks *pCB;
	CMachine *m_parentmac;
	CMachineInterface *m_parentmi;
	std::string m_label;

	CMachine *m_targetmac;
	int m_paramn, m_group, m_track;
	char m_loadedname[256];
	
	std::vector<SPeerUndoData> m_undoList;

	CMachineParameter m_attrAsParam;

	int GetParamNamesToDlgItem(CMachine *pmac, HWND DlgItem, int AddMessage, int TypesAllowed);
	int  FindMachine(HWND DlgItem, int SelectStringMessage);
	bool FindParam(HWND DlgItem, int SelectStringMessage);

	bool FindJustLoadedMachine();

protected:
	static std::vector<CPeerCtrl*> s_instances;

	static bool s_initialised;
	static void InitStatic(CMachine *pmac, CMachineInterface *pmi);
	static int s_hostversion;
	
	static bool s_immediatebatchmode;
	static std::set<CMachine*> s_immediatebatch, s_attributebatch;

	static HMODULE s_palModule;

	// Function pointers from PAL dll
	typedef void (__cdecl *pf_addCMICallbacks)(CMICallbacks*);
	typedef void (__cdecl *pf_setPeerAssignment)(
		void* ctrl, CMachine* source, const char* label,
		CMachine* target, int group, int param, int track);

	// Notify PAL of the current assignment.
	void TellPAL();
};
