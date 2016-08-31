#ifndef Structure_H
#define Structure_H 1

#include "gObject.h"

/**
* @brief A packet with an object and extra information.
*
* In order to work, each object must be associated with other information used by the arena.
* This structures makes the associations.
*
* @author Pedro Araujo Rosa
*/
typedef struct ArenaObjectNode
{
	G_Object *obj;
	ArenaObjectNode *next;
} ArenaObjectNode;

/*
* @brief A node to make linked list of objects
*
* This is a simple Node structure to make linked lists of Objects.
*/
typedef struct ObjectNode
{
	G_Object *obj;
	ObjectNode *next;
} ObjectNode;

/*
* @brief The Data Structure Class
*
* This class is the the one that defines how the objects are stored. If it is either through
* a list, tree, hash, or whatever. It is used to store objects only. So it plays no role on how
* we store geometrical data.
*
* @author Pedro Araujo Rosa
*/
class G_Structure
{
	protected:
		int* nextId;
		ArenaObjectNode **objects;
		ArenaObjectNode* getNode(G_Object* obj);
		ArenaObjectNode* getNode(int id, unsigned char type);
		ObjectNode* makeObjectsList(int start, int end);
	public:
		G_Structure();
		bool addObject(G_Object *obj);
		void removeObject(int id, unsigned char type);
		void removeObject(G_Object *obj);
		void destroyObject(G_Object *obj);
		G_Object* getGraphicObject(int id, unsigned char type);
		ObjectNode* getObjectsList();
		ObjectNode* getSuspendedObjectsList();
		ObjectNode* getAnimatedObjectsList();
		ObjectNode* getPlayersObjectsList();
		ObjectNode* getTemporaryObjectsList();		
};

#endif
