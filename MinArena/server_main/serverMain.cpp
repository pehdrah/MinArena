#include "../lib_server/serverNetwork.h"
#include "../lib/message.h"
#include "serverProcess.h"
#include "serverRoutines.h"
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <string>
#include <sstream>

using namespace std;

typedef struct Command
{
	int cmd, target;
} Command;

Command parseCommand(string command);

int main(int argc, char** argv)
{
	string s;
	int size, socketFd, recvProc;
	int *pipe1, *pipe2;
	void *buffer;
	MessageHeader *header;
	Command command;

	cout << "Starting main program:" << endl;

	size = clientBufferSize();
	buffer = prepareClientBuffer();
	header = (MessageHeader*)buffer;
	header->senderType = 'M';

	pipe1 = new int[2];
	pipe2 = new int[2];
	pipe(pipe1);
	pipe(pipe2);

	cout << "Starting Threads" << endl;
	if(fork() == 0)
	{
		return arenaManager(pipe1[0], pipe2[1]);
	}
	close(pipe1[0]);

	socketFd = createSocket();
	bindServer(socketFd);
	
	recvProc = fork();
	if(recvProc == 0)
	{
		return receiverProcess(socketFd, pipe1[1], pipe2[1]);
	}
	//close(socketFd);

	if(fork() == 0)
	{
		return senderProcess(socketFd, pipe2[0]);
	}
	close(pipe2[0]);
	
	getline(cin, s);
	command = parseCommand(s);
	cout << "Command: " << command.cmd << " Target: " << command.target << endl;
	while(command.cmd != -1)
	{
		if(command.cmd == 1)
		{
			if(command.target != -1)
			{
				header->msgType = 1;
				header->senderId = command.target;
				write(pipe1[1], buffer, size);
			}
			else
			{
				cout << "Missing parameter id" << endl;
			}
		}
		else if(command.cmd == 2)
		{
			if(command.target != -1)
			{
				header->msgType = 2;
				header->senderId = command.target;
				write(pipe1[1], buffer, size);
			}
			else
			{
				cout << "Missing parameter id" << endl;
			}
		}
		else if(command.cmd == 3)
		{
			header->msgType = 3;
			header->senderId = command.target;
			write(pipe1[1], buffer, size);
		}
		else if(command.cmd == -2)
		{
			cout << "What!?" << endl;
		}

		getline(cin, s);
		command = parseCommand(s);
		cout << "Command: " << command.cmd << " Target: " << command.target << endl;
	}

	header->msgType = -1;
	write(pipe1[1], buffer, size);
	write(pipe2[1], buffer, size);
	close(pipe1[1]);
	close(pipe2[1]);

	kill(recvProc, SIGKILL);

	cout << "Normal finishing" << endl;

	return 0;
}

Command parseCommand(string command)
{
	int pos;
	string s;
	Command cmd;

	cmd.cmd = -2;
	cmd.target = -1;

	pos = command.find_first_of(' ');
	s = command.substr(0, pos);
	if(pos != -1)
	{
		stringstream(command.substr(pos + 1)) >> cmd.target;
	}

	if(s == "create")
	{
		cmd.cmd = 1;
	}
	else if(s == "destroy")
	{
		cmd.cmd = 2;
	}
	else if(s == "list")
	{
		cmd.cmd = 3;
	}
	else if(s == "quit")
	{
		cmd.cmd = -1;
	}

	return cmd;
}
