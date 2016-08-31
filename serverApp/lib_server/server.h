#ifndef Server_H
#define Server_H 1

#include "../lib/state.h"
#include "../lib/primaryStats.h"
#include "../lib/message.h"
#include <sys/socket.h>

/**
* @brief The network layer for the server application
*
* This clas describes the abstraction for the network layer of the server application. It
* provides methods to receive and send messages. And interaction with the application layer.
*
* @author Pedro Araujo Rosa
*/
class Server
{
	private:
		int *id, pipeIn, pipeOut, messageSize, clientMessageSize;
		unsigned int *current, lastClientPosition;
		bool *clientEnabled;
		State *states, *mainState;
		ClientMessage *cmsg;
		MessageHeader *cHeader, *sHeader;
		char *messageArea, *clientMessageArea;
		int *playerStatus, *playerSlots, *backPlayers;
		void prepareServer();
	public:
		Server(int server, int pipeIn, int pipeOut);
		int getEvent();
		int getLastClient();
		int getLastMessage();
		int getLastPosition();
		int sendMessage(int client, char* message, int size);
		int sendStateToAll();
		int sendStateToAll(State state);
		int* getPlayers();
		ClientMessage getLastClientMessage();
		PrimaryStats* getObjectStats();
		int addPlayer(int playerId);
		void dropPlayer(int client);
		void nextState();
};

#endif
