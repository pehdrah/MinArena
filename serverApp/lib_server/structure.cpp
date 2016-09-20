#include "structure.h"
#include <iostream>

using namespace std;

Structure::Structure()
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
* This function must add the object and prepare the structures that will help finding the
* object and to make segmentation to reduce the running time of routines
*/
bool Structure::addObject(Object* obj)
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
		aux->strikeds = 0;
		aux->next = objects[hash];
		objects[hash] = aux;
		obj->id = i;
	}

	return add;
}

bool Structure::getStriked(Object* obj, int strikedId)
{
	IntNode *aux;
	bool value;

	value = false;
	aux = getNode(obj)->strikeds;
	while(aux != 0)
	{
		if(aux->key == strikedId)
		{
			value = true;
			aux = 0;
		}
		else
		{
			aux = aux->next;
		}
	}

	return value;
}

void Structure::addStriked(Object* obj, int strikedId)
{
	IntNode *node;
	
	node = new IntNode;
	node->key = strikedId;
	node->next = getNode(obj)->strikeds;
	getNode(obj)->strikeds = node;
}

/**
* @brief Removes an object
*
* The structure checks for the presence of that object and removes it without destroying it
*/
void Structure::removeObject(Object* obj)
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
* The structure checks for the presence of that object and removes it without destroying it
*/
void Structure::removeObject(int id, unsigned char type)
{
	ArenaObjectNode *aon, *aon2;
	IntNode *strk, *strk2;

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
* Removes the object from the structure and destroys it
*/
void Structure::destroyObject(Object* obj)
{
	unsigned char type;
	ArenaObjectNode *aon, *aon2;
	IntNode *strk, *strk2;

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
		
			if(aon->strikeds != 0)
			{
				strk = aon->strikeds;
				strk2 = strk->next;
				while(strk2 != 0)
				{
					delete strk;
					strk = strk2;
					strk2 = strk->next;
				}
				delete strk;
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

Object* Structure::getObject(int id, unsigned char type){
	return getNode(id, type)->obj;
}

ObjectNode* Structure::makeObjectsList(int start, int end)
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

ObjectNode* Structure::getObjectsList()
{
	return makeObjectsList(0, 8);
}

ObjectNode* Structure::getSuspendedObjectsList()
{
	return makeObjectsList(0, 1);
}

ObjectNode* Structure::getAnimatedObjectsList()
{
	return makeObjectsList(1, 8);
}

ObjectNode* Structure::getPlayersObjectsList()
{
	return makeObjectsList(1, 2);
}

ObjectNode* Structure::getTemporaryObjectsList()
{
	return makeObjectsList(2, 3);
}

ArenaObjectNode* Structure::getNode(Object* obj)
{
	return getNode(obj->id, obj->getType());
}

ArenaObjectNode* Structure::getNode(int id, unsigned char type)
{
	ArenaObjectNode *aon;

	aon = objects[type];
	while(aon != 0)
	{
		//cout << aon << endl;
		if(aon->obj->id == id)
		{
			break;
		}
		aon = aon->next;
	}

	return aon;
}
