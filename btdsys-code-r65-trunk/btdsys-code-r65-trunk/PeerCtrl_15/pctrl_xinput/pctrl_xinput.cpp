// pctrl_xinput.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

#ifdef _MANAGED
#pragma managed(push, off)
#endif

int abs(int x) { return (x > 0) ? x : -x ; }

class XInputPlug : public CPCtrlPlug
{
public:
	XInputPlug();
	virtual ~XInputPlug();
	virtual SPlugInfo *GetInfo();
	virtual bool Init();
	virtual void MenuCommand(int nCommand);
	virtual void ReadFileData(CPCtrlDataRead *dr);
	virtual void InitNoFileData();
	virtual void WriteFileData(CPCtrlDataWrite *dw);
	virtual void ValueChanged(int nAxis, float fValue);
	virtual void Timer(int numSamples);

protected:
	void CheckPads();
	bool m_connected[4];
	bool m_deadzone;
	DWORD m_lastpacket[4];
	XINPUT_VIBRATION m_rumble[4];
};

enum {
	axisLeftX,
	axisLeftY,
	axisRightX,
	axisRightY,
	axisLeftTrigger,
	axisRightTrigger,
	axisDpadX,
	axisDpadY,
	axisButtonStart,
	axisButtonBack,
	axisButtonLeftThumb,
	axisButtonRightThumb,
	axisButtonLeftShoulder,
	axisButtonRightShoulder,
	axisButtonA,
	axisButtonB,
	axisButtonX,
	axisButtonY,
	axisRumbleLeft,
	axisRumbleRight,
	NUM_AXES_PER_PAD
};

const char *axisnames[] = {
	"Left stick X",
	"Left stick Y",
	"Right stick X",
	"Right stick Y",
	"Left trigger",
	"Right trigger",
	"D-Pad X",
	"D-Pad Y",
	"Start button",
	"Back button",
	"Left stick button",
	"Right stick button",
	"Left shoulder button",
	"Right shoulder button",
	"A button",
	"B button",
	"X button",
	"Y button",
	"Left rumble speed",
	"Right rumble speed",
};

XInputPlug::XInputPlug()
{
}

XInputPlug::~XInputPlug()
{
}

SPlugInfo *XInputPlug::GetInfo()
{
	static SPlugInfo info = {
		PCTRL_VERSION,
		"XInput",
		"Ed Powley",
		"Plugin for XBox360 controller input",
		"Re-check for controllers\nToggle dead zones\nStop rumbling\nAbout...",
		PCPF_CALL_TIMER
	};
	return &info;
}

bool XInputPlug::Init()
{
	char txt[64];
	for (int c=0; c<4; c++)
	{
		m_rumble[c].wLeftMotorSpeed = m_rumble[c].wRightMotorSpeed = 0;
		m_lastpacket[c] = 0xDEAD;
		for (int a=0; a<NUM_AXES_PER_PAD; a++)
		{
			sprintf_s(txt,64,"Pad %i: %s",c+1,axisnames[a]);
			pCB->AddAxis(txt);
		}
	}

	CheckPads();

	return true;
}

void XInputPlug::ReadFileData(CPCtrlDataRead *dr)
{
	int version = dr->ReadInt();
	switch (version)
	{
	case 1:
		m_deadzone = (dr->ReadChar() != 0);
		break;
	default:
		MessageBox(NULL, "Version mismatch", "PeerCtrl XInput plugin", MB_OK | MB_ICONERROR);
		break;
	}
}

void XInputPlug::InitNoFileData()
{
	m_deadzone = true;
}

void XInputPlug::WriteFileData(CPCtrlDataWrite *dw)
{
	dw->Write((int)1);
	dw->Write((char)(m_deadzone ? 1 : 0));
}

void XInputPlug::MenuCommand(int nCommand)
{
	switch (nCommand)
	{
	case 0:
		{
			CheckPads();
			char txt[128];
			txt[0] = 0;
			for (int c=0; c<4; c++)
				sprintf_s(txt,128,"%sController %i is%s connected\n", txt,c+1, m_connected[c]?"":" not");
			MessageBox(NULL,txt,"PeerCtrl XInput plugin", MB_OK | MB_ICONINFORMATION);
		}
		break;
	case 1:
		{
			m_deadzone = !m_deadzone;
			char txt[64];
			sprintf_s(txt,64,"Dead zones are now %s", m_deadzone?"on":"off");
			MessageBox(NULL,txt,"PeerCtrl XInput plugin", MB_OK | MB_ICONINFORMATION);
		}
		break;
	case 2:
		{
			for (int c=0; c<4; c++)
			{
				pCB->SetValue(c+axisRumbleLeft,  0.0f);
				pCB->SetValue(c+axisRumbleRight, 0.0f);
				m_rumble[c].wLeftMotorSpeed = m_rumble[c].wRightMotorSpeed = 0;
				XInputSetState(c, &m_rumble[c]);
			}
		}
		break;
	case 3:
		MessageBox(NULL,"©2008 Ed Powley\nhttp://btdsys.cjb.net","PeerCtrl XInput plugin",MB_OK | MB_ICONINFORMATION);
		break;
	}
}

void XInputPlug::CheckPads()
{
	XINPUT_STATE state;
	for (int c=0; c<4; c++)
	{
		if (XInputGetState(c, &state) == ERROR_SUCCESS)
		{
			m_connected[c] = true;
			for (int a=0; a<NUM_AXES_PER_PAD; a++)
				pCB->ShowAxis(c*NUM_AXES_PER_PAD+a);
		}
		else
		{
			m_connected[c] = false;
			for (int a=0; a<NUM_AXES_PER_PAD; a++)
				pCB->HideAxis(c*NUM_AXES_PER_PAD+a);
		}
	}
}

void XInputPlug::Timer(int numSamples)
{
	XINPUT_STATE state;
	for (int c=0; c<4; c++)
	{
		if (m_connected[c] && XInputGetState(c, &state) == ERROR_SUCCESS && state.dwPacketNumber != m_lastpacket[c])
		{
			m_lastpacket[c] = state.dwPacketNumber;
			int o = c*NUM_AXES_PER_PAD;

			// dead zones
			if (m_deadzone)
			{
				if (abs(state.Gamepad.sThumbLX) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE && 
					abs(state.Gamepad.sThumbLY) < XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE )
					state.Gamepad.sThumbLX = state.Gamepad.sThumbLY = 0;

				if (abs(state.Gamepad.sThumbRX) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE && 
					abs(state.Gamepad.sThumbRY) < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE )
					state.Gamepad.sThumbRX = state.Gamepad.sThumbRY = 0;
			}

			pCB->SetValue(o+axisLeftX,  (float)state.Gamepad.sThumbLX / 65536.f + 0.5f);
			pCB->SetValue(o+axisLeftY,  (float)state.Gamepad.sThumbLY / 65536.f + 0.5f);
			pCB->SetValue(o+axisRightX, (float)state.Gamepad.sThumbRX / 65536.f + 0.5f);
			pCB->SetValue(o+axisRightY, (float)state.Gamepad.sThumbRY / 65536.f + 0.5f);

			pCB->SetValue(o+axisLeftTrigger,  (float)state.Gamepad.bLeftTrigger  / 255.f);
			pCB->SetValue(o+axisRightTrigger, (float)state.Gamepad.bRightTrigger / 255.f);

			WORD btns = state.Gamepad.wButtons;

			if (btns & XINPUT_GAMEPAD_DPAD_LEFT)
				pCB->SetValue(o+axisDpadX, 0.0f);
			else if (btns & XINPUT_GAMEPAD_DPAD_RIGHT)
				pCB->SetValue(o+axisDpadX, 1.0f);
			else
				pCB->SetValue(o+axisDpadX, 0.5f);

			if (btns & XINPUT_GAMEPAD_DPAD_DOWN)
				pCB->SetValue(o+axisDpadY, 0.0f);
			else if (btns & XINPUT_GAMEPAD_DPAD_UP)
				pCB->SetValue(o+axisDpadY, 1.0f);
			else
				pCB->SetValue(o+axisDpadY, 0.5f);

			pCB->SetValue(o+axisButtonStart,		(btns & XINPUT_GAMEPAD_START)			? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonBack,			(btns & XINPUT_GAMEPAD_BACK)			? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonLeftThumb,	(btns & XINPUT_GAMEPAD_LEFT_THUMB)		? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonRightThumb,	(btns & XINPUT_GAMEPAD_RIGHT_THUMB)		? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonLeftShoulder,	(btns & XINPUT_GAMEPAD_LEFT_SHOULDER)	? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonRightShoulder,(btns & XINPUT_GAMEPAD_RIGHT_SHOULDER)	? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonA,			(btns & XINPUT_GAMEPAD_A)				? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonB,			(btns & XINPUT_GAMEPAD_B)				? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonX,			(btns & XINPUT_GAMEPAD_X)				? 1.0f : 0.0f);
			pCB->SetValue(o+axisButtonY,			(btns & XINPUT_GAMEPAD_Y)				? 1.0f : 0.0f);
		}
	}
}

void XInputPlug::ValueChanged(int nAxis, float fValue)
{
	int c = nAxis / NUM_AXES_PER_PAD;
	int a = nAxis % NUM_AXES_PER_PAD;
	if (a == axisRumbleLeft)
	{
		m_rumble[c].wLeftMotorSpeed = (WORD)(fValue * 65535.f);
		XInputSetState(c, &m_rumble[c]);
	}
	if (a == axisRumbleRight)
	{
		m_rumble[c].wRightMotorSpeed = (WORD)(fValue * 65535.f);
		XInputSetState(c, &m_rumble[c]);
	}
}

DLL_EXPORT(XInputPlug)

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

