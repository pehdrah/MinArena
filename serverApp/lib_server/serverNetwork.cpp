#include "serverNetwork.h"
#include "../lib/message.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include <iostream>

using namespace std;

typedef struct ClientAddress
{
	sockaddr_in addr;
	int clientId;
} ClientAddress;

typedef struct ClientAddressNode
{
	ClientAddress address;
	ClientAddressNode *next;
} ClientAddressNode;

sockaddr_in findClientAddress(ClientAddressNode *list, int client);
bool clientIn(ClientAddressNode *list, int client);

int createSocket()
{
	return socket(AF_INET, SOCK_DGRAM, 0);
}

int bindServer(int socketFd)
{
	sockaddr_in portAddr;

	portAddr.sin_family = AF_INET;
	portAddr.sin_port = htons(7000);
	portAddr.sin_addr.s_addr = inet_addr("0.0.0.0");

	return bind(socketFd, (struct sockaddr *)&portAddr, sizeof(sockaddr_in));
}

int receiverProcess(int socketFd, int pipeOut, int sockPipeOut)
{
	int status, bufferSize, stuff;
	char *buffer;
	MessageHeader *header;
	sockaddr_in addr;
	sockaddr_in *toSend;
	socklen_t len;

	len = sizeof(sockaddr_in);
	bufferSize = clientBufferSize();
	buffer = (char *)prepareClientBuffer();
	header = (MessageHeader *)buffer;
	toSend = (sockaddr_in *)(buffer + sizeof(MessageHeader));

	status = recvfrom(socketFd, buffer, bufferSize, 0, (struct sockaddr *)&addr, &len);
	while(status > 0)
	{
		//stuff = ntohs(addr.sin_port);
		if(header->senderType == 'C')
		{
			write(pipeOut, buffer, bufferSize);
			if(header->msgType == 1)
			{
				cout << "addr - " << inet_ntoa(addr.sin_addr) << ":" << ntohs(addr.sin_port) << ";" << len << ";" << sizeof(sockaddr_in) << endl;
				*toSend = addr;
				//toSend->sin_addr.s_addr = addr.sin_addr.s_addr;
				//toSend->sin_port = addr.sin_port;
				//cout << "To send - " << inet_ntoa(toSend->sin_addr) << ":" << ntohs(toSend->sin_port) << endl;
				write(sockPipeOut, buffer, bufferSize);
			}
			else if(header->msgType == 2)
			{
				write(sockPipeOut, buffer, bufferSize);
			}
		}
		status = recvfrom(socketFd, buffer, bufferSize, 0, (struct sockaddr *)&addr, &len);
	}

	return status;
}

int senderProcess(int socketFd, int pipeIn)
{
	int status, bufferSize;
	char *buffer;
	MessageHeader *header;
	sockaddr_in addr;
	sockaddr_in *toSend;
	ClientAddressNode *list, *node, *aux;
	socklen_t len;

	bufferSize = serverBufferSize();
	buffer = (char *)prepareServerBuffer();
	header = (MessageHeader *)buffer;
	toSend = (sockaddr_in *)(buffer + sizeof(MessageHeader));
	list = 0;
	len = sizeof(sockaddr_in);

	status = read(pipeIn, buffer, bufferSize);
	while(status > 0)
	{
		if(header->senderType == 'C')
		{
			if(header->msgType == 1)
			{
				if(!clientIn(list, header->senderId))
				{
					node = new ClientAddressNode;
					node->address.addr = *toSend;
					node->address.clientId = header->senderId;
					node->next = list;
					list = node;
				}
			}
			else if(header->msgType == 2)
			{
				if(list->address.clientId == header->senderId)
				{
					node = list;
					list = list->next;
					delete node;
				}
				else
				{
					node = list;
					aux = node->next;
					while(aux != 0)
					{
						if(aux->address.clientId == header->senderId)
						{
							node->next = aux->next;
							delete aux;
							aux = 0;
						}
						else
						{
							node = aux;
							aux = node->next;
						}
					}
				}
			}
		
		}
		else if(header->senderType == 'S')
		{
			if(clientIn(list, header->dest))
			{
				addr = findClientAddress(list, header->dest);
				sendto(socketFd, buffer, bufferSize, 0, (struct sockaddr *)&addr, len);
			}
		}
		else if(header->senderType == 'M')
		{
			if(header->msgType == -1)
			{
				close(pipeIn);
			}
		}
		status = read(pipeIn, buffer, bufferSize);
	}
}

sockaddr_in findClientAddress(ClientAddressNode *list, int client)
{
	ClientAddressNode *it, *walk;
	sockaddr_in addr;

	walk = list;
	while(walk != 0)
	{
		if(walk->address.clientId == client)
		{
			it = walk;
			walk = 0;
		}
		else
		{
			walk = walk->next;
		}
	}

	addr = it->address.addr;
	return addr;
}

bool clientIn(ClientAddressNode *list, int client)
{
	bool value;
	ClientAddressNode *walk;

	value = false;

	walk = list;
	while(walk != 0)
	{
		if(walk->address.clientId == client)
		{
			value = true;
			walk = 0;
		}
		else
		{
			walk = walk->next;
		}
	}

	return value;
}
