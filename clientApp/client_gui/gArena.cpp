#include "gArena.h"
#include "../lib/util.h"
#include <iostream>

using namespace std;

int G_Arena::captureEvent()
{
	int value;
	SDL_Event *event;

	event = new SDL_Event;
	
	value = keys;
	if(SDL_PollEvent(event))
	{
		switch(event->type)
		{
			case SDL_QUIT:
				value = -2;
				break;
			case SDL_KEYDOWN:
				if(event->key.keysym.sym == downKey){ keys = keys | 0x01; }
				if(event->key.keysym.sym == rightKey){ keys = keys | 0x02; }
				if(event->key.keysym.sym == upKey){ keys = keys | 0x04; }
				if(event->key.keysym.sym == leftKey){ keys = keys | 0x08; }
				if(event->key.keysym.sym == meleeKey){ keys = keys | 0x10; }
				if(event->key.keysym.sym == distanceKey){ keys = keys | 0x20; }
				if(event->key.keysym.sym == SDLK_ESCAPE){ keys = -2; }
				value = keys;
				break;
			case SDL_KEYUP:
				if(event->key.keysym.sym == downKey){ keys = keys & 0xFE; }
				if(event->key.keysym.sym == rightKey){ keys = keys & 0xFD; }
				if(event->key.keysym.sym == upKey){ keys = keys & 0xFB; }
				if(event->key.keysym.sym == leftKey){ keys = keys & 0xF7; }
				if(event->key.keysym.sym == meleeKey){ keys = keys & 0xEF; }
				if(event->key.keysym.sym == distanceKey){ keys = keys & 0xDF; }
				value = keys;
				break;
			default:
				break;
		}
	}

	return value;
}

G_Arena::G_Arena(SDL_Surface *screen, int w, int h)
{
	bigBox.x = 0;
	bigBox.y = 0;
	bigBox.w = w;
	bigBox.h = h;
	objects = new G_Structure;
	this->screen = screen;

	background = loadImage("background.bmp", bigBox.w, bigBox.h);
	mainSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, bigBox.w, bigBox.h, 32, 0, 0, 0, 0);

	keys = 0;

	upKey = SDLK_w;
	rightKey = SDLK_d;
	downKey = SDLK_s;
	leftKey = SDLK_a;
	meleeKey = SDLK_g;
	distanceKey = SDLK_h;
}

void G_Arena::makeArenaPiece(int x, int y, int w, int h)
{
	int i;
	bool* overlaps;
	ObjectNode* aux;
	SDL_Rect sr, dstRect, lifebar, lostLife;
	int red, green;
	Rect r, r2;
	Rect *box;
	G_Object *obj;

	r = centerViewport(x, y, w, h);
	sr = toSDL_Rect(r);

	lifebar.w = 100;
	lifebar.h = 5;

	lostLife.h = 5;
	
	SDL_BlitSurface(background, 0, mainSurface, 0);

	red = SDL_MapRGB(mainSurface->format, 255, 0, 0);
	green = SDL_MapRGB(mainSurface->format, 0, 255, 0);

	drawObjectList(objects->getSuspendedObjectsList(), &r, mainSurface);
	drawObjectList(objects->getPlayersObjectsList(), &r, mainSurface);
	drawObjectList(objects->getTemporaryObjectsList(), &r, mainSurface);

	aux = objects->getPlayersObjectsList();
	while(aux != 0)
	{
		obj = aux->obj;
		box = obj->getBoxRef();
		lifebar.x = box->x + (box->w - lifebar.w)/2;
		lifebar.y = box->y - 15;
		if(lifebar.y < 0)
		{
			lifebar.y = 0;
		}
		if(lifebar.x < 0)
		{
			lifebar.x = 0;
		}
		if(lifebar.x + lifebar.w > r.x + r.w)
		{
			lifebar.x = bigBox.w - lifebar.w;
		}
		SDL_FillRect(mainSurface, &lifebar, green);

		lostLife.x = lifebar.x;
		lostLife.y = lifebar.y;
		lostLife.w = lifebar.w - lifebar.w * obj->getLife() / obj->getMaxLife();

		SDL_FillRect(mainSurface, &lostLife, red);
		aux = aux->next;
	}
}

void G_Arena::drawObjectList(ObjectNode *list, Rect *r, SDL_Surface *surface)
{
	G_Object *obj;

	while(list != 0)
	{
		obj = list->obj;
		drawObject(obj, r, surface);
		list = list->next;
	}
}

void G_Arena::drawObject(G_Object *obj, Rect *r, SDL_Surface *surface)
{
	SDL_Rect dstRect;
	SDL_Rect *srcRect;
	Rect *box;
	Rect stateBox;
	int green;

	box = obj->getBoxRef();
	green = SDL_MapRGB(surface->format, 0, 255, 0);
	srcRect = 0;

	if(areaOverlap(r, box))
	{
		dstRect = toSDL_Rect(*box);
		if(obj->getType() == 1)
		{
			srcRect = new SDL_Rect;
			*srcRect = toSDL_Rect(obj->getAnimationBox());
		}
		SDL_BlitSurface(graphics[obj->getType()][obj->getKind()], srcRect, surface, &dstRect);
		obj->updateGeometry();
		drawGeometry(obj->surface, surface);
		if(obj->getType() == 1)
		{
			dstRect = toSDL_Rect(makeStateBox(box, obj->getState()));
			SDL_FillRect(surface, &dstRect, green);
		}
	}
}

void G_Arena::drawArenaPiece(int x, int y, int w, int h)
{
	Rect r;
	SDL_Rect vp;

	makeArenaPiece(x, y, w, h);

	r = centerViewport(x, y, w, h);
	vp.x = r.x;
	vp.y = r.y;
	vp.w = r.w;
	vp.h = r.h;

	SDL_BlitSurface(mainSurface, &vp, screen, 0);
	SDL_Flip(screen);
}

void G_Arena::drawArenaPiece(G_Object *o)
{
	Rect* box;
	Rect r;
	SDL_Rect sr, vp;
	unsigned char state;
	int green;

	box = o->getBoxRef();
	state = o->getState();
	green = SDL_MapRGB(screen->format, 0, 255, 0);

	makeArenaPiece(box->x, box->y, box->w, box->h);

	r = centerViewport(box->x, box->y, box->w, box->h);
	vp.x = r.x;
	vp.y = r.y;
	vp.w = r.w;
	vp.h = r.h;

	SDL_BlitSurface(mainSurface, &vp, screen, 0);
	SDL_Flip(screen);
}

Rect G_Arena::centerViewport(int x, int y, int w, int h)
{
	Rect r;
	int sw, sh, aw, ah;

	sw = screen->w;
	sh = screen->h;
	aw = bigBox.w;
	ah = bigBox.h;

	x = x + w/2 - sw/2;
	y = y + h/2 - sh/2;

	if(x < 0)
	{
		x = 0;
	}
	else if(x + sw > aw)
	{	
		x = aw - sw;
	}

	if(y < 0)
	{
		y = 0;
	}
	else if(y + sh > ah)
	{
		y = ah - sh;
	}

	r.x = x;
	r.y = y;
	r.w = sw;
	r.h = sh;

	return r;
}


/**
* @brief Remove objects whose time to live expired
*
* This function must remove objects and it must make sure to free their memories to avoid leaks
*/
void G_Arena::removeTemporaries()
{
	ObjectNode *aux;

	aux = objects->getTemporaryObjectsList();
	while(aux != 0)
	{
		aux->obj->stats->life = aux->obj->getLife() - 1;
		if(aux->obj->getLife() < 1)
		{
			removeObject(aux->obj);
		}
		aux = aux->next;
	}
}

/**
* @brief Removes an object given the id and the type
*
* Removes the object from the arena.
*/
void G_Arena::removeObject(G_Object* obj)
{
	objects->destroyObject(obj);
}

/**
* @brief This function adds an Object to the arena.
*
* This function must add the object and prepare the structures that will help finding the
* object and to make segmentation to reduce the running time of routines
*/
void G_Arena::addObject(G_Object* obj)
{
	objects->addObject(obj);
}
