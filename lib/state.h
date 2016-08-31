#ifndef State_H
#define State_H 1

#include "primaryStats.h"

/**
* @brief A state of the game
*
* A game state is basically a set of the player`s states. This structure is used to manage game
* States. 
* 
* players is an array that checks which positions of the vector are active and stores the
* identifier of the player.
*
* Stats is an array of statuses, each status is related to a player.
*
* @author Pedro Araujo Rosa
*/

typedef struct State
{
	int* players;
	PrimaryStats* stats;
} State;

#endif
