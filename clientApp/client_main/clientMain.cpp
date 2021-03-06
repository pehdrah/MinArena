#include "../lib_client/client.h"
#include "../client_gui/gArena.h"
#include "../client_gui/gCharSel.h"
#include "clientArena.h"
#include "clientMain.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int client_id, server_id;

void addBox(G_Arena* arena, int x, int y);

int main(int argc, char** argv)
{
	bool quit;
	int character, port;
	SDL_Surface* screen;

	loadPrimaryStats("primaryTable.dat");
	cout << "Objects basics loaded" << endl;

	loadGeometries();
	cout << "Geometries loaded" << endl;

	loadGraphics();
	cout << "Graphics loaded" << endl;

	stringstream(argv[1]) >> port;
	stringstream(argv[2]) >> client_id;
	stringstream(argv[3]) >> server_id;

	if(startEngine() != 0)
	{
		cout << "Engine problems" << endl;
		return 2;
	}

	screen = callWindow(640, 480);

	quit = false;
	while(!quit)
	{
		character = callCharSelect(screen);
		if(character > -1)
		{
			callArena(screen, character, port);
		}
		quit = true;
	}

	shutdown();

	return 0;
}

int callArena(SDL_Surface* screen, int character, int port)
{
	bool quit, sendEvent;
	bool *drawed;
	int i, id, event, arenaState, status, fainted, turnTime, attempts, draw, lost;
	Client* client;
	ClientArena* cArena;
	G_Object *playerObj;
	PrimaryStats *s;
	State current;
	G_Arena *gArena;
	long long unsigned int t0, t1, te, tgraph, sum, spike, turns;
	ifstream file;
	string line, address;

	attempts = 5;
	drawed = new bool[256];
	i = 0;
	while(i < 256)
	{
		drawed[i] = false;
		i++;
	}

	file.open("patch.txt");
	if(!file.is_open())
	{	
		cout << "Patch file not found" << endl;
		return 3;
	}

	getline(file, address);

	file.close();

	client = new Client(client_id, server_id);
	event = client->connect(address.c_str(), character, port);
	if(event < 0)
	{
		cout << "Couldn`t connect" << endl;
		return 1;
	}
	cout << "Client connected" << endl;

	current = client->getCurrentState();

	gArena = new G_Arena(screen, 800, 1200);
	cArena = new ClientArena(gArena);
	cArena->checkEvents(current);
  
	s = new PrimaryStats;
	addBox(gArena, 200, 200);
	addBox(gArena, 200, 400);
	addBox(gArena, 200, 600);
	addBox(gArena, 200, 800);
	addBox(gArena, 200, 1000);
	addBox(gArena, 600, 200);
	addBox(gArena, 600, 400);
	addBox(gArena, 600, 600);
	addBox(gArena, 600, 800);
	addBox(gArena, 600, 1000);
/*
	addBox(gArena, 300, 200);
	addBox(gArena, 300, 400);
	addBox(gArena, 300, 600);
	addBox(gArena, 300, 800);
	addBox(gArena, 300, 1000);
	addBox(gArena, 500, 200);
	addBox(gArena, 500, 400);
	addBox(gArena, 500, 600);
	addBox(gArena, 500, 800);
	addBox(gArena, 500, 1000);
	addBox(gArena, 100, 200);
	addBox(gArena, 100, 400);
	addBox(gArena, 100, 600);
	addBox(gArena, 100, 800);
	addBox(gArena, 100, 1000);
	addBox(gArena, 400, 200);
	addBox(gArena, 400, 400);
	addBox(gArena, 400, 600);
	addBox(gArena, 400, 800);
	addBox(gArena, 400, 1000);
*/

	turnTime = 100;
	i = 0;
	sum = 0;
	spike = 0;
	turns = 0;
	lost = 0;
	t0 = SDL_GetTicks();
	sendEvent = true;
	quit = false;
	fainted = 0;
	arenaState = client->getCurrentStateNumber();
	playerObj = cArena->getPlayerObject(client_id);
	while(!quit)
	{
		status = client->getPacket();
		if(fainted == 0)
		{
			event = gArena->captureEvent();
			cArena->treatEvent(event);
		
			switch(event)
			{
				case -2:
					quit = true;
					break;
				default:
					if(sendEvent)
					{
						//cout << event << endl;
						client->sendEvent(cArena->getEvent(), arenaState);
						sendEvent = false;
					}
					break;
			}
		}

		te = SDL_GetTicks();
		if(te - t0 >= turnTime / attempts)
		{
			te = t0;
			sendEvent = true;
		}

		t1 = SDL_GetTicks();
		if(t1 - t0 >= turnTime)
		{
			if(arenaState < client->getCurrentStateNumber())
			{
				arenaState++;
			}
			else if(arenaState > client->getCurrentStateNumber())
			{
				arenaState = arenaState - 1;
			}
			draw = arenaState & 0x000000FF;

//			cout << arenaState << "vs" << ca->getCurrentStateNumber() << endl;
//			if(ca->stateReceived(arenaState) == 0)
			if(arenaState != client->getCurrentStateNumber())
			{
				if(t1 - t0 >= turnTime + turnTime / 2)
				{
					cout << arenaState << " gotta use prediction? " << client->getCurrentStateNumber() << endl;
					arenaState++;
					lost++;
					t0 = t0 + turnTime;

					sendEvent = true;
				}

				sendEvent = false;
			}
			else if(client->stateReceived(arenaState) == 1 && !drawed[draw])
			{
				cArena->removeTemporaries();
				current = client->getState(arenaState);
				cArena->checkEvents(current);
				cArena->zeroEvent();

				t0 = t0 + turnTime;
				if(playerObj == 0)
				{
					playerObj = cArena->getPlayerObject(client_id);
				}

				if(playerObj->getLife() <= 0)
				{
					fainted++;
				}

				t1 = SDL_GetTicks();
				//cout << "Drawing! ";
				gArena->drawArenaPiece(playerObj);
				//cout << arenaState << endl;
				tgraph = SDL_GetTicks() - t1;
				if(tgraph > spike)
				{
					spike = tgraph;
				}
				sum = sum + tgraph;

				drawed[draw] = true;

				draw = (arenaState - 1) & 0x000000FF;
				drawed[draw] = false;

				te = t0;
				sendEvent = true;
			}
/*
			else if(t1 - t0 >= turnTime + turnTime / 2)
			{
				cout << arenaState << " gotta use prediction? " << client->getCurrentStateNumber() << endl;
				arenaState++;
				t0 = t0 + turnTime;
				te = t0;
				sendEvent = true;
			}
*/
		}

		if(fainted > 50)
		{
			quit = true;
		}

		turns++;
	}
	client->disconnect();
	cout << "Average time to draw(ms): " << (sum*1.0)/turns << endl;
	cout << "Time to draw spike(ms): " << spike << endl;
	cout << "Lost packets: " << lost << " tax:" << (lost*1.0)/turns*100 << endl;

	return 0;
}

int callCharSelect(SDL_Surface* screen)
{
	bool quit;
	char event;
	int ogof, gaua, erart, ontev, selection, value;
	G_CharSel *charSel;
	
	ogof = 0;
	gaua = 1;
	erart = 2;
	ontev = 3;

	charSel = new G_CharSel(screen);
	charSel->loadChar("ogof_box.bmp", 224, 144, ogof);
	charSel->loadChar("gaua_box.bmp", 272, 144, gaua);
	charSel->loadChar("erart_box.bmp", 320, 144, erart);
	charSel->loadChar("ontev_box.bmp", 368, 144, ontev);

	selection = ogof;
	charSel->setSelection(ogof);
	while(!quit)
	{
		event = charSel->captureEvent();
		switch(event)
		{
			case 'a':
				if(selection != 0 || selection != 4)
				{
					selection = selection - 1;
				}
				break;
			case 'd':
				if(selection != 3 || selection != 7)
				{
					selection = selection + 1;
				}
				break;
			case 'g':
				quit = true;
				break;	
			case -2:
				quit = true;
				selection = -2;
				break;
			default:
				break;
		}

		charSel->setSelection(selection);
		charSel->draw();
	}

	return selection;
}


void addBox(G_Arena* arena, int x, int y)
{
	PrimaryStats *s;

	s = new PrimaryStats();
	*s = craftPrimaryStats(9);
	s->box.x = x;
	s->box.y = y;
	arena->addObject(new G_Object(s));
}
