#include "../lib_server/server.h"
#include "../lib/message.h"
#include "serverArena.h"
#include "serverProcess.h"
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

int serverProcess(int serverId, int pipeIn, int pipeOut)
{
	struct timeval t0, t1;
	long full, first, elapsed;
	Server* server;
	ServerArena* sa;
	State state;
	ClientMessage cmsg;
	Arena* arena;
	MessageHeader *header;
	void *buffer;
	int *playerStatus, *players;
	bool* messageReceived;
	bool quit;
	int i, j, w, h, message, event, status, turnTime, client, bufferSize, attempts;
	ObjectStats *s;
	unsigned long turn;

	bufferSize = clientBufferSize();
	buffer = prepareClientBuffer();
	header = (MessageHeader *)buffer;

	messageReceived = new bool[8];
	playerStatus = new int[8];
	i = 0;
	while(i < 8)
	{
		messageReceived[i] = false;
		playerStatus[i] = 0;
		i++;
	}

	loadObjectsBasics("primaryTable.dat", "secondaryTable.dat");
	loadGeometries();

	arena = new Arena(800, 600);
	s = new ObjectStats();
	*s = craftObjectStats(9);
	s->ps->box.x = 200;
	s->ps->box.y = 200;
	arena->addObject(new Object(s));

	server = new Server(serverId, pipeIn, pipeOut);
	sa = new ServerArena(arena);

	gettimeofday(&t0, 0);
	turnTime = 100000;
	full = 0;
	first = 0;
	turn = 0;
	attempts = 4;
	quit = false;
	i = 0;
	while(!quit)
	{
		status = server->getEvent();
		if(status == -4)
		{
			quit = true;
		}
		else if(status != -1 && status != -2 && status != -3)
		{
			cmsg = server->getLastClientMessage();
			client = server->getLastClient();
			message = cmsg.message;
			if(status == 1)
			{
				if(!sa->playerConnected(client))
				{
					cout << "Player connecting: " << client << endl;
					server->addPlayer(client);
					s = new ObjectStats();
					*s = craftObjectStats(message & 0x000000FF);
					sa->addPlayer(s, client);
					playerStatus[server->getLastPosition()] = 0;
//					cout << "Player connected" << endl;
				}
			}
			else if(status == 2)
			{
				server->dropPlayer(client);
				sa->removePlayer(client);
			}
			else
			{
				sa->prepareCommand(client, cmsg.message);
//				sa->markRtt(client, elapsed - first);
				messageReceived[server->getLastPosition()] = true;
			}
		}

		gettimeofday(&t1, 0);
		elapsed = t1.tv_sec*1000000 + t1.tv_usec;
		full = elapsed - t0.tv_sec*1000000 - t0.tv_usec;

		//Game Turn
		if(full >= turnTime - 10000)
		{
			sa->changeState();
			gettimeofday(&t1, 0);
			full = t1.tv_sec*1000000 + t1.tv_usec;
			//cout << full - elapsed << endl;
			first = elapsed;
			server->sendStateToAll(sa->getCurrentState());
			server->nextState();
			t0.tv_usec = t0.tv_usec + turnTime;
			if(t0.tv_usec > 1000000)
			{
				t0.tv_sec = t0.tv_sec + 1;
				t0.tv_usec = t0.tv_usec - 1000000;
			}

			//Disconnection checking
			cout << "Arena " << serverId << " - " << turn << " ";
			players = server->getPlayers();
			i = 0;
			while(i < 8)
			{
				if(players[i] != -1)
				{
					//cout << players[i] << ": " << playerStatus[i] << " ";
					if(playerStatus[i] > 200)
					{	
						server->dropPlayer(players[i]);
						sa->removePlayer(players[i]);
					}
				}
				else
				{
					playerStatus[i] = 0;
				}
				i++;
			}
			cout << endl;

			j = 0;
			while(j < 8)
			{
				if(!messageReceived[j])
				{
					playerStatus[j]++;
				}
				else
				{
					playerStatus[j] = 0;
					messageReceived[j] = false;
				}
				j++;
			}

			j = 1;
			turn++;
		}

		//Message Resending
//		if(full >= j*turnTime/8 && j < 4)
		if(full >= j*turnTime/attempts)
		{
			//cout << "resending " << server->sendState() << endl;
			server->sendStateToAll();
			j++;
		}
	}

	close(pipeIn);
	close(pipeOut);

	cout << "Arena Closed" << endl;

	return 0;
}
