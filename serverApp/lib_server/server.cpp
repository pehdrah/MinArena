#include "server.h"
#include <cstdlib>
#include <algorithm>
#include <sys/time.h>
#include <iostream>

using namespace std;

/**
* @brief Constructs a new server network layer with the specified id
*
* Constructs a new server layer.  The constructor is also responsible for allocating space
* to create the message sent by server, and it keeps track of the people who is active receiving
* messages. The server also has an identification number for security purposes. This layer is
* responsible to make comunications more abstract.
*
* @param server The server identification
* @param pipeOut A pipe to talk with the manager
* @param pipeIn From where the messages comes from
*/
Server::Server(int server, int pipeIn, int pipeOut)
{
	prepareServer();

	sHeader->senderId = server;

	*current = 0;
	this->pipeOut = pipeOut;
	this->pipeIn = pipeIn;
}

void Server::prepareServer()
{
	int i, size;

	size = 8;

	playerSlots = new int[size];
	backPlayers = new int[size];
	clientEnabled = new bool[size];
	mainState = new State;

	messageSize = serverBufferSize();
	clientMessageSize = clientBufferSize();
	messageArea = (char *)prepareServerBuffer();
	clientMessageArea = (char *)prepareClientBuffer();

	sHeader = (MessageHeader *) messageArea;
	current = (unsigned int *) (messageArea + sizeof(MessageHeader));
	mainState->players = (int *) (messageArea + sizeof(MessageHeader) + sizeof(int));
	mainState->stats = (PrimaryStats *) (messageArea + sizeof(MessageHeader) + (1 + 8)*sizeof(int));

	sHeader->senderType = 'S';

	*current = 0;

	cHeader = (MessageHeader *) clientMessageArea;
	cmsg = (ClientMessage *) (clientMessageArea + sizeof(MessageHeader));

	i = 0;
	while(i < 8)
	{
		mainState->players[i] = -1;
		backPlayers[i] = -1;
		playerSlots[i] = 0;
		clientEnabled[i] = false;
		i++;
	}
}

/**
* @brief Register a player to receive messages
*
* Adds a player on the list to receive messages sent by the server
*/
int Server::addPlayer(int playerId)
{
	bool add;
	int i;

	add = true;
	i = 0;
	while(i < 8)
	{
		if(backPlayers[i] == playerId)
		{
			add = false;
		}
		i++;
	}

	if(add)
	{
		i = 0;
		while(i < 8)
		{
			if(backPlayers[i] == -1)
			{
				backPlayers[i] = playerId;
				playerSlots[i] = 0;
				clientEnabled[i] = true;
				i = 8;
			}
			i++;
		}
	}

	return 0;
}

/**
* @brief Receives a package from the network interface
*
* Receives a package from the network interface and checks if it came from a registered client
* or it is a new connection request.
*
* @returns The amount of bytes read or -1 in case of failure
*/
int Server::getEvent()
{
	int i, status;
	socklen_t len;

	status = read(pipeIn, clientMessageArea, clientMessageSize);

	lastClientPosition = -1;

	if(status != -1)
	{
		if(cHeader->senderType == 'M' && cHeader->msgType == 2)
		{
			status = -4;
		}
		else if(cHeader->senderType != 'C')
		{
			status = -2;
		}
	}

	if(status > 0)
	{
		if(cHeader->msgType == 64)
		{
			status = -3;
		}
		else
		{
			status = cHeader->msgType;
		}

		i = 0;
		while(i < 8)
		{
			if(backPlayers[i] == cHeader->senderId)
			{
				if(cHeader->msgType == 0)
				{
					if(*current + 2 > cmsg->turn && *current - 2 < cmsg->turn)
					{
						playerSlots[i] = 1;
					}
					else
					{
						status = -1;
					}
				}
				lastClientPosition = i;
				i = 8;
			}
			i++;
		}
	}

	return status;
}

/**
* @brief Sends states to a single client
*
* This method sends a message to a registered clients for the game to work. It will
* not send messages to clients who signalized receiving the last message
*
* @returns The number of bytes sent or -1 in case of failure
*/
int Server::sendMessage(int client, char* message, int size)
{
	int sent, total, pos, i;
	char* saveContent;

	saveContent = new char[messageSize];

	copy(messageArea, messageArea + messageSize, saveContent);

	i = 0;
	while(i < size)
	{
		messageArea[i + 2] = message[i];
		i++;
	}

	pos = 8;
	i = 0;
	while(i < 8)
	{
		if(mainState->players[pos] == client)
		{
			pos = i;
		}
		i++;
	}
	
	if(pos < 8)
	{
		total = 0;
		if(mainState->players[pos] != -1)
		{
			sent = write(pipeOut, messageArea, size + 2);
			total = total + sent;
		}
	}

	return total;
}

/**
* @brief Sends states to the registered clients
*
* This method sends the current state to the registered clients for the game to work. It will
* not send messages to clients who signalized receiving the last message
*
* @returns The number of bytes sent or -1 in case of failure
*/
int Server::sendStateToAll()
{
	int i, sent, total, c;

	c = 0x000000FF & *current;

	sHeader->msgType = 0;

	total = 0;
	i = 0;
	while(i < 8)
	{
		if(backPlayers[i] != -1 && clientEnabled[i])
		{
			sHeader->dest = backPlayers[i];
			sent = write(pipeOut, messageArea, messageSize);
			total = total + sent;
		}
		i++;
	}

	return total;
}

/**
* @brief Sends states to the registered clients
*
* This method sends the current state to the registered clients for the game to work. It will
* not send messages to clients who signalized receiving the last message
*
* @returns The number of bytes sent or -1 in case of failure
*/
int Server::sendStateToAll(State state)
{
	int i, j;

	i = 0;
	while(i < 8)
	{
		if(backPlayers[i] != -1 && !clientEnabled[i])
		{
			j = 0;
			while(j < 8)
			{
				if(backPlayers[i] == state.players[j])
				{
					clientEnabled[i] = true;
				}
				j++;
			}
		}
		i++;
	}

	copy(state.players, state.players + 8, mainState->players);
	copy(state.stats, state.stats + 8, mainState->stats);

	return sendStateToAll();
}

/**
* @brief Unregiste a player from the list
*
* Unregisters a player from the connection list so it is not going to receive any messages
*/
void Server::dropPlayer(int client)
{
	int i;

	i = 0;
	while(i < 8)
	{
		if(backPlayers[i] == client)
		{
			backPlayers[i] = -1;
			playerSlots[i] = -1;
			clientEnabled[i] = false;
			i = 8;
		}
		i++;
	}
}

/**
* @brief Get the last client message received
*
* This method returns a copy of the last message structure received by the server
*
* @returns The last ClientMessage received
*/
ClientMessage Server::getLastClientMessage()
{
	ClientMessage cm;

	cm.message = cmsg->message;
	cm.turn = cmsg->turn;

	return cm;
}

/**
* @brief Get the last client who sent a message
*
* This method returns the identification of the client of the last received message
*
* @returns The identification number of the last client
*/
int Server::getLastClient()
{
	return cHeader->senderId;
}

/**
* @brief Get the last message received
*
* This method returns the last message received.
*
* @returns The integer which is the message sent by the client
*/
int Server::getLastMessage()
{
	return cmsg->message;
}

/**
* @brief Get the position in the array of the client who sent the last message
*
* This method returns in information internal of the network layer which is the position of the
* client of the last message received in the internal array
*
* @returns An integer that is the position of the last client in the internal array
*/
int Server::getLastPosition()
{
	return lastClientPosition;
}

/**
* @brief Get the connected players
*
* This method returns the array in which the connected players are stored. A position with a
* A negative number means that there is no client connected at that position.
*
* @returns An array of integers with the connected players
*/
int* Server::getPlayers()
{
	return mainState->players;
}

/**
* @brief Get the primary stats for all the objects of the players
*
* This method returns the array of primary stats that is being manipulated on that moment. Each
* element in the array belongs to a player.
*
* @returns The array with the primary stats being currently manipulated
*/
PrimaryStats* Server::getObjectStats()
{
	return mainState->stats;
}

/**
* @brief Changes the state of the server
*
* Increments the server state to synchronize it with the application state, it also marks the
* players as the did not received the last message
*/
void Server::nextState()
{
	int i;

	*current = *current + 1;

	i = 0;
	while(i < 8)
	{
		playerSlots[i] = 0;
		i++;
	}
}
