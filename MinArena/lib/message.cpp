#include "message.h"

void* prepareServerBuffer()
{
	return (void *)new char[serverBufferSize()];
}

void* prepareClientBuffer()
{
	return (void *)new char[clientBufferSize()];
}

char* getMessageAsString(void *buffer)
{
	return (char*)buffer;
}

int serverBufferSize()
{
	return sizeof(MessageHeader) + (1 + 8)*sizeof(int) + 8*sizeof(PrimaryStats);
}


int clientBufferSize()
{
	return sizeof(MessageHeader) + 83;
}
