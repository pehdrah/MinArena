all: mainExec

mainExec: clientArena clientMain
	c++ -o test clientMain.o clientArena.o ../lib_client/client.o -Wl -L../lib -L../client_gui -Wl -lBase -lGraphics

clientMain:
	c++ -c clientMain.cpp -I/usr/local/include

clientArena:
	c++ -c clientArena.cpp -I/usr/local/include

bots: bot
	c++ -c botSpawner.cpp
	c++ -o bots botSpawner.o bot.o clientArena.o ../lib_client/client.o -Wl -L../lib -L../client_gui -Wl -lBase -lGraphics

bot:
	c++ -c bot.cpp -I/usr/local/include
