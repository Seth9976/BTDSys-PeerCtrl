/************************************************************************************/
/*                                                                                  */
/*                             P. DooM's BUZZHACK 1.01                              */
/*                                   headerfile                                     */
/*                                                                                  */
/*  You may only use this library if you follow these simple rules:                 */
/*                                                                                  */
/*  - Your machine must have the word HACK in the machine name (capital letters)    */
/*  - If the machine has a help file, you must state in the help file that it is    */
/*    a HACK-machine, which could cause problems with older or newer versions of    */
/*    BUZZ. BUZZHACK was designed for the Oct. 2000 version of BUZZ.                */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/*  May 2001 by Peter "P. DooM" Kaufmann             p_doom@space.ch                */
/*                                                   http://combi.agri.ch/kaufmann/ */
/*                                                                                  */
/************************************************************************************/

#ifndef __BUZZHACK_H
#define __BUZZHACK_H

//Call this function first before using any other buzzhack functions
//RETURN:      FALSE: couldn't initialize buzzhack
extern int buzzhack_Initbuzzhack();

//Set the current song position (in ticks)
//PARAMETERS:  pos:   song position (ticks)
//RETURN:      FALSE: couldn't set song position
extern int buzzhack_SetSongPosition(int pos);

//Get the current song position
//RETURN:      song position (ticks)
extern int buzzhack_GetSongPosition();

//Get the current song tempo (BPM)
//RETURN:      song tempo (BPM)
extern int buzzhack_GetTempo();

//Set the current song tempo (BPM). Note: The tempo won't
//be displayed in the tempo control of BUZZ!
//PARAMETERS:  tempo: song tempo (BPM)
//RETURN:      (no meaning)
extern int buzzhack_SetTempo(int tempo);

//Sets the current song tempo (BPM) and displays it in the
//BUZZ tempo control. Note: If you use this function frequently, BUZZ
//might pop up some warning messages from time to time.
//PARAMETERS:  tempo: song tempo (BPM)
//RETURN:      FALSE: couldn't set tempo
extern int buzzhack_SetTempoEdit(int tempo);

//Start playing
//RETURN:      FALSE: couldn't play
extern int buzzhack_Play();

//Stop playing
//RETURN:      FALSE: couldn't stop
extern int buzzhack_Stop();

//Enables or disables the tempo control in BUZZ.
//PARAMETERS:  enable: 1=enable edit, 0=disable
//RETURN:      FALSE: couldn't enable tempo edit
extern int buzzhack_EnableTempoEdit(int enable);

//Get current TPB setting
//RETURN:      TPB
extern int buzzhack_GetTPB();

//Get the loop start position in ticks
//RETURN:      loop start (ticks)
extern int buzzhack_GetLoopStart();

//Get the loop end position in ticks
//RETURN:      loop end (ticks)
extern int buzzhack_GetLoopEnd();

//Get the song end position in ticks
//RETURN:      song end (ticks)
extern int buzzhack_GetSongEnd();
#endif