all: objectStats object collisions structure arena server serverNetwork library

library:
	c++ -shared -o libServer.a objectStats.o object.o collisions.o structure.o arena.o server.o serverNetwork.o -I../

serverNetwork:
	c++ -c serverNetwork.cpp -I../

server:
	c++ -c server.cpp -I../

arena:
	c++ -c arena.cpp -I../

structure:
	c++ -c structure.cpp -I../

collisions:
	c++ -c collisions.cpp -I../

object:
	c++ -c object.cpp -I../

objectStats:
	c++ -c objectStats.cpp -I../
