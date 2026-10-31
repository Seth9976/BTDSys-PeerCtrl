/*
Copyright (c) 2005-2011, Edward Powley
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

/* note: the above refers to the code, not the chords ;) */

#include "machine.h"

SChord ChordTable[NUMCHORDS] =
{
//name      numocts
//            numnotes
//              <--notes---->
"none",		1,1,1,1,1,1,1,1,
"maj",		1,3,1,5,8,1,1,1,
"min",		1,3,1,4,8,1,1,1,
"sus4",		1,3,1,6,8,1,1,1,
"sus2",		1,3,1,3,8,1,1,1,
"aug",		1,3,1,5,9,1,1,1,
"dim",		1,3,1,4,7,1,1,1,
"°",		1,4,1,4,7,10,1,1,
"6",		1,4,1,5,8,10,1,1,
"m6",		1,4,1,4,8,10,1,1,
"7",		1,4,1,5,8,11,1,1,
"m7",		1,4,1,4,8,11,1,1,
"maj7",		1,4,1,5,8,12,1,1,
"mmaj7",	1,4,1,4,8,12,1,1,
"9",		2,5,1,5,8,11,15,1,
"m9",		2,5,1,4,8,11,15,1,
"maj9",		2,5,1,5,8,12,15,1,
"mmaj9",	2,5,1,4,8,12,15,1,
"11",		2,6,1,5,8,11,15,18,
"m11",		2,6,1,4,8,11,15,18,
"maj11",	2,6,1,5,8,12,15,18,
"mmaj11",	2,6,1,4,8,12,15,18,
"13",		2,6,1,5,8,11,15,22,
"m13",		2,6,1,4,8,11,15,22,
"maj13",	2,6,1,5,8,12,15,22,
"mmaj13",	2,6,1,4,8,12,15,22,
"add9",		2,4,1,5,8,15,1,1,
"madd9",	2,4,1,4,8,15,1,1,
"6add9",	2,5,1,5,8,10,15,1,
"m6add9",	2,5,1,4,8,10,15,1,
"7add11",	2,5,1,5,8,11,18,1,
"m7add11",	2,5,1,4,8,11,18,1,
"maj7add11",2,5,1,5,8,12,18,1,
"mmaj7add11",2,5,1,4,8,12,18,1,
"7add13",	2,5,1,5,8,11,22,1,
"m7add13",	2,5,1,4,8,11,22,1,
"maj7add13",2,5,1,5,8,12,22,1,
"mmaj7add13",2,5,1,4,8,12,22,1,
"7b5",		1,4,1,5,7,11,1,1,
"7#5",		1,4,1,5,9,11,1,1,
"7b9",		2,5,1,5,8,11,14,1,
"7#9",		2,5,1,5,8,11,16,1,
"7#5b9",	2,5,1,5,9,11,14,1,
"m7b5",		1,4,1,4,7,11,1,1,
"m7#5",		1,4,1,4,9,11,1,1,
"m7b9",		2,5,1,4,8,11,14,1,
"9#11",		2,6,1,5,8,11,15,19,
"9b13",		2,6,1,5,8,11,15,21,
"6sus4",	1,4,1,6,8,10,1,1,
"7sus4",	1,4,1,6,8,11,1,1,
"maj7sus4",	1,4,1,6,8,12,1,1,
"9sus4",	2,5,1,6,8,11,15,1,
"maj9sus4",	2,5,1,6,8,12,15,1,
"custom",	1,1,1,1,1,1,1,1		//custom filled in at runtime
};
