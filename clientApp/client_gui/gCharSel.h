#ifndef G_CharSel_H
#define G_CharSel_H 1

#include "gBasics.h"

typedef struct EntryNode
{
	int sel;
	SDL_Surface *surface;
	SDL_Rect *rect;
	EntryNode* next;
} EntryNode;

class G_CharSel
{
	public:
		EntryNode *list;
		SDL_Surface *screen, *selector, *background;
		SDL_Rect *selRect;
		int upKey, leftKey, downKey, rightKey, startKey, selection;
		char* selections;
		G_CharSel(SDL_Surface* screen);
		char captureEvent();
		void setSelection(int selection);
		void loadChar(const char* filename, int x, int y, int selection);
		void addEntry(SDL_Surface* surface, int x, int y, int selection);
		int getSelection();
		void draw();
};

#endif
