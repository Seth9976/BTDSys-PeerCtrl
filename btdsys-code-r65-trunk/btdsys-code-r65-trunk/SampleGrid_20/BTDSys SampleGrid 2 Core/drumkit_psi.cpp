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

#include "stdafx.h"

#include <windows.h>
#include "common.h"
#include "resource.h"
#include "24bit.h"
#include "drumkit.h"

static char const *pszPSIGroups[] = {"Kick","Snare","Hats","Cym","FX","Samp"};

void CDrumkit::LoadPSIFromFile(FILE *f, char *filename)
{
	char stat[256];
	Reset();
	nKitType = ktPSI;

	CLoadStatusDlg dlg;

	dlg.SetWindowText("BTDSys SampleGrid - Loading PSI Drumkit");
	strcpy(stat, "Loading...");	PUT_STATUS;

	unsigned int numSamples;

	fseek(f, 4, SEEK_SET);
	fread(&numSamples, 4, 1, f);
	fread(Author, 1, 64, f); Author[63] = 0;
	fread(Comment,1,128, f); Comment[127] = 0;

	unsigned char t;
	unsigned int datalen;

	fseek(f, 8+64+128, SEEK_SET);

	char Name[33];
//	CDrumkitSample samp;

	for (unsigned int i=0; i<numSamples; i++)
	{
		sprintf(stat, "At file pos %i", ftell(f));
		PUT_STATUS;
		sprintf(stat, "Loading sample %i of %i...", i, numSamples);
		PUT_STATUS;

		fread(Name,	1, 32, f); Name[32] = 0;
		char *nsp = &Name[31];
		while (*nsp == ' ' && nsp > Name) *nsp-- = 0; //remove trailing spaces

		fread(&t,	1,  1, f);
		t--;
		int ngrp = t;
		if (t<0 || t>=6) //eep! skip it
		{
			sprintf(stat, "Skipping '%s' - bad class %i", Name, t); PUT_STATUS;
			fseek(f, 2, SEEK_CUR);
			fread(&datalen, 4, 1, f);
			if (datalen)
				fseek(f, 2*datalen, SEEK_CUR);
			else
			{
				sprintf(stat, "Zero data"); PUT_STATUS;
			}

		}
		else
		{
			bool b = true;
			int add=0;
			int idx;
			while (b && add < 6)
			{
				idx = ngrp + 1 + add;
				while (sample[idx].HasData()) idx += 6;

				if (idx > 200)
					add++;
				else
					b = false;
			}

			if (b)
			{
				MessageBox(NULL,"Sorry, this kit has too many samples","",MB_OK);
				break;
			}

			sample[idx].Reset();

			sprintf(stat, "Sample '%s'",Name); PUT_STATUS;
			sprintf(sample[idx].Name, "%s (%s)", Name, pszPSIGroups[ngrp]);

			CDrumkitLayerT<short> *lyr = new CDrumkitLayerT<short>;
			lyr->nVelMin = 0;
			lyr->nVelMax = 127;
			lyr->fVol = 1.0f;
			lyr->fVolCompensate = 1.0f;
			lyr->bStereo = false;
			lyr->nSampleRate = 44100;
			lyr->bDataLoaded = false;
			strcpy(lyr->dataFileName, filename);
			sample[idx].layers.push_back(lyr);

			fread(&t,	1,  1, f);
			lyr->fRate = (float)t / 100.0f;
			fread(&t,	1,  1, f);
			lyr->fVol *= (float)t / 100.0f;

			fread(&lyr->numSamples,	4,  1, f);
			lyr->datasize = lyr->numSamples * 2;

			lyr->dataoffset = ftell(f);
			fseek(f,lyr->datasize,SEEK_CUR);

/*			if (lyr->numSamples > 0)
			{
				sprintf(stat, "%i samples", lyr->numSamples); PUT_STATUS;
				lyr->data = new short[lyr->numSamples];

				fread(lyr->data, 2, lyr->numSamples, f);
			}
			else
			{
				sprintf(stat, "Zero data !!!"); PUT_STATUS;
			}
*/		}

	}

	sprintf(stat, "Bye bye"); PUT_STATUS;
}
