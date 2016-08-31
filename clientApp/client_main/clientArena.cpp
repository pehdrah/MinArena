#include "clientArena.h"
#include <iostream>

using namespace std;

ClientArena::ClientArena(G_Arena* arena)
{
	int i;

	this->arena = arena;
	playerObjects = new G_Object*[8];
	playerIds = new int[8];

	i = 0;
	while(i < 8)
	{
		playerIds[i] = -1;
		playerObjects[i] = 0;
		i++;
	}
}

void ClientArena::removeTemporaries()
{
	arena->removeTemporaries();
}

void ClientArena::checkEvents(State current)
{
	int i;
	PrimaryStats *s;
	G_Object *obj;

	i = 0;
	while(i < 8)
	{
		/*
		if(playerIds[i] != -1)
		{
			cout << playerIds[i] << " ";
		}
		*/

		//Check special events
		if(playerIds[i] == -1 && current.players[i] != -1)
		{
			s = current.stats + i;
			obj = new G_Object(s);
			addObject(obj);
			playerIds[i] = current.players[i];
			playerObjects[i] = obj;
		}
		else if(playerIds[i] != -1 && current.players[i] == -1)
		{
			cout << "removendo: " << playerIds[i] << " ";
			arena->removeObject(playerObjects[i]);
			cout << "removido ";
			playerObjects[i] = 0;
			cout << "test 1 ";
			playerIds[i] = -1;
			cout << "test 2 ";
		}

		//Check players
		if(playerObjects[i] != 0 && playerIds[i] != -1)
		{
			//cout << i << ": " << (int)playerObjects[i]->stats->ps->state << ", " << (int)current.stats[i].state << ", " << (int)playerObjects[i]->stats->ss.ticks << endl;

			playerObjects[i]->stats = current.stats + i;

			if(isMeleeAttacking(playerObjects[i]->getState()))
			{
				playerObjects[i]->addTicks();
			}
			else
			{
				playerObjects[i]->zeroTicks();
			}
			if(playerObjects[i]->isStriking())
			{
				s = new PrimaryStats();
				*s = craftPrimaryStats(meleeAttackId(current.stats[i].kind));
				s->box.x = current.stats[i].box.x + (current.stats[i].box.w - s->box.w)/2;
				s->box.y = current.stats[i].box.y + (current.stats[i].box.h - s->box.h)/2;
				s->state = current.stats[i].state;
				obj = new G_Object(s);
				addObject(obj);
			}
		}

		i++;
	}
}

void ClientArena::addObject(G_Object *object)
{
	arena->addObject(object);
}

G_Object* ClientArena::getPlayerObject(int player)
{
	G_Object *obj;
	int i;
	
	obj = 0;

	i = 0;
	while(i < 8)
	{
		if(player == playerIds[i])
		{
			obj = playerObjects[i];
			i = 8;
		}
		i++;
	}

	return obj;
}

void ClientArena::zeroEvent()
{
	event = 0;
}

void ClientArena::treatEvent(int event)
{
	event = event | (this->event & 0x10);
	this->event = event;
}

int ClientArena::getEvent()
{
	return event;
}
