#ifndef G_Basics_H
#define G_Basics_H 1

#include "../lib/util.h"
#include "../lib/geometry.h"
#include <SDL/SDL.h>

extern SDL_Surface*** graphics;

int startEngine();
void loadGraphics();
void shutdown();
void drawLine(float x0, float y0, float x1, float y1, SDL_Surface *surface);
void drawGeometry(Surface *s, SDL_Surface *surface);
SDL_Surface* callWindow(int w, int h);
SDL_Surface* loadImage(const char* filename, int x, int y);
SDL_Surface* loadCharacter(const char* filename, int r, int g, int b);
Rect makeStateBox(Rect *box, unsigned char state);

#endif
