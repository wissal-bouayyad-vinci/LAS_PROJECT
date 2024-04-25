CC=gcc
CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror

all:  server client game ipc  network_client network_server

#SERVER
server : server.o utils_v1.o
	$(CC) $(CCFLAGS) -o server server.o utils_v1.o
server.o: server.c utils_v1.h
	$(CC) $(CCFLAGS) -c server.c

#CLIENT
client : client.o utils_v1.o
	$(CC) $(CCFLAGS) -o client client.o utils_v1.o
client.o: client.c utils_v1.h
	$(CC) $(CCFLAGS) -c client.c
	
#GAME
game : game.o utils_v1.o
	$(CC) $(CCFLAGS) -o game game.o utils_v1.o
game.o: game.c utils_v1.h
	$(CC) $(CCFLAGS) -c game.c

#IPC
ipc : ipc.o utils_v1.o
	$(CC) $(CCFLAGS) -o ipc ipc.o utils_v1.o
ipc.o: ipc.c utils_v1.h
	$(CC) $(CCFLAGS) -c ipc.c

#NETWORK_CLIENT
network_client : network_client.o utils_v1.o
	$(CC) $(CCFLAGS) -o network_client network_client.o utils_v1.o
network_client.o: network_client.c utils_v1.h
	$(CC) $(CCFLAGS) -c network_client.c

#NETWORK_SERVER
network_server : network_server.o utils_v1.o
	$(CC) $(CCFLAGS) -o network_server network_server.o utils_v1.o
network_server.o: network_server.c utils_v1.h
	$(CC) $(CCFLAGS) -c network_server.c



#UTILS
utils_v1.o: utils_v1.c utils_v1.h
	$(CC) $(CCFLAGS) -c utils_v1.c 

clean : 
	rm *.o
	rm client
	rm game
	rm ipc
	rm network_client
	rm network_server