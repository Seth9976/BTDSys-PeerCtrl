#ifndef __GLOBALSH
#define __GLOBALSH

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "../../mdk/mdk.h"

__forceinline void ShowLastError(const char *src)
{
	DWORD a=GetLastError();
	if((a!=ERROR_SUCCESS)&&(a!=ERROR_CANCELLED))
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,NULL,a,MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),(LPTSTR)&lpMsgBuf,0,NULL);
		char *text=new char[strlen((LPCTSTR)lpMsgBuf)+strlen(src)+8];
		sprintf(text,"%s:\n\n%s",src,(LPCTSTR)lpMsgBuf);
		MessageBox(NULL,text,"Error",MB_OK|MB_ICONINFORMATION);
		LocalFree(lpMsgBuf);
		delete[] text;
	}
}

enum LOADERR
{
	LOADERR_SUCCESS,
		LOADERR_NOFILENAME,
		LOADERR_NOFILE,
		LOADERR_NOMAGIC,
		LOADERR_NEWERVERSION,
		LOADERR_FAIL,
		LOADERR_WTF
};

__forceinline void ShowLoadError(int err)
{
	switch(err)
	{
	case LOADERR_SUCCESS:MessageBox(NULL,"Load error:\nNo error.","Error",0);break;
	case LOADERR_NOFILENAME:MessageBox(NULL,"Load error:\nNo filename given.","Error",0);break;
	case LOADERR_NOFILE:MessageBox(NULL,"Load error:\nCouldn't open file.","Error",0);break;
	case LOADERR_NOMAGIC:MessageBox(NULL,"Load error:\nInvalid magic.","Error",0);break;
	case LOADERR_NEWERVERSION:MessageBox(NULL,"Load error:\nUnknown file version.","Error",0);break;
	case LOADERR_FAIL:MessageBox(NULL,"Load error:\nRead failure.","Error",0);break;
	default:MessageBox(NULL,"Load error:\nUnknown error.","Error",0);break;
	}
}

#endif