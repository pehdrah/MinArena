#ifndef ClientArena_H
#define ClientArena_H 1

#include "../client_gui/gArena.h"
#include "../lib/state.h"

class ClientArena
{
	private:
		G_Arena* arena;
		int event;
	public:
		int* playerIds;
		G_Object** playerObjects;
		ClientArena(G_Arena *arena);
		void checkEvents(State current);
		void removeTemporaries();
		void addObject(G_Object *object);
		G_Object* getPlayerObject(int player);
		void zeroEvent();
		void treatEvent(int event);
		int getEvent();
};

#endif
