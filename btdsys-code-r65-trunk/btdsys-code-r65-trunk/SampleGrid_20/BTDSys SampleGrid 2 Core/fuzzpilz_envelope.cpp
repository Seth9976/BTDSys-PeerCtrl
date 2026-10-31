#include "stdafx.h"
#include "fuzzpilz_envelope.h"
#include "resource.h"
#include <commdlg.h>
#include "common.h"

static const int POINTRANGE=6;
static const int OTPOINTRANGE=12;
static const int OTWIDTH=2;
static const float MIN_ZOOM=0.5f;
static const float MAX_ZOOM=32.0f;

CEnvelope::CEnvelope()
{
	SetEnvelope(10,0,100,0.75f,100,0.5f,1000,NULL,1.0f,5250);
	Reset();
}

void CEnvelope::SetEnvelope(int AttackLength,int HoldLength,int Decay1Length,float Breakpoint,int Decay2Length,float Sustain,int ReleaseLength,CCustomEnvelopeShape *pShape,float Speed,int Spt)
{
	attacks=AttackLength;
	if(attacks<1)attacks=1;
	holds=HoldLength;
	decay1s=Decay1Length;
	if(decay1s<1)decay1s=1;
	breakpoint=Breakpoint;
	if(breakpoint<0.001f)breakpoint=0.001f;
	decay2s=Decay2Length;
	if(decay2s<1)decay2s=1;
	sus=Sustain;
	if(sus<0.001f)sus=0.001f;
	float r=Speed*ReleaseLength;
	if(r<128)r=128;
	releasem=powf(0.001f,1.0f/r);
	pshape=pShape;
	speed=Speed;
	spt=Spt;
}

void CEnvelope::Reset()
{
	envphase=epOff;
	level=0;
	cnt=0;
	bdd=0;
	custerr=0;
	custm=1;
	tlev=1;
	dlev=0;
	custpoint=0;
	custdir=1;
}

void CEnvelope::Trigger(int triggermode)
{
	if((envphase!=epOff)&&(level>0.00001f))
	{
		if(triggermode==ermDown)
		{
			if(envphase==epCustomReleaseLoop)level=level*custrelev;
			envphase=epBackDown;
			cnt=256;
			custerr=256;
			bdd=-level/256;
			return;
		}
		else if((triggermode==ermAttack)||(envphase==epRelease)||(envphase>epCustom))
		{
			if(pshape)
			{
				if(envphase==epCustomReleaseLoop)level=level*custrelev;
				envphase=epCustom;
				custpoint=0;
				custerr=0;
				custdir=1;
				if(pshape->GetNext(custpoint,custdir,cnt,custerr,custm,level,tlev,dlev,slope,speed,spt,0))envphase=epOff;
			}
			else
			{
				float a=speed*attacks;
				if(a<128)a=128;
				cnt=(int)((1.0f-level)*a);
				if(cnt>0)
				{
					envphase=epAttack;
					attackd=(1.0f-level)/cnt;
					return;
				}
				else
				{
					envphase=epDecay1;
					level=1.0f;
					cnt=(int)(decay1s*speed);
					if(cnt<128)cnt=128;
					decay1m=powf(breakpoint,1.0f/cnt);
					return;
				}
			}
		}
	}
	else
	{
		if(pshape)
		{
			envphase=epCustom;
			custpoint=0;
			custerr=0;
			level=0;
			custdir=1;
			if(pshape->GetNext(custpoint,custdir,cnt,custerr,custm,level,tlev,dlev,slope,speed,spt,0))envphase=epOff;
		}
		else
		{
			envphase=epAttack;
			level=0;
			cnt=(int)(speed*attacks);
			if(cnt<128)cnt=128;
			attackd=1.0f/cnt;
		}
	}
}

void CEnvelope::Release()
{
	if(envphase==epCustom)if(pshape)if(pshape->releasemode!=cermParam)
	{
		if(pshape->ReleaseJump(custpoint,custdir,cnt,custerr,custm,level,tlev,dlev,slope,spt))envphase=epRelease;
		else
		{
			switch(pshape->releasemode)
			{
			case cermParamLoop:
				custrelev=1;
				envphase=epCustomReleaseLoop;
				break;
			case cermJump:
			case cermContinue:
			default:
				envphase=epCustomRelease;
				break;
			}
		}
		return;
	}
	if((envphase!=epOff)&&(envphase!=epRelease)&&(envphase<epCustom))
	{
		envphase=epRelease;
		cnt=0;
	}
}

float CEnvelope::NextSample()
{
	if(envphase==epCustom)
	{
		cnt--;
		float t=cnt*custm;
		if(slope<0)
		{
			t=1-t;
			float s=t*t;s*=s;
			t=1+t*(slope*(s-1)-1);
		}
		else
		{
			float s=t*t;s*=s;
			t*=1+slope*(s-1);
		}
		level=tlev+t*dlev;
		if(cnt<0)
		{
			if(pshape)
			{
				if(pshape->GetNext(custpoint,custdir,cnt,custerr,custm,level,tlev,dlev,slope,speed,spt,0))envphase=epRelease;
			}
			else envphase=epRelease;
		}
		return level;
	}
	else if(envphase==epCustomRelease)
	{
		cnt--;
		float t=cnt*custm;
		if(slope<0)
		{
			t=1-t;
			float s=t*t;s*=s;
			t=1+t*(slope*(s-1)-1);
		}
		else
		{
			float s=t*t;s*=s;
			t*=1+slope*(s-1);
		}
		level=tlev+t*dlev;
		if(cnt<0)
		{
			if(pshape)
			{
				if(pshape->GetNext(custpoint,custdir,cnt,custerr,custm,level,tlev,dlev,slope,speed,spt,1))envphase=epRelease;
			}
			else envphase=epRelease;
		}
		return level;
	}
	else if(envphase==epCustomReleaseLoop)
	{
		cnt--;
		float t=cnt*custm;
		if(slope<0)
		{
			t=1-t;
			float s=t*t;s*=s;
			t=1+t*(slope*(s-1)-1);
		}
		else
		{
			float s=t*t;s*=s;
			t*=1+slope*(s-1);
		}
		level=tlev+t*dlev;
		if(cnt<0)
		{
			if(pshape)
			{
				if(pshape->GetNext(custpoint,custdir,cnt,custerr,custm,level,tlev,dlev,slope,speed,spt,0))envphase=epRelease;
			}
			else envphase=epRelease;
		}
		custrelev*=releasem;
		if(custrelev<0.00001f)
		{
			envphase=epOff;
			return 0;
		}
		return level*custrelev;
	}
	else if(envphase==epBackDown)
	{
		level+=bdd;
		cnt--;
		if(cnt<0)
		{
			if(pshape)
			{
				custpoint=0;
				level=0;
				envphase=epCustom;
				custdir=1;
				if(pshape->GetNext(custpoint,custdir,cnt,custerr,custm,level,tlev,dlev,slope,speed,spt,0))envphase=epOff;
			}
			else
			{
				cnt=(int)(speed*attacks-256);
				if(cnt<128)cnt=128;
				level=0;
				attackd=1.0f/cnt;
				envphase=epAttack;
			}
		}
		return level;
	}
	else if(envphase==epAttack)
	{
		level+=attackd;
		cnt--;
		if(cnt<0)
		{
			if(holds>0)
			{
				cnt=(int)(speed*holds);
				if(cnt<1)cnt=1;
				level=1.0f;
				envphase=epHold;
			}
			else
			{
				cnt=(int)(speed*decay1s);
				if(cnt<128)cnt=128;
				level=1.0f;
				decay1m=powf(breakpoint,1.0f/cnt);
				envphase=epDecay1;
			}
		}
		return level;
	}
	else if(envphase==epHold)
	{
		cnt--;
		if(cnt<0)
		{
			cnt=(int)(speed*decay1s);
			if(cnt<128)cnt=128;
			level=1.0f;
			decay1m=powf(breakpoint,1.0f/cnt);
			envphase=epDecay1;
		}
		return 1.0f;
	}
	else if(envphase==epDecay1)
	{
		level*=decay1m;
		cnt--;
		if(cnt<0)
		{
			cnt=(int)(speed*decay2s);
			if(cnt<128)cnt=128;
			level=breakpoint;
			decay2m=powf(sus/breakpoint,1.0f/cnt);
			envphase=epDecay2;
		}
		return level;
	}
	else if(envphase==epDecay2)
	{
		level*=decay2m;
		cnt--;
		if(cnt<0)
		{
			cnt=0;
			level=sus;
			envphase=epSustain;
		}
		return level;
	}
	else if(envphase==epSustain)
	{
		return sus;
	}
	else if(envphase==epRelease)
	{
		level*=releasem;
		if(level<0.00001f)
		{
			envphase=epOff;
			return 0;
		}
		return level;
	}
	envphase=epOff;
	return 0;
}

bool CEnvelope::Active()
{
	return envphase!=epOff;
}

bool CEnvelope::Sustaining()
{
	if(pshape)return pshape->Sustaining(custpoint);
	return envphase==epSustain;
}

float CEnvelope::GetSustain()
{
	if(envphase<epCustom)return sus;
	if(pshape)return pshape->GetSustain();
	return 0;
}

CEnvelopePoint::CEnvelopePoint(CEnvelopePoint *pred,CEnvelopePoint *succ,float px,float py,float s)
{
	pprev=pred;
	pnext=succ;
	if(succ)succ->pprev=this;
	if(pred)pred->pnext=this;
	posx=px;
	posy=py;
	slope=s;
	if(pred)index=pred->index;else index=-1;
	RaiseIndex();
}

CEnvelopePoint::~CEnvelopePoint()
{
	if(pprev)pprev->pnext=pnext;
	if(pnext)
	{
		pnext->pprev=pprev;
		pnext->LowerIndex();
	}
}

void CEnvelopePoint::LowerIndex()
{
	index--;
	if(pnext)pnext->LowerIndex();
}

void CEnvelopePoint::RaiseIndex()
{
	index++;
	if(pnext)pnext->RaiseIndex();
}

void CEnvelopePoint::DestroyUtterly()
{
	if(pnext)pnext->DestroyUtterly();
	delete this;
}

CEnvelopePoint* CEnvelopePoint::GetPoint(int num)
{
	if(num==index)return this;
	if(pnext)return pnext->GetPoint(num);
	return NULL;
}

CEnvelopePoint* CEnvelopePoint::GetPoint(float px,float py,float rx,float ry)
{
dbgprintf("GetPoint  my x is %f and my y is %f",posx,posy);
	if((fabsf(px-posx)<rx)&&(fabsf(py-posy)<ry))return this;
	if(pnext)return pnext->GetPoint(px,py,rx,ry);
	return NULL;
}

void CEnvelopePoint::SetPoint(int num,float px,float py,float s)
{
	if(index==num)
	{
		posx=px;
		posy=py;
		slope=s;
		return;
	}
	if(pnext)pnext->SetPoint(num,px,py,s);
}

int CEnvelopePoint::AddPoint(float px,float py,float s)
{
	if(px>=posx)
	{
		if(pnext)return pnext->AddPoint(px,py,s);
		else
		{
			pnext=new CEnvelopePoint(this,NULL,px,py,s);
			return pnext->index;
		}
	}
	else
	{
		CEnvelopePoint *tmp=pprev;
		pprev=new CEnvelopePoint(tmp,this,px,py,s);
		return pprev->index;
	}
	return 0;
}

int CEnvelopePoint::DelPoint(int num)
{
	if(index==num)
	{
		delete this;
		return 1;
	}
	if(pnext)return pnext->DelPoint(num);
	return 0;
}

int CEnvelopePoint::Dir()
{
	if(pnext)if(pnext->posy>posy)return -1;
	return 1;
}

CCustomEnvelopeShape::CCustomEnvelopeShape()
{
	points=new CEnvelopePoint(NULL,NULL,0,0,0.8f);
	numpoints=0;
	loopmode=elmSustain;
	releasemode=cermContinue;
	AddPoint(0.5f,1.0f,0.5f);
	AddPoint(0.75f,0.5f,-0.1f);
	AddPoint(2.0f,0.25f,0.9f);
	AddPoint(3.0f,0,0);
	loopstart=2;
	loopend=3;
	suspoint=3;
}

CCustomEnvelopeShape::~CCustomEnvelopeShape()
{
	points->DestroyUtterly();
}

void CCustomEnvelopeShape::ToDefault()
{
	Reset();
	points->slope=0.8f;
	AddPoint(0.5f,1.0f,0.5f);
	AddPoint(0.75f,0.5f,-0.1f);
	AddPoint(2.0f,0.25f,0.9f);
	AddPoint(3.0f,0,0);
	loopstart=2;
	loopend=3;
	suspoint=3;
	loopmode=elmSustain;
	releasemode=cermContinue;
}

void CCustomEnvelopeShape::Copy(CCustomEnvelopeShape *pshape)
{
	Reset();
	points->slope=pshape->points->slope;

	CEnvelopePoint *ppoint=pshape->points->pnext;

	while(ppoint)
	{
		AddPoint(ppoint->posx,ppoint->posy,ppoint->slope);
		ppoint=ppoint->pnext;
	}

	loopstart=pshape->loopstart;
	loopend=pshape->loopend;
	suspoint=pshape->suspoint;

	loopmode=pshape->loopmode;
	releasemode=pshape->releasemode;
}

float CCustomEnvelopeShape::GetSustain()
{
	CEnvelopePoint *ppoint=points->GetPoint(suspoint);
	if(ppoint)return ppoint->posy;
	return 0;
}

bool CCustomEnvelopeShape::Sustaining(int numpoint)
{
	return (numpoint==suspoint)&&(loopmode==elmSustain);
}

__forceinline float slopemul(int cnt)
{
	return 0.5f*(tanhf((float)(cnt-256)/256-2.4f)+1);
}

int CCustomEnvelopeShape::GetNext(int &numpoint,int &dir,int &cnt,int &err,float &mul,float lev,float &tlev,float &dlev,float &slope,float speed,int spt,int releasing)
{
	CEnvelopePoint *ppoint=points->GetPoint(numpoint);
	if(ppoint)
	{
		switch(loopmode)
		{
		case elmSustain:
			if((numpoint!=suspoint)||releasing)
			{
				if(ppoint->pnext)
				{
					numpoint++;
					dir=1;
					cnt=(int)(speed*fabsf(ppoint->pnext->posx-ppoint->posx)*spt)-err;
					slope=ppoint->slope;
					if(cnt<256)
					{
						err=256-cnt;
						cnt=256;
					}
					else err=0;
					slope*=slopemul(cnt);
					mul=1.0f/cnt;
					tlev=ppoint->pnext->posy;
					if(tlev<0.0001f)tlev=0.0001f;
					dlev=lev-tlev;
					return 0;
				}
			}
			else
			{
				tlev=ppoint->posy;
				if(tlev<0.0001f)tlev=0.0001f;
				dlev=lev-tlev;
				cnt=spt;
				mul=1.0f/cnt;
				dir=1;
				err=0;
				slope=0;
				return 0;
			}
			break;
		case elmLoopForward:
			if((numpoint<loopend)||releasing)
			{
				if(ppoint->pnext)
				{
					numpoint++;
					dir=1;
					cnt=(int)(speed*fabsf(ppoint->pnext->posx-ppoint->posx)*spt)-err;
					slope=ppoint->slope;
					if(cnt<256)
					{
						err=256-cnt;
						cnt=256;
					}
					else err=0;
					slope*=slopemul(cnt);
					mul=1.0f/cnt;
					tlev=ppoint->pnext->posy;
					if(tlev<0.0001f)tlev=0.0001f;
					dlev=lev-tlev;
					return 0;
				}
			}
			else
			{
				CEnvelopePoint *pnp=points->GetPoint(loopstart);
				if(!pnp)return 1;
				numpoint=loopstart;

				cnt=0;
				dir=1;

				if(cnt<256)
				{
					err=256-cnt;
					cnt=256;
				}
				else err=0;
				mul=1.0f/cnt;

				tlev=pnp->posy;
				if(tlev<0.0001f)tlev=0.0001f;
				dlev=lev-tlev;
				slope=0;
				return 0;
			}
			break;
		case elmLoopBackward:
			if(releasing)dir=1;
			if(dir>0)
			{
				if((numpoint<loopend)||releasing)
				{
					if(ppoint->pnext)
					{
						numpoint++;
						dir=1;
						cnt=(int)(speed*fabsf(ppoint->pnext->posx-ppoint->posx)*spt)-err;
						slope=ppoint->slope;
						if(cnt<256)
						{
							err=256-cnt;
							cnt=256;
						}
						else err=0;
						slope*=slopemul(cnt);
						mul=1.0f/cnt;
						tlev=ppoint->pnext->posy;
						if(tlev<0.0001f)tlev=0.0001f;
						dlev=lev-tlev;
						return 0;
					}
				}
				else
				{
					numpoint=loopend-1;
					CEnvelopePoint *pnp=points->GetPoint(numpoint);
					if(!pnp)return 1;
					dir=-1;
					cnt=(int)(speed*fabsf(pnp->posx-ppoint->posx)*spt)-err;
					slope=-pnp->slope;
					if(cnt<256)
					{
						err=256-cnt;
						cnt=256;
					}
					else err=0;
					slope*=slopemul(cnt);
					mul=1.0f/cnt;
					tlev=pnp->posy;
					if(tlev<0.0001f)tlev=0.0001f;
					dlev=lev-tlev;
					return 0;
				}
			}
			else
			{
				if(numpoint>loopstart)
				{
					if(ppoint->pprev)
					{
						numpoint--;
						dir=-1;
						cnt=(int)(speed*fabsf(ppoint->posx-ppoint->pprev->posx)*spt)-err;
						slope=-ppoint->pprev->slope;
						if(cnt<256)
						{
							err=256-cnt;
							cnt=256;
						}
						else err=0;
						slope*=slopemul(cnt);
						mul=1.0f/cnt;
						tlev=ppoint->pprev->posy;
						if(tlev<0.0001f)tlev=0.0001f;
						dlev=lev-tlev;
						return 0;
					}
				}
				else
				{
					numpoint=loopend;
					CEnvelopePoint *pnp=points->GetPoint(numpoint);
					if(!pnp)return 1;
					dir=-1;
					cnt=0;
					if(cnt<256)
					{
						err=256-cnt;
						cnt=256;
					}
					else err=0;
					mul=1.0f/cnt;
					tlev=pnp->posy;
					if(tlev<0.0001f)tlev=0.0001f;
					dlev=lev-tlev;
					slope=0;
					return 0;
				}
			}
			break;
		case elmLoopBidir:
			if(releasing)dir=1;
			if(dir>0)
			{
				if((numpoint<loopend)||releasing)
				{
					if(ppoint->pnext)
					{
						numpoint++;
						dir=1;
						cnt=(int)(speed*fabsf(ppoint->pnext->posx-ppoint->posx)*spt)-err;
						slope=ppoint->slope;
						if(cnt<256)
						{
							err=256-cnt;
							cnt=256;
						}
						else err=0;
						slope*=slopemul(cnt);
						mul=1.0f/cnt;
						tlev=ppoint->pnext->posy;
						if(tlev<0.0001f)tlev=0.0001f;
						dlev=lev-tlev;
						return 0;
					}
				}
				else
				{
					numpoint=loopend-1;
					CEnvelopePoint *pnp=points->GetPoint(numpoint);
					if(!pnp)return 1;
					dir=-1;
					cnt=(int)(speed*fabsf(pnp->posx-ppoint->posx)*spt)-err;
					slope=-pnp->slope;
					if(cnt<256)
					{
						err=256-cnt;
						cnt=256;
					}
					else err=0;
					slope*=slopemul(cnt);
					mul=1.0f/cnt;
					tlev=pnp->posy;
					if(tlev<0.0001f)tlev=0.0001f;
					dlev=lev-tlev;
					return 0;
				}
			}
			else
			{
				if(numpoint>loopstart)
				{
					if(ppoint->pprev)
					{
						numpoint--;
						dir=-1;
						cnt=(int)(speed*fabsf(ppoint->posx-ppoint->pprev->posx)*spt)-err;
						slope=-ppoint->pprev->slope;
						if(cnt<256)
						{
							err=256-cnt;
							cnt=256;
						}
						else err=0;
						slope*=slopemul(cnt);
						mul=1.0f/cnt;
						tlev=ppoint->pprev->posy;
						if(tlev<0.0001f)tlev=0.0001f;
						dlev=lev-tlev;
						return 0;
					}
				}
				else
				{
					numpoint=loopstart+1;
					CEnvelopePoint *pnp=points->GetPoint(numpoint);
					if(!pnp)return 1;
					dir=1;
					cnt=(int)(speed*fabsf(pnp->posx-ppoint->posx)*spt)-err;
					slope=ppoint->slope;
					if(cnt<256)
					{
						err=256-cnt;
						cnt=256;
					}
					else err=0;
					slope*=slopemul(cnt);
					mul=1.0f/cnt;
					tlev=pnp->posy;
					if(tlev<0.0001f)tlev=0.0001f;
					dlev=lev-tlev;
					return 0;
				}
			}
			break;
		default:
		case elmOff:
			if(ppoint->pnext)
			{
				numpoint++;
				dir=1;
				cnt=(int)(speed*fabsf(ppoint->pnext->posx-ppoint->posx)*spt)-err;
				slope=ppoint->slope;
				if(cnt<256)
				{
					err=256-cnt;
					cnt=256;
				}
				else err=0;
				slope*=slopemul(cnt);
				mul=1.0f/cnt;
				tlev=ppoint->pnext->posy;
				if(tlev<0.0001f)tlev=0.0001f;
				dlev=lev-tlev;
				return 0;
			}
			break;
		}
	}
	return 1;
}

int CCustomEnvelopeShape::ReleaseJump(int &numpoint,int &dir,int &cnt,int &err,float &mul,float lev,float &tlev,float &dlev,float &slope,int spt)
{
	if(releasemode==cermJump)
	{
		numpoint=loopend;
		dir=1;
		CEnvelopePoint *ppoint=points->GetPoint(numpoint);
		if(!ppoint)return 1;
		
		cnt=0;
		if(cnt<256)
		{
			err=256-cnt;
			cnt=256;
		}
		else err=0;
		mul=1.0f/cnt;
		tlev=ppoint->posy;
		if(tlev<0.0001f)tlev=0.0001f;
		dlev=lev-tlev;
		slope=0;
	}
	return 0;
}

void CCustomEnvelopeShape::AddPoint(float px,float py,float s)
{
	if(px<0)px=0;
	int num=points->AddPoint(px,py,s);
	if(num)
	{
		numpoints++;
		if(num<=loopstart)loopstart++;
		if(num<=loopend)loopend++;
		if(num<=suspoint)suspoint++;
	}
}

void CCustomEnvelopeShape::DelPoint(int num)
{
	if(num<=0)return;
	if(points->DelPoint(num))
	{
		numpoints--;
		if(num<loopstart)loopstart--;
		if(num<loopend)loopend--;
		if(num<suspoint)suspoint--;
	}
}

CEnvelopePoint* CCustomEnvelopeShape::GetFirstPoint(float px)
{
	CEnvelopePoint* ppoint=points;
	while(ppoint)
	{
		if(ppoint->pnext)
		{
			if(ppoint->pnext->posx>=px)return ppoint;
		}
		else return ppoint;
		ppoint=ppoint->pnext;
	}
	return NULL;
}

CEnvelopePoint* CCustomEnvelopeShape::GetPointAt(float px,float py,float rx,float ry)
{
	return points->GetPoint(px,py,rx,ry);
}

void CCustomEnvelopeShape::Test()
{
	char text[4096];
	text[0]=0;
	sprintf(text,"%i\n",numpoints);
	CEnvelopePoint *ppoint=points;
	while(ppoint)
	{
		sprintf(text,"%s%i: %.2f %.2f %.2f\n",text,ppoint->index,ppoint->posx,ppoint->posy,ppoint->slope);
		ppoint=ppoint->pnext;
	}
	MessageBox(NULL,text,"a",0);
}

void CCustomEnvelopeShape::Reset()
{
	if(points->pnext)points->pnext->DestroyUtterly();
	numpoints=0;
	points->slope=0;
}

void CCustomEnvelopeShape::Save(CMachineDataOutput *po)
{
	po->Write(releasemode);
	po->Write(loopmode);
	po->Write(loopstart);
	po->Write(loopend);
	po->Write(suspoint);
	po->Write(numpoints);
	po->Write(points->slope);
	CEnvelopePoint *ppoint=points->pnext;
	while(ppoint)
	{
		po->Write(ppoint->posx);
		po->Write(ppoint->posy);
		po->Write(ppoint->slope);
		ppoint=ppoint->pnext;
	}
}

void CCustomEnvelopeShape::Save(const char *filename)
{
	if(!filename)return;
	FILE *of=fopen(filename,"wb");
	if(of)
	{
		fputs("COTWENVSNG",of);
		int version=1;
		fwrite(&version,sizeof(int),1,of);
		fwrite(&releasemode,sizeof(int),1,of);
		fwrite(&loopmode,sizeof(int),1,of);
		fwrite(&loopstart,sizeof(int),1,of);
		fwrite(&loopend,sizeof(int),1,of);
		fwrite(&suspoint,sizeof(int),1,of);
		fwrite(&numpoints,sizeof(int),1,of);
		fwrite(&(points->slope),sizeof(float),1,of);
		CEnvelopePoint *ppoint=points->pnext;
		while(ppoint)
		{
			fwrite(&(ppoint->posx),sizeof(float),1,of);
			fwrite(&(ppoint->posy),sizeof(float),1,of);
			fwrite(&(ppoint->slope),sizeof(float),1,of);
			ppoint=ppoint->pnext;
		}
		fclose(of);
	}
}

/*
version 1:

  int releasemode;
  int loopmode;
  int loopstart;
  int loopend;
  int suspoint;
  int numpoints;
  float p0slope;
  numpoints times:
	float px;
	float py;
	float s;
*/

void CCustomEnvelopeShape::Load(CMachineDataInput *pi,int v)
{
	Reset();

	if(v==1)
	{
		pi->Read(releasemode);
		pi->Read(loopmode);
		pi->Read(loopstart);
		pi->Read(loopend);
		pi->Read(suspoint);
		pi->Read(numpoints);
		pi->Read(points->slope);
		int i=numpoints;
		while(i>0)
		{
			float px,py,s;
			pi->Read(px);
			pi->Read(py);
			pi->Read(s);
			points->AddPoint(px,py,s);
			i--;
		}
	}
}

LOADERR CCustomEnvelopeShape::Load(const char *filename)
{
	if(!filename)return LOADERR_NOFILENAME;
	CCustomEnvelopeShape backup;
	backup.Copy(this);
	Reset();

	FILE *inf=fopen(filename,"rb");

	try
	{
		if(inf)
		{
			char magic[12];
			magic[11]=0;
			fgets(magic,11,inf);
			if(strcmp(magic,"COTWENVSNG"))throw LOADERR_NOMAGIC;
			int version;
			fread(&version,sizeof(int),1,inf);
			int i;
			switch(version)
			{
			case 1:
				if(fread(&releasemode,sizeof(int),1,inf)!=1)throw LOADERR_FAIL;
				if(fread(&loopmode,sizeof(int),1,inf)!=1)throw LOADERR_FAIL;
				if(fread(&loopstart,sizeof(int),1,inf)!=1)throw LOADERR_FAIL;
				if(fread(&loopend,sizeof(int),1,inf)!=1)throw LOADERR_FAIL;
				if(fread(&suspoint,sizeof(int),1,inf)!=1)throw LOADERR_FAIL;
				if(fread(&numpoints,sizeof(int),1,inf)!=1)throw LOADERR_FAIL;
				if(fread(&(points->slope),sizeof(float),1,inf)!=1)throw LOADERR_FAIL;
				i=numpoints;
				while(i>0)
				{
					float px,py,s;
					if(fread(&px,sizeof(float),1,inf)!=1)throw LOADERR_FAIL;
					if(fread(&py,sizeof(float),1,inf)!=1)throw LOADERR_FAIL;
					if(fread(&s,sizeof(float),1,inf)!=1)throw LOADERR_FAIL;
					points->AddPoint(px,py,s);
					i--;
				}
				break;
			default:
				throw LOADERR_NEWERVERSION;
			}
			fclose(inf);
			return LOADERR_SUCCESS;
		}
		else throw LOADERR_NOFILE;
	}
	catch(LOADERR err)
	{
		if(inf)fclose(inf);
		Copy(&backup);
		return err;
	}

	if(inf)fclose(inf);	
	Copy(&backup);

	return LOADERR_WTF;
}

#define WM_MOUSEWHEEL                   0x020A

CCustomEnvelopeShape envecopybuffer;

extern HINSTANCE dllInstance;

extern HMENU henvebgmenu;
extern HMENU henvelmmenu;
extern HMENU henvermmenu;
extern HMENU henveptmenu;

WNDPROC enve_oldwndproc=NULL;

BOOL APIENTRY EnvelopeWindowProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		return SendMessage(GetParent(hWnd),WM_MOUSEMOVE,2001+wParam,lParam);
	case WM_LBUTTONDOWN:
		return SendMessage(GetParent(hWnd),WM_LBUTTONDOWN,2001+wParam,lParam);
	case WM_LBUTTONUP:
		return SendMessage(GetParent(hWnd),WM_LBUTTONUP,2001+wParam,lParam);
	case WM_MBUTTONDOWN:
		return SendMessage(GetParent(hWnd),WM_MBUTTONDOWN,2001+wParam,lParam);
	}
	if(enve_oldwndproc)return CallWindowProc(enve_oldwndproc,hWnd,uMsg,wParam,lParam);
	else return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

BOOL APIENTRY EnvelopeEditorDialog(HWND hDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(uMsg==WM_INITDIALOG)
	{
		SetWindowLong(hDlg,GWL_USERDATA,lParam);
		CEnvelopeEditor *penve=(CEnvelopeEditor *)lParam;
		SetWindowLong(GetDlgItem(hDlg,IDC_ENVDISPLAY),GWL_STYLE,GetWindowLong(GetDlgItem(hDlg,IDC_ENVDISPLAY),GWL_STYLE)|SS_OWNERDRAW);
		enve_oldwndproc=(WNDPROC)GetWindowLong(GetDlgItem(hDlg,IDC_ENVDISPLAY),GWL_WNDPROC);
		SetWindowLong(GetDlgItem(hDlg,IDC_ENVDISPLAY),GWL_WNDPROC,(LONG)EnvelopeWindowProc);
		SetDlgItemInt(hDlg,IDC_ENVNUM,1,FALSE);
		CheckDlgButton(hDlg,IDC_ENVESNAP,BST_UNCHECKED);
		ShowWindow(GetDlgItem(hDlg,IDC_TEST),SW_HIDE);
		return 1;
	}
	
	CEnvelopeEditor *penve=(CEnvelopeEditor *)GetWindowLong(hDlg,GWL_USERDATA);

	switch(uMsg)
	{
	case WM_CLOSE:
		if(penve)delete penve;
		return 0;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			if(penve)
			{
				delete penve;
			}
			return 1;
		case IDC_ENVERESET:
			if(penve)
			{
				penve->ToDefault();
				RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			}
			return 1;
		case IDC_ENVECOPY:
			if(penve)
			{
				penve->Copy();
				RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			}
			return 1;
		case IDC_ENVEPASTE:
			if(penve)
			{
				penve->Paste();
				RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			}
			return 1;
		case IDC_NEXTENV:
			if(penve)
			{
				penve->SetEdnum(penve->GetEdnum()+1);
				SetDlgItemInt(hDlg,IDC_ENVNUM,penve->GetEdnum()+1,FALSE);
				RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			}
			return 1;
		case IDC_PREVENV:
			if(penve)
			{
				penve->SetEdnum(penve->GetEdnum()-1);
				SetDlgItemInt(hDlg,IDC_ENVNUM,penve->GetEdnum()+1,FALSE);
				RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			}
			return 1;
		case IDC_ENVESNAP:
			if(penve)
			{
				penve->snap=!!IsDlgButtonChecked(hDlg,IDC_ENVESNAP);
			}
			return 1;
		case IDC_MOVEREST:
			if(penve)
			{
				penve->moverest=!!IsDlgButtonChecked(hDlg,IDC_MOVEREST);
			}
			return 1;
		case IDC_TEST:
			if(penve)
			{
				penve->Test();
			}
			return 1;
		case IDC_IMPORT:
			if(penve)
			{
				penve->Import();
				RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			}
			return 1;
		case IDC_EXPORT:
			if(penve)
			{
				penve->Export();
			}
			return 1;
		case ID_BGPOPUP_ADDPOINT:
			if(penve)
			{
				penve->MenuAddPoint();
			}
			return 1;
		case ID_POINTPOPUP_DELETEPOINT:
			if(penve)
			{
				penve->MenuDelPoint();
			}
			return 1;
		case ID_POINTPOPUP_LOOPSTART:
			if(penve)
			{
				penve->MenuSetLoopStart();
			}
			return 1;
		case ID_POINTPOPUP_LOOPEND:
			if(penve)
			{
				penve->MenuSetLoopEnd();
			}
			return 1;
		case ID_POINTPOPUP_SUSTAINLEVEL:
			if(penve)
			{
				penve->MenuSetSustainLev();
			}
			return 1;
		default:
			if(penve)if(LOWORD(wParam)>=44096)
			{
				if(LOWORD(wParam)>=44096+NumEnvLoopModes)
				{
					penve->MenuSetReleaseMode(LOWORD(wParam)-44096-NumEnvLoopModes);
				}
				else
				{
					penve->MenuSetLoopMode(LOWORD(wParam)-44096);
				}
			}
		}
		if(penve)penve->menupoint=NULL;
		break;
	case WM_MOUSEMOVE:
		if(penve)if(wParam>2000)
		{
			bool ch=false;

			wParam-=2001;
			int x=LOWORD(lParam);
			int y=HIWORD(lParam);

			if(x>1000)x=penve->oldmx;
			if(y>1000)y=penve->oldmy;

			int xdif=x-penve->oldmx;
			int ydif=y-penve->oldmy;

			if(wParam==(MK_LBUTTON|MK_SHIFT))
			{
				penve->do_scroll(xdif);
				ch=true;
			}
			else if(wParam==(MK_LBUTTON|MK_CONTROL))
			{
				penve->do_zoom(xdif,ydif);
				ch=true;
			}
			else if((wParam&MK_LBUTTON)&&(!(wParam&(MK_SHIFT|MK_CONTROL))))
			{
				ch=penve->do_drag(x,y);
				if(!ch)ch=penve->do_slope_drag(ydif);
			}
			else
			{
				penve->dragpoint=NULL;
				penve->slopedragpoint=NULL;
			}

			penve->oldmx=x;
			penve->oldmy=y;

			if(ch)RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);

			return 1;
		}
		break;
	case WM_MOUSEWHEEL:
		if(penve)
		{
			RECT rc;
			GetWindowRect(GetDlgItem(hDlg,IDC_ENVDISPLAY),&rc);
			penve->do_wheel_zoom((short)HIWORD(wParam),LOWORD(lParam)-rc.left);
			RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			return 1;
		}
		break;
	case WM_LBUTTONDOWN:
		if(penve)if(wParam>2000)
		{
			wParam-=2001;
			int x=LOWORD(lParam);
			int y=HIWORD(lParam);

			if((wParam&MK_SHIFT)&&(wParam&MK_CONTROL))
			{
				penve->AddPoint(x,y);
				RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
			}
			else if(!((wParam&MK_SHIFT)||(wParam&MK_CONTROL)))
			{
				if(penve->StartDrag(x,y))penve->slopedragpoint=NULL;
				else
				{
					penve->StartSlopeDrag(x);
					penve->dragpoint=NULL;
				}
			}

			penve->oldmx=x;
			penve->oldmy=y;

			return 1;
		}
		break;
	case WM_MBUTTONDOWN:
		if(penve)if(wParam>2000)
		{
			int x=LOWORD(lParam);
			int y=HIWORD(lParam);

			penve->AddPoint(x,y);
			RedrawWindow(GetDlgItem(hDlg,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);

			penve->oldmx=x;
			penve->oldmy=y;

			return 1;
		}
		break;
	case WM_LBUTTONUP:
		if(penve)if(wParam>2000)
		{
			int x=LOWORD(lParam);
			int y=HIWORD(lParam);

			penve->slopedragpoint=NULL;
			penve->dragpoint=NULL;

			penve->oldmx=x;
			penve->oldmy=y;
			return 1;
		}
		break;
	case WM_CONTEXTMENU:
		if(penve)if((HWND)wParam==GetDlgItem(hDlg,IDC_ENVDISPLAY))
		{
			int x=LOWORD(lParam);
			int y=HIWORD(lParam);
			penve->menux=penve->oldmx;
			penve->menuy=penve->oldmy;
			BOOL a;
			if(penve->GetMenuPoint())a=TrackPopupMenu(henveptmenu,0,x,y,0,hDlg,NULL);
			else a=TrackPopupMenu(henvebgmenu,0,x,y,0,hDlg,NULL);
			
			if(!a)ShowLastError("Envelope editor context menu");
		}
		break;
	case WM_DRAWITEM:
		if(wParam==IDC_ENVDISPLAY)
		{
			if(penve)
			{
				HDC drawz0r=((LPDRAWITEMSTRUCT)lParam)->hDC;
				RECT rec=((LPDRAWITEMSTRUCT)lParam)->rcItem;
				penve->DrawEnv(drawz0r,rec);
			}
			return 1;
		}
		break;
	}
	return 0;
}
CEnvelopeEditor::CEnvelopeEditor(CCustomEnvelopeShape *pShapes,CEnvelopeEditor **pPt,const char *macname)
{
	pshapes=pShapes;
	pshape=pshapes;
	ednum=0;
	ppt=pPt;

	w=h=1;

	zoom=4.0f;
	scrollpos=0.0f;

	oldmx=oldmy=0;
	menux=menuy=0;
	
	slopedragpoint=NULL;
	dragpoint=NULL;
	menupoint=NULL;

	snap=false;
	moverest=false;

	dhandle=CreateDialogParam(dllInstance,MAKEINTRESOURCE(IDD_CUSTOMENVDIALOG),GetDesktopWindow(),EnvelopeEditorDialog,(LPARAM)this);
	if(!dhandle)ShowLastError("Creating envelope editor window");
	char *text=new char[strlen(macname)+strlen(" Envelope Editor")+1];
	sprintf(text,"%s Envelope Editor",macname);
	SetWindowText(dhandle,text);
	delete[] text;
}

CEnvelopeEditor::~CEnvelopeEditor()
{
	if(dhandle)DestroyWindow(dhandle);
	if(ppt)*ppt=NULL;
}

void CEnvelopeEditor::Show(const char *macname)
{
	ShowWindow(dhandle,SW_RESTORE);
	char *text=new char[strlen(macname)+strlen(" Envelope Editor")+1];
	sprintf(text,"%s Envelope Editor",macname);
	SetWindowText(dhandle,text);
	delete[] text;
}

__forceinline float snappify(float a)
{
	return floorf(32*a+0.5f)/32;
}

void CEnvelopeEditor::AddPoint(int x,int y)
{
	if(snap)pshape->AddPoint(snappify(x_to_px(x)),y_to_py(y),0);
	else pshape->AddPoint(x_to_px(x),y_to_py(y),0);
}

void CEnvelopeEditor::DelPoint(int num)
{
	if(num>0)pshape->DelPoint(num);
}

float CEnvelopeEditor::x_to_px(int x)
{
	return zoom*x/w+scrollpos;
}

float CEnvelopeEditor::y_to_py(int y)
{
	return 1.0f-(float)y/h;
}

int CEnvelopeEditor::px_to_x(float px)
{
	return (int)((px-scrollpos)/zoom*w);
}

int CEnvelopeEditor::py_to_y(float py)
{
	return (int)((1.0f-py)*h);
}

void CEnvelopeEditor::DrawEnv(HDC hDrawDC,RECT rect)
{
	h=rect.bottom-rect.top;
	w=rect.right-rect.left;

	HDC hDC=CreateCompatibleDC(hDrawDC);
	HBITMAP hbm=CreateCompatibleBitmap(hDrawDC,w,h);
	HBITMAP tempbm=(HBITMAP)SelectObject(hDC,hbm);

	HBRUSH hbr=CreateSolidBrush((COLORREF)0x00000000);
	FillRect(hDC,&rect,hbr);
	DeleteObject(hbr);
	
	float leftedge=x_to_px(0);
	float rightedge=x_to_px(w);
	
	float h2=(float)h/2;
	float w2=(float)w/2;
	float wmul=256.0f/w;

	float gridlen=(float)w/zoom;
	int griddiv=32;

	while(gridlen/griddiv<8)griddiv>>=1;

	HPEN hpen;
	HGDIOBJ temp;

	float edz,diff;
	int numgrid,gridpos;

	if(griddiv>1)
	{
		hpen=CreatePen(PS_SOLID,0,(COLORREF)0x00C08000);
		temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);
		numgrid=(int)(zoom*griddiv)+1;
		edz=1.0f/(zoom*griddiv);
		diff=griddiv*scrollpos-floorf(griddiv*scrollpos);
		do
		{
			gridpos=(int)(edz*(numgrid-diff)*w);
			MoveToEx(hDC,gridpos,0,NULL);
			LineTo(hDC,gridpos,h);
			numgrid--;
		}
		while(numgrid>0);
		SelectObject(hDC,temp);
		DeleteObject((HGDIOBJ)hpen);
	}

	hpen=CreatePen(PS_SOLID,0,(COLORREF)0x00FFC000);
	temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);
	numgrid=(int)zoom+1;
	edz=1.0f/zoom;
	diff=scrollpos-floorf(scrollpos);
	do
	{
		gridpos=(int)(edz*(numgrid-diff)*w);
		MoveToEx(hDC,gridpos,0,NULL);
		LineTo(hDC,gridpos,h);
		numgrid--;
	}
	while(numgrid>0);
	SelectObject(hDC,temp);
	DeleteObject((HGDIOBJ)hpen);

	HGDIOBJ temp2=GetCurrentObject(hDC,OBJ_BRUSH);
	LOGBRUSH lb;
	lb.lbColor=0;
	lb.lbHatch=0;
	lb.lbStyle=BS_HOLLOW;
	hbr=CreateBrushIndirect(&lb);
	SelectObject(hDC,hbr);

	CEnvelopePoint *ppoint=pshape->points->GetPoint(pshape->loopstart);

	SetArcDirection(hDC,AD_COUNTERCLOCKWISE);

	if(ppoint)if((ppoint->posx>=leftedge)&&(ppoint->posx<=rightedge))
	{
		hpen=CreatePen(PS_SOLID,OTWIDTH,(COLORREF)0x00A0A0A0);
		temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);

		int xpos=px_to_x(ppoint->posx);
		int ypos=py_to_y(ppoint->posy);

		Arc(hDC,xpos-OTPOINTRANGE,ypos-OTPOINTRANGE,xpos+OTPOINTRANGE,ypos+OTPOINTRANGE,xpos,ypos-OTPOINTRANGE,xpos,ypos+OTPOINTRANGE);

		SelectObject(hDC,temp);
		DeleteObject((HGDIOBJ)hpen);
	}

	ppoint=pshape->points->GetPoint(pshape->loopend);

	if(ppoint)if((ppoint->posx>=leftedge)&&(ppoint->posx<=rightedge))
	{
		hpen=CreatePen(PS_SOLID,OTWIDTH,(COLORREF)0x00C0C0C0);
		temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);

		int xpos=px_to_x(ppoint->posx);
		int ypos=py_to_y(ppoint->posy);

		Arc(hDC,xpos-OTPOINTRANGE,ypos-OTPOINTRANGE,xpos+OTPOINTRANGE,ypos+OTPOINTRANGE,xpos,ypos+OTPOINTRANGE,xpos,ypos-OTPOINTRANGE);

		SelectObject(hDC,temp);
		DeleteObject((HGDIOBJ)hpen);
	}

	ppoint=pshape->points->GetPoint(pshape->suspoint);

	if(ppoint)
	{
		if((ppoint->posx>=leftedge)&&(ppoint->posx<=rightedge))
		{
			hpen=CreatePen(PS_SOLID,OTWIDTH,(COLORREF)0x0000A080);
			temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);

			int xpos=px_to_x(ppoint->posx);
			int ypos=py_to_y(ppoint->posy);

			Ellipse(hDC,xpos-OTPOINTRANGE-OTWIDTH-1,ypos-OTPOINTRANGE-OTWIDTH-1,xpos+OTPOINTRANGE+OTWIDTH+1,ypos+OTPOINTRANGE+OTWIDTH+1);

			SelectObject(hDC,temp);
			DeleteObject((HGDIOBJ)hpen);
		}
		
		hpen=CreatePen(PS_SOLID,0,(COLORREF)0x0000C0A0);
		temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);

		MoveToEx(hDC,0,py_to_y(ppoint->posy),NULL);
		LineTo(hDC,w,py_to_y(ppoint->posy));

		SelectObject(hDC,temp);
		DeleteObject((HGDIOBJ)hpen);
	}
	else
	{
		hpen=CreatePen(PS_SOLID,0,(COLORREF)0x0000C0A0);
		temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);

		MoveToEx(hDC,0,h/2,NULL);
		LineTo(hDC,w,h/2);

		SelectObject(hDC,temp);
		DeleteObject((HGDIOBJ)hpen);
	}

	ppoint=pshape->GetFirstPoint(leftedge);

	if(ppoint)
	{
		hpen=CreatePen(PS_SOLID,0,(COLORREF)0x008080FF);
		temp=(HPEN)SelectObject(hDC,(HGDIOBJ)hpen);
		
		MoveToEx(hDC,px_to_x(ppoint->posx),py_to_y(ppoint->posy),NULL);
		
		float slope=ppoint->slope;
		int xpos=px_to_x(ppoint->posx);
		float ypos=ppoint->posy;
		ppoint=ppoint->pnext;		

		int iypos=py_to_y(ypos);
		Ellipse(hDC,xpos-POINTRANGE,iypos-POINTRANGE,xpos+POINTRANGE,iypos+POINTRANGE);
		
		while(ppoint)
		{
			int nxpos=px_to_x(ppoint->posx);
			float nypos=ppoint->posy;

			if(xpos==nxpos)
			{
				LineTo(hDC,nxpos,py_to_y(nypos));
				xpos=nxpos;
				ypos=nypos;
			}
			else if(slope<0) // 1+t*(p*(t^3-1)-1)
			{
				float t=0;
				float df=(float)(nxpos-xpos);
				if(df>1)df=1.0f/df;
				while(xpos<=nxpos)
				{
					float s=t*t;s*=s;
					float typos=1+t*(slope*(s-1)-1);
					typos=typos*(ypos-nypos)+nypos;
					t+=df;
					LineTo(hDC,xpos,py_to_y(typos));
					xpos++;
				}
				ypos=nypos;
				xpos=nxpos;
			}
			else // s=1-t, s*(1+p*(s^3-1))
			{
				float t=0;
				float s;
				float df=(float)(nxpos-xpos);
				if(df>1)df=1.0f/df;
				while(xpos<=nxpos)
				{
					s=1-t;
					float u=s*s;u*=u;
					float typos=s*(1+slope*(u-1));
					typos=typos*(ypos-nypos)+nypos;
					t+=df;
					LineTo(hDC,xpos,py_to_y(typos));
					xpos++;
				}
				ypos=nypos;
				xpos=nxpos;
			}

			iypos=py_to_y(ypos);
			Ellipse(hDC,xpos-POINTRANGE,iypos-POINTRANGE,xpos+POINTRANGE,iypos+POINTRANGE);
			
			if(xpos>=w)break;
			slope=ppoint->slope;
			ppoint=ppoint->pnext;
		}
		
		SelectObject(hDC,temp);
		DeleteObject((HGDIOBJ)hpen);
	}

	SelectObject(hDC,temp2);
	DeleteObject((HGDIOBJ)hbr);

	BitBlt(hDrawDC,0,0,w,h,hDC,0,0,SRCCOPY);

	SelectObject(hDC,tempbm);
	DeleteObject(hbm);
	DeleteDC(hDC);
}

void CEnvelopeEditor::do_scroll(int amount)
{
	scrollpos-=zoom*amount/w;
	if(scrollpos<0)scrollpos=0;
	else if(scrollpos>8192)scrollpos=8192;
}

void CEnvelopeEditor::do_zoom(int xamount,int yamount)
{
	zoom-=zoom*(2.0f*xamount/w-0.125f*yamount/h);
	if(zoom>MAX_ZOOM)zoom=MAX_ZOOM;
	else if(zoom<MIN_ZOOM)zoom=MIN_ZOOM;
}

void CEnvelopeEditor::do_wheel_zoom(short amount,int xpos)
{
	float f=(float)xpos/w;
	scrollpos+=f*zoom;
	zoom-=zoom*amount/720;
	if(zoom>MAX_ZOOM)zoom=MAX_ZOOM;
	else if(zoom<MIN_ZOOM)zoom=MIN_ZOOM;
	scrollpos-=f*zoom;
	if(scrollpos<0)scrollpos=0;
	else if(scrollpos>8192)scrollpos=8192;
}

void CEnvelopeEditor::StartSlopeDrag(int x)
{
	slopedragpoint=pshape->GetFirstPoint(x_to_px(x));
}

bool CEnvelopeEditor::do_slope_drag(int amount)
{
	if(slopedragpoint)
	{
		slopedragpoint->slope+=slopedragpoint->Dir()*4.0f*(float)amount/h;
		if(slopedragpoint->slope>1)slopedragpoint->slope=1;
		else if(slopedragpoint->slope<-1)slopedragpoint->slope=-1;
		return true;
	}
	return false;
}

bool CEnvelopeEditor::StartDrag(int x,int y)
{
	dragpoint=pshape->GetPointAt(x_to_px(x),y_to_py(y),zoom*POINTRANGE/w,(float)POINTRANGE/h);
	if(dragpoint&&(dragpoint!=pshape->points))  //<--- don't allow dragging first point
	{
		if(dragpoint->pprev)dragmin=dragpoint->pprev->posx;
		else dragmin=0;
		if(dragpoint->pnext)dragmax=dragpoint->pnext->posx;
		else dragmax=8192;
		return true;
	}
	dragpoint=NULL;
	return false;
}

bool CEnvelopeEditor::do_drag(int xpos,int ypos)
{
	if(dragpoint)
	{
		if(moverest)
		{
			float oldx=dragpoint->posx;
			if(snap)dragpoint->posx=snappify(x_to_px(xpos));
			else dragpoint->posx=x_to_px(xpos);
			if(dragpoint->posx>8192)dragpoint->posx=8192;			
			else if(dragpoint->posx<dragmin)dragpoint->posx=dragmin;
			dragpoint->posy=y_to_py(ypos);
			if(dragpoint->posy>1.0f)dragpoint->posy=1.0f;
			else if(dragpoint->posy<0)dragpoint->posy=0;

			CEnvelopePoint *ppoint=dragpoint->pnext;
			while(ppoint)
			{
				ppoint->posx+=dragpoint->posx-oldx;
				ppoint=ppoint->pnext;
			}

			if(dragpoint->pnext)dragmax=dragpoint->pnext->posx;
			else dragmax=8192;

			if(dragpoint->posx>dragmax)dragpoint->posx=dragmax;
		}
		else
		{
			if(snap)dragpoint->posx=snappify(x_to_px(xpos));
			else dragpoint->posx=x_to_px(xpos);
			dragpoint->posy=y_to_py(ypos);
			if(dragpoint->posx>dragmax)dragpoint->posx=dragmax;
			else if(dragpoint->posx<dragmin)dragpoint->posx=dragmin;
			if(dragpoint->posy>1.0f)dragpoint->posy=1.0f;
			else if(dragpoint->posy<0)dragpoint->posy=0;
		}
		return true;
	}
	return false;
}

CEnvelopePoint* CEnvelopeEditor::GetMenuPoint()
{
	menupoint=pshape->GetPointAt(x_to_px(menux),y_to_py(menuy),zoom*POINTRANGE/w,(float)POINTRANGE/h);
	if(menupoint)
	{
		CheckMenuItem(henveptmenu,ID_POINTPOPUP_LOOPSTART,(menupoint->index==pshape->loopstart)?MF_CHECKED:MF_UNCHECKED);
		CheckMenuItem(henveptmenu,ID_POINTPOPUP_LOOPEND,(menupoint->index==pshape->loopend)?MF_CHECKED:MF_UNCHECKED);
		CheckMenuItem(henveptmenu,ID_POINTPOPUP_SUSTAINLEVEL,(menupoint->index==pshape->suspoint)?MF_CHECKED:MF_UNCHECKED);
	}
	else
	{
		for(int i=0;i<NumEnvLoopModes;i++)CheckMenuItem(henvelmmenu,i,MF_UNCHECKED|MF_BYPOSITION);
		CheckMenuItem(henvelmmenu,pshape->loopmode,MF_CHECKED|MF_BYPOSITION);
		for(i=0;i<NumEnvReleaseModes;i++)CheckMenuItem(henvermmenu,i,MF_UNCHECKED|MF_BYPOSITION);
		CheckMenuItem(henvermmenu,pshape->releasemode,MF_CHECKED|MF_BYPOSITION);
	}
	return menupoint;
}

void CEnvelopeEditor::MenuAddPoint()
{
	AddPoint(menux,menuy);
	menupoint=NULL;
	RedrawWindow(GetDlgItem(dhandle,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
}

void CEnvelopeEditor::MenuDelPoint()
{
	if(menupoint)
	{
		DelPoint(menupoint->index);
		menupoint=NULL;
		RedrawWindow(GetDlgItem(dhandle,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
	}
}

void CEnvelopeEditor::MenuSetLoopMode(int lm)
{
	pshape->loopmode=lm;
}

void CEnvelopeEditor::MenuSetReleaseMode(int rm)
{
	pshape->releasemode=rm;
}

void CEnvelopeEditor::MenuSetLoopStart()
{
	if(menupoint)
	{
		pshape->loopstart=menupoint->index;
		if(pshape->loopstart>pshape->loopend)pshape->loopend=pshape->loopstart;
		RedrawWindow(GetDlgItem(dhandle,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
		menupoint=NULL;
	}
}

void CEnvelopeEditor::MenuSetLoopEnd()
{
	if(menupoint)
	{
		pshape->loopend=menupoint->index;
		if(pshape->loopstart>pshape->loopend)pshape->loopstart=pshape->loopend;
		RedrawWindow(GetDlgItem(dhandle,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
		menupoint=NULL;
	}
}

void CEnvelopeEditor::MenuSetSustainLev()
{
	if(menupoint)
	{
		pshape->suspoint=menupoint->index;
		RedrawWindow(GetDlgItem(dhandle,IDC_ENVDISPLAY),NULL,NULL,RDW_ALLCHILDREN|RDW_INTERNALPAINT|RDW_INVALIDATE);
		menupoint=NULL;
	}
}

void CEnvelopeEditor::SetEdnum(int i)
{
	if(i<0)i=NumCustomEnvelopes-1;
	else if(i>=NumCustomEnvelopes)i=0;
	ednum=i;
	pshape=pshapes+ednum;
}

int CEnvelopeEditor::GetEdnum()
{
	return ednum;
}

void CEnvelopeEditor::ToDefault()
{
	if(pshape)pshape->ToDefault();
}

void CEnvelopeEditor::Copy()
{
	if(pshape)envecopybuffer.Copy(pshape);
}

void CEnvelopeEditor::Paste()
{
	if(pshape)pshape->Copy(&envecopybuffer);
}

void CEnvelopeEditor::Test()
{
	if(pshape)pshape->Test();
}

void CEnvelopeEditor::Export()
{
	if(pshape)
	{
		OPENFILENAME ofn;
		char filename[1024];
		filename[0]=0;
		ofn.hwndOwner=dhandle;
		ofn.lpstrFilter="COTW Envelope\0*.cev\0All Files\0*.*\0\0";
		ofn.lpstrCustomFilter=NULL;
		ofn.nFilterIndex=1;
		ofn.lpstrFile=filename;
		ofn.nMaxFile=1023;
		ofn.lpstrFileTitle=NULL;
		ofn.lpstrInitialDir=NULL;
		ofn.lpstrTitle="Export Envelope";
		ofn.Flags=OFN_HIDEREADONLY;
		ofn.lpstrDefExt="cev";
		ofn.lStructSize=sizeof(OPENFILENAME);
		if(GetSaveFileName(&ofn))pshape->Save(filename);
		else ShowLastError("Exporting envelope");
	}
}

void CEnvelopeEditor::Import()
{
	if(pshape)
	{
		OPENFILENAME ofn;
		char filename[1024];
		filename[0]=0;
		ofn.hwndOwner=dhandle;
		ofn.lpstrFilter="COTW Envelope\0*.cev\0All Files\0*.*\0\0";
		ofn.lpstrCustomFilter=NULL;
		ofn.nFilterIndex=1;
		ofn.lpstrFile=filename;
		ofn.nMaxFile=1023;
		ofn.lpstrFileTitle=NULL;
		ofn.lpstrInitialDir=NULL;
		ofn.lpstrTitle="Import Envelope";
		ofn.Flags=OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
		ofn.lpstrDefExt="cev";
		ofn.lStructSize=sizeof(OPENFILENAME);
		if(GetOpenFileName(&ofn))
		{
			int err=pshape->Load(filename);
			if(err!=LOADERR_SUCCESS)ShowLoadError(err);
		}
		else ShowLastError("Importing envelope");
	}
}