#ifndef Geometry_H
#define Geometry_H 1

#include "util.h"

extern int** nVertexesTable;

/**
* @brief A 2D vertex
*
* A 2D vertex used to create geometries.
*
* @author Pedro Araujo Rosa
*/
typedef struct Vertex
{
	float x, y;
} Vertex;

extern Vertex*** originalsTable;

/**
* @brief A 2D edge.
*
* A 2D edge composed by a point and a direction vector described by x and y.
*
* @author Pedro Araujo Rosa
*/
typedef struct Edge
{
	Vertex* p0;
	float x, y;
} Edge;

/**
* @brief A 2D surface.
*
* A 2D surface composed by a containing box in which every vertex must be inside. This checking
* must be made and ensured by the programmer. The surface is composed by two arrays of vertexes,
* in which one is centered at the 0, 0 and the other contains the transladed vertexes.
*
* Even tough it would not be needed, the surface try also to remember something as a center of
* mass.
*
* @author Pedro Araujo Rosa
*/
typedef struct Surface
{
	Rect* box;
	int numVertexes;
	Vertex* originals;
	Vertex* vertexes;
	Vertex center;
	Vertex oCenter;
} Surface;

typedef struct SurfaceList
{
	Rect box;
	int numSurfaces;
	Surface* surfaces;
} SurfaceList;

void loadGeometries();
void readGeometries();
Vertex rotation(float degrees, Vertex toRotate);
void rotateSurface(float degrees, Surface* surface);
bool edgeCrossing(Edge* r, Edge* s);
bool edgeCrossSurface(Edge* r, Surface* s);
bool surfaceCrossing(Surface* r, Surface* s);
void makeRectangle(float w, float h, Surface* s);
void makeSurface(unsigned char type, unsigned char kind, Surface* s);

#endif
