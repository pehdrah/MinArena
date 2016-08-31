#include "serverArena.h"
#include <iostream>
#include <cstdlib>

using namespace std;

ServerArena::ServerArena(Arena* arena)
{
	int i, size;

	size = 8;

	this->arena = arena;
	playerObjects = new Object*[size];
	playerStatus = new int[size];
	calculateRtt = new bool[size];
	playerIds = new int[size];
	rtts = new long[size];
	qtrtts = new long[size];
	commands = new int[size];

	i = 0;
	while(i < 8)
	{
		calculateRtt[i] = false;
		rtts[i] = 0;
		qtrtts[i] = 0;
		playerStatus[i] = -1;
		playerIds[i] = -1;
		commands[i] = 0;
		i++;
	}
}

void ServerArena::addPlayer(ObjectStats *stats, int client)
{
	bool add;
	int i, position;

	add = true;
	position = -1;
	i = 0;
	while(i < 8)
	{
		if(playerIds[i] < 0 && position < 0)
		{
			position = i;
		}

		if(playerIds[i] == client)
		{
			add = false;
		}
		i++;
	}

	if(position < 0)
	{
		add = false;
	}

	if(add)
	{
		playerIds[position] = client;
		playerStatus[position] = 0;
		calculateRtt[position] = true;
		playerObjects[position] = new Object(stats);
		stats->ps->box.x = rand() % arena->bigBox.w;
		stats->ps->box.y = rand() % arena->bigBox.h;
		arena->addObject(playerObjects[position]);
	}

	//arena->printObjects();
}

void ServerArena::changeState()
{
	int i;

	i = 0;
	while(i < 8)
	{
		if(playerIds[i] > -1)
		{
			calculateRtt[i] = true;
			playerStatus[i] = 0;
			procCommand(i, commands[i]);
		}
		i++;
	}

	arena->turn();
/*
	i = 0;
	while(i < 8)
	{
		if(playerIds[i] != -1)
		{
//			cout << "player " << i << ": " << playerObjects[i]->stats->ps->box.x << ", " << playerObjects[i]->stats->ps->box.y << ", " << (int)playerObjects[i]->stats->ps->state << ", " << (int)playerObjects[i]->stats->ps->dx << ", " << (int)playerObjects[i]->stats->ps->dy << ", " << endl;
			cout << "player " << i << ": " << playerObjects[i]->getType() << endl;
		}
		i++;
	}
*/
}

void ServerArena::procCommand(int position, int action)
{
	playerObjects[position]->receiveInput(action);
}

void ServerArena::removePlayer(int client)
{
	int pos;

	pos = findClientSpot(client);
	if(pos > -1)
	{
		playerIds[pos] = -1;
		arena->removeObject(playerObjects[pos]);
		cout << "player " << client << " disconnected" << endl;
	}
}

bool ServerArena::playerConnected(int client)
{
	bool v;

	v = false;
	if(findClientSpot(client) > -1)
	{
		v = true;
	}

	return v;
}

void ServerArena::prepareCommand(int client, int action)
{
	int pos;

	pos = findClientSpot(client);

	if(pos > -1)
	{
		commands[pos] = action;
	}
}

int ServerArena::findClientSpot(int client)
{
	int i, pos;

	pos = -1;
	i = 0;
	while(i < 8)
	{
		if(playerIds[i] == client)
		{
			pos = i;
			i = 8;
		}
		i++;
	}

	return pos;
}

void ServerArena::markRtt(int client, long rtt)
{
	int pos;

	pos = findClientSpot(client);
	if(pos > -1)
	{
		if(calculateRtt[pos])
		{
			calculateRtt[pos] = false;
			rtts[pos] = rtts[pos] + rtt;
			qtrtts[pos]++;
			cout << "player " << client << ": " << (double)rtts[pos]/(double)qtrtts[pos] << endl;
		}
	}
}

State ServerArena::getCurrentState()
{
	State state;
	int i;

	state.players = new int[8];
	state.stats = new PrimaryStats[8];

	i = 0;
	while(i < 8)
	{
		state.players[i] = playerIds[i];
		if(playerIds[i] != -1)
		{
			copyPrimaryStats(playerObjects[i]->getPrimaryStats(), state.stats + i);
		}
		i++;
	}

	return state;
}
