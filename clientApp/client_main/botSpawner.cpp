#include<iostream>
#include<unistd.h>
#include<signal.h>
#include"bot.h"

using namespace std;

int spawner(int pipeOut);
int threadManager(int pipeIn);

typedef struct Node
{
	int pid;
	Node *next;
} Node;

int main(int argc, char **argv)
{
	char a;
	int pid;
	int *channel;

	channel = new int[2];
	pipe(channel);

	pid = fork();
	if(pid == 0)
	{
		threadManager(channel[0]);
		return 0;
	}
	close(channel[0]);

	pid = fork();
	if(pid == 0)
	{
		spawner(channel[1]);
	}
	else
	{
		a = 'C';
		while(a != 'Q')
		{
			cin >> a;
		}
		pid = -1;
		write(channel[1], &pid, sizeof(int));
	}

	return 0;
}

int spawner(int pipeOut)
{
	int port, cid, sid, i, pid;

	i = 0;
	while(i < 2)
	{
		cid = 0;
		while(cid < 8)
		{
			sid = i + 1;
			port = 10000 + cid + 1 + 10*sid;
			pid = fork();
			if(pid == 0)
			{
				bot(port, 10*sid + cid + 1, sid);
				i = 6;
				cid = 9;
			}
			else
			{
				write(pipeOut, &pid, sizeof(int));
			}
			sleep(2);
			
			cid++;
		}

		i++;
	}

	return 0;
}

int threadManager(int pipeIn)
{
	int msg;
	Node *list, *aux;

	list = 0;
	read(pipeIn, &msg, sizeof(int));
	while(msg > 0)
	{
		aux = new Node;
		aux->pid = msg;
		aux->next = list;
		list = aux;
		read(pipeIn, &msg, sizeof(int));
	}

	aux = list->next;
	while(aux != 0)
	{
		cout << "kill " << list->pid << endl;
		kill(list->pid, SIGKILL);
		delete list;
		list = aux;
		aux = aux->next;
	}
	kill(list->pid, SIGKILL);
	delete list;

	return 0;
}
