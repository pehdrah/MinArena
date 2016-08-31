#ifndef Message_H
#define Message_H 1

#include "primaryStats.h"

/**
* @brief The message sent by the client
*
* This is the structure of a command message sent by the client
*
* @author Pedro Araujo Rosa
*/
typedef struct MessageHeader
{
	char senderType, msgType;
	int senderId, msgSize, dest;
} MessageHeader;

typedef struct ClientMessage
{
	int message, turn;
} ClientMessage;

typedef struct ServerMessage
{
	int *players, *points;
	PrimaryStats *stats;
} ServerMessage;

void* prepareServerBuffer();
void* prepareClientBuffer();
char* getMessageAsString(void* buffer);
int serverBufferSize();
int clientBufferSize();

#endif
