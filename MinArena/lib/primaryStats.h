#ifndef PrimaryStats_H
#define PrimaryStats_H 1

#include "util.h"

/**
* @brief Primordial object attributes
*
* This is the main struct that stores the primary attributes of the objects. These are the only
* attributes that are actually important for the communications.
*
* chars are being used due to the 8 bit storage, not to be exactly characters.
*
* @author Pedro Araujo Rosa
*/
typedef struct PrimaryStats
{
	Rect box;
	char dx, dy;
	int maxLife, life;
	unsigned char state, type, kind;
} PrimaryStats;

extern PrimaryStats **primaryTable;

void copyPrimaryStats(PrimaryStats *src, PrimaryStats *dst);
void loadPrimaryStats(const char* file);
PrimaryStats craftPrimaryStats(int id);

#endif
