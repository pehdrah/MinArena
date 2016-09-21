#include "gStructure.h"
#include <iostream>

using namespace std;

G_Structure::G_Structure()
{
	int i;
	nextId = new int[8];
	objects = new ArenaObjectNode*[8];

	i = 0;
	while(i < 8)
	{
		nextId[i] = 1;
		objects[i] = 0;
		i++;
	}
}


/**
* @brief This function adds an Object to the arena.
*
* This function must add the object and prepare the G_Structures that will help finding the
* object and to make segmentation to reduce the running time of routines
*/
bool G_Structure::addObject(G_Object* obj)
{
	int i, hash;
	bool add;
	ArenaObjectNode *aux, *aux2;

	hash = obj->getType();

	add = true;
	if(obj->id != 0)
	{
		i = obj->id;
		if(getNode(obj->id, obj->getType()) != 0)
		{
			add = false;
		}
	}
	else
	{
		i = nextId[hash];
		nextId[hash]++;
	}

	if(add)
	{
		aux = new ArenaObjectNode;
		aux->obj = obj;
		aux->next = objects[hash];
		objects[hash] = aux;
		obj->id = i;
		//cout << "added type: " << (int)obj->getType() << " id: " << obj->id << endl;
	}

	return add;
}

/**
* @brief Removes an object
*
* The G_Structure checks for the presence of that object and removes it without destroying it
*/
void G_Structure::removeObject(G_Object* obj)
{
	unsigned char type;
	int id;
	ArenaObjectNode *aon, *aon2;

	type = obj->getType();
	id = obj->id;
	aon2 = 0;
	aon = objects[type];
	while(aon != 0)
	{
		if(aon->obj->id == id)
		{			
			if(aon2 == 0)
			{
				objects[type] = aon->next;
			}
			else
			{
				aon2->next = aon->next;
			}

			delete aon;

			break;
		}
		else
		{
			aon2 = aon;
			aon = aon->next;
		}
	}
}



/**
* @brief Removes an object given the id and the type of the object
*
* The G_Structure checks for the presence of that object and removes it without destroying it
*/
void G_Structure::removeObject(int id, unsigned char type)
{
	ArenaObjectNode *aon, *aon2;

	aon2 = 0;
	aon = objects[type];
	while(aon != 0)
	{
		if(aon->obj->id == id)
		{			
			if(aon2 == 0)
			{
				objects[type] = aon->next;
			}
			else
			{
				aon2->next = aon->next;
			}

			delete aon;

			break;
		}
		else
		{
			aon2 = aon;
			aon = aon->next;
		}
	}
}

/**
* @brief Destroys an object
*
* Removes the object from the G_Structure and destroys it
*/
void G_Structure::destroyObject(G_Object* obj)
{
	unsigned char type;
	ArenaObjectNode *aon, *aon2;

	type = obj->getType();
	aon2 = 0;
	aon = objects[type];
	while(aon != 0)
	{
		if(aon->obj->id == obj->id)
		{			
			if(aon2 == 0)
			{
				objects[type] = aon->next;
			}
			else
			{
				aon2->next = aon->next;
			}

			if(aon->obj != 0)
			{
				delete aon->obj;
			}

			delete aon;

			break;
		}
		else
		{
			aon2 = aon;
			aon = aon->next;
		}
	}
}

G_Object* G_Structure::getGraphicObject(int id, unsigned char type){
	return getNode(id, type)->obj;
}

ObjectNode* G_Structure::makeObjectsList(int start, int end)
{
	ArenaObjectNode *aon;
	ObjectNode *on, *on2;
	
	on2 = 0;
	on = 0;
	while(start < end)
	{
		aon = objects[start];
		while(aon != 0)
		{
			on2 = new ObjectNode;
			on2->obj = aon->obj;
			on2->next = on;
			on = on2;
			aon = aon->next;
		}
		start++;
	}

	return on;
}

ObjectNode* G_Structure::getObjectsList()
{
	return makeObjectsList(0, 8);
}

ObjectNode* G_Structure::getSuspendedObjectsList()
{
	return makeObjectsList(0, 1);
}

ObjectNode* G_Structure::getAnimatedObjectsList()
{
	return makeObjectsList(1, 8);
}

ObjectNode* G_Structure::getPlayersObjectsList()
{
	return makeObjectsList(1, 2);
}

ObjectNode* G_Structure::getTemporaryObjectsList()
{
	return makeObjectsList(2, 3);
}

ArenaObjectNode* G_Structure::getNode(G_Object* obj)
{
	return getNode(obj->id, obj->getType());
}

ArenaObjectNode* G_Structure::getNode(int id, unsigned char type)
{
	ArenaObjectNode *aon;

	aon = objects[type];
	while(aon != 0)
	{
		if(aon->obj->id == id)
		{
			break;
		}
		else
		{
			aon = aon->next;
		}
	}

	return aon;
}
