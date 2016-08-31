#include "../lib/message.h"
#include "serverProcess.h"
#include "serverRoutines.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

using namespace std;

typedef struct ArenaPipes
{
	int arenaId, pipeIn, pipeOut;
} ArenaPipes;

int findOpenSpot(ArenaPipes** aPipes, int size);
int findServ(ArenaPipes** aPipes, int size, int serv);

int arenaManager(int pipeIn, int pipeOut)
{
	int *pipe1;
	int size, status, forked, i, aSize, arena, clientPoolSize;
	char *buffer;
	bool *clients;
	ArenaPipes **aPipes;
	MessageHeader *header;
	ClientMessage *msg;

	aSize = 256;
	aPipes = new ArenaPipes*[aSize];

	pipe1 = new int[2];

	clientPoolSize = 4096;
	clients = new bool[clientPoolSize];

	size = clientBufferSize();
	buffer = (char *)prepareClientBuffer();
	header = (MessageHeader *)buffer;
	msg = (ClientMessage *)(buffer + sizeof(MessageHeader));

	i = 0;
	while(i < aSize)
	{
		aPipes[i] = 0;
		i++;
	}

	i = 0;
	while(i < clientPoolSize)
	{
		clients[i] = false;
		i++;
	}

	cout << "Manager is Ready!" << endl;

	forked = 1;
	status = read(pipeIn, buffer, size);
	while(status > 0)
	{
		if(header->senderType == 'C')
		{
			arena = findServ(aPipes, aSize, header->dest);
			if(arena > -1)
			{
				write(aPipes[arena]->pipeIn, buffer, size);
				if(header->msgType == 1)
				{
//					cout << "Someone Arrived! " << endl;
					if(!clients[header->senderId])
					{
						clients[header->senderId] = true;
					}
					else
					{
						//header->senderType = 'S';
						//header->msgType = -2;
						//write(pipeOut, buffer, size);
					}
				}
				else if(header->msgType == 2)
				{
					clients[header->senderId] = false;
				}
			}
			else
			{
				header->senderType = 'S';
				header->msgType = -2;
				write(pipeOut, buffer, size);
			}
		}
		else if(header->senderType == 'M')
		{
			if(header->msgType == 1)
			{
				pipe(pipe1);
				fcntl(pipe1[0], F_SETFL, O_NONBLOCK);
				forked = fork();
				if(forked == 0)
				{
					delete aPipes;
					delete buffer;
					delete clients;
					serverProcess(header->senderId, pipe1[0], pipeOut);
				}
				else
				{
					i = findOpenSpot(aPipes, aSize);
					aPipes[i] = new ArenaPipes;
					aPipes[i]->arenaId = header->senderId;
					aPipes[i]->pipeIn = pipe1[1];
					aPipes[i]->pipeOut = pipeOut;
				}
			}
			else if(header->msgType == 2)
			{
				i = findServ(aPipes, aSize, header->senderId);
				write(aPipes[i]->pipeIn, buffer, size);
				close(aPipes[i]->pipeIn);
				delete aPipes[i];
				aPipes[i] = 0;
			}
			else if(header->msgType == -1)
			{
				forked = 0;
			}
			else if(header->msgType == 3)
			{
				cout << "Running Arena IDs: " << endl;
				i = 0;
				while(i < aSize)
				{
					if(aPipes[i] != 0)
					{
						cout << aPipes[i]->arenaId << ", ";
					}
					i++;
				}

				cout << endl;
			}
		}

		if(forked == 0)
		{
			status = -1;
		}
		else
		{
			status = read(pipeIn, buffer, size);
		}
	}

	close(pipeIn);
	close(pipeOut);

	i = 0;
	while(i < size)
	{
		if(aPipes[i] != 0)
		{
			header->senderType = 'M';
			header->msgType = 2;
			write(aPipes[i]->pipeIn, buffer, size);
			close(aPipes[i]->pipeIn);
			close(aPipes[i]->pipeOut);
			delete aPipes[i];
		}
		i++;
	}

	return 0;
}



int findOpenSpot(ArenaPipes** vec, int size)
{
	int i;

	i = 0;
	while(vec[i] != 0 && i < size)
	{
		i++;
	}

	if(i >= size)
	{
		i = -1;
	}

	return i;
}

int findServ(ArenaPipes** vec, int size, int serv)
{
	int i, a;

	a = -1;
	i = 0;
	while(i < size)
	{
		if(vec[i]->arenaId == serv)
		{
			a = i;
			i = size;
		}
		i++;
	}

	return a;
}
