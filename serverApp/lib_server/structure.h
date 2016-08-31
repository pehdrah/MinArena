#ifndef Structure_H
#define Structure_H 1

#include "object.h"

/*
* @brief A node to make linked list of stuff hitted by a strike
*
* This is a simple Node structure to make linked lists of associations between objects and
* booleans
*/
typedef struct IntNode
{
	int key;
	IntNode *next;
} StrikedNode;

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
	Object *obj;
	ArenaObjectNode *next;

	IntNode *strikeds;
} ArenaObjectNode;

/*
* @brief A node to make linked list of objects
*
* This is a simple Node structure to make linked lists of Objects.
*/
typedef struct ObjectNode
{
	Object *obj;
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
class Structure
{
	protected:
		int* nextId;
		ArenaObjectNode **objects;
		ArenaObjectNode* getNode(Object* obj);
		ArenaObjectNode* getNode(int id, unsigned char type);
		ObjectNode* makeObjectsList(int start, int end);
	public:
		Structure();
		bool addObject(Object *obj);
		void removeObject(int id, unsigned char type);
		void removeObject(Object *obj);
		void destroyObject(Object *obj);
		void addStriked(Object* obj, int strikedId);
		bool getStriked(Object* obj, int strikedId);
		Object* getObject(int id, unsigned char type);
		ObjectNode* getObjectsList();
		ObjectNode* getSuspendedObjectsList();
		ObjectNode* getAnimatedObjectsList();
		ObjectNode* getPlayersObjectsList();
		ObjectNode* getTemporaryObjectsList();		
};

#endif
