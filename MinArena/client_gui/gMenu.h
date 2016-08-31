#ifndef G_Menu_H
#define G_Menu_H 1

#include "gBasics.h"

typedef struct MenuEntryNode
{
	int sel;
	SDL_Surface *surface;
	SDL_Rect *rect;
	MenuEntryNode* next;
} MenuEntryNode;

class G_Menu
{
	public:
		MenuEntryNode *list;
		SDL_Surface *screen, *selector, *background;
		SDL_Rect *selRect;
		int upKey, leftKey, downKey, rightKey, startKey;
		int selection;
		G_Menu(SDL_Surface* screen);
		char captureEvent();
		void drawMenu();
		void setSelection(int selection);
		void addEntry(const char* text, int x, int y, int sel);
		int getSelection();
};

#endif
