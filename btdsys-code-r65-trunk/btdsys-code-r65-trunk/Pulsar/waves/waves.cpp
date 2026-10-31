/*
Copyright (c) 2004-2006, Edward Powley
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

// waves.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdio.h>
#include <windows.h>
#include <vector>
using namespace std;

char *ProcessFile(char *fname)
{
	printf("%s\n",fname);
	static char hname[256], noext[256];
	strcpy(noext,fname);
	noext[strlen(noext)-4] = 0;
	strcpy(hname,noext);
	strcat(hname,".h");

	FILE *ff = fopen(fname,"rb");
	FILE *fh = fopen(hname,"wt");
	if (ff && fh)
	{
		char dname[256]; strcpy(dname,fname);
		dname[strlen(dname)-3] = 0; strcat(dname,"dat");
		FILE *fd = fopen(dname,"rb");
		bool stereo = (fd != NULL);
		if (fd) fclose(fd);

		fprintf(fh,"short ps_%s[] = {\n",noext);
		signed short smp;
		int numsamples = 0;
		while (!feof(ff))
		{
			fread(&smp,2,1,ff);
			fprintf(fh,"\t%i,",smp);
			numsamples++;
			if (numsamples % 8 == 0)
				fprintf(fh,"\n");
		}
		fprintf(fh," };\n\n");

		if (stereo) numsamples /= 2;

		fprintf(fh,"#define NS_%s %i\n",noext,numsamples);
		fprintf(fh,"#define BS_%s %s\n",noext,stereo ? "true" : "false");
		fclose(fh);
		fclose(ff);
	}

	return noext;
}

int main(int argc, char* argv[])
{
	WIN32_FIND_DATA fd;
	HANDLE hf = FindFirstFile("*.pcm",&fd);
	if (hf)
	{
		FILE *fh = fopen("../iwaves.cpp","wt");
		if (fh)
		{
			vector<char *> files;
			char *s;
			do
			{
				s = new char[256];
				strcpy(s, ProcessFile(fd.cFileName));
				files.push_back(s);
				fprintf(fh,"#include \"waves/%s.h\"\n",s);
			}
			while (FindNextFile(hf,&fd));

			fprintf(fh,"\n#define NUM_INTERNAL_WAVES %i\n",files.size());

			char **p;

			fprintf(fh,"short *g_pWaveData[] = {");
			for (p = files.begin(); p < files.end(); p++) fprintf(fh,"ps_%s, ",*p);
			fprintf(fh,"0};\n");

			fprintf(fh,"int g_pWaveLen[] = {");
			for (p = files.begin(); p < files.end(); p++) fprintf(fh,"NS_%s, ",*p);
			fprintf(fh,"0};\n");

			fprintf(fh,"bool g_bWaveStereo[] = {");
			for (p = files.begin(); p < files.end(); p++) fprintf(fh,"BS_%s, ",*p);
			fprintf(fh,"false};\n");

			fprintf(fh,"char *g_pWaveName[] = {");
			for (p = files.begin(); p < files.end(); p++) fprintf(fh,"\"%s\", ",*p);
			fprintf(fh,"\"\"};\n");

			fclose(fh);

			for (p = files.begin(); p < files.end(); p++)
			{
				delete[] *p;
			}
		}
		FindClose(hf);
	}

	return 0;
}
