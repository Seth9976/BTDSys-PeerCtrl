/************************************************************************************/
/*                                                                                  */
/*                             P. DooM's BUZZHACK 1.01                              */
/*                                                                                  */
/*                         ! read the rules (buzzhack.h) !                          */
/*                                                                                  */
/*                                                                                  */
/*  May 2001 by Peter "P. DooM" Kaufmann             p_doom@space.ch                */
/*                                                   http://combi.agri.ch/kaufmann/ */
/*                                                                                  */
/************************************************************************************/


#include <windows.h>
#include <stdio.h>
#include "buzzhack.h"

int *HACKsong_pos_pointer = (int *)0x004F01B0;
int *HACKsong_pos = (int *)(*HACKsong_pos_pointer + 0x39C);   //current song position pointer. write and read.

int *HACKsong_end = (int *)(*HACKsong_pos_pointer + 0x398);   //song end position
int *HACKloop_start = (int *)(*HACKsong_pos_pointer + 0x390); //loop start position
int *HACKloop_end = (int *)(*HACKsong_pos_pointer + 0x394);   //loop end position

int *HACKbuzz_bpm = (int *)0x004F0180;                        //current BPM. write and read.
int *HACKbuzz_tpb = (int *)0x004F0184;                        //current TPB. write and read.

HWND buzzhwnd  = NULL;
HWND tempohwnd = NULL;

HWND FindBUZZhwnd()
{
  char t[1024];
  HWND wnd = GetActiveWindow();

  if(wnd == NULL)
    return NULL;

  GetWindowText(wnd, t, 1000);
  while(strstr(t, "Buzz") == NULL && wnd != NULL)
  {
    wnd = GetWindow(wnd, GW_HWNDNEXT);
    if(wnd != NULL)
      GetWindowText(wnd, t, 1000);
  }

  if(strstr(t, "Buzz") != NULL)
    return wnd;
  
  GetWindowText(wnd, t, 1000);
  while(strstr(t, "Buzz") == NULL && wnd != NULL)
  {
    wnd = GetWindow(wnd, GW_HWNDPREV);
    if(wnd != NULL)
      GetWindowText(wnd, t, 1000);
  }

  if(strstr(t, "Buzz") != NULL)
    return wnd;
  
  return NULL;
}

HWND BUZZGetTempoHWND(HWND hwnd)
{
  HWND child;
  char s[128] = {""};
  int  i;

  child = GetWindow(hwnd, GW_CHILD);

  if(child != NULL)
  {
    GetWindowText(child, s, 100);
    while(GetNextWindow(child, GW_HWNDNEXT) != NULL && stricmp(s, "Standard") != 0)
    {
      child = GetNextWindow(child, GW_HWNDNEXT);
      GetWindowText(child, s, 100);
    }
  }

  if(stricmp(s, "Standard") != 0)
    return NULL;

  child = GetWindow(child, GW_CHILD);

  if(child != NULL)
  {
    GetWindowText(child, s, 100);
    while(GetNextWindow(child, GW_HWNDNEXT) != NULL && stricmp(s, "Master") != 0)
    {
      child = GetNextWindow(child, GW_HWNDNEXT);
      GetWindowText(child, s, 100);
    }
  }

  if(stricmp(s, "Master") != 0)
    return NULL;

  child = GetWindow(child, GW_CHILD);

  if(child != NULL)
  {
    i=0;
    while(GetNextWindow(child, GW_HWNDNEXT) != NULL)
    {
      child = GetNextWindow(child, GW_HWNDNEXT);

      if(i == 2)
      {
        return child;
      }

      i++;
    }
  }

  return NULL;
}

int buzzhack_SetTempoEdit(int tempo)
{
  char s[128];

  if(buzzhwnd == NULL || tempohwnd == NULL)
    return FALSE;

  if(tempo < 16)
    tempo = 16;

  if(tempo > 512)
    tempo = 512;

  sprintf(s, "%i", tempo);
  if(!SendMessage(tempohwnd, WM_SETTEXT, 0, (int)s))
    return FALSE;

  SendMessage(tempohwnd, WM_KILLFOCUS, 0, 0);

  return TRUE;
}

//1 = enable, 0 = disable
int buzzhack_EnableTempoEdit(int enable)
{
  if(buzzhwnd == NULL || tempohwnd == NULL)
    return FALSE;

  SendMessage(tempohwnd, WM_ENABLE, enable, 0);

  return TRUE;
}

int buzzhack_Initbuzzhack()
{
  buzzhwnd = FindBUZZhwnd();

  if(buzzhwnd == NULL)
    return FALSE;

  tempohwnd = BUZZGetTempoHWND(buzzhwnd);

  return TRUE;
}

int buzzhack_SetSongPosition(int pos)
{
  if(buzzhwnd != NULL)
  {
    if(pos < 0)
      pos = 0;

    *HACKsong_pos = pos;

    return TRUE;
  }

  return FALSE;
}

int buzzhack_GetSongPosition()
{
  return *HACKsong_pos;
}

int buzzhack_GetSongEnd()
{
  return *HACKsong_end;
}

int buzzhack_SetTempo(int tempo)
{
  if(tempo < 16)
    tempo = 16;

  if(tempo > 512)
    tempo = 512;

  *HACKbuzz_bpm = tempo;

  return TRUE;
}

int buzzhack_GetTempo()
{
  return *HACKbuzz_bpm;
}

int buzzhack_GetLoopStart()
{
  return *HACKloop_start;
}

int buzzhack_GetLoopEnd()
{
  return *HACKloop_end;
}

int buzzhack_Play()
{
  if(buzzhwnd != NULL)
  {
    SendMessage(buzzhwnd, WM_COMMAND, 0x00018015, 0); //start (F5)

    return TRUE;
  }

  return FALSE;
}

int buzzhack_Stop()
{
  if(buzzhwnd != NULL)
  {
    SendMessage(buzzhwnd, WM_COMMAND, 0x00018016, 0); //stop (F8)

    return TRUE; 
  }

  return FALSE;
}

int buzzhack_GetTPB()
{
  return *HACKbuzz_tpb;
}