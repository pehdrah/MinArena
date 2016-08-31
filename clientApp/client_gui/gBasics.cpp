#include "gBasics.h"

SDL_Surface*** graphics;

int startEngine()
{	
	return SDL_Init(SDL_INIT_VIDEO);
}

void shutdown()
{
	SDL_Quit();
}

void loadGraphics()
{
	int i;
	SDL_Rect r;

	graphics = new SDL_Surface**[256];
	i = 0;
	while(i < 256)
	{
		graphics[i] = new SDL_Surface*[256];
		i++;
	}

	graphics[0][0] = SDL_CreateRGBSurface(SDL_SWSURFACE, 30, 30, 32, 0, 0, 0, 0);
	SDL_FillRect(graphics[0][0], 0, SDL_MapRGB(graphics[0][0]->format, 0, 0, 255));

	graphics[1][0] = loadCharacter("OgofSprites.bmp", 255, 128, 128);
	graphics[1][1] = loadCharacter("GauaSprites.bmp", 128, 128, 255);
	graphics[1][1] = loadCharacter("ErartSprites.bmp", 255, 255, 128);
	graphics[1][1] = loadCharacter("OntevSprites.bmp", 255, 255, 255);

	graphics[2][0] = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32, 0, 0, 0, 0);
	SDL_FillRect(graphics[2][0], 0, SDL_MapRGB(graphics[2][0]->format, 0, 0, 0));

	graphics[2][1] = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32, 0, 0, 0, 0);
	SDL_FillRect(graphics[2][1], 0, SDL_MapRGB(graphics[2][1]->format, 0, 0, 0));

	graphics[2][2] = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32, 0, 0, 0, 0);
	SDL_FillRect(graphics[2][2], 0, SDL_MapRGB(graphics[2][2]->format, 0, 0, 0));

	graphics[2][3] = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32, 0, 0, 0, 0);
	SDL_FillRect(graphics[2][3], 0, SDL_MapRGB(graphics[2][3]->format, 0, 0, 0));
}

void drawLine(float x0, float y0, float x1, float y1, SDL_Surface *surface)
{
	int ix0, iy0, ix1, iy1, dx, dy, i;
	float slope;
	SDL_Rect r;

	r.w = 1;
	r.h = 1;

	ix0 = (int)x0;
	iy0 = (int)y0;
	ix1 = (int)x1;
	iy1 = (int)y1;

	dx = ix1 - ix0;
	dy = iy1 - iy0;
	if(dx*dx > dy*dy)
	{
		if(ix1 < ix0)
		{
			dx = ix0;
			ix0 = ix1;
			ix1 = dx;
			dy = iy0;
			iy0 = iy1;
			iy1 = dy;
		}
		dx = ix1 - ix0;
		dy = iy1 - iy0;
		slope = (float)dy/(float)dx;

		i = 0;
		while(i < dx + 1)
		{
			r.x = ix0 + i;
			r.y = (int)(iy0 + i*slope);
			SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 255, 255, 255));
			i++;
		}
	}
	else if(dy*dy > dx*dx)
	{
		if(iy1 < iy0)
		{
			dx = ix0;
			ix0 = ix1;
			ix1 = dx;
			dy = iy0;
			iy0 = iy1;
			iy1 = dy;
		}
		dx = ix1 - ix0;
		dy = iy1 - iy0;
		slope = (float)dx/(float)dy;

		i = 0;
		while(i < dy + 1)
		{
			r.x = (int)(ix0 + i*slope);
			r.y = iy0 + i;
			SDL_FillRect(surface, &r, SDL_MapRGB(surface->format, 255, 255, 255));
			i++;
		}
	}
}

void drawGeometry(Surface *s, SDL_Surface *surface)
{
	int i;
	Vertex *v0, *v1;

	v0 = s->vertexes;
	
	i = 1;
	while(i < s->numVertexes)
	{
		v1 = s->vertexes + i;
		drawLine(v0->x, v0->y, v1->x, v1->y, surface);
		v0 = v1;
		i++;
	}

	v1 = s->vertexes;
	drawLine(v0->x, v0->y, v1->x, v1->y, surface);
}

SDL_Surface* callWindow(int w, int h)
{
	return SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE);
}

Rect makeStateBox(Rect *box, unsigned char state)
{
	Rect r;

	r.w = 8;
	r.h = 8;

	switch(getDirection(state))
	{
		case 0:
			r.x = box->x + box->w/2 - r.w/2;
			r.y = box->y + box->h - r.h;
			break;
		case 1:
			r.x = box->x + box->w - r.w;
			r.y = box->y + box->h - r.h;
			break;
		case 2:
			r.x = box->x + box->w - r.w;
			r.y = box->y + box->h/2 - r.h/2;
			break;
		case 3:
			r.x = box->x + box->w - r.w;
			r.y = box->y;
			break;
		case 4:
			r.x = box->x + box->w/2 - r.w/2;
			r.y = box->y;
			break;
		case 5:
			r.x = box->x;
			r.y = box->y;
			break;
		case 6:
			r.x = box->x;
			r.y = box->y + box->h/2 - r.h/2;
			break;
		case 7:
			r.x = box->x;
			r.y = box->y + box->h - r.h;
			break;
		default:
			break;
	}

	return r;
}

SDL_Surface* loadImage(const char* filename, int w, int h)
{
	SDL_Surface* image;

	image = SDL_LoadBMP(filename);
	if(image == 0)
	{
		image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
		SDL_FillRect(image, 0, SDL_MapRGB(image->format, 127, 127, 127));
	}

	return image;
}

SDL_Surface* loadCharacter(const char* filename, int r, int g, int b)
{
	int w, h;
	SDL_Surface* image;

	w = 30*8;
	h = 30*12;

	image = SDL_LoadBMP(filename);
	if(image == 0)
	{
		image = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, 0, 0, 0, 0);
		SDL_FillRect(image, 0, SDL_MapRGB(image->format, r, g, b));
	}
	else
	{
		SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 255, 0, 255));
	}

	return image;
}
