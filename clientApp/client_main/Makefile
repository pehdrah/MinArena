all: clientApp

clientApp: clientArena clientMain
	c++ -o clientApp clientMain.o clientArena.o ../lib_client/client.o -Wl -L../../lib -L../client_gui -Wl -lBase -lGraphics

clientMain:
	c++ -c clientMain.cpp -I/usr/local/include -I../

clientArena:
	c++ -c clientArena.cpp -I/usr/local/include -I../

bots: bot
	c++ -c botSpawner.cpp
	c++ -o bots botSpawner.o bot.o clientArena.o ../lib_client/client.o -Wl -L../../lib -L../client_gui -Wl -lBase -lGraphics

bot:
	c++ -c bot.cpp -I/usr/local/include -I../
