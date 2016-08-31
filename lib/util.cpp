/**
* @brief Utility functions
*
* This file implements the utility functions described in the header util.h
*
* @author Pedro Araujo Rosa
*/

#include "util.h"

int tileSize = 30;

/**
* @brief Check if two rectangles are overlapping
*
* This is useful to quickly check if objects are far from each other by the rectangles that
* contains them
*/
bool areaOverlap(Rect* r, Rect* s)
{
	return r->x + r->w > s->x && r->x < s->x + s->w &&
		r->y + r->h > s->y && r->y < s->y + s->h;
}

/**
* @brief Check if a rectangle is inside of another one
*
* @param r The rectangle that is supposed to be inside
* @param s the rectangle that is supposed to be bigger
*/
bool areaInside(Rect* r, Rect* s)
{
	bool value;

	value = true;
	if((r->x < s->x || r->y < s->y || 
		r->x + r->w > s->x + s->w || r->y + r->h > s->y + s->h))
	{
		value = false;
	}

	return value;
}

/**
* @brief Finds the direction a given state has
*
* This function translates a unsigned char, which represents the state of an object, to find
* the direction that object is facing. It zeroes the other bits in order to filter producing an
* integer that ranges from 0 to 7, whereas 0 means moving upwards and following numbers are
* clockwise rotations
*
* @param state An object state
* @return A number ranging from 0 to 7
*/
unsigned char getDirection(unsigned char state)
{
	return state & 0x07;
}

/**
* @brief Given a state, find out if the charachter is moving.
*
* This function gets a state and check if the moving bit is turned on.
*
* @param state An object state
* @return True if the state says it is moving
*/
bool isMoving(unsigned char state)
{
	return ((state & 0x08) == 8);
}

/**
* @brief Given a state, find out if the charachter is attacking.
*
* This function gets a state and check if the attacking bit is turned on.
*
* @param state An object state
* @return True if the state says it is attacking
*/
bool isMeleeAttacking(unsigned char state)
{
	return ((state & 0x10) == 16);
}

/**
* @brief Given a state, find out if the charachter is throwing something.
*
* This function gets a state and check if the distance attacking bit is turned on.
*
* @param state An object state
* @return True if the state says it is throwing something
*/
bool isDistanceAttacking(unsigned char state)
{
	return ((state & 0x20) == 32);
}

/**
* @brief Given a state, find out if the charachter was hit.
*
* This function gets a state and check if the hitted bit is turned on.
*
* @param state An object state
* @return True if the state says it was hitted
*/
bool wasHitted(unsigned char state)
{
	return ((state & 0x40) == 64);
}

/**
* @brief Given a state, find out if the charachter is breaking.
*
* This function gets a state and check if the breaking bit is turned on.
*
* @param state An object state
* @return True if the state says it is breaking
*/
bool isBreaking(unsigned char state)
{
	return ((state & 0x80) == 128);
}

/**
* @brief This function finds out the next direction the character will face given the input.
*
* This function serves to treat inputs to find out the direction in which the character should
* be facing given that input
*
* @param input The char that describes the input
* @returns a number ranging from 0 to 7
*/
char directionFromInput(char input)
{
	char value;
	input = input & 0x0F;

	value = -3;
	if(input == 0x01 || input == 0x0B)
	{
		value = 0;
	}
	else if(input == 0x03)
	{
		value = 1;
	}
	else if(input == 0x02 || input == 0x7)
	{
		value = 2;
	}
	else if(input == 0x06)
	{
		value = 3;
	}
	else if(input == 0x04 || input == 0x0E)
	{
		value = 4;
	}
	else if(input == 0x0C)
	{
		value = 5;
	}
	else if(input == 0x08 || input == 0x0D)
	{
		value = 6;
	}
	else if(input == 0x09)
	{
		value = 7;
	}

	return value;
}

/**
* @brief This function finds out if the input signals an attack.
*
* This function serves to treat inputs to find if the signal to attack should be turned on
*
* @param input The char that describes the input
* @returns True if the input signalizes to attack
*/
bool meleeAttackFromInput(char input)
{
	return ((input & 0x10) == 16);
}

/**
* Given a characther kind, find its melee attack stats position at the table
*/
int meleeAttackId(unsigned char kind)
{
	return 30 + kind;
}
