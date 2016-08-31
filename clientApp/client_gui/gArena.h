#ifndef G_Arena_H
#define G_Arena_H 1

#include "gBasics.h"
#include "gObject.h"
#include "gStructure.h"

class G_Arena
{
	private:
		Rect bigBox;
		G_Structure *objects;
		void makeArenaPiece(int x, int y, int w, int h);
		Rect centerViewport(int x, int y, int w, int h);
	public:
		unsigned int state;
		SDL_Surface *screen, *background, *mainSurface;
		int upKey, leftKey, downKey, rightKey, meleeKey, distanceKey, keys;
		G_Arena(SDL_Surface *screen, int w, int h);
		void drawArenaPiece(int x, int y, int w, int h);
		void drawObjectList(ObjectNode *list, Rect *r, SDL_Surface *surface);
		void drawObject(G_Object *o, Rect *r, SDL_Surface *surface);
		void drawArenaPiece(G_Object *o);
		int captureEvent();
		void addObject(G_Object* obj);
		void removeObject(G_Object* obj);
		void destryObject(G_Object* obj);
		void craftMeleeAttack(G_Object* obj);
		void removeTemporaries();
};

#endif
