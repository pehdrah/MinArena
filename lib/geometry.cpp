#include "geometry.h"
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

Vertex*** originalsTable;
int** nVertexesTable;

/**
* This function load the set of geometries used by the game. Each object uses a specific
* geometry. This function should be called before the game starts.
*/
void loadGeometries()
{
	int i, j;
	float w, h;
	Vertex* originals;

	originalsTable = new Vertex**[8];
	nVertexesTable = new int*[8];

	i = 0;
	while(i < 8)
	{
		originalsTable[i] = new Vertex*[8];
		nVertexesTable[i] = new int[8];
		i++;
	}

	readGeometries();
}

void readGeometries()
{
	ifstream file;
	string line, token, tok;
	int pos, type, kind, size, i;
	float x, y;

	file.open("Geometries.dat");

	getline(file, line);
	while(line != "")
	{
		pos = line.find(",");
		token = line.substr(0, pos);
		stringstream(token) >> type;
		line = line.substr(pos+1);
		pos = line.find(";");
		token = line.substr(0, pos);
		stringstream(token) >> kind;
		getline(file, line);
		stringstream(line) >> size;
		nVertexesTable[type][kind] = size;
		originalsTable[type][kind] = new Vertex[size];
		getline(file, line);
		i = 0;
		while(line != "e")
		{
			pos = line.find(";");
			token = line.substr(1,pos-1);
			line = line.substr(pos+1);
			pos = token.find(",");
			tok = token.substr(0, pos);
			stringstream(tok) >> x;
			tok = token.substr(pos+1);
			stringstream(tok) >> y;
			originalsTable[type][kind][i].x = x;
			originalsTable[type][kind][i].y = y;
			i++;
		}	
		getline(file, line);

		getline(file, line);
	}

	file.close();
}



/**
* This function rotates a vertex using the 0, 0 as the center.
*/
Vertex rotation(float degrees, Vertex toRotate)
{
	Vertex rotated;
	float pi, ratio;

	pi = acos(-1);
	ratio = pi/180.0;
	rotated.x = toRotate.x*cos(ratio*degrees) + toRotate.y*sin(ratio*degrees);
	rotated.y = toRotate.x*-1*sin(ratio*degrees) + toRotate.y*cos(ratio*degrees);

	return rotated;
}

/**
* This function rotates all the vertex of a surface without changing the original ones.
*/
void rotateSurface(float degrees, Surface* surface)
{
	int i;
	Vertex v;

	i = 0;
	while(i < surface->numVertexes)
	{
		v = rotation(degrees, surface->originals[i]);
		surface->vertexes[i].x = v.x;
		surface->vertexes[i].y = v.y;
		i++;
	}

	surface->center = rotation(degrees, surface->oCenter);
}

/**
* This function check if two edges cross each other
*/
bool edgeCrossing(Edge* r, Edge* s)
{
	bool value;
	float t0, t1, a, b, c, d;

	value = false;

	a = r->y*s->x - r->x*s->y;
	b = r->x*(s->p0->y - r->p0->y) - r->y*(s->p0->x - r->p0->x);
	c = s->y*(r->p0->x - s->p0->x) - s->x*(r->p0->y - s->p0->y);

	if(a == 0 && (b == 0 || c == 0))
	{
		if(r->p0->x < s->p0->x + s->x && r->p0->x + r->x > s->p0->x)
		{
			value = true;
		}
	}
	else if(a != 0)
	{
		t1 = b/a;
		t0 = c/a;
		if(t0 >= 0 && t0 <= 1 && t1 >= 0 && t1 <= 1)
		{
			value = true;
		}
	}

	return value;
}

/**
* This function checks if a given Edge r is crossing any of the given Surface`s edges
*/
bool edgeCrossSurface(Edge* r, Surface* s)
{
	bool value;
	int j;
	Edge e;

	value = false;

	//iterates over edges of the target surface
	e.p0 = s->vertexes;
	j = 1;
	while(!value && j < s->numVertexes)
	{
		e.x = s->vertexes[j].x - e.p0->x;
		e.y = s->vertexes[j].y - e.p0->y;

		value = edgeCrossing(r, &e);

		e.p0 = s->vertexes + j;
				
		j++;
	}
	if(!value)
	{
		e.x = s->vertexes[0].x - e.p0->x;
		e.y = s->vertexes[0].y - e.p0->y;
		value = edgeCrossing(r, &e);
	}

	return value;
}

/**
* This function check if any of the edges of the surface r cross any of the edges of the
* surface s
*/
bool surfaceCrossing(Surface* r, Surface* s)
{
	int i;
	Edge e;
	bool value;

	value = false;
	e.p0 = r->vertexes;
	i = 1;
	while(!value && i < r->numVertexes)
	{
		e.x = r->vertexes[i].x - e.p0->x;
		e.y = r->vertexes[i].y - e.p0->y;

		value = edgeCrossSurface(&e, s);

		e.p0 = r->vertexes + i;
			
		i++;
	}
	if(!value)
	{
		e.x = r->vertexes[0].x - e.p0->x;
		e.y = r->vertexes[0].y - e.p0->y;

		value = edgeCrossSurface(&e, s);
	}

	return value;
}

/**
* Prepare the Surface S to work according to the surface table
*/
void makeSurface(unsigned char type, unsigned char kind, Surface* s)
{
	int i;

	s->numVertexes = nVertexesTable[type][kind];
	s->originals = originalsTable[type][kind];
	s->vertexes = new Vertex[s->numVertexes];

	i = 0;
	while(i < s->numVertexes)
	{
		s->oCenter.x = s->oCenter.x + s->originals[i].x;
		s->oCenter.y = s->oCenter.y + s->originals[i].y;
		i++;
	}

	if(i > 0)
	{
		s->oCenter.x = s->oCenter.x/i;
		s->oCenter.y = s->oCenter.y/i;
	}
}
