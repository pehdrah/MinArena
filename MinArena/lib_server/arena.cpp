/**
* @brief Implementation of the Arena
*
* This file implements the Arena, responsible for keeping the object lists and making its
* relationships as the game progress
*
* @author Pedro Araujo Rosa
*/

#include "arena.h"
#include "collisions.h"
#include <iostream>

using namespace std;

/**
* @brief Initializes the arena with given dimensions
*
* The constructor has the job to prepare the objects to be used, so it has to initialize all
* the subdivisions of the arena accordingly
*/
Arena::Arena(int w, int h)
{
	int pw, ph, mw, mh, i;

	bigBox.x = 0;
	bigBox.y = 0;
	bigBox.w = w;
	bigBox.h = h;
	objects = new Structure;
	regions = new ArenaPiece[9];

	pw = w/3;
	ph = h/3;

	mw = w - 2*pw;
	mh = h - 2*ph;

	i = 0;
	while(i < 9)
	{
		regions[i].pieceBox.x = 0;
		regions[i].pieceBox.w = pw;
		i = i + 3;
	}
	i = 1;
	while(i < 9)
	{
		regions[i].pieceBox.x = pw;
		regions[i].pieceBox.w = mw;
		i = i + 3;
	}
	i = 2;
	while(i < 9)
	{
		regions[i].pieceBox.x = pw + mw;
		regions[i].pieceBox.w = pw;
		i = i + 3;
	}

	i = 0;
	while(i < 3)
	{
		regions[i].pieceBox.y = 0;
		regions[i].pieceBox.h = ph;
		i++;
	}
	while(i < 6)
	{
		regions[i].pieceBox.y = ph;
		regions[i].pieceBox.h = mh;
		i++;
	}
	while(i < 9)
	{
		regions[i].pieceBox.y = ph + mh;
		regions[i].pieceBox.h = ph;
		i++;
	}

	i = 0;
	while(i < 9)
	{
		regions[i].content = new Structure;
		i++;
	}
}

/**
* @brief This function adds an Object to the arena.
*
* This function must add the object and prepare the structures that will help finding the
* object and to make segmentation to reduce the running time of routines
*/
void Arena::addObject(Object* obj)
{
	int i;

	objects->addObject(obj);

	i = 0;
	while(i < 9)
	{
		if(areaOverlap(obj->getBoxRef(), &(regions[i].pieceBox)))
		{
			regions[i].content->addObject(obj);
		}
		i++;
	}
}

/**
* @brief This function traps the object inside the arena.
*
* This function must ensure that all the objects inside the arena are going to either, stay, or
* disappear
*/
void Arena::trapObjects()
{
	int i;
	Rect *objBox;
	ObjectNode *aux;

	i = 1;
	aux = objects->getAnimatedObjectsList();
	while(aux != 0)
	{
		objBox = aux->obj->getBoxRef();
		if(!areaInside(objBox, &bigBox))
		{
			if(objBox->x < bigBox.x)
			{
				objBox->x = bigBox.x;
			}
			if(objBox->y < bigBox.y)
			{
				objBox->y = bigBox.y;
			}
			if(objBox->x + objBox->w > bigBox.w)
			{
				objBox->x = bigBox.w - objBox->w;
			}
			if(objBox->y + objBox->h > bigBox.h)
			{
				objBox->y = bigBox.h - objBox->h;
			}
		}
		aux = aux->next;
	}	
}

/**
* @brief This function keeps track of which object is in which region.
*
* This function organizes the region structure to optimize collision and other routines.
*/
void Arena::regionCheck()
{
	int i, j, k;
	ObjectNode *aon, *aux, *aux2;

	/* Remove objects from the regions they should be removed */
	i = 0;
	while(i < 9)
	{
		aux = regions[i].content->getAnimatedObjectsList();
		while(aux != 0)
		{
			if(!areaOverlap(aux->obj->getBoxRef(), &(regions[i].pieceBox)))
			{
				regions[i].content->removeObject(aux->obj);
			}
			aux = aux->next;
		}
		i++;
	}

	/* Add objects to where they should be added */
	aon = objects->getAnimatedObjectsList();
	while(aon != 0)
	{
		i = 0;
		while(i < 9)
		{
			if(areaOverlap(aon->obj->getBoxRef(), &(regions[i].pieceBox)))
			{
				regions[i].content->addObject(aon->obj);
			}
			i++;
		}
		aon = aon->next;
	}
}

/**
* @brief Executes a game turn.
*
* This function must execute a game turn, by performing the actions the objects are supposed to
* take, creating the objects that need to be created and removing the ones that should be
* removed. Also performing collision checks and such.
*/
void Arena::turn()
{
	int i;
	ObjectNode *aux;
	Object* obj;

	removeTemporaries();

	aux = objects->getAnimatedObjectsList();
	while(aux != 0)
	{
		obj = aux->obj;
		obj->executeLastInput();
		obj->move();
		obj->updateGeometry();
		if(obj->isStriking() && obj->getType() == 1)
		{
			craftMeleeAttack(obj);
		}
		aux = aux->next;
	}

	trapObjects();
	regionCheck();
	checkCollisions();
}


void Arena::craftMeleeAttack(Object *obj)
{
	ObjectStats *stats;
	Object *crafted;
	Rect box;

	box = obj->getBox();
	stats = new ObjectStats;
	*stats = craftObjectStats(meleeAttackId(obj->stats->ps->kind));
	stats->ps->state = obj->getState();
	crafted = new Object(stats);
	crafted->setPosition(box.x + (box.w - stats->ps->box.w)/2,
		box.y + (box.h - stats->ps->box.h)/2);
	crafted->source = obj->id;
	objects->addObject(crafted);
}

/**
* @brief Remove objects whose time to live expired
*
* This function must remove objects and it must make sure to free their memories to avoid leaks
*/
void Arena::removeTemporaries()
{
	ObjectNode *aux;

	aux = objects->getTemporaryObjectsList();
	while(aux != 0)
	{
		aux->obj->stats->ps->life = aux->obj->getLife() - 1;
		if(aux->obj->getLife() < 1)
		{
			removeObject(aux->obj);
		}
		aux = aux->next;
	}
}

void Arena::checkCollisions()
{
	ObjectNode *aux, *aux2;
	Structure* content;
	Object *strike;
	int i, j, hitted;
	bool check;

	i = 0;
	while(i < 9)
	{
		content = regions[i].content;
		aux = content->getObjectsList();
		while(aux != 0)
		{
			aux2 = aux->next;
			while(aux2 != 0)
			{				
				check = aux->obj->collision(aux2->obj);

				strike = 0;
				hitted = -1;
				if(aux->obj->getType() == 1 && aux2->obj->getType() == 2)
				{
					hitted = aux->obj->id;
					strike = aux2->obj;
				}
				else if(aux2->obj->getType() == 1 && aux->obj->getType() == 2)
				{
					hitted = aux2->obj->id;
					strike = aux->obj;
				}

				if(strike != 0 && hitted != -1)
				{
					if(!(objects->getStriked(strike, hitted)) && check)
					{
						objects->addStriked(strike, hitted);
					}
					else
					{
						check = false;
					}
				}

				if(check)
				{
					collisionEffect(aux->obj, aux2->obj);
				}
				
				aux2 = aux2->next;
			}
			aux = aux->next;		
		}
		
		i++;
	}	
}

/**
* @brief Removes an object given the id and the type
*
* Removes the object from the arena.
*/
void Arena::removeObject(Object* obj)
{
	int i;

	i = 0;
	while(i < 9)
	{
		regions[i].content->removeObject(obj);
		i++;
	}

	objects->destroyObject(obj);
}

/**
* @brief Print the central data structure's contents
*/
void Arena::printObjects()
{
	ObjectNode* a;
	a = objects->getObjectsList();
	while(a != 0)
	{
		cout << "Object: " << a->obj->id << ", type: " << (int)a->obj->getType() << endl;
		a = a->next;
	}
}
