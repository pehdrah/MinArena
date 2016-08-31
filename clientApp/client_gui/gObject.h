#ifndef G_Object_H
#define G_Object_H 1

#include "../lib/util.h"
#include "../lib/primaryStats.h"
#include "../lib/geometry.h"

/**
* @brief An object to use inside the arena
*
* Do not confuse this with the class on Java programming language. This is just
* how we should see objects in the arena.
*
* This class provides abstraction for executing specific routines and access to its attributes
* It is basically composed of a Surface and ObjectStats.
*
* @author Pedro Araujo Rosa
*/
class G_Object
{
	public:
		unsigned char animation, ticks, startMelee;
		int id;
		PrimaryStats *stats;
		Surface *surface;
		G_Object(PrimaryStats *stats);
		~G_Object();
		void zeroTicks();
		void addTicks();
		void updateGeometry();
		void move();
		unsigned char getType();
		unsigned char getKind();
		unsigned char getState();
		bool isStriking();
		bool isNotBusy();
		int getLife();
		int getMaxLife();
		Rect getBox();
		Rect* getBoxRef();
		PrimaryStats* getPrimaryStats();
		void changeState(unsigned char state);
};

#endif
