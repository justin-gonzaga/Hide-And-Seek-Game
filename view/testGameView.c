////////////////////////////////////////////////////////////////////////
// COMP2521 20T2 ... the Fury of Dracula
// testGameView.c: test the GameView ADT
//
// As supplied, these are very simple tests.  You should write more!
// Don't forget to be rigorous and thorough while writing tests.
//
// 2014-07-01	v1.0	Team Dracula <cs2521@cse.unsw.edu.au>
// 2017-12-01	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2018-12-31	v1.1	Team Dracula <cs2521@cse.unsw.edu.au>
// 2020-07-10	v1.2	Team Dracula <cs2521@cse.unsw.edu.au>
//
////////////////////////////////////////////////////////////////////////

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Game.h"
#include "GameView.h"
#include "Places.h"
#include "testUtils.h"

int main(void)
{
	{///////////////////////////////////////////////////////////////////
	
		printf("Basic initialisation\n");

		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == NOWHERE);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("After Lord Godalming's turn\n");

		char *trail =
			"GST....";
		
		Message messages[1] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DR_SEWARD);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == NOWHERE);

		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("After Mina Harker's turn\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB....";
		
		Message messages[4] = {};
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 0);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == NOWHERE);

		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("After Dracula's turn\n");

		char *trail =
			"GST.... SAO.... HZU.... MBB.... DC?.V..";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha"
		};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 1);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetHealth(gv, PLAYER_DRACULA) == GAME_START_BLOOD_POINTS);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == STRASBOURG);
		assert(GvGetPlayerLocation(gv, PLAYER_DR_SEWARD) == ATLANTIC_OCEAN);
		assert(GvGetPlayerLocation(gv, PLAYER_VAN_HELSING) == ZURICH);
		assert(GvGetPlayerLocation(gv, PLAYER_MINA_HARKER) == BAY_OF_BISCAY);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == CITY_UNKNOWN);

		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Encountering Dracula\n");

		char *trail =
			"GST.... SAO.... HCD.... MAO.... DGE.V.. "
			"GGEVD..";
		
		Message messages[] = {
			"Hello", "Goodbye", "Stuff", "...", "Mwahahahaha",
			"Aha!"
		};
		
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_DRACULA_ENCOUNTER);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - LIFE_LOSS_HUNTER_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == GENEVA);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GENEVA);
		assert(GvGetVampireLocation(gv) == NOWHERE);

		GvFree(gv);
		printf("Test passed\n");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula doubling back at sea, "
		       "and losing blood points (Hunter View)\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DS?.... "
			"GST.... SST.... HST.... MST.... DD1....";
		
		Message messages[] = {
			"Party at Geneva", "Okay", "Sure", "Let's go", "Mwahahahaha",
			"", "", "", "", "Back I go"
		};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_LORD_GODALMING);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == SEA_UNKNOWN);

		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}


	{///////////////////////////////////////////////////////////////////
	
		printf("Test for Dracula doubling back at sea, "
			   "and losing blood points (Dracula View)\n");

		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DEC.... "
			"GST.... SST.... HST.... MST.... DD1.... "
			"GFR.... SFR.... HFR.... MFR....";
		
		Message messages[14] = {
			"Hello", "Rubbish", "Stuff", "", "Mwahahah",
			"Aha!", "", "", "", "Back I go"};
		
		GameView gv = GvNew(trail, messages);

		assert(GvGetRound(gv) == 2);
		assert(GvGetPlayer(gv) == PLAYER_DRACULA);
		assert(GvGetScore(gv) == GAME_START_SCORE - 2 * SCORE_LOSS_DRACULA_TURN);
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS - (2 * LIFE_LOSS_SEA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == ENGLISH_CHANNEL);

		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Checking that hunters' health points are capped\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE....";
	
		Message messages[6] = {};
		GameView gv = GvNew(trail, messages);
	
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing a hunter 'dying'\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DSTT... "
			"GGE.... SGE.... HGE.... MGE.... DHIT... "
			"GGE.... SGE.... HGE.... MGE.... DD1T... "
			"GSTTTTD";
		
		Message messages[21] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetScore(gv) == GAME_START_SCORE
		                         - 4 * SCORE_LOSS_DRACULA_TURN
		                         - SCORE_LOSS_HUNTER_HOSPITAL);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == 0);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == HOSPITAL_PLACE);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == STRASBOURG);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing Dracula doubling back to Castle Dracula\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DCD.V.. "
			"GGE.... SGE.... HGE.... MGE.... DD1T...";
		
		Message messages[10] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_DRACULA) ==
				GAME_START_BLOOD_POINTS + (2 * LIFE_GAIN_CASTLE_DRACULA));
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CASTLE_DRACULA);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing vampire/trap locations\n");
		
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DCD.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GSZ.... SGE.... HGE.... MGE....";
		
		Message messages[19] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == GALATZ);
		assert(GvGetVampireLocation(gv) == CASTLE_DRACULA);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == GALATZ && traps[1] == KLAUSENBURG);
		free(traps);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing trap locations after one is destroyed\n");
		
		char *trail =
			"GVI.... SGE.... HGE.... MGE.... DBC.V.. "
			"GBD.... SGE.... HGE.... MGE.... DKLT... "
			"GSZ.... SGE.... HGE.... MGE.... DGAT... "
			"GBE.... SGE.... HGE.... MGE.... DCNT... "
			"GKLT... SGE.... HGE.... MGE....";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) ==
				GAME_START_HUNTER_LIFE_POINTS - LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_LORD_GODALMING) == KLAUSENBURG);
		assert(GvGetVampireLocation(gv) == BUCHAREST);
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 2);
		sortPlaces(traps, numTraps);
		assert(traps[0] == CONSTANTA && traps[1] == GALATZ);
		free(traps);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing a vampire maturing\n");
		
		char *trail =
			"GGE.... SGE.... HGE.... MGE.... DC?.V.. "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T... "
			"GGE.... SGE.... HGE.... MGE.... DC?T.V.";
		
		Message messages[35] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetScore(gv) == GAME_START_SCORE
		                         - 7 * SCORE_LOSS_DRACULA_TURN
		                         - SCORE_LOSS_VAMPIRE_MATURES);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
	
		printf("Testing move/location history\n");
		
		char *trail =
			"GLS.... SGE.... HGE.... MGE.... DST.V.. "
			"GCA.... SGE.... HGE.... MGE.... DC?T... "
			"GGR.... SGE.... HGE.... MGE.... DC?T... "
			"GAL.... SGE.... HGE.... MGE.... DD3T... "
			"GSR.... SGE.... HGE.... MGE.... DHIT... "
			"GSN.... SGE.... HGE.... MGE.... DC?T... "
			"GMA.... SSTTTV.";
		
		Message messages[32] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetHealth(gv, PLAYER_DR_SEWARD) ==
				GAME_START_HUNTER_LIFE_POINTS - 2 * LIFE_LOSS_TRAP_ENCOUNTER);
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == CITY_UNKNOWN);
		assert(GvGetVampireLocation(gv) == NOWHERE);
		
		// Lord Godalming's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_LORD_GODALMING,
			                                  &numMoves, &canFree);
			assert(numMoves == 7);
			assert(moves[0] == LISBON);
			assert(moves[1] == CADIZ);
			assert(moves[2] == GRANADA);
			assert(moves[3] == ALICANTE);
			assert(moves[4] == SARAGOSSA);
			assert(moves[5] == SANTANDER);
			assert(moves[6] == MADRID);
			if (canFree) free(moves);
		}
		
		// Dracula's move/location history
		{
			int numMoves = 0; bool canFree = false;
			PlaceId *moves = GvGetMoveHistory(gv, PLAYER_DRACULA,
			                                  &numMoves, &canFree);
			assert(numMoves == 6);
			assert(moves[0] == STRASBOURG);
			assert(moves[1] == CITY_UNKNOWN);
			assert(moves[2] == CITY_UNKNOWN);
			assert(moves[3] == DOUBLE_BACK_3);
			assert(moves[4] == HIDE);
			assert(moves[5] == CITY_UNKNOWN);
			if (canFree) free(moves);
		}
		
		{
			int numLocs = 0; bool canFree = false;
			PlaceId *locs = GvGetLocationHistory(gv, PLAYER_DRACULA,
			                                     &numLocs, &canFree);
			assert(numLocs == 6);
			assert(locs[0] == STRASBOURG);
			assert(locs[1] == CITY_UNKNOWN);
			assert(locs[2] == CITY_UNKNOWN);
			assert(locs[3] == STRASBOURG);
			assert(locs[4] == STRASBOURG);
			assert(locs[5] == CITY_UNKNOWN);
			if (canFree) free(locs);
		}
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}

	{///////////////////////////////////////////////////////////////////
	
		printf("Testing connections\n");
		
		char *trail = "";
		Message messages[] = {};
		GameView gv = GvNew(trail, messages);

		{
			printf("\tChecking Galatz road connections "
			       "(Lord Godalming, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, GALATZ, true, false,
			                                     false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Ionian Sea boat connections "
			       "(Lord Godalming, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, IONIAN_SEA, false, false,
			                                     true, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}

		{
			printf("\tChecking Paris rail connections "
			       "(Lord Godalming, Round 2)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     2, PARIS, false, true,
			                                     false, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BORDEAUX);
			assert(locs[1] == BRUSSELS);
			assert(locs[2] == COLOGNE);
			assert(locs[3] == LE_HAVRE);
			assert(locs[4] == MARSEILLES);
			assert(locs[5] == PARIS);
			assert(locs[6] == SARAGOSSA);
			free(locs);
		}
		
		{
			printf("\tChecking Athens rail connections (none)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, ATHENS, false, true,
			                                     false, &numLocs);
			
			assert(numLocs == 1);
			assert(locs[0] == ATHENS);
			free(locs);
		}

		{ // Large number of connections for hunters
			printf("\tChecking Atlantic Ocean boat connections (Hunters)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_VAN_HELSING ,
			                                     1, ATLANTIC_OCEAN, false, false,
			                                     true, &numLocs);
			assert(numLocs == 9);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ATLANTIC_OCEAN);
			assert(locs[1] == BAY_OF_BISCAY);
			assert(locs[2] == CADIZ);
			assert(locs[3] == ENGLISH_CHANNEL);
			assert(locs[4] == GALWAY);
			assert(locs[5] == IRISH_SEA);
			assert(locs[6] == LISBON);
			assert(locs[7] == MEDITERRANEAN_SEA);
			assert(locs[8] == NORTH_SEA);
	
			free(locs);
		}

		{ // Large number of connections for Dracula
			printf("\tChecking Atlantic Ocean boat connections (Dracula)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_DRACULA,
			                                     1, ATLANTIC_OCEAN, false, false,
			                                     true, &numLocs);
			assert(numLocs == 9);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ATLANTIC_OCEAN);
			assert(locs[1] == BAY_OF_BISCAY);
			assert(locs[2] == CADIZ);
			assert(locs[3] == ENGLISH_CHANNEL);
			assert(locs[4] == GALWAY);
			assert(locs[5] == IRISH_SEA);
			assert(locs[6] == LISBON);
			assert(locs[7] == MEDITERRANEAN_SEA);
			assert(locs[8] == NORTH_SEA);
	
			free(locs);
		}

		// =====================================================================
		// =====================================================================
		// =====================================================================
		// Checking type of hunter doesn't affect travel connections
		{
			printf("\tChecking Ionian Sea boat connections vs hunter type "
			       "(Lord Godalming, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, IONIAN_SEA, false, false,
			                                     true, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}

		{
			printf("\tChecking Ionian Sea boat connections vs hunter type "
			       "(Dr Seward, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_DR_SEWARD,
			                                     1, IONIAN_SEA, false, false,
			                                     true, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}

		{
			printf("\tChecking Ionian Sea boat connections vs hunter type "
			       "(Van Helsing Godalming, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_VAN_HELSING,
			                                     1, IONIAN_SEA, false, false,
			                                     true, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}

		{
			printf("\tChecking Ionian Sea boat connections vs hunter type "
			       "(Mina Harker, Round 1)\n");
			
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_MINA_HARKER,
			                                     1, IONIAN_SEA, false, false,
			                                     true, &numLocs);
			
			assert(numLocs == 7);
			sortPlaces(locs, numLocs);
			assert(locs[0] == ADRIATIC_SEA);
			assert(locs[1] == ATHENS);
			assert(locs[2] == BLACK_SEA);
			assert(locs[3] == IONIAN_SEA);
			assert(locs[4] == SALONICA);
			assert(locs[5] == TYRRHENIAN_SEA);
			assert(locs[6] == VALONA);
			free(locs);
		}
		// =====================================================================
		// =====================================================================
		// =====================================================================

		{ // Checking connections which are not present
			printf("\tChecking Atlantic Ocean boat connections\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, ATLANTIC_OCEAN, true, true,
			                                     false, &numLocs);
			
			assert(numLocs == 1);
			assert(locs[0] == ATLANTIC_OCEAN);
			free(locs);
		}

		{ // Checking connections where Dracula cannot travel
			printf("\tChecking SJSM Hospital road connection from Szeged "
					"(Dracula, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_DRACULA,
			                                     1, SZEGED, true, false,
			                                     false, &numLocs);
			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BELGRADE);
			assert(locs[1] == BUDAPEST);
			assert(locs[2] == KLAUSENBURG);
			assert(locs[3] == SZEGED);
			assert(locs[4] == ZAGREB);
			free(locs);
		}

		{ // Checking connections where Dracula cannot travel from adj city
			printf("\tChecking SJSM Hospital road connection from Sarajevo "
					"(Dracula, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_DRACULA,
			                                     1, SARAJEVO, true, false,
			                                     false, &numLocs);
			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BELGRADE);
			assert(locs[1] == SARAJEVO);
			assert(locs[2] == SOFIA);
			assert(locs[3] == VALONA);
			assert(locs[4] == ZAGREB);
			free(locs);
		}
		
		{ // Checking rail connections where Dracula cannot travel by rail
			printf("\tChecking Rail restriction from Frankfurt "
					"(Dracula, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_DRACULA,
			                                     1, FRANKFURT, false, true,
			                                     false, &numLocs);
			assert(numLocs == 1);
			assert(locs[0] == FRANKFURT);
			free(locs);
		}

		// =====================================================================
		// =====================================================================
		// =====================================================================
		// Checking round number doesn't affect connections
		{
			printf("\tChecking Galatz road connections vs round number "
			       "(Lord Godalming, Round 1)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, GALATZ, true, false,
			                                     false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Galatz road connections vs round number "
			       "(Lord Godalming, Round 2)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     2, GALATZ, true, false,
			                                     false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Galatz road connections vs round number "
			       "(Lord Godalming, Round 3)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     3, GALATZ, true, false,
			                                     false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Galatz road connections vs round number "
			       "(Lord Godalming, Round 4)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     4, GALATZ, true, false,
			                                     false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}

		{
			printf("\tChecking Galatz road connections vs round number "
			       "(Lord Godalming, Round 5)\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     5, GALATZ, true, false,
			                                     false, &numLocs);

			assert(numLocs == 5);
			sortPlaces(locs, numLocs);
			assert(locs[0] == BUCHAREST);
			assert(locs[1] == CASTLE_DRACULA);
			assert(locs[2] == CONSTANTA);
			assert(locs[3] == GALATZ);
			assert(locs[4] == KLAUSENBURG);
			free(locs);
		}
		// =====================================================================
		// =====================================================================
		// =====================================================================

		// Checking no connections returns empty
		{
			printf("\tChecking Munich with no connection types\n");
			int numLocs = -1;
			PlaceId *locs = GvGetReachableByType(gv, PLAYER_LORD_GODALMING,
			                                     1, MUNICH, false, false,
			                                     false, &numLocs);

			assert(numLocs == 1);
			free(locs);
		}

		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	{///////////////////////////////////////////////////////////////////
		printf("Testing trap locations after one drop of the trail\n");
		
		char *trail =
			"GED.... SGE.... HZU.... MCA.... DCF.V.. "
			"GMN.... SCFVD.. HGE.... MLS.... DBOT... "
			"GLO.... SMR.... HCF.... MMA.... DTOT... "
			"GPL.... SMS.... HMR.... MGR.... DBAT... "
			"GLO.... SBATD.. HMS.... MMA.... DSRT... "
			"GPL.... SSJ.... HBA.... MGR.... DALT... "
			"GPL.... SSJ.... HBA.... MGR.... DMAT... "
			"GLO.... SBE.... HMS.... MMATD.. DHIT.M. ";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == MADRID);
		assert(GvGetHealth(gv, PLAYER_DR_SEWARD) == GAME_START_HUNTER_LIFE_POINTS);
		assert(GvGetVampireLocation(gv) == NOWHERE);
		assert(GvGetScore(gv) == GAME_START_SCORE - 8 * SCORE_LOSS_DRACULA_TURN 
				- SCORE_LOSS_HUNTER_HOSPITAL);

		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 4);
		sortPlaces(traps, numTraps);
		assert(traps[0] == ALICANTE && traps[1] == MADRID && traps[2] == SARAGOSSA && traps[3] == TOULOUSE);
		free(traps);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}

	{///////////////////////////////////////////////////////////////////
		printf("Testing Hide refer to Double back\n");
		
		char *trail =
			"GED.... SGE.... HZU.... MCA.... DBE.V.. "
			"GMN.... SCF.... HGE.... MLS.... DSZT... "
			"GLO.... SMR.... HCF.... MMA.... DKLT... "
			"GPL.... SMS.... HMR.... MGR.... DGAT... "
			"GLO.... SBA.... HMS.... MMA.... DBCT... "
			"GPL.... SSJ.... HBA.... MGR.... DD5T... "
			"GPL.... SSJ.... HBA.... MGR.... DHIT... ";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == BELGRADE);
		assert(GvGetScore(gv) == GAME_START_SCORE - 7 * SCORE_LOSS_DRACULA_TURN);
		
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 6);
		sortPlaces(traps, numTraps);
		assert(traps[0] == BELGRADE && traps[1] == BELGRADE && traps[2] == BUCHAREST 
				&& traps[3] == GALATZ && traps[4] == KLAUSENBURG && traps[5] == SZEGED);
		free(traps);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}

	{///////////////////////////////////////////////////////////////////
		printf("Testing Hide refer to Double back\n");
		
		char *trail =
			"GED.... SGE.... HZU.... MCA.... DBE.V.. "
			"GMN.... SCF.... HGE.... MLS.... DSZT... "
			"GLO.... SMR.... HCF.... MMA.... DKLT... "
			"GPL.... SMS.... HMR.... MGR.... DGAT... "
			"GLO.... SBA.... HMS.... MMA.... DBCT... "
			"GPL.... SSJ.... HBA.... MGR.... DD5T... "
			"GPL.... SSJ.... HBA.... MGR.... DHIT... ";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == BELGRADE);
		assert(GvGetScore(gv) == GAME_START_SCORE - 7 * SCORE_LOSS_DRACULA_TURN);
		
		int numTraps = 0;
		PlaceId *traps = GvGetTrapLocations(gv, &numTraps);
		assert(numTraps == 6);
		sortPlaces(traps, numTraps);
		assert(traps[0] == BELGRADE && traps[1] == BELGRADE && traps[2] == BUCHAREST 
				&& traps[3] == GALATZ && traps[4] == KLAUSENBURG && traps[5] == SZEGED);
		free(traps);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}

	{///////////////////////////////////////////////////////////////////
		printf("Testing rest and encounter Dracula twice\n");
		
		char *trail =
			"GSJ.... SGE.... HZU.... MGR.... DBA.... "
            "GBAD... SCF.... HGE.... MGR.... DHI.... "
            "GBAD...";
		
		Message messages[24] = {};
		GameView gv = GvNew(trail, messages);
		
		assert(GvGetPlayerLocation(gv, PLAYER_DRACULA) == BARCELONA);
		assert(GvGetHealth(gv, PLAYER_LORD_GODALMING) == GAME_START_HUNTER_LIFE_POINTS - 2 * LIFE_LOSS_DRACULA_ENCOUNTER
				+ LIFE_GAIN_REST);
		
		GvFree(gv);
		printf("\033[1;32m");
		printf("Test passed!\n");
		printf("\033[0m");
	}
	
	return EXIT_SUCCESS;
}