#ifndef ObjectStats_H
#define ObjectStats_H 1

#include "../lib/primaryStats.h"

/**
* @brief Secondary object attributes
*
* Properties that not all objects posses are listed here.
*
* chars are being used due to the 8 bit storage, not to be exactly characters.
*
* @author Pedro Araujo Rosa
*/
typedef struct SecondaryStats
{
	unsigned char melee, defense, topSpeed, startMelee, meleeTicks, hittedTicks, ticks;
} SecondaryStats;

/**
* @brief All the object attributes
*
* This is the main struct that stores the attributes of an object.
*
* chars are being used due to the 8 bit storage, not to be exactly characters.
*
* @author Pedro Araujo Rosa
*/
typedef struct ObjectStats
{
	PrimaryStats* ps;
	SecondaryStats ss;
} ObjectStats;

extern ObjectStats *statsTable;

void loadObjectsBasics(const char* primaryFile, const char* secondFile);
void loadSecondaryStats(const char* fileName);
void loadPrimaryBasics(const char* primaryFile);
void primaryFinish();
void startStatsTable();
void copySecondaryStats(SecondaryStats *src, SecondaryStats *dst);
void copyObjectStats(ObjectStats *src, ObjectStats *dst);
ObjectStats craftObjectStats(int id);

#endif
