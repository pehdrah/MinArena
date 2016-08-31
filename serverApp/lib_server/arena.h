/**
* @brief A file containing the abstraction of an arena
*
* This file defines the arena and structures needed for it to work. These structures are not
* exactly data strucutres like stacks, but rather relationship bonds.
*
* @author Pedro Araujo Rosa
*/
#ifndef Arena_H
#define Arena_H 1

#include "structure.h"

/**
* @brief A rectangular slice of the arena
*
* This structure contains a piece of information of the arena in order to speed up routines
* It stores redundant information.
*
* @author Pedro Araujo Rosa
*/
typedef struct ArenaPiece
{
	Rect pieceBox;
	Structure* content;
} ArenaPiece;

/**
* @brief Describes the Arena class
*
* The Arena class is responsible for making the game work specially on the server part. It
* provides abstractions for handling everything that is happening in the game.
*
* @author Pedro Araujo Rosa
*/
class Arena
{
	public:
		Rect bigBox;
		Structure *objects;
		ArenaPiece *regions;
		Arena(int w, int h);
		void addObject(Object* obj);
		void removeObject(Object* obj);
		void destryObject(Object* obj);
		void trapObjects();
		void regionCheck();
		void checkCollisions();
		void removeTemporaries();
		void craftMeleeAttack(Object* obj);
		void turn();
		void printObjects();
};

#endif
