#ifndef Util_H
#define Util_H 1

/**
* @brief A 2D rectangle
*
* A 2D rectangle anchored at the northwestern corner. 
*
* @author Pedro Araujo Rosa
*/
typedef struct Rect
{
	int x, y, w, h;
} Rect;

bool areaOverlap(Rect* r, Rect* s);
bool areaInside(Rect* r, Rect* s);

unsigned char getDirection(unsigned char state);
bool isMoving(unsigned char state);
bool isMeleeAttacking(unsigned char state);
bool isDistanceAttacking(unsigned char state);
bool wasHitted(unsigned char state);
bool isBreaking(unsigned char state);

char directionFromInput(char input);
bool meleeAttackFromInput(char input);
bool distanceAttackFromInput(char input);

int meleeAttackId(unsigned char kind);
#endif
