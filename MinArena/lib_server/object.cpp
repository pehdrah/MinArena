#include "object.h"

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
Object::Object(ObjectStats *stats)
{
	this->stats = stats;
	id = 0;
	source = -1;
	lastInput = 0;
	surface = new Surface;
	makeSurface(stats->ps->type, stats->ps->kind, surface);
	surface->box = &(stats->ps->box);
	this->updateGeometry();
}

/**
* @brief Collision method
*
* This is the method of collision between two objects.
*
* @param o Another object to test the collision
*/
bool Object::collision(Object* o)
{
	bool value;

	value = false;

	//Check if the bounding box overlaps
	if(areaOverlap(&(this->stats->ps->box), &(o->stats->ps->box)))
	{
		value = surfaceCrossing(this->surface, o->surface);
	}

	return value;
}

/**
* @brief Add counting ticks to the action timers
*
* This function add to the counting ticks to make timed state changes, like attacking, possible.
*/
void Object::addTicks()
{
	if(!isNotBusy())
	{
		stats->ss.ticks = stats->ss.ticks + 1;
	}
}

void Object::zeroTicks()
{
	stats->ss.ticks = 0;
}

bool Object::isStriking()
{
	return isMeleeAttacking(getState()) && (stats->ss.ticks == stats->ss.startMelee);
}

bool Object::isNotBusy()
{
	return !wasHitted(getState()) && !isDistanceAttacking(getState()) && !isMeleeAttacking(getState());
}

/**
* @brief Object destructor
*
* Destroy the object and the memory it allocates
*/
Object::~Object()
{
	delete stats;
	delete surface;
}

/**
* @brief Stores and partially treats the input
*
* This function treats the input coming from the user. Note that the events are treated to
* generate the inputs. The parameter is not a raw character. The character type is only used to
* use 8 bit storage.
*
* @param input An unsigned char representing the input
*/
void Object::receiveInput(unsigned char input)
{
	char direction;
	unsigned char personDirection, state;

	state = getState();

	direction = directionFromInput(input);
	personDirection = getDirection(state);

	if(isNotBusy())
	{
		lastInput = input;
		if(meleeAttackFromInput(input))
		{
			if(direction == -3)
			{
				changeState(0x10 | personDirection);
			}
			else
			{
				changeState(0x10 | direction);
			}
			zeroTicks();
		}
		else if(!isBreaking(state))
		{
			if(direction != -3)
			{
				if(isMoving(state)
					&& direction != ((personDirection) & 0x07)
					&& direction != ((personDirection + 1) & 0x07)
					&& direction != ((personDirection - 1) & 0x07))
				{
					changeState(0x80 | direction);
				}
				else
				{
					changeState(0x08 | direction);
				}
			}
		}
	}
}

/**
* @brief Process the last input stored
*
* This function executes the last input stored by the function receiveInput
*
* @See receiveInput()
*/
void Object::executeLastInput()
{
	char direction;
	unsigned char personDirection, state, ticks;

	state = getState();
	ticks = stats->ss.ticks;
	
	direction = directionFromInput(lastInput);
	personDirection = getDirection(state);

	addTicks();

	if(wasHitted(state))
	{
		if(stats->ss.ticks >= stats->ss.hittedTicks)
		{
			changeState(state & 0x07);
			zeroTicks();
		}
		breakMovement();
	}
	else if(isMeleeAttacking(state))
	{
		if(stats->ss.ticks >= stats->ss.meleeTicks)
		{
			changeState(state & 0x07);
			stats->ss.ticks = 0;
		}
		breakMovement();
	}
	else if(isBreaking(state))
	{
		if(stats->ps->dx == 0 && stats->ps->dy == 0)
		{
			changeState(state & 0x07);
		}
		breakMovement();
	}
	else if(isMoving(state))
	{
		if(direction == -3)
		{
			changeState(0x80 | personDirection);
		}
		else
		{
			movement(personDirection);
		}
	}
	else
	{
		breakMovement();
	}
	lastInput = 0;
}

/**
* @brief Changes the character's speed
*
* This function moves the object according to its top speed and the direction given
*
* param direction The direction of the movement
*/
void Object::movement(char direction)
{
	unsigned char topSpeed;

	topSpeed = stats->ss.topSpeed;

	switch(direction)
	{
		case 0:
			setSpeed(0, 3*topSpeed / 4);
			break;
		case 1:
			setSpeed(topSpeed / 2, topSpeed / 2);
			break;
		case 2:
			setSpeed(3*topSpeed / 4, 0);
			break;
		case 3:
			setSpeed(topSpeed / 2, -topSpeed / 2);
			break;
		case 4:
			setSpeed(0, -3*topSpeed / 4);
			break;
		case 5:
			setSpeed(-topSpeed / 2, -topSpeed / 2);
			break;
		case 6:
			setSpeed(-3*topSpeed / 4, 0);
			break;
		case 7:
			setSpeed(-topSpeed / 2, topSpeed / 2);
			break;
		default:
			break;
	}
}

/**
* @brief reduces speed
*
* This function made to make the breaking process abstract
*/
void Object::breakMovement()
{
//	setSpeed(4*stats->ps->dx / 5, 4*stats->ps->dy / 5);
	setSpeed(9*stats->ps->dx / 10, 9*stats->ps->dy / 10);
//	setSpeed(stats->ps->dx - 1, stats->ps->dy - 1);
}

/**
* @brief Sets the object's speed
*
* Sets the vertical and horizontal speeds
*
* @param speedX the horizontal speed
* @param speedY the vertical speed
*/
void Object::setSpeed(char speedX, char speedY)
{
	stats->ps->dx = speedX;
	stats->ps->dy = speedY;
}

/**
* @brief Moves the vertexes of the object
*
* Updates the geometry so it can position the vertex correctly on the space in order to make
* collision and rendering correct
*/
void Object::updateGeometry()
{
	int i;
	float degrees;
	Vertex *v;

	degrees = 0;
	if(stats->ps->type > 1)
	{
		degrees = 45.0;
	}
	rotateSurface(getDirection(stats->ps->state)*degrees, surface);

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
* @brief Change the object's position
*
* Sets the anchor of the object at the point x, y
*
* @param x The leftmost corner of the surface
* @param y The uppermost corner of the surface
*/
void Object::setPosition(int x, int y)
{
	stats->ps->box.x = x;
	stats->ps->box.y = y;
}

/**
* 
*
* Returns the type of the object
*/
unsigned char Object::getType()
{
	return stats->ps->type;
}

/**
* 
*
* Returns the type of the object
*/
unsigned char Object::getKind()
{
	return stats->ps->kind;
}

/**
* Returns the state of the object
*/
unsigned char Object::getState()
{
	return stats->ps->state;
}

/**
* Returns the current life of the object
*/
int Object::getLife()
{
	return stats->ps->life;
}

/**
* Returns the maximum amount of life this object has
*/
int Object::getMaxLife()
{
	return stats->ps->maxLife;
}

/**
* Returns a copy of the rectangle that contains the object
*/
Rect Object::getBox()
{
	return stats->ps->box;
}

/**
* Returns the reference of the rectangle that contains the object
*/
Rect* Object::getBoxRef()
{
	return &(stats->ps->box);
}

/**
* Returns a referece to the primary stats of the object
*/
PrimaryStats* Object::getPrimaryStats()
{
	return stats->ps;
}

void Object::move()
{
	moveX();
	moveY();
}

void Object::moveBackwards()
{
	moveBackwardsX();
	moveBackwardsY();
}

void Object::moveX()
{
	stats->ps->box.x += stats->ps->dx;
}

void Object::moveY()
{
	stats->ps->box.y += stats->ps->dy;
}

void Object::moveBackwardsX()
{
	stats->ps->box.x -= stats->ps->dx;
}

void Object::moveBackwardsY()
{
	stats->ps->box.y -= stats->ps->dy;
}

void Object::changeState(unsigned char state)
{
	stats->ps->state = state;
}
