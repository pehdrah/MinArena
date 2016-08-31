all: gObject gStructure	gBasics gMenu gCharSel gArena gLibrary

gLibrary:
	c++ -shared -o libGraphics.a gObject.o gStructure.o gBasics.o gMenu.o gCharSel.o gArena.o -L/usr/local/lib -lSDL

gArena:
	c++ -c gArena.cpp -I/usr/local/include

gCharSel:
	c++ -c gCharSel.cpp -I/usr/local/include

gMenu:
	c++ -c gMenu.cpp -I/usr/local/include

gBasics:
	c++ -c gBasics.cpp -I/usr/local/include

gStructure:
	c++ -c gStructure.cpp

gObject:
	c++ -c gObject.cpp
