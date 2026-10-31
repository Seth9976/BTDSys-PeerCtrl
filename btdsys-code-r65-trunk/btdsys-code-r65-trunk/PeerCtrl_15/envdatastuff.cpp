/*
envdatastuff.cpp: .....
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

#include "stdafx.h"
#include "ctrl.h"
#include "../mdk/mdk.h"

//#include <fstream.h>

CEnvData::CEnvData()
{
//	numpoints = 2;
	points.push_back (0);
	points.push_back (65535);
	points.push_back (65535);
	points.push_back (0);

//	points = new long[4];
}

/////////////////////////////////////////////////////////////////////////////

CEnvData::~CEnvData()
{
/*	if (points != NULL)
		delete[] points;
*/
	points.clear();
}

/////////////////////////////////////////////////////////////////////////////

void CEnvData::Set(int NumPoints, long *Points)
{
	points.clear();

	for (int i=0; i < NumPoints*2; i++)
	{
		points.push_back(Points[i]);
	}
}

/////////////////////////////////////////////////////////////////////////////

void CEnvData::CopyFrom(const CEnvData& from)
{
	points = from.points;
}

void CEnvData::ReadData(CMachineDataInput * const pi)
{
	int n;
	pi->Read(n);

	long *p;
	p = new long[n*2];
	for (int i=0; i<n*2; i++)
		pi->Read(&p[i], sizeof(long));

	Set(n, p);
}

/////////////////////////////////////////////////////////////////////////////

void CEnvData::WriteData(CMachineDataOutput * const po)
{
	po->Write(numpoints());
	for (int i=0; i<points.size(); i++)
		po->Write(points[i]);
}