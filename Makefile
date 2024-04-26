CC=gcc
CCFLAGS=-D_DEFAULT_SOURCE -D_XOPEN_SOURCE -D_BSD_SOURCE -std=c11 -pedantic -Wvla -Wall -Werror
 
all: server client 
 
# SERVER 
server: server.o utils_v1.o network_serveur.o ipc.o 
	$(CC) $(CCFLAGS) -o server server.o utils_v1.o network_serveur.o ipc.o

<<<<<<< HEAD
all:  server client game ipc  network_client network_server

#SERVER
server : server.o utils_v1.o network_serveur.o
	$(CC) $(CCFLAGS) -o server server.o utils_v1.o network_serveur.o
server.o: server.c server.h utils_v1.h config.h message.h ipc.h network_serveur.h player.h
	$(CC) $(CCFLAGS) -c server.c

#CLIENT
client : client.o utils_v1.o
	$(CC) $(CCFLAGS) -o client client.o utils_v1.o
=======
server.o: server.c server.h utils_v1.h config.h message.h ipc.h network_serveur.h player.h
	$(CC) $(CCFLAGS) -c server.c

# CLIENT 
client: client.o utils_v1.o game.o network_client.o
	$(CC) $(CCFLAGS) -o client client.o utils_v1.o game.o network_client.o

>>>>>>> 48cfc972f06b663f7c186b1e372c782273df7bd9
client.o: client.c client.h utils_v1.h config.h player.h game.h network_client.h message.h
	$(CC) $(CCFLAGS) -c client.c

# GAME 
game: game.o utils_v1.o
	$(CC) $(CCFLAGS) -o game game.o utils_v1.o
<<<<<<< HEAD
game.o: game.c game.h utils_v1.h 
	$(CC) $(CCFLAGS) -c game.c

#IPC
ipc : ipc.o utils_v1.o
	$(CC) $(CCFLAGS) -o ipc ipc.o utils_v1.o
ipc.o: ipc.c ipc.h utils_v1.h config.h
	$(CC) $(CCFLAGS) -c ipc.c

#NETWORK_CLIENT
network_client : network_client.o utils_v1.o
	$(CC) $(CCFLAGS) -o network_client network_client.o utils_v1.o
=======

game.o: game.c game.h utils_v1.h 
	$(CC) $(CCFLAGS) -c game.c

# IPC 
ipc.o: ipc.c ipc.h utils_v1.h config.h
	$(CC) $(CCFLAGS) -c ipc.c

# NETWORK_CLIENT 
>>>>>>> 48cfc972f06b663f7c186b1e372c782273df7bd9
network_client.o: network_client.c network_client.h utils_v1.h
	$(CC) $(CCFLAGS) -c network_client.c

# NETWORK_SERVER 
network_server.o: network_server.c utils_v1.h
	$(CC) $(CCFLAGS) -c network_server.c

# UTILS 
utils_v1.o: utils_v1.c utils_v1.h
	$(CC) $(CCFLAGS) -c utils_v1.c

clean : 
	rm *.o
	rm client
