all: objectStats object collisions structure arena server serverNetwork library

library:
	c++ -shared -o libServer.a objectStats.o object.o collisions.o structure.o arena.o server.o serverNetwork.o

serverNetwork:
	c++ -c serverNetwork.cpp

server:
	c++ -c server.cpp

arena:
	c++ -c arena.cpp

structure:
	c++ -c structure.cpp

collisions:
	c++ -c collisions.cpp

object:
	c++ -c object.cpp

objectStats:
	c++ -c objectStats.cpp
