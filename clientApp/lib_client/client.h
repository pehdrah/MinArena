#ifndef Client_H
#define Client_H 1

#include "../lib/state.h"
#include "../lib/message.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/**
* @brief The Client Network Abstraction
*
* This class represents the client. It should handle the communication by sending inputs
* to the server and receiving back game states.
*
* @author Pedro Araujo Rosa
*/
class Client
{
	private:
		int socketFd, position, serverMessageSize, clientMessageSize;
		int *players, current, last;
		unsigned int *msgNumber;
		PrimaryStats *stats;
		ClientMessage *cmsg;
		MessageHeader *cHeader, *sHeader;
		char *receivedStatus;
		char *clientMessageArea;
		char *serverMessageArea; //The buffer that stores the message from the server
		State* states; //An array of states to keep track of changes
		sockaddr_in sendAddr; //The ip send address
		sockaddr_in recvAddr; //The ip receive address, got to be bound to a port.
	public:
		Client(int client, int server);
		void sendAck();
		int connect(const char* addr, int character, int port);
		int disconnect();
		int servSynchronize();
		int getPacket();
		int sendEvent(int ev, int turn);
		int sendString(char* string, int size);
		int getlastState();
		int getCurrentStateNumber();
		int getLastStateNumber();
		int getClientArrayPosition();
		int stateReceived(int state);
		char* getLastMessageAsString();
		State getCurrentState();
		State getLastState();
		State getState(int state);
		PrimaryStats* getStats();
};

#endif
