#include "gCharSel.h"
#include <iostream>

using namespace std;

char G_CharSel::captureEvent()
{
	char value;
	SDL_Event *event;

	event = new SDL_Event;
	
	value = 0;
	if(SDL_PollEvent(event))
	{
		switch(event->type)
		{
			case SDL_QUIT:
				value = -2;
				break;
			case SDL_KEYDOWN:
				if(event->key.keysym.sym == downKey){ value = 's'; }
				if(event->key.keysym.sym == rightKey){ value = 'd'; }
				if(event->key.keysym.sym == upKey){ value = 'w'; }
				if(event->key.keysym.sym == leftKey){ value = 'a'; }
				if(event->key.keysym.sym == startKey){ value = 'g'; }
				break;
			case SDL_KEYUP:
				break;
			default:
				break;
		}
	}

	return value;
}

G_CharSel::G_CharSel(SDL_Surface *screen)
{
	this->screen = screen;
	background = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 32, 0, 0, 0, 0);

	list = 0;
	selection = 0;
	selRect = new SDL_Rect;
	selRect->x = 1;
	selRect->y = 1;
	selRect->w = 46;
	selRect->h = 46;

	selector = SDL_CreateRGBSurface(SDL_SWSURFACE, 48, 48, 32, 0, 0, 0, 0);
	SDL_SetColorKey(selector, SDL_SRCCOLORKEY, SDL_MapRGB(selector->format, 255, 0, 255));
	SDL_FillRect(selector, 0, SDL_MapRGB(selector->format, 0, 0, 255));
	SDL_FillRect(selector, selRect, SDL_MapRGB(selector->format, 255, 0, 255));

	upKey = SDLK_w;
	rightKey = SDLK_d;
	downKey = SDLK_s;
	leftKey = SDLK_a;
	startKey = SDLK_g;
}

void G_CharSel::draw()
{
	EntryNode *aux;

	SDL_BlitSurface(background, 0, screen, 0);

	aux = list;
	while(aux != 0)
	{
		SDL_BlitSurface(aux->surface, 0, screen, aux->rect);
		aux = aux->next;
	}

	SDL_BlitSurface(selector, 0, screen, selRect);
		
	SDL_Flip(screen);
}

void G_CharSel::setSelection(int selection)
{
	EntryNode *aux;

	aux = list;
	while(aux != 0)
	{
		if(selection == aux->sel)
		{
			selRect = aux->rect;
			this->selection = selection;
		}
		aux = aux->next;
	}
}

int G_CharSel::getSelection()
{
	return selection;
}

void G_CharSel::addEntry(SDL_Surface* surface, int x, int y, int selection)
{
	EntryNode *aux;

	aux = new EntryNode;

	aux->surface = surface;
	aux->rect = new SDL_Rect;
	aux->rect->x = x;
	aux->rect->y = y;
	aux->sel = selection;
	aux->next = list;
	
	list = aux;
}

void G_CharSel::loadChar(const char* filename, int x, int y, int selection)
{
	SDL_Surface* surface;

	surface = loadImage(filename, 48, 48);

	addEntry(surface, x, y, selection);
}
