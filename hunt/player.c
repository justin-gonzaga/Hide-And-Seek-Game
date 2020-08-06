////////////////////////////////////////////////////////////////////////
// Runs a player's game turn ...
//
// Can  produce  either a hunter player or a Dracula player depending on
// the setting of the I_AM_DRACULA #define
//
// This  is  a  dummy  version of the real player.c used when you submit
// your AIs. It is provided so that you can test whether  your  code  is
// likely to compile ...
//
// Note that this is used to drive both hunter and Dracula AIs. It first
// creates an appropriate view, and then invokes the relevant decideMove
// function,  which  should  use the registerBestPlay() function to send
// the move back.
//
// The real player.c applies a timeout, and will halt your  AI  after  a
// fixed  amount of time if it doesn 't finish first. The last move that
// your AI registers (using the registerBestPlay() function) will be the
// one used by the game engine. This version of player.c won't stop your
// decideMove function if it goes into an infinite loop. Sort  that  out
// before you submit.
//
// Based on the program by David Collien, written in 2012
//
// 2017-12-04	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.3	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#ifdef I_AM_DRACULA
# include "dracula.h"
# include "DraculaView.h"
#else
# include "hunter.h"
# include "HunterView.h"
#endif

// Moves given by registerBestPlay are this long (including terminator)
#define MOVE_SIZE 3

// The minimum static globals I can get away with
static char latestPlay[MOVE_SIZE] = "";
static char latestMessage[MESSAGE_SIZE] = "";

// A pseudo-generic interface, which defines
// - a type `View',
// - functions `ViewNew', `decideMove', `ViewFree',
// - a trail `xtrail', and a message buffer `xmsgs'.
#ifdef I_AM_DRACULA

typedef DraculaView View;

# define ViewNew DvNew
# define decideMove decideDraculaMove
# define ViewFree DvFree

# define xPastPlays "GMN.... SGA.... HRO.... MGR.... DPA.V.. GED.... SBC.... HBI.... MAL.... DSTT... GLV.... SBE.... HAS.... MBA.... DHIT... GIR.... SKL.... HIO.... MBO.... DD2T... GAO.... SBD.... HAT.... MBB.... DNUT... GCA.... SPR.... HVA.... MAO.... DMUT... GGR.... SBR.... HSA.... MCA.... DZUT.V. GAL.... SHA.... HSO.... MLS.... DSTT.M. GMA.... SCO.... HBE.... MSN.... DHIT.M. GLS.... SAM.... HBC.... MMA.... DD2T.M. GSN.... SBU.... HCN.... MSR.... DNUT.M. GBA.... SLE.... HBS.... MPA.... DMUT.M. GMS.... SEC.... HVR.... MBU.... DZUT.M. GCG.... SAO.... HSZ.... MAM.... DST.VM. GTS.... SBB.... HBD.... MCO.... DHIT.M. GGO.... SBO.... HKL.... MFR.... DD5T.M. GFL.... SCF.... HCD.... MBR.... DNUT.M. GMI.... SGE.... HGA.... MHA.... DPRT.M. GMR.... SFL.... HBC.... MLI.... DVIT.M. GBO.... SGO.... HBE.... MMU.... DBDT.V. GBB.... SMR.... HSJ.... MMI.... DHIT.M. GNA.... SMS.... HJM.... MGO.... DD2T.M. GCF.... SAL.... HZA.... MFL.... DKLT.M. GGE.... SGR.... HMU.... MNP.... DCDT.M. GMR.... SAL.... HVE.... MFL.... DGAT.M. GGO.... SMS.... HBDTTT. MVE.... DBCT... GVE.... SMR.... HKLT... MVI.... DSO.V.. GBD.... SGO.... HCDT... MBD.... DD2T... GKL.... SVE.... HBE.... MGAT... DCNT... GGA.... SBD.... HGA.... MBCTT.. DVRT... GBC.... SBC.... HBC.... MBE.... DHIT... GCNT... SCN.... HCN.... MBC.... DBS.... GBS.... SVRTT.. HVR.... MCN.... DIO..V. GVR.... SBS.... HSA.... MVR.... DTS.... GBE.... SVR.... HSO.... MBS.... DROT... GVR.... SSO.... HVR.... MVR.... DFLT... GSO.... SVR.... HBS.... MSA.... DHIT... GVR.... SSA.... HVR.... MSO.... DD2T... GSA.... SVR.... HBS.... MVR.... DVET... GVR.... SCN.... HVR.... MBE.... DMU.V.. GBS.... SVR.... HCN.... MVR.... DSTT.M. GIO.... SBE.... HSZ.... MBS.... DNUT.M. GAS.... SSJ.... HJM.... MIO.... DHIT.M. GBI.... SVA.... HSJ.... MAS.... DD2T.M. GNP.... SAT.... HVA.... MAS.... DPRT.M. GRO.... SIO.... HAT.... MBI.... DVIT.V. GFL.... SAS.... HIO.... MRO...."
# define xMsgs { "", "", "", "" }

#else

typedef HunterView View;

# define ViewNew HvNew
# define decideMove decideHunterMove
# define ViewFree HvFree

// # define xPastPlays "GZA.... SED.... HZU...."
// Test strings
# define xPastPlays "GED.... SGA.... HRO.... MGR.... DST.V.. GMN.... SCD.... HMI.... MMA.... DC?T... GED.... SGA.... HMR.... MAL.... DC?T... GLV.... SBC.... HMS.... MSR.... DC?T... GSW.... SBE.... HAL.... MMR.... DPAT... GLO.... SSJ.... HLS.... MZU.... DC?T... GLO.... SSJ.... HLS.... MZU.... DC?T.V. GED.... SBE.... HAO.... MMR.... DC?T.M. GMN.... SSJ.... HGW.... MPAT... DC?T.M. GLO.... SZA.... HAO...."
# define xMsgs { "", "", "" }

#endif

int main(void)
{
	char *pastPlays = xPastPlays;
	Message msgs[] = xMsgs;

	View state = ViewNew(pastPlays, msgs);
	decideMove(state);
	ViewFree(state);

	printf("Move: %s, Message: %s\n", latestPlay, latestMessage);
	return EXIT_SUCCESS;
}

// Saves characters from play (and appends a terminator)
// and saves characters from message (and appends a terminator)
void registerBestPlay(char *play, Message message)
{
	strncpy(latestPlay, play, MOVE_SIZE - 1);
	latestPlay[MOVE_SIZE - 1] = '\0';

	strncpy(latestMessage, message, MESSAGE_SIZE - 1);
	latestMessage[MESSAGE_SIZE - 1] = '\0';
}
