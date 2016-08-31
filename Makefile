#This is a test makefile

all: library client server

server:
	cd serverApp && $(MAKE)

client:
	cd clientApp && $(MAKE)

library:
	cd lib && $(MAKE)
