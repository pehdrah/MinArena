#include "../lib_client/client.h"
#include "../client_gui/gArena.h"
#include "clientArena.h"
#include "clientMain.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

int callArena(int port, int character, int client_id, int server_id);

int bot(int port, int client_id, int server_id)
{
	bool quit;
	int character, status;
	SDL_Surface* screen;

	loadPrimaryStats("primaryTable.dat");
	cout << "Objects basics loaded" << endl;

	loadGeometries();
	cout << "Geometries loaded" << endl;

	loadGraphics();
	cout << "Graphics loaded" << endl;

	if(startEngine() != 0)
	{
		cout << "Engine problems" << endl;
		return 2;
	}

	status = 0;
	character = 0;
	status = callArena(character, port, client_id, server_id);

	shutdown();

	return status;
}

int callArena(int character, int port, int client_id, int server_id)
{
	bool quit, sendEvent;
	int i, id, event, clientPosition, arenaState, status, fainted, turnTime, lost;
	Client* ca;
	ClientArena* cArena;
	G_Object *playerObj;
	PrimaryStats *s;
	State current;
	G_Arena *gArena;
	long long unsigned int t0, t1, te, tgraph, turns;
	ifstream file;
	string line, address;

	file.open("patch.txt");
	if(!file.is_open())
	{	
		cout << "Patch file not found" << endl;
		return 3;
	}

	getline(file, address);

	file.close();

	ca = new Client(client_id, server_id);
	event = ca->connect(address.c_str(), character, port);
	if(event < 0)
	{
		cout << "Couldn`t connect" << endl;
		return 1;
	}
	cout << "Client connected" << endl;

	turnTime = 80;
	i = 0;
	t0 = SDL_GetTicks();
	sendEvent = true;
	quit = false;
	fainted = 0;
	//ca->servSynchronize();
	lost = 0;
	arenaState = ca->getCurrentStateNumber();
	while(!quit)
	{
		status = ca->getPacket();
		if(fainted == 0)
		{
			switch(event)
			{
				case -2:
					quit = true;
					break;
				default:
					if(sendEvent)
					{
						event = rand() % 64;
						ca->sendEvent(event, arenaState);
						sendEvent = false;
					}
					break;
			}
		}

		te = SDL_GetTicks();
		if(te - t0 >= turnTime / 10 && te - t0 < turnTime / 2)
		{
			te = t0;
			sendEvent = true;
		}

		t1 = SDL_GetTicks();
		if(t1 - t0 >= turnTime)
		{
//			cout << arenaState << "vs" << ca->getCurrentStateNumber() << endl;
//			if(ca->stateReceived(arenaState) == 0)
			if(arenaState != ca->getCurrentStateNumber())
			{
				if(t1 - t0 >= turnTime + turnTime / 2)
				{
					lost++;
					cout << arenaState << " gotta use prediction? " << ca->getCurrentStateNumber() << " " << lost << endl;
					arenaState++;
					t0 = t0 + turnTime;

					sendEvent = true;
				}

				sendEvent = false;
			}
			else if(ca->stateReceived(arenaState) == 1)
			{
				arenaState++;

				t0 = t0 + turnTime;
				current = ca->getCurrentState();
				i = 0;
				while(i < 8)
				{
					if(current.players[i] == client_id)
					{
						if(current.stats[i].life < 0)
						{
							fainted = 51;
						}
					}
					i++;
				}

				te = t0;
				sendEvent = true;
			}
			else if(t1 - t0 >= turnTime + turnTime / 2)
			{
				lost++;
				cout << arenaState << " gotta use prediction? " << ca->getCurrentStateNumber() << " " << lost << endl;
				arenaState++;
				t0 = t0 + turnTime;
				te = t0;
				sendEvent = true;
			}
		}

		if(arenaState < ca->getCurrentStateNumber())
		{
			arenaState++;
		}
		else if(arenaState > ca->getCurrentStateNumber())
		{
			arenaState = arenaState - 1;
		}

		if(fainted > 50)
		{
			quit = true;
		}
	}
	ca->disconnect();

	cout << "Lost packets: " << lost << " tax:" << (lost*1.0)/turns*100 << endl;

	return 0;
}
