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

#ifndef __BTDSYS_BLOB_H
#define __BTDSYS_BLOB_H

#include "particle.h"

class CMemDC;

class CXYBlob
{
public:
	CXYBlob();
	virtual ~CXYBlob() {};

	void SetX(float fx);
	void SetY(float fy);

	void SetScreenPos(CPoint pos);

	float GetX();
	float GetY();

	void SetParentRect(CRect r);

	virtual void SetFGColour(COLORREF c);
	virtual void SetBGColour(COLORREF c);

	virtual void Update(CDC *pdc) {};

protected:
	float px, py;
	int parw, parh;
	int xoff, yoff;

	int screenx, screeny;

	COLORREF fgcolour, bgcolour;
};



class CXYNoBlob : public CXYBlob
{
public:
	CXYNoBlob(){};
	virtual ~CXYNoBlob(){};

	virtual void Update(CDC *pdc) {};
};

class CXYDotBlob : public CXYBlob
{
public:
	CXYDotBlob(){};
	virtual ~CXYDotBlob(){};

	virtual void Update(CDC *pdc);
};

class CXYCrosshairBlob : public CXYBlob
{
public:
//	CXYDotBlob(){};
//	virtual ~CXYDotBlob(){};

	virtual void Update(CDC *pdc);
};

class CXYTrailBlob : public CXYBlob
{
public:
	CXYTrailBlob();
//	virtual ~CXYDotBlob(){};

	virtual void Update(CDC *pdc);

	virtual void SetFGColour(COLORREF c);
	virtual void SetBGColour(COLORREF c);

	void RespreadCols();

	COLORREF cols[16];
	CPoint oldpts[16];
	char oldpts_head;
};

class CXYParticleBlob : public CXYBlob
{
public:
//	CXYDotBlob(){};
//	virtual ~CXYDotBlob(){};

	virtual void Update(CDC *pdc);

	virtual void SetFGColour(COLORREF c);
	virtual void SetBGColour(COLORREF c);

	void RespreadCols();

	COLORREF cols[16];
	CParticleStream strm;
};


#endif
