#ifndef Object_H
#define Object_H 1

#include "../lib/util.h"
#include "objectStats.h"
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
class Object
{
	public:
		char source;
		unsigned char lastInput;
		int id;
		ObjectStats *stats;
		Surface *surface;
		Object(ObjectStats *stats);
		~Object();
		bool collision(Object* o);
		void receiveInput(unsigned char input);
		void executeLastInput();
		void movement(char direction);
		void changeState(unsigned char state);
		void breakMovement();
		void zeroTicks();
		void addTicks();
		void setSpeed(char speedX, char speedY);
		void setPosition(int x, int y);
		void updateGeometry();
		void move();
		void moveBackwards();
		void moveX();
		void moveY();
		void moveBackwardsX();
		void moveBackwardsY();
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
};

#endif
