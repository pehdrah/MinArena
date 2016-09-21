#ifndef ServerArena_H
#define ServerArena_H 1

#include "../lib_server/arena.h"
#include "../lib/state.h"


class ServerArena
{
	private:
		int *playerIds, *playerStatus, *lost, *count;
		bool *calculateRtt, *receipt;
		long *rtts, *qtrtts;
		Object** playerObjects;
		int* commands;
		Arena* arena;
	public:
		ServerArena(Arena *arena);
		void changeState();
		void prepareCommand(int client, int action);
		void procCommand(int position, int action);
		void addPlayer(ObjectStats *stats, int client);
		void removePlayer(int client);
		void markRtt(int client, long rtt);
		int findClientSpot(int client);
		int* getPlayerStatus();
		bool playerConnected(int client);
		State getCurrentState();
};

#endif
