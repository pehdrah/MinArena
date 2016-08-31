#ifndef ServerNetwork_H
#define ServerNetwork_H 1

int createSocket();
int bindServer(int socketFd);
int receiverProcess(int socketFd, int pipeOut, int sockPipeOut);
int senderProcess(int socketFd, int pipeIn);

#endif
