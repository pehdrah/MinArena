all: library

library: util primaryStats message geometry
	c++ -shared -o libBase.a util.o primaryStats.o message.o geometry.o

geometry:
	c++ -c geometry.cpp

message:
	c++ -c message.cpp

primaryStats:
	c++ -c primaryStats.cpp

util:
	c++ -c util.cpp
