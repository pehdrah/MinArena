#include "gMenu.h"
#include <iostream>

using namespace std;

char G_Menu::captureEvent()
{
	char value;
	SDL_Event *event;

	event = new SDL_Event;
	
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
				value = 0;
				break;
			default:
				break;
		}
	}

	return value;
}

G_Menu::G_Menu(SDL_Surface *screen)
{
	this->screen = screen;
	background = SDL_CreateRGBSurface(SDL_SWSURFACE, screen->w, screen->h, 32, 0, 0, 0, 0);
	list = 0;
	selection = 0;

	upKey = SDLK_w;
	rightKey = SDLK_d;
	downKey = SDLK_s;
	leftKey = SDLK_a;
	startKey = SDLK_g;
}

void G_Menu::drawMenu()
{
	MenuEntryNode *aux;

	aux = list;
	while(aux != 0)
	{
		SDL_BlitSurface(aux->surface, 0, screen, aux->rect);
		aux = aux->next;
	}
		
	SDL_Flip(screen);
}

void G_Menu::setSelection(int selection)
{
	MenuEntryNode *aux;

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

int G_Menu::getSelection()
{
	return selection;
}
