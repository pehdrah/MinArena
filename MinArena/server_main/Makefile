all: serverArena serverProcess serverRoutines serverMain serverExec

serverExec:
	c++ -o test serverMain.o serverProcess.o serverRoutines.o serverArena.o -L../lib_server -L../lib/ -lBase -lServer

serverMain:
	c++ -c serverMain.cpp

serverRoutines:
	c++ -c serverRoutines.cpp

serverProcess:
	c++ -c serverProcess.cpp

serverArena:
	c++ -c serverArena.cpp
