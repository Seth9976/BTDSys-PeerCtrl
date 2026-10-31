#ifndef __ENVELOPEH
#define __ENVELOPEH

#include "fuzzpilz_globals.h"
#include "windows.h"

enum
{
	elmOff,
		elmSustain,
		elmLoopForward,
		elmLoopBackward,
		elmLoopBidir,
		NumEnvLoopModes
};

static const char* EnvLoopModeNames[NumEnvLoopModes]=
{
	"&Off",
		"&Sustain (no loop)",
		"Loop &Forward",
		"Loop &Backward",
		"Loop Bi&dir"
};

enum
{
	cermParam,
		cermParamLoop,
		cermContinue,
		cermJump,
		NumEnvReleaseModes
};

static const char* EnvReleaseModeNames[NumEnvReleaseModes]=
{
	"Release &param",
		"Param with &loop",
		"&Continue",
		"&Jump"
};

enum	// parametric envelope phases
{
	epOff,
		epBackDown,
		epAttack,
		epHold,
		epDecay1,
		epDecay2,
		epSustain,
		epRelease,
		epCustom,
		epCustomRelease,
		epCustomReleaseLoop
};

enum	// envelope retrigger modes
{
	ermDown,
		ermAttack,
		ermContinue
};

static const int NumCustomEnvelopes=64;

class CEnvelopePoint
{
public:
	CEnvelopePoint(CEnvelopePoint *pred,CEnvelopePoint *succ,float px,float py,float s);
	~CEnvelopePoint();
	CEnvelopePoint* GetPoint(int num);
	CEnvelopePoint* GetPoint(float px,float py,float rx,float ry);
	void SetPoint(int num,float px,float py,float s);
	void LowerIndex();
	void RaiseIndex();
	void DestroyUtterly();
	int AddPoint(float px,float py,float s);
	int DelPoint(int num);
	int Dir();
public:
	float posx,posy;
	float slope;
	int index;
	CEnvelopePoint *pprev,*pnext;
};

class CCustomEnvelopeShape
{
public:
	CCustomEnvelopeShape();
	~CCustomEnvelopeShape();

	CEnvelopePoint* GetFirstPoint(float px);
	CEnvelopePoint* GetPointAt(float px,float py,float rx,float ry);

	void AddPoint(float px,float py,float s);
	void DelPoint(int num);

	void Reset();
	void ToDefault();
	void Load(CMachineDataInput *pi,int v);
	void Save(CMachineDataOutput *po);
	LOADERR Load(const char *filename);
	void Save(const char *filename);
	void Copy(CCustomEnvelopeShape *pshape);

	void Test();

	float GetSustain();
	int GetNext(int &numpoint,int &dir,int &cnt,int &err,float &mul,float lev,float &tlev,float &dlev,float &slope,float speed,int spt,int releasing);
	int ReleaseJump(int &numpoint,int &dir,int &cnt,int &err,float &mul,float lev,float &tlev,float &dlev,float &slope,int spt);
	bool Sustaining(int numpoint);
private:
public:
	CEnvelopePoint *points;
	int loopstart,loopend,suspoint;
	int loopmode,releasemode;
	int numpoints;
private:
};

class CEnvelopeEditor
{
public:
	CEnvelopeEditor(CCustomEnvelopeShape *pShapes,CEnvelopeEditor **pPt,const char *macname);
	~CEnvelopeEditor();
	void Show(const char *macname);
	void DrawEnv(HDC hDC,RECT rect);

	void do_scroll(int amount);
	void do_zoom(int xamount,int yamount);
	void do_wheel_zoom(short amount,int xpos);
	void StartSlopeDrag(int x);
	bool do_slope_drag(int amount);
	bool StartDrag(int x,int y);
	bool do_drag(int xpos,int ypos);

	void AddPoint(int x,int y);
	void DelPoint(int num);

	CEnvelopePoint* GetMenuPoint();
	void MenuAddPoint();
	void MenuDelPoint();
	void MenuSetLoopMode(int lm);
	void MenuSetReleaseMode(int rm);
	void MenuSetLoopStart();
	void MenuSetLoopEnd();
	void MenuSetSustainLev();

	void SetEdnum(int i);
	int GetEdnum();

	void ToDefault();
	void Copy();
	void Paste();
	void Test();

	void Import();
	void Export();
private:
	float x_to_px(int x);
	float y_to_py(int y);
	int px_to_x(float px);
	int py_to_y(float py);
public:
	float zoom;
	float scrollpos;
	int oldmx,oldmy;
	int menux,menuy;
	CEnvelopePoint *slopedragpoint;
	float dragmax,dragmin;
	CEnvelopePoint *dragpoint;
	CEnvelopePoint *menupoint;
	bool snap;
	bool moverest;
private:
	int ednum;
	int w,h;
	CCustomEnvelopeShape *pshapes;
	CCustomEnvelopeShape *pshape;
	CEnvelopeEditor **ppt;
	HWND dhandle;
};

class CEnvelope
{
public:
	CEnvelope();
	void SetEnvelope(int AttackLength,int HoldLength,int Decay1Length,float Breakpoint,int Decay2Length,float Sustain,int ReleaseLength,CCustomEnvelopeShape *pShape,float Speed,int Spt);
	void Reset();
	void Trigger(int triggermode);
	void Release();
	float NextSample();
	bool Active();
	bool Sustaining();
	float GetSustain();
private:
	int attacks;
	float attackd;
	int holds;
	int decay1s;
	float decay1m;
	float breakpoint;
	int decay2s;
	float decay2m;
	float sus;
	float releasem;
	
	int envphase;
	float level;
	int cnt;
	float bdd;

	int spt;
	CCustomEnvelopeShape *pshape;
	float tlev,dlev;
	int custpoint;
	float custm;
	int custdir;
	int custerr;
	float slope;
	float custrelev;
	float speed;
};

#endif