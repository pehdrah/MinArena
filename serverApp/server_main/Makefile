all: serverArena serverProcess serverRoutines serverMain serverExec

serverExec:
	c++ -o serverApp serverMain.o serverProcess.o serverRoutines.o serverArena.o -L../lib_server -L../../lib/ -lBase -lServer

serverMain:
	c++ -c serverMain.cpp -I../

serverRoutines:
	c++ -c serverRoutines.cpp -I../

serverProcess:
	c++ -c serverProcess.cpp -I../

serverArena:
	c++ -c serverArena.cpp -I../
