#include "gObject.h"

/**
* @brief Object construction method
*
* Constructs an Object by giving it stats.
*
* It tracks down the geometry by using the stats and initializes the necessary attributes to
* work properly.
*
* @param stats A reference to a set of object properties
*/
G_Object::G_Object(PrimaryStats *stats)
{
	this->stats = stats;
	id = 0;
	animation = 0;
	startMelee = 4;
	surface = new Surface;
	makeSurface(stats->type, stats->kind, surface);
	surface->box = &(stats->box);
	this->updateGeometry();
}

/**
* @brief Add counting ticks to the action timers
*
* This function add to the counting ticks to make timed state changes, like attacking, possible.
*/
void G_Object::addTicks()
{
	if(!isNotBusy())
	{
		ticks = ticks + 1;
	}
}

void G_Object::zeroTicks()
{
	ticks = 0;
}

bool G_Object::isStriking()
{
	return isMeleeAttacking(getState()) && (ticks == startMelee);
}

bool G_Object::isNotBusy()
{
	return !wasHitted(getState()) && !isDistanceAttacking(getState()) && !isMeleeAttacking(getState());
}

/**
* @brief Object destructor
*
* Destroy the object and the memory it allocates
*/
G_Object::~G_Object()
{
	delete stats;
	delete surface;
}

/**
* @brief Moves the vertexes of the object
*
* Updates the geometry so it can position the vertex correctly on the space in order to make
* collision and rendering correct
*/
void G_Object::updateGeometry()
{
	int i;
	float degrees;
	Vertex *v;

	degrees = 0;
	if(stats->type > 1)
	{
		degrees = 45.0;
	}
	rotateSurface(getDirection(stats->state)*degrees, surface);

	surface->box = getBoxRef();

	surface->center.x = surface->center.x + surface->box->x + (surface->box->w/2.0);
	surface->center.y = surface->center.y + surface->box->y + (surface->box->h/2.0);

	i = 0;
	while(i < surface->numVertexes)
	{
		v = surface->vertexes + i;
		v->x = v->x + surface->box->x + (surface->box->w/2.0);
		v->y = v->y + surface->box->y + (surface->box->h/2.0);
		i++;
	}
}

/**
* 
*
* Returns the type of the object
*/
unsigned char G_Object::getType()
{
	return stats->type;
}

/**
* 
*
* Returns the type of the object
*/
unsigned char G_Object::getKind()
{
	return stats->kind;
}

/**
* Returns the state of the object
*/
unsigned char G_Object::getState()
{
	return stats->state;
}

/**
* Returns the current life of the object
*/
int G_Object::getLife()
{
	return stats->life;
}

/**
* Returns the maximum amount of life this object has
*/
int G_Object::getMaxLife()
{
	return stats->maxLife;
}

/**
* Returns a copy of the rectangle that contains the object
*/
Rect G_Object::getBox()
{
	return stats->box;
}

/**
* Returns the reference of the rectangle that contains the object
*/
Rect* G_Object::getBoxRef()
{
	return &(stats->box);
}

/**
* Returns a referece to the primary stats of the object
*/
PrimaryStats* G_Object::getPrimaryStats()
{
	return stats;
}

void G_Object::changeState(unsigned char state)
{
	stats->state = state;
	animation = 0;
}
